#include "servidor.h"

#define BUFSZ 1024

int main(int argc, char const *argv[]) {
  Servidor *server = new Servidor(atoi(argv[1]));
  server->run();
  exit(EXIT_SUCCESS);
}

Servidor::Servidor(int porta) {
  this->porta = porta;

  this->s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(this->s == -1) this->logexit("socket");

  this->slen = sizeof(this->si_other);

  memset((char *) &this->si_me, 0, sizeof(this->si_me));

  this->si_me.sin_family = AF_INET;
  this->si_me.sin_port = htons(this->porta);
  this->si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(this->s ,(struct sockaddr*) &this->si_me, sizeof(this->si_me)) == -1) this->logexit("bind");
}

void Servidor::run() {
  while(1) {
    fflush(stdout);
    int read_size;
    char line[BUFSZ];

    while((read_size = recvfrom(this->s, line, BUFSZ, 0, (struct sockaddr *) &this->si_other, &this->slen)) > 0) {
      this->parse(line);
    }

    close(this->s);
    sleep(1);
  }
}

void Servidor::logexit(const char *str) {
  perror(str);
  exit(EXIT_FAILURE);
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
  close(this->s);
}

void Servidor::sendToClient(string s) {
  sendto(this->s, (s + "\n").c_str(), (s.size() + 1), 0, (struct sockaddr*) &this->si_other, this->slen);
}

Tempo* Servidor::returnThePosition(unsigned int position) {
  return (position > this->tempos.size() || position <= 0) ? 0 : this->tempos[position - 1];
}
