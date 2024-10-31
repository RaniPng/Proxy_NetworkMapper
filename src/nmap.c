#include "nmap.h"
#include <HashTable.h>

// "private" undelcare header functions
// nmap
static in_addr_t startRange = 0;
static in_addr_t endRange = 0;
static in_addr_t currentIp = 0;
static inline void initIpRange(const char *start, const char *last)
{
    startRange = inet_addr(start);
    endRange = inet_addr(last);
    currentIp = startRange;
}

static inline in_addr_t setNextIP()
{
    if (!currentIp)
        currentIp = startRange; // start
    else if (currentIp == endRange)
        currentIp = 0; // end
    else
        currentIp = ipInc(currentIp); // inc by 1
}

// threads
pthread_mutex_t ptAddToTable;
HashTable *mapTable = null;
static inline void addMapResult(in_addr_t ip, bool res)
{
    pthread_mutex_lock(&ptAddToTable); // lock for thread safe

    hashTableInsert(mapTable, ipToStr(ip), (void *)res);

    pthread_mutex_unlock(&ptAddToTable); // unlock func for next thread
}

struct targetAddrInfo
{
    in_addr_t ip;
    int16 port;
};
static int checkConnectionProxyThread(void *inf)
{
    struct targetAddrInfo *data = (struct targetAddrInfo *)inf;
    in_addr_t ip;

    ip = data->ip;
    int16 port = data->port;

    int sfd = 0;
    int ret = 0;

    if (connectProxy(&sfd))
        ret = reqesutProxy(&sfd, ip, port);
    close(sfd);
    addMapResult(ip, ret);
    pthread_exit(NULL);
    return ret;
}

void printPair(const char *key, void *value)
{
    if ((bool)value)
        printf("%sIP: %s - Connected%s\n", BACKGROUND_LIGHT_GREEN, key, RESET);
    else
        printf("%sIP: %s - Could Not Connect%s\n", BACKGROUND_LIGHT_RED, key, RESET);
}

// declare functions
void mapRange(const char *start, const char *last, const int port)
{
    int16 p = htons(port);
    int64 ipRange = ipGetRange(start, last);
    mapTable = createHashTable(ipRange);
    pthread_t ptid[ipRange];
    initIpRange(start, last);
    pthread_mutex_init(&ptAddToTable, NULL);
    int64 i = 0;
    while (currentIp)
    {
        struct targetAddrInfo inf;
        inf.ip = currentIp;
        inf.port = p;

        pthread_create((ptid + i++), null, (void *)&checkConnectionProxyThread, ((void *)&inf));
        setNextIP();
    }

    // join threads
    for (i = 0; i < ipRange; i++)
        pthread_join(ptid[i], null);

    // aftermeth display and stuff
    printf("==================================\n\n");
    hashTableForEach(mapTable, printPair);

    printf("\n");
    freeHashTable(mapTable);
    pthread_mutex_destroy(&ptAddToTable);
}
