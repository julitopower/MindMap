%{
#include <stdio.h>
#include <string.h>

#include "mindmap/cmm.h"
int yylex(void);
void yyerror(const char*);
static char* empty_str="";

extern MM_HDL mmap;

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
mindmap: nodes { mm_print(mmap); }
nodes: nodes node
       | node
node: LEVEL content attributes '\n' {
  mm_add_node(mmap, $1, $2);
}
/* A node content can mix unquoted single IDs and quoted strings N times */
content: content NAME {
  // Allocate memory to hold both strings and a space
  int l = strlen($1) + strlen($2) + 1;
  // Allocate one extra character for the null terminator
  char* outptr = calloc(l + 1, sizeof(char));
  strcat(outptr, $1);
  strcat(outptr, " ");
  strcat(outptr, $2);
  $$ = outptr;
  // $1 is necessarily manually allocated memory that we have
  // just copied, so we need to relesease it
  free($1);
}
         | NAME { $$ = strdup($1); }
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

MM_HDL mmap;
/* int */
/* main (void) */
/* { */
/*   mmap = mm_new(); */
/*   printf("Starting parser\n"); */
/*   return yyparse (); */
/* } */

/* Called by yyparse on error. */
void
yyerror (char const *s)
{
  fprintf (stderr, "%s\n", s);
}
