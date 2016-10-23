#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSZ 1024

void logexit(const char *str) {
  perror(str);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int s;
  s = socket(AF_INET, SOCK_STREAM, 0);
  if(s == -1) logexit("socket");

  struct in_addr addr;
  if(inet_pton(AF_INET, argv[1], &addr) < 1) logexit("pton");

  struct sockaddr_in dst;

  dst.sin_family = AF_INET;
  dst.sin_port = htons(atoi(argv[2]));
  dst.sin_addr = addr;

  struct sockaddr *sa_dst = (struct sockaddr *)&dst;

  if(connect(s, sa_dst, sizeof(*sa_dst))) logexit("connect");

  char line[BUFSZ] = "";

  while(1) {
    char line2[BUFSZ] = "";
    fgets(line, BUFSZ, stdin);

    if( send(s , line , BUFSZ , 0) < 0) return 1;
    if( recv(s , line2 , BUFSZ , 0) < 0) break;

    if(strcmp(line2, "Z\n") == 0)
      break;
    else if(strcmp(line2, "O\n") == 0 || strcmp(line2, "D\n") == 0 || strcmp(line2, "C\n") == 0)
      continue;
    else {
      printf("%s", line2);
      continue;
    }
  }

  close(s);
  exit(EXIT_SUCCESS);
}
