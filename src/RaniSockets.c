#include <RaniSockets.h>

ClientsInfo *serverClients;

int rServer()
{
    pthread_t ptidMgmt;
    int sock, newSock;
    int opt = 1;
    char buffer[BUFFER_SIZE] = {0};
    char msg[BUFFER_SIZE] = {0};
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    socklen_t addrlen = sizeof(addr);
    serverClients = malloc(sizeof(ClientsInfo));
    serverClients->clientsNum = 0;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        assert(false);
        return 1;
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        assert(false);
        return 1;
    }

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(sock);
        perror("bind");
        assert(false);
        return 1;
    }
    printf("listening...\n");
    if (listen(sock, 3) < 0)
    {
        close(sock);
        perror("listen");
        assert(false);
        return 1;
    }

    pthread_create(&ptidMgmt, null, (void *)&rServerMgmt, ((void *)&sock));
    printf("%sacceting clients....%s\n", YELLOW, RESET);
    while ((newSock = accept(sock, (struct sockaddr *)&addr, &addrlen)) >= 0)
    {
        printf("\n%srecived client%s\n", GREEN, RESET);
        serverClients->connections = realloc(serverClients->connections, (serverClients->clientsNum + 1) * sizeof(ClientsInfo));
        serverClients->connections[serverClients->clientsNum] = newSock;
        serverClients->clientsNum++;

        pthread_t ptidListen;
        pthread_create(&ptidListen, NULL, (void *)&rListenClient, ((void *)&newSock));
    }
    pthread_join(ptidMgmt, null);
    close(sock);
    free(serverClients->connections);
    free(serverClients);
    return 0;
}

int rClient(const char *ip)
{
    pthread_t ptidListen, ptidSend;
    int sock;
    char buffer[BUFFER_SIZE] = {0};
    char msg[BUFFER_SIZE] = "hello MF";
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(PORT);
    socklen_t addrlen = sizeof(addr);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        assert(false);
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&addr, addrlen) < 0)
    {
        perror("connect");
        assert(false);
        return 1;
    }

    pthread_create(&ptidSend, NULL, (void *)&rSend, ((void *)&sock));
    pthread_create(&ptidListen, NULL, (void *)&rRecv, ((void *)&sock));
    pthread_join(ptidListen, NULL);
    printf("%s[x]Listen thread was closed%s\n", RED, RESET);
    pthread_cancel(ptidSend);
    pthread_join(ptidSend, NULL);
    printf("%s[x]Send thread was closed%s\n", RED, RESET);
    close(sock);
    return 0;
}

void rServerMgmt(void *s)
{
    // char command[21] = {0};
    char inputHeader[17];
    sprintf(inputHeader, "%scommand> ", CYAN);
    char *command = null;
    int sock = *(int *)s;
    while (true)
    {
        // scanf("%20s", command);
        getInputHeader(&command, inputHeader);
        if (strcmp(command, "close") == 0)
        {
            printf("%ssocket asked for shuting%s\n", RED, RESET);
            close(sock);
            break;
        }
        free(command);
        // memset(command, 0, 21);
    }
    pthread_exit(NULL);
}

void rNotifyClients(void *msg)
{
    int i = serverClients->clientsNum - 1;
    int *socks = serverClients->connections;
    for (; i >= 0; i--)
    {
        pthread_t ptidSend;
        ClientMsgInfo *inf = malloc(sizeof(ClientMsgInfo));
        inf->msg = calloc(strlen((char *)msg), sizeof(char));
        strcpy(inf->msg, (char *)msg);
        inf->sock = socks[i];
        pthread_create(&ptidSend, NULL, (void *)&rNotifyClient, ((void *)inf));
    }
    free((char *)msg);
    pthread_exit(NULL);
}

void rNotifyClient(void *msgInfo)
{
    ClientMsgInfo *inf = (ClientMsgInfo *)msgInfo;
    send(inf->sock, inf->msg, strlen(inf->msg), 0);
    free(inf->msg);
    free(inf);
    pthread_exit(NULL);
}

void rListenClient(void *clientSock)
{
    int sock = *(int *)clientSock;
    char buffer[BUFFER_SIZE];
    printf("%s[-]Triggered client listner Thread - %d%s\n", YELLOW, sock, RESET);
    while (read(sock, buffer, BUFFER_SIZE - 1) > 0)
    {
        // printf("%sSocket %d> %s%s\n", BLUE, sock, buffer, RESET);
        char *msg = calloc(BUFFER_SIZE + 100, sizeof(char));            // just a magic number that make sure the capasity of the special chars wont overflow the msg buffer
        sprintf(msg, "%sSocket %d> %s%s\n", BLUE, sock, buffer, RESET); // needs to be copy to dynamic allocated mem and make sure the thread frees it afterwards
        printf("%s", msg);
        pthread_t sending;
        pthread_create(&sending, null, (void *)&rNotifyClients, ((void *)msg));
        memset(buffer, 0, BUFFER_SIZE);
        // memset(msg, 0, BUFFER_SIZE);
    }
    printf("%s[x]Client Closed - %d%s\n", MAGENTA, sock, RESET);
    close(sock);
    pthread_exit(NULL);
}

void rSend(void *sendStruct)
{
    int sock = *((int *)sendStruct);
    // char msg[BUFFER_SIZE];
    char *msg = null;
    printf("%s[-]Triggered rSend Thread - %d%s\n", YELLOW, sock, RESET);
    while (true)
    {
        // memset(msg, 0, BUFFER_SIZE);
        // scanf("%1024s", msg);
        getInput(&msg);
        if (send(sock, msg, strlen(msg), 0) == -1)
        {
            printf("closeingggggggggggggggggggggg!\n");
            // pthread_cancel(pthread_self());
            break;
        }
        free(msg);
    }
    // later potential usage
    /*
   printf("\033[s\r");
   scanf("%1024s", msg);
   printf("\033[u\r");
   printf("\033[1K\r");
    */
    close(sock);
    pthread_exit(NULL);
}

void rRecv(void *recvStruct)
{
    char buffer[BUFFER_SIZE];
    int sock = (*(int *)recvStruct);
    printf("%s[-]Triggered rRecv Thread - %d%s\n", YELLOW, sock, RESET);
    while (read(sock, buffer, BUFFER_SIZE - 1) > 0)
    {
        if (strcmp(buffer, "close") == 0) // just for testing not shouldnt really apply for "last build"
        {
            printf("closeingggggggggggggggggggggg!\n");
            pthread_cancel(pthread_self());
        }
        printf("%s", buffer);
        memset(buffer, 0, BUFFER_SIZE);
    }
    printf("Client Closed - %d\n", sock);
    close(sock);
    pthread_exit(NULL);
}

int smain(int argc, const char **argv)
{
    char input = 'r';
    if (argc < 2)
    {
        printf("s is 4 server c is 4 client>");
        scanf("%c", &input);
    }
    else
        input = *argv[2];
    if (input == 's')
        rServer();
    elif (input == 'c')
    {
        if (argc > 1)
            rClient(argv[1]);
        else
        {
            // perror("yo man you did not include ip - default ip usage");
            printf("enter an ip to connect> ");
            char ip[17];
            scanf("%15s", ip);
            rClient(ip);
        }
    }
    printf("%s\nCompiled:)\n%s", GREEN, RESET);
    return 0;
}