#include "HTTPRequest.hpp"

#include <iostream>
#include <sstream>

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>

const char* const HTTPRequest::delimiter = "\r\n\r\n";

HTTPRequest::HTTPRequest() {
  
}

void HTTPRequest::SetHeader(const std::string& key, const std::string& value) {
  std::string header_key = ToHeaderCase(key);
  if (headers_.find(header_key) != headers_.end()) {
    // i dont think anything is necessary here
  }

  headers_.insert(std::make_pair(header_key, value));
}

void HTTPRequest::SetResponseCallback(std::function<void(HTTPResponse)> response_func) {
  callback_ = response_func;
}

void HTTPRequest::Send(Method method, const std::string& domain_name) {
  // do synchronous for now
  // but move this to a thread later
  URLParser url(domain_name);

  socket_ = std::move(HTTPSocket(url));
  
    // socket is valid
    // compile http message
  std::string message;
  switch (method) {
    case Method::GET:
      message.append("GET ");
      break;
  }

  message.append(url.GetPath());
  message.append(" HTTP/1.1\r\n");
  // host parameter is now required
  // per RFC2616


  // dont fuck me up dude
  SetHeader("Host", url.GetDomainName());
  // i can do gzip but im not gonna yet
  SetHeader("Accept", "identity");

  // append all headers to the request
  for (auto header : headers_) {
    message.append(header.first);
    message.append(": ");
    message.append(header.second);
    message.append("\r\n");

    
  }

  // end of header
  message.append("\r\n");

  // TODO: allow user to set body, and parse that appropriately

  // add a content length header to deal with it

  // TODO: handle keep-alive sockets

  int bytes_left = message.length();
  int bytes_processed;
  const char* c_message = message.c_str();
  // get a response
  while (bytes_left > 0) {
    bytes_processed = socket_.Write(c_message, bytes_left);
    if (bytes_processed == -1) {
      // error occurred while reading
      // TODO: handle error case lol
    }

    bytes_left -= bytes_processed;
    c_message += bytes_processed;
  }

  // wait for write to return

  // allocate some space to write to
  std::string response;
  char* buffer = new char[1024];
  char* body_start;
  int bytes_read = 0;

  size_t header_length;

  for (;;) {
    bytes_processed = socket_.Read(buffer, 1024);
    if (bytes_processed != -1) {
      response.append(buffer, bytes_processed);
      // could have a situation where the EOF is spread across two reads
      header_length = response.find(delimiter, std::max(bytes_read - 4, 0));
      if (header_length != response.npos) {
        // we've found our delimiter.
        body_start = buffer + (header_length - bytes_read + 4);
        break;
        
      }
    }
    
    bytes_read += bytes_processed;
  }

  // parse header
  // do something to retain the current state of the buffer

  HTTPResponse(response.substr(0, header_length));



  // and then we can construct an HTTPResponse object
  // and call our response callback with that
}