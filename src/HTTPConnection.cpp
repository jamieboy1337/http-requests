#include "HTTPConnection.hpp"

#include <iostream>
#include <sstream>

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>

HTTPConnection::HTTPConnection() {  
}

void HTTPConnection::SetHeader(const std::string& key, const std::string& value) {
  std::string header_key = ToHeaderCase(key);
  if (headers_.find(header_key) != headers_.end()) {
    // i dont think anything is necessary here
  }

  headers_.insert(std::make_pair(header_key, value));
}

void HTTPConnection::SetResponseCallback(std::function<void(HTTPResponse)> response_func) {
  callback_ = response_func;
}

// TODO: isolate the socket calls into a class and then that will be the part that's os-dependent
void HTTPConnection::Send(Method method, const std::string& domain_name) {
  // do synchronous for now
  // but move this to a thread later
  URLParser url(domain_name);
  if (ConnectToServer(domain_name, url)) {
    // socket is valid
    // compile http message
    std::string message;
    switch (method) {
      case Method::GET:
        message.append("GET ");
        break;
    }

    message.append(url.GetPath());
    message.append(" HTTP/1.1\r\n");
    // host parameter is now required
    // per RFC2616


    // dont fuck me up dude
    SetHeader("Host", url.GetDomainName());
    // i can do gzip but im not gonna yet
    SetHeader("Accept", "identity");

    // append all headers to the request
    for (auto header : headers_) {
      message.append(header.first);
      message.append(": ");
      message.append(header.second);
      message.append("\r\n");
    }

    // todo: allow user to set body, and parse that appropriately

    // can probably send it now

    // need to batch the write calls and write to the socket
    // verify that those writes are sent successfully
    // then one we're done we await the read
    // and start reading
    // once we reach the end we want to store the socket

    // TODO: handle keep-alive sockets

    int bytes_left = message.length();
    while (bytes_left > 0) {
      
    }

    // and then we can construct an HTTPResponse object
    // and call our response callback with that
  }

  close(socket_fd_);
}

bool HTTPConnection::ConnectToServer(const std::string& domain_name, URLParser& parser) {
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
  
  success = getaddrinfo(parser.GetDomainName().c_str(), NULL, &hints, &info);
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
      // urlparser
      address->sin_port = htons(HTTP_PORT);
    } else if (info->ai_family == AF_INET6) {
      sockaddr_in6* address = reinterpret_cast<sockaddr_in6*>(info->ai_addr);
      address->sin6_port = htons(HTTP_PORT);
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

std::string HTTPConnection::ToHeaderCase(std::string header) {
  char last = '\0';
  std::string result;

  for (char cur : header) {
    if (last == '\0' || last == '-') {
      cur = toupper(cur);
    }

    result.push_back(cur);
    last = cur;
  }

  return result;
}