%{ 
    
%} 

%s B BA BAB BABA

%% 
<INITIAL>[b]    BEGIN B;
<INITIAL>[a]    BEGIN INITIAL;
<INITIAL>[^ab]   {printf("\nrejected\n"); return 0;} 

<B>[a]      BEGIN BA; 
<B>[b]      BEGIN B;
<B>[^ab]    {printf("\nrejected\n"); return 0;}

<BA>[a]      BEGIN INITIAL;
<BA>[b]      BEGIN BAB;
<BA>[^ab]  {printf("\nrejected\n"); return 0;}

<BAB>[a]      BEGIN BABA;
<BAB>[b]      BEGIN B;
<BAB>[^ab]  {printf("\nrejected\n"); return 0;}

<BABA>[a]      BEGIN INITIAL;
<BABA>[b]      BEGIN BAB;
<BABA>[\n]      {printf("\naccepted\n"); return 0;}  
<BABA>[^ab\n]  {printf("\nrejected\n"); return 0;}


<<EOF>>     {if (YY_START == BABA) printf("\naccepted\n"); else printf("\nrejected\n"); return 0;}
%% 

int main() 
{  
	yylex(); 
	return 0; 
} 
