%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "y.tab.h"

int yylex();
void yyerror(char*);

extern FILE* checkfile;
	
%}

%%

[ \t]+
[\n]	{fprintf(checkfile, "\n");  ++yylineno;}

"const"	{fprintf(checkfile, "CONST "); yylval.name = strdup(yytext); return CONST;}
"int"	{fprintf(checkfile, "INT "); yylval.name = strdup(yytext); return INT;}
"void"	{fprintf(checkfile, "VOID "); yylval.name = strdup(yytext); return VOID;}
"if"	{fprintf(checkfile, "IF "); yylval.name = strdup(yytext); return IF;}
"else"	{fprintf(checkfile, "ELSE "); yylval.name = strdup(yytext); return ELSE;}
"while"	{fprintf(checkfile, "WHILE "); yylval.name = strdup(yytext); return WHILE;}
"break"	{fprintf(checkfile, "BREAK "); yylval.name = strdup(yytext); return BREAK;}
"continue"	{fprintf(checkfile, "CONTINUE "); yylval.name = strdup(yytext); return CONTINUE;}
"return"	{fprintf(checkfile, "RETURN "); yylval.name = strdup(yytext); return RETURN;}

[_a-zA-Z]([_a-zA-Z0-9])*	{fprintf(checkfile, "IDENT(%s) ", yytext); yylval.name = strdup(yytext); return IDENT;}
[0-9]+	{fprintf(checkfile, "INTCONST(%d) ", atoi(yytext)); yylval.value = atoi(yytext); return INTCONST;}

"!"	{fprintf(checkfile, "OP_1 "); yylval.name = strdup(yytext); return OP_1;}
"*"|"%"|"/"	{fprintf(checkfile, "OP_2 "); yylval.name = strdup(yytext); return OP_2;}
"+"|"-"	{fprintf(checkfile, "OP_3 "); yylval.name = strdup(yytext); return OP_3;}
"<"|">"|"<="|">="	{fprintf(checkfile, "OP_4 "); yylval.name = strdup(yytext); return OP_4;}
"=="|"!="	{fprintf(checkfile, "OP_5 "); yylval.name = strdup(yytext); return OP_5;}
"||"	{fprintf(checkfile, "OP_6 "); yylval.name = strdup(yytext); return OP_6;}
"&&"	{fprintf(checkfile, "OP_7 "); yylval.name = strdup(yytext); return OP_7;}
"="	{fprintf(checkfile, "OP_8 "); yylval.name = strdup(yytext); return OP_8;}

"("	{fprintf(checkfile, "PRN_L "); yylval.name = strdup(yytext); return PRN_L;}
")"	{fprintf(checkfile, "PRN_R "); yylval.name = strdup(yytext); return PRN_R;}
"["	{fprintf(checkfile, "ARR_L "); yylval.name = strdup(yytext); return ARR_L;}
"]"	{fprintf(checkfile, "ARR_R "); yylval.name = strdup(yytext); return ARR_R;}
"{"	{fprintf(checkfile, "BRA_L "); yylval.name = strdup(yytext); return BRA_L;}
"}"	{fprintf(checkfile, "BRA_R "); yylval.name = strdup(yytext); return BRA_R;}
";"	{fprintf(checkfile, "EOL"); yylval.name = strdup(yytext); return EOL;}
","	{fprintf(checkfile, "COMMA "); yylval.name = strdup(yytext); return COMMA;}

"//"[^\n]*

.	yyerror("Unknown character");

%%

int yywrap() {
	return 1;
}
