#include "alphabet.h"

const char *
get_raw_identifier(char **cursor)
{
  char *s;
  char *p;
  char c;

  s = *cursor;

  if (*s != '/')
fail:
    return 0;

  for (p = ++s; (c = *p); p++)
  {
    if (c == '/')
    {
      *p = '\0';
      *cursor = p + 1;
      return s;
    }

    if (!is_alphabet(c))
      break;
  }

  goto fail;
}
