#include "lex.h"

struct lexer* lexer_new(const char* src)
{
  struct lexer* lex = malloc(sizeof* lex);
  if (!lex) die((stderr, "malloc() failed"));
  lex->src = src;
  lex->pos = 0;
  return lex;
}

struct token lexer_next(struct lexer* lex)
{
  struct token tok;
  const char*  src = lex->src;
  double       value;
  char         c;

  tok.value = 0.0;
  value     = 0.0;

  /* skip whitespace */
  while (isspace(src[lex->pos]))
    lex->pos++;

  c = src[lex->pos];

  /* digits. dot is for float nums */
  if (isdigit(c) || (c == '.' && isdigit(src[lex->pos + 1])))
  {
    /* integer part */
    while (isdigit(src[lex->pos]))
    {
      value = value * 10.0 + (src[lex->pos] - '0');
      lex->pos++;
    }

    /* fractional part */
    if (src[lex->pos] == '.')
    {
      double place = 0.1;
      lex->pos++; /* consume the dot */

      while (isdigit(src[lex->pos]))
      {
        value    += (src[lex->pos] - '0') * place;
        place    *= 0.1;
        lex->pos++;
      }
    }

    tok.type  = TOK_NUM;
    tok.value = value;
    return tok;
  }

  lex->pos++; /* consume the character handled below */

  switch (c)
  {
    case '+':  tok.type = TOK_ADD;    return tok;
    case '-':  tok.type = TOK_SUB;    return tok;
    case '*':  tok.type = TOK_MUL;    return tok;
    case '/':  tok.type = TOK_DIV;    return tok;
    case '(':  tok.type = TOK_LBRACK; return tok;
    case ')':  tok.type = TOK_RBRACK; return tok;

    case '\0':
      lex->pos--; /* don't advance past NULL */
      tok.type = TOK_END;
      return tok;

    default:
      die((stderr, "unexpected character '%c' at position %d", c, lex->pos - 1));
  }

  /* to fix -Wreturn-type */
  tok.type = TOK_END;
  return tok;
}
