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
  struct sockaddr_in si_other;
  int s;
  unsigned int slen = sizeof(si_other);

  if((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) logexit("socket");

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(atoi(argv[2]));

  if(inet_aton(argv[1], &si_other.sin_addr) == 0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }

  while(1) {
    char buf[BUFSZ];
    char message[BUFSZ];

    fgets(message, BUFSZ, stdin);

    if (sendto(s, message, BUFSZ, 0 , (struct sockaddr *) &si_other, slen) < 0) return 1;
    if (recvfrom(s, buf, BUFSZ, 0, (struct sockaddr *) &si_other, &slen) < 0) break;

    if(strcmp(buf, "Z\n") == 0)
      break;
    else if(strcmp(buf, "O\n") == 0 || strcmp(buf, "D\n") == 0 || strcmp(buf, "C\n") == 0)
      continue;
    else {
      printf("%s", buf);
      continue;
    }
  }

  close(s);
  exit(EXIT_SUCCESS);
}
