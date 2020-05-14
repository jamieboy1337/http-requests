# http-requests
no one has ever done a library for http requests. i will be the first

## The Steps
- create an abstraction which handles connections
  - abstract the socket creation/destruction process behind a class
  - create these in some connection manager which spawns a given number of connections
  - reuse those connections as necessary via some sort of queue system
  
- create two implementations
  - a "dummy" implementation which can connect over plain routes (between processes for instance)
  - a "smarter" implementation which maintains HTTP standards
  
- create an abstraction for a particular
- support some basic request types (probably get/post will do for now)

- target HTTP/1.1 for now since i had to do that for a class
  - get stable before moving on

  


