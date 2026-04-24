int
is_alphabet(char c)
{
  if (c > 'z')
    return 0;
  if (c < 'a')
    return 0;
  return 1;
}
