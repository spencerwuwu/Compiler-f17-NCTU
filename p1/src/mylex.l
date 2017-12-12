%{
#define LIST               strcat(buf, yytext)
#define token(t)           {LIST; if (Opt_T && !Opt_end) printf("<%s>\n", #t);}
#define tokenChar(t)       {LIST; if (Opt_T && !Opt_end) printf("<%c>\n", (t));}
#define tokenInteger(t, i) {LIST; if (Opt_T && !Opt_end) printf("<%s: %d>\n", #t, (i));}
#define tokenString(t, s)  {LIST; if (Opt_T && !Opt_end) printf("<%s: %s>\n", #t, (s));}
#define MAX_LINE_LENG      256

int Opt_S = 1;
int Opt_T = 1;
int linenum = 1;
char buf[MAX_LINE_LENG];

int Opt_st_flag = 1;
int Opt_end = 0;
%}

digit	[0-9]
space	[ \t]+
letter	[a-zA-Z]
%x COMMENT

%%


[ \t]+$ ;
{space}+ { LIST; }


"&S+" 		{ if (Opt_st_flag) Opt_S = 1; }
"&S-"[\n]? 	{ if (Opt_st_flag) Opt_S = 0; }
"&T+" 		{ if (Opt_st_flag) Opt_T = 1; }
"&T-" 		{ if (Opt_st_flag) Opt_T = 0; }
"//".*		{ 
		LIST;
		if (yytext[2] == '&') {
			Opt_st_flag = 1; yyless(2); }
		}

"/*"			{ LIST; BEGIN(COMMENT); }
<COMMENT>.		{ LIST; }
<COMMENT>\n      	{
			  LIST;
				  Opt_st_flag = 0;
			  if (Opt_S && !Opt_end)
			    printf("%d: %s", linenum, buf);
			  linenum++;
			  buf[0] = '\0';
        		}
<COMMENT>"*/"		{ LIST; BEGIN(INITIAL); }


","		{ tokenChar(','); }
";"		{ token(;); }
":"		{ token(:); }
"("		{ tokenChar('('); }
")"		{ tokenChar(')'); }
"["		{ tokenChar('['); }
"]"		{ tokenChar(']'); }

"+"		{ token(+); }
"-"		{ token(-); }
"*"		{ token(*); }
"/"		{ token(/); }
mod		{ token(mod); }
:= 		{ token(:=); }
"<"		{ token(<); }
"<="		{ token(<=); }
"<>"		{ token(<>); }
">="		{ token(>=); }
">"		{ token(>); }
"="		{ tokenChar('='); }
and		{ token(and); }
or		{ token(and); }
not		{ token(not); }


array		{ token(KWarray); }
begin		{ token(KWbegin); }
boolean		{ token(KWboolean); }
def		{ token(KWdef); }
do		{ token(KWdo); }
else		{ token(KWelse); }
end		{ token(KWend); }
false		{ token(KWfalse); }
for		{ token(KWfor); }
integer		{ token(KWinteger); }
if		{ token(KWif); }
of		{ token(KWor); }
print		{ token(KWprint); }
read		{ token(KWread); }
real		{ token(KWreal); }
string		{ token(KWstring); }
then		{ token(KWthen); }
to		{ token(KWto); }
true		{ token(KWtrue); }
return		{ token(KWreturn); }
var		{ token(KWvar); }
while		{ token(KWwhile); }

[1-9]{digit}*					{ tokenString(integer, yytext); }
0{digit}+					{ tokenString(oct_integer, yytext); }
{digit}+"."{digit}+				{ tokenString(float, yytext); }
{digit}+"."{digit}+(e|E)(-|"+")?{digit}+	{ tokenString(scientific, yytext); }
{letter}({letter}|{digit})* 	{ tokenString(id, yytext); }


\"([^"]*(["]["])?[^"]*)*\"   {
					char copied[yyleng-2];
					int current = 0;
					strncpy(copied, yytext+sizeof(char), yyleng-2);
					copied[yyleng-2] = '\0';
					for ( int i = 0; i < yyleng - 2; i++ ) {
						if (copied[i] == '"' && copied[i+1] == '"' ) ;
						else {
							copied[current] = copied[i];
							current ++;
						}
					}
					copied[current] = '\0';
					tokenString(string, copied); 
          			}



\n      {
          LIST;
		  Opt_st_flag = 0;
          if (Opt_S && !Opt_end)
            printf("%d: %s", linenum, buf);
          linenum++;
          buf[0] = '\0';
        }

.	{ 
		if (!Opt_end) {
			printf("error at line %d: bad character \"%s\"\n",linenum,yytext); Opt_end = 1;
		}
	}


%%
int main( int argc, char **argv )
{
  if( argc != 2 ) {
    fprintf( stderr, "Usage: ./scanner [filename]\n" );
    exit(0);
  }
	
  FILE *fp = fopen( argv[1], "r" );
  if( fp == NULL ) {
    fprintf( stderr, "Open file error\n" );
    exit(-1);
  }
	
  yyin = fp;

  yylex();

  exit(0);
}
