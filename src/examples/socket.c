#include <siege/siege.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* trim_tail(char* str)
{
    str[strcspn(str, "\r\n")] = 0;
    return str;
}
int main(void)
{
    char buf[512];
    printf("Host: "); fflush(stdout);
    fgets(buf, sizeof(buf) - 1, stdin);
    buf[sizeof(buf)-1] = 0;
    trim_tail(buf);

    unsigned short port;
    printf("Port: "); fflush(stdout);
    fscanf(stdin, "%hu", &port);

    SGAddress* addr;

    printf("Creating and resolving '%s'...\n", buf);
    addr = sgAddressCreateHost(buf, port, 0);
    if(!addr)
    {
        printf("Could not resolve host! Exiting...\n");
        exit(1);
    }

    sgAddressToString(addr, buf, sizeof(buf), SG_TRUE);
    printf("Done! IP: %s\n", buf);

    sgAddressDestroy(addr);
    
    return 0;
}
