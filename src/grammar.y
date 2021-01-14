%{
#include <stdio.h>
#include <string.h>

#include "mindmap/cmm.h"
int yylex(void);
void yyerror(const char*);
static char* empty_str="";

extern MM_HDL mm;

%}

%union value {
  int i;
  double d;
  char* str;
}

%token <i>LEVEL
%token <str>NAME
%token ARROW
%token <d>REAL
%token <i>INT
%token <str>QSTR

%type <str>content
%type <str>attributes

%%
mindmap: nodes { mm_print(mm); mm_delete(mm); }
nodes: nodes node
       | node
node: LEVEL content attributes '\n' {
  mm_add_node(mm, $1, $2);
}
content: NAME { $$ = strdup($1); }
| QSTR { $$ = strdup($1); }
attributes: %empty {$$ = empty_str;}
            | attributes_list {$$ = empty_str;}
            | attributes_list ARROW attributes_list {$$ = empty_str;}
            | ARROW attributes_list {$$ = empty_str;}
attributes_list: '[' attributes_l ']' {}
attributes_l: attributes_l ',' attribute
           | attribute
attribute: NAME '=' value
value: NAME | INT unit | REAL unit
unit: %empty | NAME
%%

MM_HDL mm;
int
main (void)
{
  mm = mm_new();
  printf("Starting parser\n");
  return yyparse ();
}

/* Called by yyparse on error. */
void
yyerror (char const *s)
{
  fprintf (stderr, "%s\n", s);
}
