
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "interface.h"

#define BUFSIZE 1024

int sockfd;
int optval = 1;
struct sockaddr_in myaddr; 

int prepif(int myport) {
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)
    return -1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));
  bzero((char *) &myaddr, sizeof(myaddr));
  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr.sin_port = htons((unsigned short)myport);
  if (bind(sockfd, (struct sockaddr *) &myaddr, 
	   sizeof(myaddr)) < 0) 
    return -1;
  fcntl(sockfd, F_SETFL, O_NONBLOCK);

  return 0;
}

int checkif(void){

  struct hostent *theirinfo;
  char buf[BUFSIZE];
  char *theiraddrp;
  int n;
  struct sockaddr_in theiraddr;
  unsigned int theirlen = sizeof(theiraddr);

  fd_set readfds;
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  int rv;

//  while (1) {
  FD_ZERO(&readfds);
  FD_SET(sockfd, &readfds);
  rv = select(sockfd + 1, &readfds, NULL, NULL, &tv); 

  while(rv != 0){ 

    bzero(buf, BUFSIZE);
    n = recvfrom(sockfd, buf, BUFSIZE, 0,
		 (struct sockaddr *) &theiraddr, &theirlen);
    if (n < 0)
      return -1;

    theirinfo = gethostbyaddr((const char *)&theiraddr.sin_addr.s_addr, 
			  sizeof(theiraddr.sin_addr.s_addr), AF_INET);
    if (theirinfo == NULL)
      return -1;
    theiraddrp = inet_ntoa(theiraddr.sin_addr);
    if (theiraddrp == NULL)
      return -1;
    printf("server received datagram from %s (%s)\n", 
	   theirinfo->h_name, theiraddrp);
    printf("server received %lu/%d bytes: %s\n", strlen(buf), n, buf);
    
    n = sendto(sockfd, buf, strlen(buf), 0, 
	       (struct sockaddr *) &theiraddr, theirlen);
    if (n < 0) 
      return -1;
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);
    rv = select(sockfd + 1, &readfds, NULL, NULL, &tv); 
  }
  return 0;
}

