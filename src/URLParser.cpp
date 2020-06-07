#include "URLParser.hpp"

URLParser::URLParser(const std::string& url) {
  // convert to a char*
  // read first for the protocol (a `://`)
  // if its found go backand read what precedes it as the protocol
  // then read forward to a break char (/, #, ?)
  // parse appropriately based on that character

  char char_encountered;

  const char* url_ptr = url.c_str();
  size_t phrase_end = url.find("://");
  if (phrase_end != url.npos) {
    // skip past protocol (always going to use HTTP for now)
    url_ptr += (phrase_end + 2);
  }

  phrase_end = ReadUntilNextInvalidChar(url_ptr, &char_encountered);
  domain_name_ = std::string(url_ptr, phrase_end);

  while (char_encountered != '\0') {
    switch(char_encountered) {
      case '/':
        // start reading the path itself
        // while the character encountered is still a slash, keep reading
        break;
      case '?':
        // url params
        // just read until the next one
        break;
      case '#':
        // fragment
        // no idea what this is doing here
        // just like ignore up until the next invalid
        break;
      case ':':
        // port number
        // read to an int and store somewhere
        // up until next invalid char
        break;
      default:
        // this shouldn't occur but if it does idk lol
        break;
    }

    url_ptr += phrase_end;

    // url_ptr points at the invalid char
    // i think this is right
    phrase_end = ReadUntilNextInvalidChar(url_ptr, &char_encountered);
  }

}

size_t URLParser::ReadUntilNextInvalidChar(const char* inputl, char* char_encountered) {
  return 1;
}