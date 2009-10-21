#include <stdio.h>
#include <ctype.h>


int main(){
   int c;
   while ((c = getchar()) != EOF){ if (isprint(c) || isspace(c)) printf("%c", (char)c); }
   return 0;
}
