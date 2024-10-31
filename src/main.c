#include <main.h>
#include <RaniUtility.h>

#include <nmap.h>

int main(int argc, char const *argv[])
{
    mapRange("142.250.75.1", "142.250.75.255", 80);
    printf("%s\nCompiled:)\n%s", GREEN, RESET);
    return 0;
}
