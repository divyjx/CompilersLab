%{
#include <stdio.h>
#include <stdlib.h>
void yyerror (char *s);
int yylex();
%}

%union {int num;}
%start input
%token plus minus multi divi
%token <num> value 
%type <num> expr

%%

input : expr '\n'        {printf("Valid Expression, Value : %d\n", $1); exit(0);}
expr : value {$$ = $1;}
     |  expr minus value {$$ = $1 - $3;}   
     |  expr plus value  {$$ = $1 + $3;}
     |  expr multi value {$$ = $1 * $3;}
     |  expr divi value  {if ($3==0) {printf("Division by zero exception"); exit(1);}  $$ = $1 / $3;}
     ;
%%

int main(void){
    return yyparse();
}

void yyerror (char *s) {printf ("Invalid Expression\n");}