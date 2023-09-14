#ifndef __IMAN_H
#define __IMAN_H

#include "headers.h"

int resolveDNS(const char *hostname, const char *port, struct addrinfo **res);
int createAndConnectSocket(struct addrinfo *res) ;
void sendGETRequest(int sockfd, const char *command_name, const char *hostname);
void readHTTPResponse(int sockfd);
void man( char* command);
#endif

