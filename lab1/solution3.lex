%{ 
	
%} 

%s STATE

%% 
<INITIAL>[a]        BEGIN INITIAL;
<INITIAL>[b]        BEGIN STATE; 
<INITIAL>[^ab]    {printf("\nrejected\n"); return 0;} 


<STATE>[a]          BEGIN STATE; 
<STATE>[b]          BEGIN INITIAL;
<STATE>[\n]         {printf("\naccepted\n"); return 0;}
<STATE>[^ab\n]      {printf("\nrejected\n"); return 0;}

<<EOF>>             {if (YY_START == STATE) printf("\naccepted\n"); else printf("\nrejected\n"); return 0;}
%% 

int main() 
{  
	yylex(); 
	return 0; 
} 
