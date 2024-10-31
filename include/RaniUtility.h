#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <Fonts.h>

#define elif else if
// New Types :)
typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long int int64;
#define null NULL

// Macros

// funcs
int randomNumber(int min, int max);
void swapArrVars(void **arr, int indexA, int indexB);
void arrPrint(void *arr, int size);
int8 getFirstSetBit(int64 n);
void getInput(char **buffer);
void getInputHeader(char **buffer, char *header);