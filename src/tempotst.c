#include "tempo.h"

#ifndef MAX
#define MAX 100
#endif

int main(int argc, char const *argv[]) {

  char *tempo;

  tempo = "D 1h 20m 10s 20ms";

  Tempo *t = new Tempo();
  t->setFromString(tempo);

  t->print();

  return 0;
}
