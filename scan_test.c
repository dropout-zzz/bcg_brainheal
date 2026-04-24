#include <stdio.h>

char *
get_raw_keyword(char **);

char *
get_raw_identifier(char **);

static void
test01(void)
{
  char test_code[] = "impl /main/ bringit\nthatsit\n";
  char *p;

  p = test_code;

  printf("first keyword: '%s'\n", get_raw_keyword(&p));
  printf("identifier: '%s'\n", get_raw_identifier(&p));
}

int
main()
{
  test01();
}
