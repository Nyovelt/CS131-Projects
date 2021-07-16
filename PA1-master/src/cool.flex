/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>
#include <string.h>
/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */
int layer = 0; /* match comment block */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
  if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
    YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int debug;
extern int curr_lineno;
extern YYSTYPE cool_yylval;
int check();
/*
 *  Add Your own definitions here
 */

%}

%option noyywrap

/*
 * Define names for regular expressions here.
 */

%x COMMENTS
%x STRING
%x ANOTHER_COMMENTS
%%




 /*
  * Define regular expressions for the tokens of COOL here. Make sure, you
  * handle correctly special cases, like:
  *   - Nested comments
  *   - String constants: They use C like systax and can contain escape
  *     sequences. Escape sequence \c is accepted for all characters c. Except
  *     for \n \t \b \f, the result is c.
  *   - Keywords: They are case-insensitive except for the values true and
  *     false, which must begin with a lower-case letter.
  *   - Multiple-character operators (like <-): The scanner should produce a
  *     single token for every such operator.
  *   - Line counting: You should keep the global variable curr_lineno updated
  *     with the correct line number
  */


 /*
  *  BLOCK COMMENTS BEGIN
  */
<COMMENTS>{
  "\n"          ++curr_lineno;
  "(*"          ++layer;
  "*)"  {
      --layer;
      if(!layer)
        BEGIN 0;
  }
  <<EOF>> {
    yylval.error_msg = "EOF in comment";
    BEGIN 0; 
    return ERROR;
  }
  .  // do nothing
} // BLOCK COMMENTS END



<ANOTHER_COMMENTS>{
    \n  {++curr_lineno;BEGIN 0; }
    . // LINE COMMENTS
}

 /*
  * STRING BEGIN
  */

\"  {memset(string_buf, 0 , sizeof(string_buf)); string_buf_ptr = string_buf; BEGIN STRING;}
<STRING>{
    \" {
        check();
        yylval.symbol = stringtable.add_string(string_buf);
        BEGIN 0;
        return STR_CONST;
    }
    \\\n       {++curr_lineno;*string_buf_ptr++ = '\n';}
    \0 {
        yylval.error_msg = "String contains null character.";
        BEGIN 0;
        return ERROR;
        }
    \n {
        yylval.error_msg = "Unterminated string constant";
        ++curr_lineno;
        BEGIN 0;
        return ERROR;
    }
    <<EOF>> {
        yylval.error_msg = "EOF in string constant";
        BEGIN 0;
        return ERROR;
    }
    \"   check();*string_buf_ptr++ = '\"';
    \\n  check();*string_buf_ptr++ = '\n';
    \\t  check();*string_buf_ptr++ = '\t';
    \\c  check();*string_buf_ptr++ = '\c';
    \\r  check();*string_buf_ptr++ = '\r';
    \\b  check();*string_buf_ptr++ = '\b';
    \\f  check();*string_buf_ptr++ = '\f';
    \\(.|\n)  check();*string_buf_ptr++ = yytext[1];
    

    .   *string_buf_ptr++ = yytext[0];
}

 /*
  * STRING END
  */

"(*" {
    ++layer;
    BEGIN COMMENTS;
}

"--"  BEGIN ANOTHER_COMMENTS;




[ \f\r\t\v]+ { }
(?i:esac)         return ESAC;
(?i:class)        return CLASS;
(?i:else)         return ELSE;
(?i:fi)           return FI;
(?i:if)           return IF;
(?i:in)           return IN;
(?i:inherits)     return INHERITS;
(?i:let)          return LET;
(?i:loop)         return LOOP;
(?i:pool)         return POOL;
(?i:then)         return THEN;
(?i:while)        return WHILE;
(?i:assign)       return ASSIGN;
(?i:case)         return CASE;
(?i:of)           return OF;
(?i:new)          return NEW;
(?i:str_const)    return STR_CONST;
(?i:not)          return NOT;
(?i:isvoid)       return ISVOID;

[0-9]*            { yylval.symbol = inttable.add_string(yytext);return INT_CONST; }

t(?i:rue)         yylval.boolean = 1;return BOOL_CONST;
f(?i:alse)         yylval.boolean = 0;return BOOL_CONST;



"=>"    return DARROW;
"<="    return LE;
"<-"    return ASSIGN;

"+"     return int('+');
"/"     return int('/');
"-"     return int('-');
"*"     return int('*');
"="     return int('=');
"<"     return int('<');
"."     return int('.');
"~"     return int('~');
","     return int(',');
";"     return int(';');
":"     return int(':');
"("     return int('(');
")"     return int(')');
"@"     return int('@');
"{"     return int('{');
"}"     return int('}');

"*)" {
    yylval.error_msg = "Unmatched *)";
    return ERROR;
}

[a-z][A-Za-z0-9_]* {
    yylval.symbol = idtable.add_string(yytext);
    return OBJECTID;
}

[A-Z][A-Za-z0-9_]* {
    yylval.symbol = idtable.add_string(yytext);
    return TYPEID;
}

"\n"    ++curr_lineno;



. {
	yylval.error_msg = yytext;
	return ERROR;
}

%%

int check(){
    if (string_buf_ptr - string_buf -1 >= MAX_STR_CONST){
        yylval.error_msg = "String constant too long";
        BEGIN 0;
        return ERROR;
    }
}