static int
is_gap_(char c)
{
  if (c == ' ' || c == '\n')
    return 1;
  return 0;
}

static int
is_alphabet_(char c)
{
  if (c > 'z')
    return 0;
  if (c < 'a')
    return 0;
  return 1;
}

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

    if (!is_alphabet_(c))
      return 0;
  }

  *cursor = p + 1;
  return s;
}
