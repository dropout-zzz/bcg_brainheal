#include <stdio.h>
#include "diagnosis.h"

void
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

void
print_full_line_info(struct line_info *li)
{
  printf("At line #%d, char #%d:\n", li->line_no, li->nth);
  puts(li->line);
  printf("%*s^\n", li->nth - 1, "");
}
