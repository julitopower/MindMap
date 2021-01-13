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
#include "parser.h"
int yylex(void);
char* es = "";
%}

%option noyywrap

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
{QSTR} { yylval.str=yytext; return QSTR; }
 /* The number of starts is the level depth */
{LEVEL} {
  yylval.i = strlen(yytext);
  return LEVEL;
}
{ID}    { yylval.str=yytext; return NAME; }
{ARROW} { return ARROW; }
{INT} { return INT; }
{REAL} { return REAL; }
\n    {  yylval.str=es; return '\n'; }
\[       { return yytext[0]; }
\]       { return yytext[0]; }
= { return yytext[0]; }
, { return yytext[0]; }
. {}
%%

