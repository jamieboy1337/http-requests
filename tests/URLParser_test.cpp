#include "gtest/gtest.h"
#include "URLParser.hpp"


TEST(URLParserTest, CreateURL) {
  URLParser parser("http://google.com");
  // just ensure it doesn't crash lol
  ASSERT_EQ(parser.GetDomainName(), "google.com");
  ASSERT_EQ(parser.GetPath(), "/");
  ASSERT_EQ(parser.GetQueryParameters(), "");
}

// create test to verify several URLs

TEST(URLParserTest, CheckMultipleURLs) {
  URLParser twitter_test("https://twitter.com/home");
  ASSERT_EQ(twitter_test.GetDomainName(), "twitter.com");
  ASSERT_EQ(twitter_test.GetPath(), "/home");
  ASSERT_EQ(twitter_test.GetQueryParameters(), "");
  ASSERT_EQ(twitter_test.GetPort(), 80);

  URLParser gmail_test("https://mail.google.com/mail/u/1/#inbox");
  ASSERT_EQ(gmail_test.GetDomainName(), "mail.google.com");
  ASSERT_EQ(gmail_test.GetPath(), "/mail/u/1/");
  ASSERT_EQ(gmail_test.GetQueryParameters(), "");
  ASSERT_EQ(gmail_test.GetPort(), 80);

  URLParser youtube_test("https://www.youtube.com/watch?v=UUjdgaVvc6w");
  ASSERT_EQ(youtube_test.GetDomainName(), "www.youtube.com");
  ASSERT_EQ(youtube_test.GetPath(), "/watch");
  ASSERT_EQ(youtube_test.GetQueryParameters(), "v=UUjdgaVvc6w");
  ASSERT_EQ(youtube_test.GetPort(), 80);

  URLParser google_test("https://www.google.com/search?q=monstre&client=firefox-b-1-d&sxsrf=ALeKk00-4ff7uw_noFQfWPPaJGiZ-fIF8g:1592013736141&source=lnms&tbm=isch&sa=X&ved=2ahUKEwjpiOGp2f3pAhV5GTQIHW4YAoMQ_AUoAXoECBQQAw&biw=1403&bih=764#imgrc=h-MsaHGUJwf8GM");
  ASSERT_EQ(google_test.GetDomainName(), "www.google.com");
  ASSERT_EQ(google_test.GetPath(), "/search");
  ASSERT_EQ(google_test.GetQueryParameters(), "q=monstre&client=firefox-b-1-d&sxsrf=ALeKk00-4ff7uw_noFQfWPPaJGiZ-fIF8g:1592013736141&source=lnms&tbm=isch&sa=X&ved=2ahUKEwjpiOGp2f3pAhV5GTQIHW4YAoMQ_AUoAXoECBQQAw&biw=1403&bih=764");
  ASSERT_EQ(google_test.GetPort(), 80);

  URLParser bible_test("www.bible.ca/indexsalvation.htm");
  ASSERT_EQ(bible_test.GetDomainName(), "www.bible.ca");
  ASSERT_EQ(bible_test.GetPath(), "/indexsalvation.htm");
  ASSERT_EQ(bible_test.GetQueryParameters(), "");
  ASSERT_EQ(bible_test.GetPort(), 80);

  URLParser local_test("localhost:3000/themonkey/livetweets.html");
  ASSERT_EQ(local_test.GetDomainName(), "localhost");
  ASSERT_EQ(local_test.GetPath(), "/themonkey/livetweets.html");
  ASSERT_EQ(local_test.GetQueryParameters(), "");
  ASSERT_EQ(local_test.GetPort(), 3000);
}