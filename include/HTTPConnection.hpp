#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <string>
#include <functional>

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
  HTTPResponse(std::string response);

  /**
   *  Fetch a header from the connection
   */ 
  std::string GetHeader(const std::string& key);
  const std::string& GetBody();

 private:
  std::unordered_map<std::string, std::string> headers_;
  std::string body_;
  HTTPMethod method_;
};

/**
 *  Represents a single connection instance between server and client
 *  Abstract
 */ 

class HTTPConnection {
 public:

  HTTPConnection();

  /**
   *  Set a given HTTP header on the connection
   *  @param key - the http key
   *  @param value - the value associated with that key
   */ 
  void SetHeader(const std::string& key, const std::string& value);

  /**
   *  Send a request to the specified domain name
   */ 
  void send(const std::string& domain_name);

  // probably spin up a thread for that, its an implementation detail though

  /**
   *  Associates a function callback with the connection, called once the
   *  response is received and parsed.
   *  @param response_func - the function which is run once the response is received.
   */ 
  void SetResponseCallback(std::function<void(HTTPResponse)> response_func);
};

#endif // CONNECTION_H_