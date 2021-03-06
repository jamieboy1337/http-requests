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
   *  Fetch a header from the response
   *  Returns empty string if not found
   */ 
  std::string GetHeader(const std::string& key) const;

  /**
   *  @return the response code associated with the request
   */ 
  int GetResponseCode();

  /**
   *  @return the response description as a string as provided by response
   */ 
  const std::string& GetResponseDescription();

  /**
   *  Body of the response message
   */ 
  std::string body_;

 private:
  std::unordered_map<std::string, std::string> headers_;
  HTTPMethod method_;

  int response_code_;
  std::string response_desc_;

  /**
   *  Trims white space in string.
   */ 
  std::string& Trim(std::string& target);

};

#endif // CONNECTION_H_