#include <stdio.h>

char *
get_raw_keyword(char **);

static void
test01(void)
{
  char test_code[] = "impl /main/ bringit\nthatsit\n";
  char *p;

  p = test_code;

  printf("first keyword: '%s'\n", get_raw_keyword(&p));
}

int
main()
{
  test01();
}
