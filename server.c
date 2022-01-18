#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT "2000"
#define BACKLOG 20

int main() {
  struct addrinfo hints, *res;
  
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int status;
  if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
    fprintf(stderr, "Error with getaddrinfo: %s\n", gai_strerror(status));
    return 1;
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  int YES = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &YES, sizeof YES)) {
    perror("setsockopt");
    return 1;
  }

  bind(sockfd, res->ai_addr, res->ai_addrlen);
  listen(sockfd, BACKLOG);

  struct sockaddr_in client;
  socklen_t client_addr_len = sizeof client;
  int client_sockfd = accept(sockfd, (struct sockaddr *)&client, &client_addr_len);

  const char *msg = "Hello world, internet!";
  int len = strlen(msg);
  int bytes_sent = send(client_sockfd, msg, len, 0);

  char response[20];
  char client_addrstr[INET_ADDRSTRLEN]; 
  inet_ntop(client.sin_family, &(client.sin_addr), client_addrstr, sizeof client_addrstr);

  if (!recv(sockfd, response, sizeof response, 0)) {
    printf("Connection to client %s closed!\n", client_addrstr); 
  } 
  printf("Response from client: %s\n", response);
  return 0; 
}
