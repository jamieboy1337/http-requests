#include <iostream>

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>

#define PORT 1620

int main(int argc, char** argv) {
  addrinfo hints;
  addrinfo* results;
  memset(&hints, 0, sizeof(addrinfo));

  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE;

  int success = getaddrinfo("localhost", NULL, &hints, &results);

  if (success != 0) {
    std::cout << "oops" << std::endl;
    exit(EXIT_FAILURE);
  }

  char shitspace[128];

  std::cout << "money" << std::endl;

  addrinfo* res = results;
  for (res = results; res != NULL; res = res->ai_next) {
    std::cout << "ok" << std::endl;
    if (res->ai_family == AF_INET) {
      std::cout << "AF_INET" << std::endl;
      sockaddr_in* addr = reinterpret_cast<sockaddr_in*>(res->ai_addr);
      std::cout << inet_ntop(res->ai_family, (const void*)&addr->sin_addr, shitspace, 127) << std::endl;
      addr->sin_port = htons(PORT);
    } else if (res->ai_family == AF_INET6) {
      std::cout << "AF_INET6" << std::endl;
      sockaddr_in6* addr = reinterpret_cast<sockaddr_in6*>(res->ai_addr);
      std::cout << inet_ntop(res->ai_family, (const void*)&addr->sin6_addr, shitspace, 127) << std::endl;
      addr->sin6_port = htons(PORT);
    } else {
      std::cout << "no dice" << std::endl;
      continue;
    }

    sockaddr* addr = reinterpret_cast<sockaddr*>(res->ai_addr);
    std::cout << "getting ready" << std::endl;
    int fd = socket(addr->sa_family, SOCK_STREAM, 0);
    if (fd == -1) {
      std::cout << "bad socket" << std::endl;
    }

    std::cout << "connecting " << std::endl;
    int success = connect(fd, addr, res->ai_addrlen);
    if (success == -1) {
      std::cout << "bad connect" << std::endl;
      std::cout << errno << std::endl;
      close(fd);
    }

    write(fd, (void*)"hello lady\n", 11);
    close(fd);
  }

  freeaddrinfo(results);
}