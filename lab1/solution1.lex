%{
#include <stdio.h>

int words = 0;
int special_chars = 0;
int lines = 0;
int spaces = 0;
int tabs = 0;
%}

%%
[\n]     { lines++;}
[ ]  { spaces++; }
[\t]     { tabs++; }
[a-zA-Z0-9]+ { words++; }
[^a-zA-Z0-9] { special_chars++; }
<<EOF>> {if (spaces || tabs || special_chars || words)lines++; return 0;}
%%

int main() {
    yylex();
    printf("\nCounts\n");
    printf("Word count: %d\n", words);
    printf("Special character count: %d\n", special_chars);
    printf("Line count: %d\n", lines);
    printf("Space count: %d\n", spaces);
    printf("Tab count: %d\n", tabs);
    return 0;
}
