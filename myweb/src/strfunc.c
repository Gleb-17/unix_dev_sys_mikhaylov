#include "strfunc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



char* concat_str(const char* str1, const char* str2)
{
    unsigned long len1 = strlen(str1);
    unsigned long len2 = strlen(str2);

    char* result = malloc(len1 + len2 + 1);

    if(!result)
    {
        perror(result);
        return NULL;
    }

    memcpy(result, str1, len1);
    memcpy(result + len1, str2, len2 + 1);

    return result;
}

char* set_time(struct tm* u)
{
  char s[40];
  char *tmp;
  for (int i = 0; i<40; i++) s[i] = 0;
  strftime(s, 40, "%d.%m.%Y %H:%M:%S, %A", u);
  tmp = (char*)malloc(sizeof(s));
  strcpy(tmp, s);
  return(tmp);
}
