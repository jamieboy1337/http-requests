#ifndef TEST_LISTENER_H_
#define TEST_LISTENER_H_

#include <atomic>
#include <mutex>

/**
 *  Simple listener class which spins up and down
 *  Specifically for testing socket functionality locally
 */ 
class TestListener {
 public:
  // constructor does whatever
  // listen call actually gets the message
  // perform this on some separate thread so we can actually get the message
  // on accept, write the message to a buffer, then tick the proper flag
  // the main thread will create the socket and send a message to the appropriate port
  // then it will spin while the flag is false
  // then once the flag is true we'll read the message and verify it

  /**
   *  Constructs a new listener with the specified buffer size
   */ 
  TestListener(int buf_size);

  /**
   *  Kickstart the listening server on a particular port.
   * 
   *  @param port - The local port on which we are starting the listener.
   */ 
  void Listen(int port);

  /**
   *  Spins until a message is received, then outputs that message to the internal buffer
   *  and closes the connected socket.
   */ 
  void Accept();

  /**
   *  Returns the message if it has been successfully processed
   */ 
  const char* GetMessage();

  ~TestListener();
  TestListener(const TestListener& other) = delete;
  TestListener& operator=(const TestListener& rhs) = delete;

 private:
  char* buffer_;  // internal buffer for storing the message :)
  int host_fd_;   // fd for the local socket
  bool is_listening_;  // whether we've got set up properly
  const int buffer_size_;
};

#endif  // TEST_LISTENER_H_