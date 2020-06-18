#include "gtest/gtest.h"

#include <thread>

#include "HTTPSocket.hpp"
#include "tests/TestListener.hpp"


TEST(HTTPSocketTests, VerifyConnection) {

  const std::string message = "this is a placeholder message\n";
  TestListener listener(1024);
  listener.Listen(1201);

  std::cout << "listening..." << std::endl;

  int success;

  URLParser parser("http://127.0.0.1:1201");

  // create thread to accept
  std::thread accept_thread([&listener]() { listener.Accept(); });
  HTTPSocket testsocket(parser);

  success = testsocket.Write(message.c_str(), message.size());

  ASSERT_NE(success, -1);

  std::cout << "the" << std::endl;

  accept_thread.join();

  ASSERT_NE(listener.GetMessage(), nullptr);

  std::string response(listener.GetMessage());

  ASSERT_EQ(response, message);

}