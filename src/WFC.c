#include <WFC.h>
#include <Stack.h>

// private funcs
static Cell *entropy(Graph *graph);
static void collapse(Graph *graph, int8 collapseVal, int8 row, int8 col);
static void progregate(Graph *graph, int8 row, int8 col);
static void reProgregate(Graph *graph, int8 row, int8 col);
static void setCellsSudukoValue(Graph *graph);
static Cell **getNibs(Graph *graph, Cell **nibs, int8 row, int8 col);

// header funcs

void displayGraphValues(Graph *graph)
{
    int8 rows = graph->rows;
    int8 cols = graph->cols;
    printf("\n");
    for (size_t i = 0; i < rows; i++)
    {
        printf("{");
        for (size_t j = 0; j < cols; j++)
        {
            // printf("%d - %d", graph->grid[i][j]->finalValue, graph->grid[i][j]->options);
            if (graph->grid[i][j]->finalValue)
                printf(GREEN);
            printf("%d", graph->grid[i][j]->finalValue);
            printf(RESET);
            if (j + 1 < cols)
                printf(", ");
        }
        printf("}");
        printf("\n");
    }
    printf("\n");
}

void freeGraph(Graph *graph)
{
    for (size_t i = 0; i < graph->rows; i++)
    {
        if (!graph->grid[i])
            break;
        for (size_t j = 0; j < graph->cols; j++)
        {
            if (!graph->grid[i][j])
                break;
            free(graph->grid[i][j]);
        }
        free(graph->grid[i]);
    }
    free(graph->grid);
    free(graph);
}

Graph *createGraph(int8 rows, int8 cols)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (!graph)
        return null;
    Cell ***grid = (Cell ***)calloc(rows, sizeof(Cell **)); // create rows
    if (!grid)
    {
        free(graph);
        assert(!graph);
        return null;
    }
    graph->grid = grid;
    graph->cols = cols;
    graph->rows = rows;
    graph->haveNotCollapsedYet = rows * cols;
    bool madeIt = true;
    for (size_t i = 0; i < rows; i++)
    {
        Cell **col = (Cell **)calloc(cols, sizeof(Cell *)); // create cols
        grid[i] = col;
        if (!col)
        {
            madeIt = false;
            break;
        }

        for (size_t j = 0; j < cols; j++) // create cells
        {
            Cell *cell = createNewCell(i, j);
            grid[i][j] = cell;
            if (!madeIt)
            {
                madeIt = false;
                break;
            }
        }

        if (!madeIt) // check if there was a problem creating cell
            break;
    }

    if (!madeIt)
    {
        freeGraph(graph);
        assert(!grid);
    }
    return graph;
}

Cell *createNewCell(int8 x, int8 y)
{
    Cell *cell = (Cell *)malloc(sizeof(Cell));
    if (cell)
    {
        cell->x = x;
        cell->y = y;
        cell->finalValue = O0;
        cell->options = ALL_OPTIONS;
        cell->optionsLeft = 8;
    }
    return cell; // handles null outside func
}

void initSodukuGrid(Graph *graph)
{
    int8 i = O1;
    while (true)
    {
        while (true)
        {
            int8 row = randomNumber(0, graph->rows - 1);
            int8 col = randomNumber(0, graph->cols - 1);
            if (!graph->grid[row][col]->finalValue)
            {
                collapse(graph, i, row, col);
                progregate(graph, row, col); // reflect collapse over grid
                // printf("%d ", i);
                // printf("%d - %d,%d\n", graph->grid[row][col]->finalValue, row, col);
                break;
            }
        }
        if (i == O8)
            break;
        i *= 2; // next on bit
    }
}

static Cell **getNibs(Graph *graph, Cell **nibs, int8 row, int8 col)
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
            nibs[i] = graph->grid[newRow][newCol];
        }
        else
        {
            nibs[i] = NULL;
        }
    }
}

Cell *entropy(Graph *graph)
{
    Cell *best = null;
    for (size_t i = 0; i < graph->rows; i++)
        for (size_t j = 0; j < graph->cols; j++)
            if (!graph->grid[i][j]->finalValue && graph->grid[i][j]->optionsLeft > 0)
            {
                if (!best || graph->grid[i][j]->optionsLeft < best->optionsLeft)
                    best = graph->grid[i][j];
            }
    return best;
}

static void collapse(Graph *graph, int8 collapseVal, int8 row, int8 col)
{
    graph->haveNotCollapsedYet--;
    graph->grid[row][col]->finalValue = collapseVal; // temperary set as bit set location later use macro BIT_POSITION
}

static void progregate(Graph *graph, int8 row, int8 col)
{
    // row and col is reflect a cell that just collapsed
    int toRemove = graph->grid[row][col]->finalValue;
    for (size_t i = 0; i < graph->rows; i++)
    {
        if (graph->grid[i][col]->options & toRemove)
            graph->grid[i][col]->optionsLeft--;
        graph->grid[i][col]->options = REMOVE_OPTION(graph->grid[i][col]->options, toRemove);
    }
    for (size_t j = 0; j < graph->cols; j++)
    {
        if (graph->grid[row][j]->options & toRemove)
            graph->grid[row][j]->optionsLeft--;
        graph->grid[row][j]->options = REMOVE_OPTION(graph->grid[row][j]->options, toRemove);
    }
}

void reProgregate(Graph *graph, int8 row, int8 col)
{
    int toAdd = graph->grid[row][col]->finalValue;
    for (size_t i = 0; i < graph->rows; i++)
    {
        if (!(graph->grid[i][col]->options & toAdd))
            graph->grid[i][col]->optionsLeft++;
        graph->grid[i][col]->options = ADD_OPTION(graph->grid[i][col]->options, toAdd);
    }
    for (size_t j = 0; j < graph->cols; j++)
    {
        if (!(graph->grid[row][j]->options & toAdd))
            graph->grid[row][j]->optionsLeft++;
        graph->grid[row][j]->options = ADD_OPTION(graph->grid[row][j]->options, toAdd);
    }
}

static void iritate(Graph *graph)
{
    Stack *backtrackStack = createStack();
    while (graph->haveNotCollapsedYet)
    {
        Cell *current = entropy(graph);
        if (current)
        {
            int8 bit = BIT_POSITION_TO_VALUE(getFirstSetBit(current->options));
            collapse(graph, bit, current->x, current->y);
            stackPush(backtrackStack, current);
            progregate(graph, current->x, current->y);
            // printf("%d", backtrackStack->size);
            continue;
        }
        if (!backtrackStack->size)
        {
            printf("\n==================================\n%snooooooooooooo soulllllltionnnnnnnnn\n==================================%s", RED, RESET);
            break;
        }
        current = (Cell *)stackPop(backtrackStack);
        graph->haveNotCollapsedYet++;
        int8 badChooise = current->finalValue;
        reProgregate(graph, current->x, current->y);
        current->finalValue = O0;
        REMOVE_OPTION(current->options, badChooise);
        current->optionsLeft--;
    }
    // printf("%d", backtrackStack->size);
    stackFree(backtrackStack);
}

#define ROWS 8
#define COLS 8
#define loops 30

void WFC_Main()
{
    for (size_t i = 0; i < loops; i++)
    {
        Graph *graph = createGraph(ROWS, COLS);
        displayGraphValues(graph);
        initSodukuGrid(graph);
        // displayGraphValues(graph);
        WFC(graph);
        // displayGraphValues(graph);
        freeGraph(graph);
    }
}
void WFC(Graph *graph)
{
    iritate(graph);
    setCellsSudukoValue(graph);
}

static void setCellsSudukoValue(Graph *graph)
{
    for (size_t i = 0; i < graph->rows; i++)
        for (size_t j = 0; j < graph->cols; j++)
            graph->grid[i][j]->finalValue = BIT_POSITION(graph->grid[i][j]->finalValue);
}