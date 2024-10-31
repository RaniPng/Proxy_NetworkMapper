#include <RaniUtility.h>

int randomNumber(int min, int max)
{
    static bool initedTimer = false;
    if (!initedTimer)
    {
        srand((unsigned int)time(NULL));
        initedTimer = true;
    }
    return min + rand() % (max - min + 1);
}

void swapArrVars(void **arr, int indexA, int indexB)
{
    void **ptr = arr[indexA];
    arr[indexA] = arr[indexB];
    arr[indexB] = ptr;
}

void arrPrint(void *arr, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%d", ((int *)(arr))[i]);
        if (i + 1 < size)
            printf(", ");
    }
    printf("\n");
}

int8 getFirstSetBit(int64 n)
{
    int8 position = 0;
    while (n >= 1)
    {
        n >>= 1;
        position++;
    }

    return position;
}

void getInput(char **buffer)
{
    size_t size = 0; // Variable to hold the size of the buffer
    ssize_t len;     // Variable to hold the length of input

    // printf("");
    len = getline(buffer, &size, stdin);

    // Check for errors
    if (len == -1)
    {
        perror("Error reading input");
        exit(1);
    }

    // Remove the newline character if it exists
    if (len > 0 && (*buffer)[len - 1] == '\n')
    {
        (*buffer)[len - 1] = '\0';
    }
}

void getInputHeader(char **buffer, char *header)
{
    size_t size = 0; // Variable to hold the size of the buffer
    ssize_t len;     // Variable to hold the length of input

    printf("%s", header); 
    len = getline(buffer, &size, stdin);

    // Check for errors
    if (len == -1)
    {
        perror("Error reading input");
        exit(1);
    }

    // Remove the newline character if it exists
    if (len > 0 && (*buffer)[len - 1] == '\n')
    {
        (*buffer)[len - 1] = '\0';
    }
}
