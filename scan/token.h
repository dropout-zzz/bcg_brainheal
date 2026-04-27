#ifndef TOKEN_H
#define TOKEN_H

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
  int len;
};

int
get_tokens(char **, struct token *, int);

const char *
pretty_tokenizer_err(int);

void
del_tokens(struct token *, int);

#endif
