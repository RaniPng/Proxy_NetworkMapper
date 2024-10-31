#pragma once
#include <NetworkUtility.h>
#include <stdio.h>
#include <stdlib.h>

// proxy info
#define PROXYIP "127.0.0.1"
#define PROXYPORT 9050
#define BUFFER_SIZE 1024
#define USERID "NULL"

// define sizes
#define reqsize sizeof(Req)
#define ressize sizeof(Res)
struct proxy_rquest
{
    int8 vn;
    int8 cd;
    int16 dstport;
    int32 dstip;
    unsigned char userid[5];
    int8 _ : 1; // the null header
    /*
        +----+----+----+----+----+----+----+----+----+----+....+----+
        | VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
        +----+----+----+----+----+----+----+----+----+----+....+----+
          1     1      2              4           variable       1
     */
};
typedef struct proxy_rquest Req;

struct proxy_response
{
    int8 vn;
    int8 cd;
    int16 _;
    int32 __;
    /*
        +----+----+----+----+----+----+----+----+
        | VN | CD | DSTPORT |      DSTIP        |
        +----+----+----+----+----+----+----+----+
           1    1      2              4
     */
};
typedef struct proxy_response Res;

Req *createRequest(in_addr_t ip, int16 port);
int connectProxy(int *sockfd);
int reqesutProxy(int *sockfd, in_addr_t ip, int16 port);
int sendCommandConnectedProxy(int *sockfd);
