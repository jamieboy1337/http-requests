#include "tests/TestListener.hpp"
#include "HTTPSocket.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>

#include <fcntl.h>

#include <cstdlib>

#include <iostream>

TestListener::TestListener(int buf_size) : buffer_size_(buf_size) {
  is_listening_ = false;
  buffer_ = new char[buffer_size_];
}

void TestListener::Listen(int port) {
  int success;
  
  host_fd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (host_fd_ == -1) {
    std::cout << "failed to create socket" << std::endl;
    std::cout << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  addrinfo hints;

  memset((void*)&hints, 0, sizeof(addrinfo));
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  addrinfo* res;

  getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &res);

  sockaddr_in* address = reinterpret_cast<sockaddr_in*>(res->ai_addr);
  address->sin_port = htons(port);

  success = bind(host_fd_, reinterpret_cast<sockaddr*>(address), sizeof(sockaddr_in));
  if (success != 0) {
    std::cout << "failed to bind!" << std::endl;
    std::cout << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  success = listen(host_fd_, SOMAXCONN);
  if (success != 0) {
    std::cout << "failed to listen!" << std::endl;
    std::cout << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  is_listening_ = true;
}

void TestListener::Accept() {

  sockaddr_storage addr;
  socklen_t addrlen;

  memset(&addr, 0, sizeof(sockaddr_storage));
  memset(&addrlen, 0, sizeof(socklen_t));
  int accept_fd = accept(host_fd_, reinterpret_cast<sockaddr*>(&addr), &addrlen);

  if (accept_fd == -1) {
    // err while accepting
    std::cout << "failed to accept new connection..." << std::endl;
    exit(EXIT_FAILURE);
  }

  // socket is closed by HTTPSocket dtor
  HTTPSocket accepted_socket(accept_fd);


  char* bufptr = buffer_;
  for (;;) {
    accepted_socket.Read(bufptr, 1);
    if (*bufptr++ == '\n') {
      break;
    }
  }
}

const char* TestListener::GetMessage() {
  if (is_listening_) {
    return buffer_;
  }

  return nullptr;
}

TestListener::~TestListener() {
  delete[] buffer_;
}

