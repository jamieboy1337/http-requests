#include "gtest/gtest.h"
#include "URLParser.hpp"


TEST(URLParserTest, CreateURL) {
  URLParser parser("http://google.com");
  // just ensure it doesn't crash lol
  ASSERT_EQ(parser.GetDomainName(), "google.com");
  ASSERT_EQ(parser.GetPath(), "/");
  ASSERT_EQ(parser.GetQueryParameters(), "");
}