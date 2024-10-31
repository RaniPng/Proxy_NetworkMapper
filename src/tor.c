#include <tor.h>

Req *createRequest(in_addr_t ip, int16 port)
{
    Req *req = null;
    req = malloc(reqsize);
    if (req)
    {
        req->vn = 4;
        req->cd = 1;
        req->dstport = port;
        req->dstip = ip;
        strcpy(req->userid, USERID);
    }
    else
        perror("request allocation");

    return req;
}

int connectProxy(int *sockfd)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PROXYPORT);
    addr.sin_addr.s_addr = inet_addr(PROXYIP);
    int opt = 1;
    struct timeval timeout;
    timeout.tv_sec = 6;  // seconds
    timeout.tv_usec = 0; // microseconds

    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("open proxy socket");
        return 0;
    }
    if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0
    || setsockopt(*sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) < 0 
    || setsockopt(*sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout)) < 0)
    {
        perror("setting socket options config");
        close(*sockfd);
        return 0;
    }
    if ((connect(*sockfd, ((struct sockaddr *)&addr), sizeof(addr))) < 0)
    {
        perror("connect to proxy");
        close(*sockfd);
        return 0;
    }
    printf("%s[v]connected to proxy%s\n", BLUE, RESET);

    return 1;
}

int reqesutProxy(int *sockfd, in_addr_t ip, int16 port)
{
    Req *req;
    Res res[ressize];
    int8 ret = 0;
    if (sockfd)
        if (req = createRequest(ip, port))
        {
            if (write(*sockfd, req, reqsize) == -1)
            {
                perror("sending proxy request");
                return ret;
            }
            printf("%s[-]proxy request was sent\n%s", CYAN, RESET);
            memset(res, 0, ressize);
            if (read(*sockfd, res, ressize) < 1)
            {
                perror("reading response");
                return ret;
            }
            printf("%s[-]proxy responsed\n%s", BLUE, RESET);
            switch (res->cd)
            {
            case 90:
                printf("%s[v]proxy request granted %s[IP - %s]%s\n", GREEN, BACKGROUND_LIGHT_BLUE, ipToStr(ip), RESET);
                ret = 1;
                break;
            case 91:
                printf("%s[x]request rejected or failed %s[IP - %s]%s\n", RED, BACKGROUND_LIGHT_BLUE, ipToStr(ip), RESET);
                break;
            case 92:
                printf("%s[x]request rejected becasue SOCKS server cannot connect to identd on the client %s[IP - %s]%s\n", RED, BACKGROUND_LIGHT_BLUE, ipToStr(ip), RESET);
                break;
            case 93:
                printf("%s[x]request rejected because the client program and identd report different user-ids %s[IP - %s]%s\n", RED, BACKGROUND_LIGHT_BLUE, ipToStr(ip), RESET);
                break;

            default:
                printf("%s[x]WTF happend?? lol - %d %s[IP - %s]%s\n", RED, res->cd, BACKGROUND_LIGHT_BLUE, ipToStr(ip), RESET);
                break;
            }
            free(req);
        }

    return ret;
}
