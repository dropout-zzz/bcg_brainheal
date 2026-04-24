static int
is_gap_(char c)
{
  if (c == ' ' || c == '\n')
    return 1;
  return 0;
}

#include "alphabet.h"

char *
get_raw_keyword(char **cursor)
{
  char *s;
  char *p;
  char c;

  for (p = s = *cursor; (c = *p); p++)
  {
    if (is_gap_(c))
    {
      *p = '\0';
      break;
    }

    if (!is_alphabet(c))
      return 0;
  }

  *cursor = p + 1;
  return s;
}
