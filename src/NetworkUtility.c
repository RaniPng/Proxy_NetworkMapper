#include <NetworkUtility.h>
#include <math.h>

int randomN(int min, int max)
{
    static bool initedTimer = false;
    if (!initedTimer)
    {
        srand((unsigned int)time(NULL));
        initedTimer = true;
    }
    return min + rand() % (max - min + 1);
}

in_addr_t ipInc(in_addr_t ip)
{
    ip = reverseBytes(ip);
    ip++;
    ip = reverseBytes(ip);
    return ip;
}

void ipPrint(in_addr_t ip)
{
    struct in_addr addr;
    addr.s_addr = ip;
    printf("%sIP Address: %s%s\n", BLUE, inet_ntoa(addr), RESET);
}

void binaryPrint(int ip)
{
    printf("%sIP Address binnary: 0b%b%s\n", YELLOW, ip, RESET);
}

void hexaPrint(int ip)
{
    printf("%sIP Address Hexa: 0x%x%s\n", GREEN, ip, RESET);
}

char *ipToStr(in_addr_t ip)
{
    struct in_addr addr;
    addr.s_addr = ip;
    return inet_ntoa(addr);
}

int ipToArry(const char *ip, int16 numbers[4])
{
    // Split the IP string into tokens based on the '.' delimiter
    char ip_copy[16]; // Ensure enough space for the IP string
    strncpy(ip_copy, ip, sizeof(ip_copy));
    ip_copy[sizeof(ip_copy) - 1] = '\0'; // Ensure null termination

    char *token = strtok(ip_copy, ".");
    int count = 0;

    // Process each token
    while (token != NULL && count < 4)
    {
        // Convert token to integer
        int num = atoi(token);

        // Check for valid range
        if (num < 0 || num > 255)
        {
            fprintf(stderr, "Invalid IP address: %s\n", ip);
            return 0; // Return -1 on error
        }

        // Store the number in the array
        numbers[count++] = num;

        // Get the next token
        token = strtok(NULL, ".");
    }

    // Ensure exactly 4 octets were found
    if (count != 4)
    {
        fprintf(stderr, "Invalid IP address: %s\n", ip);
        return 0; // Return -1 on error
    }

    return 1; // Success
}

int64 ipToDecimal(const char *ip)
{
    int64 decimal = 0;
    int octet;

    // Split the IP string into octets
    char *ipCopy = strdup(ip); // Make a mutable copy
    char *token = strtok(ipCopy, ".");

    while (token != NULL)
    {
        octet = atoi(token);                       // Convert the string to an integer
        decimal = (decimal << 8) | (octet & 0xFF); // Shift left and add the octet
        token = strtok(NULL, ".");
    }

    free(ipCopy); // Free the copied string
    return decimal;
}

int64 ipCalcDistance(const char *ip1, const char *ip2)
{
    int64 decimalIp1 = ipToDecimal(ip1);
    int64 decimalIp2 = ipToDecimal(ip2);
    return abs(decimalIp1 - decimalIp2);
}

int64 ipGetRange(const char *ip1, const char *ip2)
{
    return ipCalcDistance(ip1, ip2) + 1;
}
