#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <string>
#include <unordered_map>

/**
 *  All of the methods we support
 *  Look at that there's only one right now
 */ 
enum HTTPMethod {
  GET
};

class HTTPResponse {
 public:
  // i dont know what we need yet lol

  /**
   *  Creates an HTTP response from a parsed response string.
   */ 
  HTTPResponse(std::string header);

  /**
   *  Fetch a header from the connection
   */ 
  std::string GetHeader(const std::string& key) const;

  /**
   *  Body of the response message
   */ 
  std::string body_;

 private:
  std::unordered_map<std::string, std::string> headers_;
  HTTPMethod method_;
};

#endif // CONNECTION_H_