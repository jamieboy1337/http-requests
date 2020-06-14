#ifndef HTTP_SOCKET_H_
#define HTTP_SOCKET_H_

#include "URLParser.hpp"

#include <memory>

// pimpl
struct SocketData;

/**
 *  Abstraction of a socket. Used to handle differences between platforms.
 */ 
class HTTPSocket {
 public:
  /**
   *  Constructs a new HTTPSocket from a parsed URL and connects to the specified resource.
   *  @param url - A parsed URL representing the address we are connecting to.
   */ 
  HTTPSocket(const URLParser& url);
  /**
   *  Read `size` characters from the buffer and into the socket.
   *  @param buf - The data we are reading.
   *  @param size - The number of characters we are reading.
   * 
   *  @return - number of characters which could be read.
   */ 
  int Read(char* buf, int size);

  /**
   *  Write `size` characters from the buffer and into the socket.
   *  @param buf - The location we are writing to.
   *  @param size - The number of bytes we are trying to write to the buffer.
   */ 
  void Write(char* buf, int size);


  ~HTTPSocket();
  HTTPSocket(const HTTPSocket& rhs) = delete;
  HTTPSocket& operator=(const HTTPSocket& rhs) = delete;

 private:
  // pimpl
  std::unique_ptr<SocketData> data_;
  // whether the socket was connected suffessfully
  bool is_connected_;
};

#endif