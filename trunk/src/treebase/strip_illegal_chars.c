#include <ctype.h>
#include <wchar.h>
#include <stdio.h>
#include <limits.h>
int main( ){
  int ch;
  int prevch;
  while ( (ch = getchar()) != EOF ){
      if (ch > UCHAR_MAX){ printf("&#%d;", ch);  }
      else if ( ch == '&'){ printf("&#38;");  }
      else if ( ch == '\"'){ printf("&#34;");  }
      else if ( ch == '\'' && prevch == '\\'){ printf("&#39;"); }
      else if ( ch == '\\' ){ /* consume */ }
      else if ( ch == '<' ){ printf("&#60;"); }
      else if ( ch == '>'){ printf("&#62;"); }
      else if (  isalnum( ch ) || isspace( ch ) || ispunct( ch ) ){ printf("%c", (char)ch); }
      else { printf(" ");  }
      prevch = ch;
  
  }
  return 0;
}
