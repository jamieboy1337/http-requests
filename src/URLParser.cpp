#include "URLParser.hpp"

#include <iostream>

URLParser::URLParser(const std::string& url) {

  char char_encountered;

  path_ = "/";
  query_params_ = "";
  port_ = 80;

  const char* url_ptr = url.c_str();
  size_t phrase_end = url.find("://");
  if (phrase_end != url.npos) {
    // skip past protocol (always going to use HTTP for now)
    // if (url.substr(0, phrase_end) == "")
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
    switch(char_encountered) {
      case '/':
        while (char_encountered == '/') {
          phrase_end = ReadUntilNextInvalidChar(url_ptr + read_size, &char_encountered, false);
          read_size += phrase_end;
        }
        // this shoots past the next invalid character so we subtract one
        path_.append(std::string(url_ptr, (read_size - 1)));
        url_ptr += read_size;
        break;
      case '?':
        phrase_end = ReadUntilNextInvalidChar(url_ptr, &char_encountered, false);
        query_params_ = std::string(url_ptr, (phrase_end - 1));
        url_ptr += phrase_end;
        break;
      case '#':
        phrase_end = ReadUntilNextInvalidChar(url_ptr, &char_encountered, false);
        url_ptr += phrase_end;
        break;
      case ':':
        phrase_end = ReadUntilNextInvalidChar(url_ptr, &char_encountered, false);
        port_ = std::stoi(std::string(url_ptr, (phrase_end - 1)));
        url_ptr += phrase_end;
        break;
    }
  }
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