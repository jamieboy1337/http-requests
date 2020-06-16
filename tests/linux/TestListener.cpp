#include "tests/TestListener.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>

#include <iostream>

TestListener::TestListener(int buf_size) {
  is_listening_ = false;
  buffer_ = new char[buf_size];
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
  }

  success = listen(host_fd_, SOMAXCONN);
  if (success == -1) {
    std::cout << "failed to listen!" << std::endl;
  }

  is_listening_ = true;
}