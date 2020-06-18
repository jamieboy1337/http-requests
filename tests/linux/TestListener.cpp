#include "tests/TestListener.hpp"
#include "HTTPSocket.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>

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
    exit(EXIT_FAILURE);
  }

  sockaddr_in localhost_sock;
  inet_pton(AF_INET, "127.0.0.1", &(localhost_sock.sin_addr));
  localhost_sock.sin_port = htons(port);

  success = bind(host_fd_, reinterpret_cast<sockaddr*>(&localhost_sock), sizeof(sockaddr_in));
  if (success != 0) {
    std::cout << "failed to bind!" << std::endl;
    exit(EXIT_FAILURE);
  }

  success = listen(host_fd_, SOMAXCONN);
  if (success != 0) {
    std::cout << "failed to listen!" << std::endl;
    exit(EXIT_FAILURE);
  }

  is_listening_ = true;
}

void TestListener::Accept() {

  sockaddr_storage addr;
  socklen_t addrlen;
  int accept_fd = accept(host_fd_, reinterpret_cast<sockaddr*>(&addr), &addrlen);

  if (accept_fd == -1) {
    // err while accepting
    std::cout << "failed to accept new connection..." << std::endl;
    exit(EXIT_FAILURE);
  }

  // socket is closed by HTTPSocket dtor
  HTTPSocket accepted_socket(accept_fd);
  accepted_socket.Read(buffer_, buffer_size_);
}

const char* TestListener::GetMessage() {
  if (is_listening_) {
    return buffer_;
  }

  return nullptr;
}

