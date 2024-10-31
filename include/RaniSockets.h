#pragma once
#include <RaniUtility.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h> // For inet_ntop, inet_pton, etc.
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // For inet_ntop, inet_pton, etc.
#endif

#define PORT 12345
#define BUFFER_SIZE 1024
#define XSTR(x) STR(x)
#define STR(x) #x

int rServer();
int gServer();
int rClient(const char *ip);

// Threads structs
struct ClientMsgInfo
{
    int sock;
    char *msg;
};
typedef struct ClientMsgInfo ClientMsgInfo;

struct ClientsInfoStruct
{
    int *connections;
    int clientsNum;
};
typedef struct ClientsInfoStruct ClientsInfo;

struct AcceptStruct
{
    int sock;
    struct sockaddr_in addr;
    socklen_t addrlen;
    int *socketList;
};
typedef struct AcceptStruct AcceptStruct;

// Server
void rServerMgmt(void *s);
void rNotifyClients(void *msg);
void rNotifyClient(void *msgInfo); // for thread
void rListenClient(void *clientSock);

// Client
void rSend(void *sendStruct);
void rRecv(void *recvStruct);
int smain(int,const char**);