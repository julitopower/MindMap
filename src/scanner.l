/* This gets copied verbatim in the generated file */
/*
LEVEL: "*"+
NAME: String - {"="}
ARROW: "->"
NL: "\n"

Mindmap: Version (Comment | Node)+
Comment: # String NL
Version: NUMBER.NUMBER NL
Node: LEVEL NAME [AttributesList] [ARROW AttributesList] NL
AttributesList: "[" (EdgeAttributes | NodeAttributes) "]"
EdgeAttributes: EdgeAttribute {"," EdgeAttribute)}
NodeAttributes: NodeAttribute {"," NodeAttribute}
EdgeAttribute: EdgeAttributeName "=" Value 
NodeAttribute: ModeAttributeName "=" Value
EdgeAttributeName: "line" | "color" | "width" | "arrow"
NodeAttributeName: "line" | "color"  
 */
%{
// Needed because we are generating C++  
extern "C" int yylex();
%}

STAR     \*
LETTER   [a-zA-Z]
ID       {LETTER}({LETTER}|[0-9])*
ARROW    ->
NL       \n
KW       line|color|width|arrow

/* Rules go here */
%%
{KW}    { printf("KW(%s)", yytext); }
{STAR}+ { printf("L(%s)", yytext); }
{ID}    { printf("Name(%s)", yytext); }
{ARROW} { printf("ARR"); }
\n    { printf("NL\n"); }
\[       { printf("L_SBR[ "); }
\]       { printf(" ]R_SBR"); }
%%

int main(int argc, char *argv[])
{
  ++argv, --argc; /* Do not consider the program name */
  yyin = fopen(argv[0], "r");
  yylex();
  return 0;
}
