#include "HTTPSocket.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>

#include <iostream>

struct SocketData {
  int socket_fd;
};

HTTPSocket::HTTPSocket() {
  data_ = nullptr;
  is_connected_ = false;
}

HTTPSocket::HTTPSocket(const URLParser& url) {
  data_ = std::make_unique<SocketData>();
  is_connected_ = false;
  
  addrinfo hints;
  addrinfo* info;
  memset(&hints, 0, sizeof(addrinfo));
  
  int success;

  hints.ai_family = AF_UNSPEC;      // v4 / v6
  hints.ai_flags = AI_PASSIVE;      // wildcard for ip
  hints.ai_socktype = SOCK_STREAM;  // stream vs. datagram

  success = getaddrinfo(url.GetDomainName().c_str(), NULL, &hints, &info);

  if (success != 0) {
    // failed to get addr info
    std::cout << "Could not resolve domain name." << std::endl;
  } else {

    addrinfo* infoptr = info;
    sockaddr_storage storage;
    int addr_size;

    int port = url.GetPort();

    // TODO: use functions for handling socket creation based on protocol (probably wont happen)

    do {
      addr_size = infoptr->ai_addrlen;
      // set port appropriately
      if (infoptr->ai_family == AF_INET) {
          sockaddr_in* address = reinterpret_cast<sockaddr_in*>(infoptr->ai_addr);
          address->sin_port = htons(port);
      } else if (infoptr->ai_family == AF_INET6) {
          sockaddr_in6* address = reinterpret_cast<sockaddr_in6*>(infoptr->ai_addr);
          address->sin6_port = htons(port);
      } else {
        continue;
      }
      // copy from addrinfo (which will be freed) to storage
      // this is not necessary :)
      memcpy(&storage, infoptr->ai_addr, addr_size); 
      data_->socket_fd = socket(storage.ss_family, SOCK_STREAM, 0);
      if (data_->socket_fd == -1) {
        // error case in creating socket
        continue;
      }

      success = connect(data_->socket_fd, reinterpret_cast<sockaddr*>(&storage), infoptr->ai_addrlen);
      if (success == 0) {
        freeaddrinfo(info);
        is_connected_ = true;
        break;
      }

      close(data_->socket_fd);
      
    } while ((infoptr = infoptr->ai_next) != NULL);

    if (infoptr == NULL) {
      freeaddrinfo(info);
    } 

  }

}

HTTPSocket::HTTPSocket(int fd) {
  data_ = std::make_unique<SocketData>();
  data_->socket_fd = fd;
  is_connected_ = true;
}

HTTPSocket& HTTPSocket::operator=(HTTPSocket&& rhs) {
  data_->socket_fd = rhs.data_->socket_fd;
  is_connected_ = rhs.is_connected_;
}

int HTTPSocket::Read(char* buf, int size) {
  if (!is_connected_) {
    // not connected
    return -1;
  }
  
  int read_result = read(data_->socket_fd, buf, size);
  while (read_result == -1) {
    if (errno == EINTR) {
      read_result = read(data_->socket_fd, buf, size);
    }

    return -1;
  }

  return read_result;
}

int HTTPSocket::Write(const char* buf, int size) {
  if (!is_connected_) {
    return -1;
  }

  int write_result = write(data_->socket_fd, buf, size);
  while (write_result == -1) {
    if (errno == EINTR) {
      write_result = write(data_->socket_fd, buf, size);
    }

    return -1;
  }

  return write_result;
}

HTTPSocket::~HTTPSocket() {
  close(data_->socket_fd);
}

