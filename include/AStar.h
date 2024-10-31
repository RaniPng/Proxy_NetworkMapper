#pragma once
#include <RaniUtility.h>

enum NodeType
{
    OPEN = 0,
    CLOSE = 1
};
typedef enum NodeType NodeType;

struct Node
{
    NodeType status;
    int x, y;
};
typedef struct Node Node;

typedef struct
{
    Node **grid;
    int rows;
    int cols;
} Graph;

struct AStarNode
{
    Node *node;
    bool visited;
    int h; // distance from target
    int g; // enter cost (how many steps it took to reach, but also can be the price itself)
    // int f;  // g + h
    struct AStarNode *prv; // the connected node that was part of the path (bad explain lol)
};
typedef struct AStarNode AStarNode;

Graph *createAstarGraph(int rows, int cols);
void printNode(Node *ptr);
int getF(AStarNode *node);
void AStar(Graph *graph, int srcRow, int srcCol, int targetRow, int TargetCol);
