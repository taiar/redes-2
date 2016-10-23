#ifndef __TEMPO_H
#define __TEMPO_H TEMPO

#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include <cctype>
#include <cstdlib>

#include "token.h"

using namespace std;

class Tempo {
public:
  int hours;
  int minutes;
  int seconds;
  int milisseconds;

  Tempo();
  Tempo(int, int, int, int);
  int compare(Tempo);
  Tempo* setFromString(char[]);
  string toString();
  void print();
  bool biggerThan(Tempo*);
  bool smallerOrEqualThan(Tempo*);

private:
  void parseUnit(string);
  bool isNumber(char);
  void setTimeUnit(string, string);
  int toPseudoMs();
};

#endif
