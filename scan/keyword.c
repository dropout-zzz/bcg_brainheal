#include "gap.h"
#include "alphabet.h"

__attribute__((__visibility__("default"))) const char *
get_raw_keyword(char **cursor)
{
  char *s;
  char *p;
  char c;

  for (p = s = *cursor; (c = *p); p++)
  {
    if (is_gap(c))
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
