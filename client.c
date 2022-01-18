#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
  struct addrinfo hints, *res;
  
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int status;
  if ((status = getaddrinfo(NULL, "2000", &hints, &res)) != 0) {
    fprintf(stderr, "Error in getaddrinfo: %s\n", gai_strerror(status));
    return 1;
  }
  
  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  connect(sockfd, res->ai_addr, res->ai_addrlen);
  
  char response[100];
  int bytes = recv(sockfd, response, sizeof response, 0);

  if (bytes == -1) {
    return 1;
  }
  //response[bytes] = '\0'; 

  printf("Response from server: %s\n", response);  
}
