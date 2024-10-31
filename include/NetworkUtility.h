#pragma once
// std
#include <stdio.h>
#include <stdlib.h>
#include <Fonts.h>
// network
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// other
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// tpyedef of size based vars
typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long int int64;
#define null NULL

// defines ip menipulation via bits
#define incSegment4(x) x + 0x01000000
#define incSegment3(x) x + 0x010000
#define incSegment2(x) x + 0x0100
#define incSegment1(x) x + 0x01

#define segment4 0xff000000
#define segment3 0xff0000
#define segment2 0xff00
#define segment1 0xff

#define endSegment4(x) (x & segment4) == segment4
#define endSegment3(x) (x & segment3) == segment3
#define endSegment2(x) (x & segment2) == segment2
#define endSegment1(x) (x & segment1) == segment1

#define resetSegment4(x) x & (~segment4)
#define resetSegment3(x) x & (~segment3)
#define resetSegment2(x) x & (~segment2)
#define resetSegment1(x) x & (~segment1)

// #define incIP(x) reverseBytes(reverseBytes(x) + 1) why the f its not work
#define reverseBytes(x) ((x & 0xFF000000) >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) | ((x & 0x000000FF) << 24)
// ip
in_addr_t ipInc(in_addr_t ip);
void ipPrint(in_addr_t ip);
void binaryPrint(int ip);
void hexaPrint(int ip);
char *ipToStr(in_addr_t ip);
int ipToArry(const char *ip, int16 numbers[4]);
int64 ipToDecimal(const char *ip);
int64 ipCalcDistance(const char *ip1, const char *ip2);
int64 ipGetRange(const char *ip1, const char *ip2);
// other
int randomN(int min, int max); // might have usage one day :)