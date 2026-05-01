#include <stdio.h>
#include "diagnosis.h"
#include "scan/token.h"

#define EXPORT __attribute__((__visibility__("default")))

EXPORT void
decode_line_info(char *s, int n, struct line_info *li)
{
  int line_no;
  char *line;
  char *p;
  char c;

  line_no = 1;

  for (line = p = s; n; n--)
  {
    c = *p++;

    switch (c)
    {
      case '\0':
        goto stop_loop;
      case '\n':
        line_no++;
        line = p;
        break;
    }
  }

stop_loop:

  li->nth = p - line + 1 + n;

  for (; (c = *p); p++)
  {
    if (c == '\n')
    {
      *p = '\0';
      break;
    }
  }

  li->line_no = line_no;
  li->line = line;
}

EXPORT void
print_full_line_info(struct line_info *li)
{
  fprintf(stderr, "At line #%d, char #%d:\n", li->line_no, li->nth);
  fprintf(stderr, "%s\n", li->line);
  fprintf(stderr, "%*s^\n", li->nth - 1, "");
}

EXPORT int
get_pos_safe(struct token *tok, struct token *arr)
{
  if (tok->kind != 0 || tok == &arr[0])
  {
    return tok->pos;
  }
  else
  {
    tok--;
    return tok->pos + tok->len;
  }
}
