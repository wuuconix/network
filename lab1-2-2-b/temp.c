#include <stdio.h>
#include <string.h>
#define BUFSIZE 1024

int main()
{
    char source[] = "123456";
    char dest[10] = {"9"};
    strcpy(dest + 1, source);
    printf("%s", dest);
}