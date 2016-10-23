#ifndef __TOKEN_H
#define __TOKEN_H TOKEN

#define DELIMITER " "

#include <string>
#include <vector>

class Token {
public:
  Token(std::string);
  std::string getToken();
  std::string getNextToken();
  unsigned int total();
  int getCurrent();

private:
  std::string input;
  std::vector<std::string> tokens;
  int current;

  void tokenize();
};

#endif
