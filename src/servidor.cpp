#include "servidor.h"

#define BUFSZ 1024

int main(int argc, char const *argv[]) {
  Servidor *server = new Servidor(atoi(argv[1]));
  server->run();
  exit(EXIT_SUCCESS);
}

Servidor::Servidor(int porta) {
  this->porta = porta;

  this->s = socket(AF_INET, SOCK_STREAM, 0);
  if(s == -1) this->logexit("socket");

  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, "eth1", 4);

  struct in_addr addr;
  if(inet_pton(AF_INET, "127.0.0.1", &addr) < 1) this->logexit("pton");
  struct sockaddr_in dst;

  dst.sin_family = AF_INET;
  dst.sin_port = htons(this->porta);
  dst.sin_addr = addr;

  struct sockaddr *sa_dst = (struct sockaddr *)&dst;

  if(bind(s, sa_dst, sizeof(*sa_dst))) this->logexit("bind");
  if(listen(s, 0)) this->logexit("listen");
}

void Servidor::run() {
  while(1) {
    int read_size;
    struct sockaddr raddr;
    socklen_t rlen = sizeof(struct sockaddr);

    this->r = accept(this->s, &raddr, &rlen);
    struct sockaddr_in *raddrptr = (struct sockaddr_in *) &raddr;
    char line[BUFSZ];

    this->fill((struct sockaddr *)raddrptr, line);

    char line2[BUFSZ];

    while((read_size = recv(this->r , line2 , BUFSZ , 0)) > 0) {
      this->parse(line2);
    }

    close(this->r);
    sleep(1);
  }
}

void Servidor::logexit(const char *str) {
  perror(str);
  exit(EXIT_FAILURE);
}

void Servidor::fill(const struct sockaddr *addr, char *line) {
  int version;
  char str[INET6_ADDRSTRLEN];
  unsigned short port;

  if(addr->sa_family == AF_INET) {
    version = 4;
    struct sockaddr_in *addr4 = (struct sockaddr_in *)addr;
    if(!inet_ntop(addr4->sin_family, &(addr4->sin_addr), str, INET6_ADDRSTRLEN)) logexit("ntop");
    port = ntohs(addr4->sin_port);
  } else {
    version = 6;
    struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)addr;
    if(!inet_ntop(addr6->sin6_family, &(addr6->sin6_addr), str, INET6_ADDRSTRLEN)) logexit("ntop");
    port = ntohs(addr6->sin6_port);
  }

  sprintf(line, "IPv%d %s %hu\n", version, str, port);
}

void Servidor::parse(char msg[]) {
  switch (msg[0]) {
    case 'C':
      this->getPosition(msg);
      break;
    case 'D':
      this->pushTime(msg);
      break;
    case 'O':
      this->dumpTimes();
      break;
    case 'Z':
      this->shutdown();
      break;
  }
}

void Servidor::pushTime(char msg[]) {
  Tempo *t = new Tempo();
  Tempo *temporary = t->setFromString(msg);

  if(tempos.size() == 0) {
    tempos.push_back(t);
  } else {
    unsigned int i = 0;
    while (t != 0) {
      if(i >= tempos.size()) {
        tempos.push_back(t);
        t = 0;
      } else {
        if(tempos.at(i)->biggerThan(t)) {
          temporary = tempos.at(i);
          tempos.at(i) = t;
          t = temporary;
        }
      }
      i += 1;
    }
  }

  this->sendToClient("D");
}

void Servidor::getPosition(char msg[]) {
  std::string mensagem(msg);
  Token *t = new Token(mensagem);
  int position = atoi(t->getNextToken().c_str());
  Tempo *tempo = this->returnThePosition(position);
  if(tempo != 0)
    this->sendToClient(tempo->toString());
  else
    this->sendToClient("C");
}

void Servidor::dumpTimes() {
  for(unsigned int i = 0; i < tempos.size(); i += 1)
    this->sendToClient(tempos.at(i)->toString());
  if(tempos.size() == 0) this->sendToClient("O");
}

void Servidor::shutdown() {
  this->sendToClient("Z");
  close(this->r);
}

void Servidor::sendToClient(string s) {
  send(this->r, (s + "\n").c_str(), (s.size() + 1), 0);
}

Tempo* Servidor::returnThePosition(unsigned int position) {
  return (position > this->tempos.size() || position <= 0) ? 0 : this->tempos[position - 1];
}
