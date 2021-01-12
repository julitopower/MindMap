/* This gets copied verbatim in the generated file */
/*

QSTR     \"(\\.|[^\\"])*\"
LEVEL     \*+
LETTER   [a-zA-Z]
ID       {LETTER}({LETTER}|[0-9])*
ARROW    ->
NL       \n
EQ       \=
INT      [1-9][0-9]*
REAL     ([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)


Mindmap: Version (Comment | Node)+
Comment: # String NL
Version: INT.INT NL
Node: LEVEL (NAME|QSTR) [AttributesList] [ARROW AttributesList] NL
AttributesList: "[" (EdgeAttributes | NodeAttributes) "]"
EdgeAttributes: EdgeAttribute {"," EdgeAttribute)}
NodeAttributes: NodeAttribute {"," NodeAttribute}
EdgeAttribute: EdgeAttributeName "=" Value
NodeAttribute: ModeAttributeName "=" Value
EdgeAttributeName: "line" | "color" | "width" | "arrow"
NodeAttributeName: "line" | "color"
Value: NAME | (INT|REAL)[unit]
Unit: "px" | "pc"
 */
%{
// Needed because we are generating C++
extern "C" int yylex();
%}

/* Quoted string */
QSTR     \"(\\.|[^\\"])*\"
/* Multiple '*' define a level */
LEVEL     \*+
LETTER   [a-zA-Z]
ID       {LETTER}({LETTER}|[0-9])*
ARROW    ->
NL       \n
EQ       \=
INT      [1-9][0-9]*
REAL     ([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)


/* Rules go here */
%%
{QSTR} { printf("QUOTED_STR(%s) ", yytext); }
 /* The number of starts is the level depth */
{LEVEL} {
  printf("L(%d) ", strlen(yytext));
}
{ID}    { printf("Name(%s) ", yytext); }
{ARROW} { printf("ARR "); }
{INT} { printf("INT(%s) ", yytext); }
{REAL} { printf("REAL(%s) ", yytext); }
\n    { printf("NL\n"); }
\[       { printf("[ "); }
\]       { printf(" ]"); }
= { printf("EQ "); }
, { printf("COMMA "); }
. {}
%%

int main(int argc, char *argv[])
{
  ++argv, --argc; /* Do not consider the program name */
  yyin = fopen(argv[0], "r");
  yylex();
  return 0;
}
