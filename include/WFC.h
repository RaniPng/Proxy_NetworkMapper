#pragma once
#include <RaniUtility.h>

enum eOptions
{
    O0 = 0,
    O1 = 1,
    O2 = 2,
    O3 = 4,
    O4 = 8,
    O5 = 16,
    O6 = 32,
    O7 = 64,
    O8 = 128
};
// 1 = 1
// 2 = 2
// 3 = 4
// 4 = 8
// 5 = 16
// 6 = 32
// 7 = 64
// 8 = 128

#define ALL_OPTIONS 255
#define ADD_OPTION(var, option) var = var | option
#define REMOVE_OPTION(var, option) var = var & (~option)
#define VALUE_REAL_NUMBER(val) ~val
#define BIT_POSITION(value) ((value) == 1 ? 1 : (value) == 2 ? 2 \
                                            : (value) == 4   ? 3 \
                                            : (value) == 8   ? 4 \
                                            : (value) == 16  ? 5 \
                                            : (value) == 32  ? 6 \
                                            : (value) == 64  ? 7 \
                                            : (value) == 128 ? 8 \
                                                             : 0)
#define BIT_POSITION_TO_VALUE(bit) ((bit) == 1 ? O1 : (bit) == 2 ? O2 \
                                                  : (bit) == 3   ? O3 \
                                                  : (bit) == 4   ? O4 \
                                                  : (bit) == 5   ? O5 \
                                                  : (bit) == 6   ? O6 \
                                                  : (bit) == 7   ? O7 \
                                                  : (bit) == 8   ? O8 \
                                                                 : O0)
struct cell_struct
{
    int8 finalValue; // value between 1 - 8 acctual int val
    int8 options;    // represented as binnary each bit defined as an avilable status where 1 is avilable and 0 is not avilable
    int8 optionsLeft;
    int8 x, y;
};
typedef struct cell_struct Cell;

struct graph_struct
{
    Cell ***grid;
    int8 rows, cols;
    int32 haveNotCollapsedYet;
};
typedef struct graph_struct Graph;

void displayGraphValues(Graph *graph);
void freeGraph(Graph *graph);
Graph *createGraph(int8 rows, int8 cols);
Cell *createNewCell(int8 x, int8 y);

void initSodukuGrid(Graph *graph);
void WFC(Graph *graph);
void WFC_Main();