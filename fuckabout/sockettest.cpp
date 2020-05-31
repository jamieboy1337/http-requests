#include <iostream>

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>

#define PORT 5000

int main(int argc, char** argv) {
  addrinfo hints;
  addrinfo* results;
  memset(&hints, 0, sizeof(addrinfo));

  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE;

  int success = getaddrinfo("localhost", NULL, &hints, &results);

  char shitspace[128];

  for (addrinfo* i = results; i != NULL; i = i->ai_next) {
    if (i->ai_family == AF_INET) {
      std::cout << "AF_INET" << std::endl;
      sockaddr_in* addr = reinterpret_cast<sockaddr_in*>(i->ai_addr);
      std::cout << inet_ntop(i->ai_family, (const void*)&addr->sin_addr, shitspace, 127) << std::endl;
      addr->sin_port = htons(PORT);
    } else if (i->ai_family == AF_INET6) {
      std::cout << "AF_INET6" << std::endl;
      sockaddr_in6* addr = reinterpret_cast<sockaddr_in6*>(i->ai_addr);
      std::cout << inet_ntop(i->ai_family, (const void*)&addr->sin6_addr, shitspace, 127) << std::endl;
      addr->sin6_port = htons(PORT);
    } else {
      std::cout << "OTHER" << std::endl;
      continue;
    }

    sockaddr* addr = reinterpret_cast<sockaddr*>(i->ai_addr);
    int fd = socket(addr->sa_family, SOCK_STREAM, 0);
    if (fd == -1) {
      std::cout << "bad socket" << std::endl;
      exit(EXIT_FAILURE);
    }

    int success = connect(fd, addr, i->ai_addrlen);
    if (success == -1) {
      std::cout << "bad connect" << std::endl;
      std::cout << errno << std::endl;
      std::cout << EAGAIN << std::endl;
      close(fd);
      exit(EXIT_FAILURE);
    }



    std::cout << "you're good" << std::endl;

    write(fd, (void*)"hello moto\n", 11);
    close(fd);
  }
}