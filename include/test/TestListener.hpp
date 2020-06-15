#ifndef TEST_LISTENER_H_
#define TEST_LISTENER_H_

/**
 *  Simple listener class which spins up and down
 */ 
class TestListener {

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
   *  Spins until a message is received.
   */ 
  void Wait();

  /**
   *  Returns the message if it has been successfully processed
   */ 
  const char* GetMessage();

  ~TestListener();
  TestListener(const TestListener& other) = delete;
  TestListener& operator=(const TestListener& rhs) = delete;

 private:
  char* buffer_;  // internal buffer for storing the message :)

};

#endif  // TEST_LISTENER_H_