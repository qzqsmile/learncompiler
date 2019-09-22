#include<stdio.h>
#include"c.h"

int Aflag;		/* >= 0 if -A specified */
Interface *IR = NULL;


int main(int argc, char *argv[])
{
    int s = 0;
    input_init(argc, argv);
    // type_init(argc, argv);
    s = gettok();
    printf("%d\n", s);
    printf("%d\n", s);
    return 0;
}