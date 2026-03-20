#include "parse.h"

static void expect(struct parser *p, enum token_type expected)
{
  if (p->cur.type != expected)
    die((stderr, "syntax error: unexpected token %d (expected %d)", p->cur.type, expected));

  p->cur = lexer_next(p->lex);
}

static struct ast_node *make_node(enum node_type type, double value,
                                  struct ast_node *left,
                                  struct ast_node *right)
{
  struct ast_node *node = alloc(sizeof *node);

  node->type  = type;
  node->value = value;
  node->left  = left;
  node->right = right;
  return node;
}

static struct ast_node *parse_expr(struct parser *p);

static struct ast_node *parse_factor(struct parser *p)
{
  struct ast_node *node = NULL;

  if (p->cur.type == TOK_NUM)
  {
      node = make_node(ND_NUM, p->cur.value, NULL, NULL);
      p->cur = lexer_next(p->lex);
      return node;
  }

  if (p->cur.type == TOK_LBRACK)
  {
      p->cur = lexer_next(p->lex);           /* consume '(' */
      node = parse_expr(p);                  /* parse inner expression */
      expect(p, TOK_RBRACK);                 /* consume ')' */
      return node;
  }

  die((stderr, "syntax error: expected number or '(', got token type %d",
       p->cur.type));

  /* unreachable; silences -Wreturn-type */
  return NULL;
}

static struct ast_node *parse_term(struct parser *p)
{
  struct ast_node *node = parse_factor(p);
  enum node_type   op;

  while (p->cur.type == TOK_MUL || p->cur.type == TOK_DIV)
  {
    op = (p->cur.type == TOK_MUL) ? ND_MUL : ND_DIV;
    p->cur = lexer_next(p->lex);
    node = make_node(op, 0.0, node, parse_factor(p));
  }

  return node;
}

static struct ast_node *parse_expr(struct parser *p)
{
  struct ast_node *node = parse_term(p);
  enum node_type   op;

  while (p->cur.type == TOK_ADD || p->cur.type == TOK_SUB)
  {
    op = (p->cur.type == TOK_ADD) ? ND_ADD : ND_SUB;
    p->cur = lexer_next(p->lex);
    node = make_node(op, 0.0, node, parse_term(p));
  }

  return node;
}

struct parser *parser_new(struct lexer *lex)
{
  struct parser *p = alloc(sizeof *p);
  p->lex = lex;
  p->cur = lexer_next(lex); /* prime the lookahead */
  return p;
}

struct ast_node *parser_run(struct parser *p)
{
  struct ast_node *root = parse_expr(p);

  if (p->cur.type != TOK_END)
    die((stderr, "unexpected trailing token %d", p->cur.type));

  return root;
}

void ast_free(struct ast_node *node)
{
  if (!node) return;
  ast_free(node->left);
  ast_free(node->right);
  free(node);
}

double eval(const struct ast_node *node)
{
  double rhs;

  switch (node->type)
  {
    case ND_NUM: return node->value;
    case ND_ADD: return eval(node->left) + eval(node->right);
    case ND_SUB: return eval(node->left) - eval(node->right);
    case ND_MUL: return eval(node->left) * eval(node->right);

    case ND_DIV:
      rhs = eval(node->right);
      if (rhs == 0.0) die((stderr, "division by zero"));
      return eval(node->left) / rhs;

    default:
      die((stderr, "eval: unknown node type %d", node->type));
  }

  /* for  -Wreturn-type */
  return 0.0;
}
