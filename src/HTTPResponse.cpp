#include "HTTPResponse.hpp"

#include <algorithm>

HTTPResponse::HTTPResponse(std::string header) {
  // TODO: parse the response once we have it ;p
  // assume response is properly formatted

  // splitting the string

  // iterative string::find on '\r\n' and ':'

  // http://www.cplusplus.com/reference/algorithm/find_if/
  // s.erase from begin to the first non-space char
  // find non-space with std::find_if and a unary predicate

  std::string header_entry;
  std::string header_key;
  std::string header_value;

  int start_pos = 0;
  int end_pos;

  int colon_pos;

  while ((end_pos = header.find("\r\n", start_pos)) != header.npos) {
    header_entry = header.substr(start_pos, end_pos);
    colon_pos = header_entry.find(":");
    header_key = header_entry.substr(0, colon_pos);
    header_value = header_entry.substr(colon_pos + 1);

    Trim(header_key);
    Trim(header_value);

    headers_.insert(std::pair<std::string, std::string>(header_key, header_value));
  }
}

std::string HTTPResponse::GetHeader(const std::string& key) const {
  if (headers_.find(key) != headers_.end()) {
    return headers_.at(key);
  }

  return "";
}


// factor this out later if i need to lol
std::string& HTTPResponse::Trim(std::string& target) {
  // find white space
  auto predicate = [](int c) {return std::isspace(c);};
  target.erase(target.begin(), std::find_if(target.begin(), target.end(), predicate));

  // base() : returns the iterator used to create the reverse iterator (returned by find_if)
  target.erase(std::find_if(target.rend(), target.rbegin(), predicate).base(), target.end());
}