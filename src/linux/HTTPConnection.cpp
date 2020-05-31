#include "HTTPConnection.hpp"

#include <iostream>

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>

HTTPConnection::HTTPConnection() {  
}

void HTTPConnection::SetHeader(const std::string& key, const std::string& value) {
  headers_.insert(std::make_pair(key, value));
}

void HTTPConnection::SetResponseCallback(std::function<void(HTTPResponse)> response_func) {
  callback_ = response_func;
}


void HTTPConnection::Send(const std::string& domain_name) {
  if (ConnectToServer(domain_name)) {
    // socket is valid
    // compile http message
  }
}

bool HTTPConnection::ConnectToServer(const std::string& domain_name) {
  addrinfo* info;
  addrinfo hints;
  memset(&hints, 0, sizeof(addrinfo));

  // type of socket (datagram vs stream)
  hints.ai_socktype = SOCK_STREAM;
  // ipv4/v6 (unspec: either)
  hints.ai_family = AF_UNSPEC;
  // ip wildcard
  hints.ai_flags = AI_PASSIVE;

  int success;

  // get all valid addresses
  success = getaddrinfo(domain_name.c_str(), NULL, &hints, &info);
  if (success != 0) {
    // failure
    std::cout << "failed to return meaningful results from addrinfo" << std::endl;
    return false;
  }

  int addr_size;

  addrinfo* infoptr = info;

  sockaddr_storage addr;
  sockaddr* sock;
  
  // iterate through available connections
  do {
    addr_size = info->ai_addrlen;
    if (info->ai_family == AF_INET) {
      sockaddr_in* address = reinterpret_cast<sockaddr_in*>(info->ai_addr);
      address->sin_port = htons(80);
    } else if (info->ai_family == AF_INET6) {
      sockaddr_in6* address = reinterpret_cast<sockaddr_in6*>(info->ai_addr);
      address->sin6_port = htons(80);
    } else {
      continue;
    }

    // found a good address
    // store it elsewhere in case we need to save it
    memcpy(&addr, info->ai_addr, info->ai_addrlen);
    sock = reinterpret_cast<sockaddr*>(&addr);

    socket_fd_ = socket(sock->sa_family, SOCK_STREAM, 0);
    if (!success) {
      continue;
    }

    success = connect(socket_fd_, sock, info->ai_addrlen);
    if (success == 0) {
      freeaddrinfo(info);
      return true;
    }

    close(socket_fd_);

    // socket is created
    
  } while ((infoptr = infoptr->ai_next) != NULL);

  freeaddrinfo(info);

  return false;
}