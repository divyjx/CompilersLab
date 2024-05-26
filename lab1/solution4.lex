%{ 
    
%} 

%s STATE FINAL

%% 
<INITIAL>[b]    BEGIN STATE; 
<INITIAL>[^b]   {printf("\nrejected\n"); return 0;} 


<STATE>[a]      BEGIN FINAL; 
<STATE>[b]      BEGIN STATE;
<STATE>[^ab]    {printf("\nrejected\n"); return 0;}

<FINAL>[a]      BEGIN FINAL;
<FINAL>[b]      BEGIN STATE;
<FINAL>[\n]     {printf("\naccepted\n"); return 0;}
<FINAL>[^ab\n]  {printf("\nrejected\n"); return 0;}

<<EOF>>     {if (YY_START == FINAL) printf("\naccepted\n"); else printf("\nrejected\n"); return 0;}
%% 

int main() 
{  
	yylex(); 
	return 0; 
} 
