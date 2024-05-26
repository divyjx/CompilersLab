%{ 
%} 

%s STATE10 STATE01 STATE11 

%% 
<INITIAL>0          BEGIN STATE10;
<INITIAL>1          BEGIN STATE01; 
<INITIAL>\n         {printf("accepted\n"); return 0;} 
<INITIAL>[^01\n]    {printf("rejected\n"); return 0;} 

<STATE10>0          BEGIN INITIAL; 
<STATE10>1          BEGIN STATE11; 
<STATE10>[^01]    {printf("rejected\n"); return 0;} 

<STATE01>0          BEGIN STATE11; 
<STATE01>1          BEGIN INITIAL; 
<STATE01>[^01]    {printf("rejected\n"); return 0;} 


<STATE11>0          BEGIN STATE01; 
<STATE11>1          BEGIN STATE10; 
<STATE11>\n         {printf("accepted\n"); return 0;} 
<STATE11>[^01\n]    {printf("rejected\n"); return 0;} 

<<EOF>>             {if (YY_START == INITIAL || YY_START == STATE11 ) printf("\naccepted\n"); else printf("\nrejected\n"); return 0;}
%% 

int main() 
{  
	yylex(); 
	return 0; 
} 
