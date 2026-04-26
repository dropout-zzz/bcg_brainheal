enum
{
  KEYWORD = 1,
  IDENTIFIER,
};

enum
{
  IMPL,
  BRINGIT,
  THATSIT,
};

enum
{
  NO_ERROR,
  SYNTAX_ERROR,
};

struct keyword
{
  int kind;
};

struct identifier
{
  const char *s;
};

struct token
{
  int kind;
  union
  {
    struct keyword *keyword;
    struct identifier *identifier;
  };
  int pos;
};
