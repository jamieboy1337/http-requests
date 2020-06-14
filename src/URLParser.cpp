#include "URLParser.hpp"

#include <iostream>

URLParser::URLParser(const std::string& url) {
  // convert to a char*
  // read first for the protocol (a `://`)
  // if its found go backand read what precedes it as the protocol
  // then read forward to a break char (/, #, ?)
  // parse appropriately based on that character

  char char_encountered;

  path_ = "/";
  query_params_ = "";
  port_ = 80;

  const char* url_ptr = url.c_str();
  size_t phrase_end = url.find("://");
  if (phrase_end != url.npos) {
    // skip past protocol (always going to use HTTP for now)
    url_ptr += (phrase_end + 3);
  } else {
    // nothing changes here yet -- no protocol so assume HTTP as usual
  }

  // should read full domain name
  phrase_end = ReadUntilNextInvalidChar(url_ptr, &char_encountered, true);
  domain_name_ = std::string(url_ptr, phrase_end - 1);
  url_ptr += phrase_end;

  std::string workspace;
  size_t read_size;

  while (char_encountered != '\0') {
    read_size = 0;
    // skips right past the last invalid char
    switch(char_encountered) {
      case '/':
        // start reading the path itself
        // while the character encountered is still a slash, keep reading


        while (char_encountered == '/') {
          phrase_end = ReadUntilNextInvalidChar(url_ptr + read_size, &char_encountered, false);
          read_size += phrase_end;
        }
        // this shoots past the next invalid character so we subtract one
        path_.append(std::string(url_ptr, (read_size - 1)));
        url_ptr += read_size;
        // next char is stored
        break;
      case '?':
        // url params
        // just read until the next one
        phrase_end = ReadUntilNextInvalidChar(url_ptr, &char_encountered, false);
        query_params_ = std::string(url_ptr, (phrase_end - 1));
        url_ptr += phrase_end;
        break;
      case '#':
        // fragment
        // no idea what this is doing here
        // just like ignore up until the next invalid
        phrase_end = ReadUntilNextInvalidChar(url_ptr, &char_encountered, false);
        url_ptr += phrase_end;
        break;
      case ':':
        // port number
        // read to an int and store somewhere
        // up until next invalid char
        phrase_end = ReadUntilNextInvalidChar(url_ptr, &char_encountered, false);
        port_ = std::stoi(std::string(url_ptr, (phrase_end - 1)));
        url_ptr += (phrase_end);
        break;
      default:
        // this shouldn't occur but if it does idk lol
        break;
    }
  }

  // should be done now -- reached the end of the URL
}

const std::string& URLParser::GetQueryParameters() const {
  return query_params_;
}

const std::string& URLParser::GetDomainName() const {
  return domain_name_; 
}

const std::string& URLParser::GetPath() const {
  return path_;
}

RequestType URLParser::GetRequestType() const {
  return protocol_;
}

int URLParser::GetPort() const {
  return port_;
}



size_t URLParser::ReadUntilNextInvalidChar(const char* input, char* char_encountered, bool parse_colon) const {
  size_t result = 0;

  for (;;) {
    *char_encountered = *input++;
    result++;
    switch (*char_encountered) {
      case '/':
      case '?':
      case '#':
      case '\0':
        return result;
        break;
      case ':':
        if (parse_colon) {
          return result;
        }
        break;
    }
  }
}