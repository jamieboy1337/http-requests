# The breakdown
- to start: single HTTP request (dont worry about keeping the connection alive)
- functions
    - set headers
    - clear headers
    - change request type (probably just get and post for now)
    - add body
    - send
- next step
    - persist connections if necessary

- aiohttp:
    - create a client session
    - response has functions

- what do i need
    - decompression (see https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Content-Encoding)
    - why not go for it -- i can start here and make that by itself09
    