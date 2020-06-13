#ifndef URL_PARSER_H_
#define URL_PARSER_H_

#include <string>
#include <unordered_map>

// all i'm worried about for this one, i just want to get it working
enum RequestType {
  HTTP,
  HTTPS
};

/**
 *  Parses a desired URL. Pretty simple i'm thinking
 */ 
class URLParser {
// as needed
 public:
  URLParser(const std::string& url);

  /**
   *  Returns a string containing the URL parameters as a string
   */ 
  const std::string& GetQueryParameters();

  /**
   *  Returns the domain name disjoint from the URL
   */ 
  const std::string& GetDomainName();

  /**
   *  Returns the file path requested by the user
   */ 
  const std::string& GetPath();

  /**
   *  Returns the port we are connecting to.
   */ 
  int GetPort();

  /**
   *  Return request type associated with request
   */ 
  RequestType GetRequestType();

 private:
  RequestType protocol_;
  std::string domain_name_;
  std::string path_;
  std::string query_params_;

  // should always be 80 but you never know you know :)
  int port_;

  /**
   *  Reads along until we encounter an invalid character.
   * 
   *  @param input - the inputted string
   *  @param char_encontered - output parameter for the character we encountered which stopped read
   *  @param parse_colon - parse colons (should only be taken into account following domain name) 
   * 
   *  @return - returns the number of characters checked, including the invalid char.
   */ 
  size_t ReadUntilNextInvalidChar(const char* input, char* char_encountered, bool parse_colon);

};

#endif