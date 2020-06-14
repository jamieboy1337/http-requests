#include "HTTPSocket.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>

#include <iostream>

struct SocketData {
  int socket_fd;
};

HTTPSocket::HTTPSocket(const URLParser& url) {
  data_ = std::make_unique<SocketData>();
  is_connected_ = false;
  
  addrinfo hints;
  addrinfo* info;
  memset(&hints, 0, sizeof(addrinfo));
  
  int success;

  hints.ai_socktype = SOCK_STREAM;  // stream vs. datagram
  hints.ai_family = AF_UNSPEC;      // v4 / v6
  hints.ai_flags = AI_PASSIVE;      // wildcard for ip

  success = getaddrinfo(url.GetDomainName().c_str(), NULL, &hints, &info);

  if (success != 0) {
    // failed to get addr info
    std::cout << "Could not resolve domain name." << std::endl;
  } else {

    addrinfo* infoptr = info;
    sockaddr_storage storage;
    int addr_size;

    int port;

    // port we are connected to
    switch (url.GetRequestType()) {
      default:
      case HTTP:
        port = 80;
        break;
      case HTTPS:
        port = 443;
        break;
    }

    // TODO: use functions for handling socket creation based on protocol (probably wont happen)

    do {
      addr_size = infoptr->ai_addrlen;
      // set port appropriately
      switch (addr_size) {
        case AF_INET:
          sockaddr_in* address = reinterpret_cast<sockaddr_in*>(infoptr->ai_addr);
          address->sin_port = htons(port);
          break;
        case AF_INET6:
          sockaddr_in6* address = reinterpret_cast<sockaddr_in6*>(infoptr->ai_addr);
          address->sin_port = htons(port);
          break;
        default:
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

    freeaddrinfo(info);

  }

}

int HTTPSocket::Read(char* buf, int size) {
  return -1;
}

void HTTPSocket::Write(char* buf, int size) {

}

