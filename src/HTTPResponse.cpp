#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse(std::string response) {
  // TODO: parse the response once we have it ;p
}

std::string HTTPResponse::GetHeader(const std::string& key) {
  return headers_[key];
}

const std::string& HTTPResponse::GetBody() {
  return body_; 
}