#include <AStar.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <Stack.h>

void printStack(Stack *s)
{
    printf("Size: %d\n", s->size);
    Node *ptr = stackTop(s);
    printNode(ptr);
}

void printNode(Node *ptr)
{
    if (ptr)
    {
        printf("Top:\n");
        printf("%d/", ptr->x);
        printf("%d = ", ptr->y);
        printf("%d", ptr->status);
        printf("\n");
    }
}

Graph *createAstarGraph(int rows, int cols)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (!graph)
        return NULL;

    graph->cols = cols;
    graph->rows = rows;
    graph->grid = (Node **)calloc(rows, sizeof(Node *));

    if (!graph->grid)
        return NULL;

    for (size_t i = 0; i < rows; i++)
    {
        graph->grid[i] = (Node *)calloc(cols, sizeof(Node));
        if (!graph->grid[i])
            return NULL;
    }
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
        {
            graph->grid[i][j].status = OPEN;
            graph->grid[i][j].x = i;
            graph->grid[i][j].y = j;
        }

    return graph;
}

int getF(AStarNode *node)
{
    return node->g + node->h;
}

static AStarNode ***createAStarGraph(Graph *graph, int targetRow, int TargetCol)
{
    AStarNode ***aStarGraph = (AStarNode ***)calloc(graph->rows, sizeof(AStarNode **));
    if (!aStarGraph)
        return NULL;
    for (size_t i = 0; i < graph->rows; i++)
    {
        aStarGraph[i] = (AStarNode **)calloc(graph->cols, sizeof(AStarNode *));
        if (!aStarGraph[i])
            return NULL;
        for (size_t j = 0; j < graph->cols; j++)
        {
            aStarGraph[i][j] = (AStarNode *)malloc(sizeof(AStarNode));
            aStarGraph[i][j]->node = &(graph->grid[i][j]);
            aStarGraph[i][j]->g = 0;
            aStarGraph[i][j]->visited = true;
            aStarGraph[i][j]->prv = NULL;
            if (aStarGraph[i][j]->node->status == OPEN)
            {
                int distance = abs(i - targetRow) + abs(j - TargetCol);
                aStarGraph[i][j]->h = distance;
                aStarGraph[i][j]->visited = false;
                // printf("%ld/%ld - ", i, j);
                // printf("%d\n", distance);
            }
        }
    }

    return aStarGraph;
}

static void clearAStarGraph(AStarNode ***aSGraph, Graph *graph)
{
    for (size_t i = 0; i < graph->rows; i++)
    {
        for (size_t j = 0; j < graph->cols; j++)
            free(aSGraph[i][j]);
        free(aSGraph[i]);
    }
    free(aSGraph);
}

static void getNigbs(Graph *graph, AStarNode ***aSGraph, int row, int col, AStarNode **nibArr)
{
    static int dRow[] = {-1, 1, 0, 0};
    static int dCol[] = {0, 0, -1, 1};
    for (int i = 0; i < 4; i++)
    {
        int newRow = row + dRow[i];
        int newCol = col + dCol[i];

        // Check if newRow and newCol are within bounds
        if (newRow >= 0 && newRow < graph->rows && newCol >= 0 && newCol < graph->cols)
        {
            nibArr[i] = aSGraph[newRow][newCol];
        }
        else
        {
            nibArr[i] = NULL;
        }
    }
}

void AStar(Graph *graph, int srcRow, int srcCol, int targetRow, int TargetCol)
{
    typedef struct AStarNodeArr
    {
        AStarNode *node;
        struct AStarNodeArr *next;
    } AStarNodeArr;

    AStarNode *nibArr[4] = {NULL};
    AStarNode ***aSGraph = createAStarGraph(graph, targetRow, TargetCol);
    AStarNodeArr *openSet = (struct AStarNodeArr *)malloc(sizeof(struct AStarNodeArr));

    openSet->node = aSGraph[srcRow][srcCol];
    openSet->node->visited = true;
    openSet->next = NULL;

    int size = 1;
    while (size > 0)
    {
        AStarNodeArr *prev = openSet; // is going to point at the last

        AStarNodeArr *best = prev;
        AStarNodeArr *bestIndexPrev = NULL;
        int bestF = getF(best->node);

        while (prev->next != NULL)
        {
            AStarNodeArr *cur = prev->next;
            if (cur->node == best->node)
                continue;

            int curF = getF(cur->node);
            if (curF < bestF || curF == bestF && cur->node->g > best->node->g)
            {
                bestF = curF;
                bestIndexPrev = prev;
                best = cur;
            }
            prev = cur;
            cur = cur->next;
        }

        // find its nibs
        getNigbs(graph, aSGraph, best->node->node->x, best->node->node->y, nibArr);

        for (size_t i = 0; i < 4; i++)
        {
            if (!nibArr[i] || nibArr[i]->visited || nibArr[i]->node->status == CLOSE)
                continue;

            // visited
            nibArr[i]->visited = true;
            nibArr[i]->prv = best->node;

            // check if reach end
            if (nibArr[i]->node->x == targetRow && nibArr[i]->node->y == TargetCol)
            {
                printf("found it bitchhhhhh\n");
                // print path
                AStarNode *path = nibArr[i];
                printf("the list");
                while (path != NULL)
                {
                    printf("-> (%d,%d) ", path->node->x, path->node->y);
                    path = path->prv;
                }
                // clear all list
                while (openSet)
                {
                    AStarNodeArr *ptr = openSet->next;
                    free(openSet);
                    openSet = ptr;
                }
                clearAStarGraph(aSGraph, graph);
                printf("\n");
                return;
            }

            // else add to list
            prev->next = (struct AStarNodeArr *)malloc(sizeof(struct AStarNodeArr)); // prev points the last index
            prev->next->node = nibArr[i];
            prev = prev->next;
            prev->next = NULL;
            size++;
        }

        // remove the best from arr
        if (bestIndexPrev == NULL) // the first node choosed
        {
            openSet = openSet->next;
            free(best);
        }
        else // cought something in the middle
        {
            bestIndexPrev->next = best->next;
            free(best);
        }

        size--;
    }
    printf("not found.");
    clearAStarGraph(aSGraph, graph);
    // check if any malocs are needed (GPT)
}
