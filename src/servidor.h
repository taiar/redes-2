#ifndef __SERVIDOR_H
#define __SERVIDOR_H SERVIDOR

#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "tempo.h"

struct sockaddr;

void logexit(const char*);
void fill(const struct sockaddr *addr, char *line);

class Servidor {
public:
  Servidor(int);
  void logexit(const char*);
  void fill(const struct sockaddr*, char*);
  void run();
  void pushTime(char[]);
  void getPosition(char[]);
  void dumpTimes();
  void shutdown();
  static int compare(const void*, const void*);
  void parse(char[]);
  void sendToClient(string);

private:
  int porta;
  int s;
  int r;
  std::vector<Tempo*> tempos;
  Tempo* returnThePosition(unsigned int);
};

#endif
