#include <stdio.h>

extern int add(int x, int y);
extern void test();

int main()
{

    int x = add(10, 15);
   
    test();

   printf("\n%d bob\n", x);
   return 0;
}