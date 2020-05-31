#include "HTTPResponse.hpp"
#include <functional>
#include <unordered_map>
#include <thread>

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
  void Send(const std::string& domain_name);

  // probably spin up a thread for that, its an implementation detail though

  /**
   *  Associates a function callback with the connection, called once the
   *  response is received and parsed.
   *  @param response_func - the function which is run once the response is received.
   */ 
  void SetResponseCallback(std::function<void(HTTPResponse)> response_func);

 private:
  std::unordered_map<std::string, std::string> headers_;
  int socket_fd_;
  std::thread socket_thread_;
  std::function<void(HTTPResponse)> callback_;

  /**
   *  Attempt to connect to the given domain name.
   *  Returns true if successful, false otherwise.
   */ 
  bool ConnectToServer(const std::string& domain_name);
};