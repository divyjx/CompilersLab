%{
#include <stdio.h>
#include "y.tab.h"

%}


%%
[1-9][0-9]*     { yylval.num = atoi(yytext); return value; }
"+"             { return plus; }
"-"             { return minus; }
"*"             { return multi; }
"/"             { return divi; }
"\n"            { return '\n';}
[\t ]           ;
.               {printf("Invalid Token: %s\n", yytext); exit(0);}
%%


int yywrap() {
    return 1;
}
