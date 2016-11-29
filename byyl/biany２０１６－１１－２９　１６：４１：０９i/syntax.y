%{
	#include<unistd.h> 
	#include "tree.h"

	//#undef YYDEBUG
	//#define YYDEBUG 1
	//yydebug = 1;

	extern int yyerror(const char* msg);
	extern int yylex(void);
	int paramnum = 0;
	extern int flag_errfind;
	int sz_dx;
	extern char* struNm;
	extern char* scope1_stru;
	extern char* scope2_var;
	char*  xdsb;
%}
%union{
struct ast* a;
double d;
}
%locations
%error-verbose
%token <a> RETURN  STRUCT  TYPE
%token <a> IF  ELSE
%token <a> WHILE
%token <a> INT  FLOAT  ID
%token <a> SEMI  COMMA  LC  RC  
%type  <a> Program  ExtDefList  ExtDef  ExtDecList  Specifier  StructSpecifier  OptTag  Tag
VarDec  FunDec  VarList  ParamDec  CompSt  StmtList  Stmt  DefList  Def  DecList  Dec  Exp  Args
%right <a> ASSIGNOP
%left  <a> OR
%left  <a> AND
%left  <a> RELOP
%left  <a> PLUS  MINUS
%left  <a> STAR  DIV

%left  <a> LP  RP  LB  RB  DOT
%right <a>NOT


%nonassoc LOWER_ELSE 
%nonassoc ELSE
%nonassoc error
%%
Program: ExtDefList 
	{
	    $$=newast("Program",1,$1);
            if(!check_vardef(varhead)&&!check_arrdef(arrayhead))
	    if(flag_errfind == 0)
 	    {
	    	printf("打印syntax tree:\n");
	    	eval($$,0);
	    	printf("syntax tree打印完毕!\n\n");
	    }
	}
;
ExtDefList: ExtDef ExtDefList {$$=newast("ExtDefList",2,$1,$2);}
   |{$$=newast("ExtDefList",0,-1);}
;
ExtDef: Specifier ExtDecList SEMI //检查变量是否重复定义
    {
        $$=newast("ExtDef",3,$1,$2,$3);
        /*if(is_var_define($2)&&$2->tag == 1)
	{
	    printf("Error type 3 at line %d：Redefined variable '%s'\n",yylineno,$2->content);
	    flag_errfind=1;
	}
	else*/
	    newvar(2,$1,$2);
    } 
    | Specifier SEMI {$$=newast("ExtDef",2,$1,$2);}
    | Specifier FunDec CompSt //检查函数返回值类型与函数类型是否一致
    {
        $$=newast("ExtDef",3,$1,$2,$3);
		newfunc(4,$1);
		//printf("%s \n",$2->content);getchar();getchar();
		//xdsb=$2->content;
		//scopevar(NULL);
	
    }
;
ExtDecList: VarDec {$$=newast("ExtDecList",1,$1);}
    | VarDec COMMA ExtDecList {$$=newast("ExtDecList",3,$1,$2,$3);}
;
Specifier: TYPE {$$=newast("Specifire",1,$1);}
     | StructSpecifier {$$=newast("Specifire",1,$1); $$->scope = $1->scope;}
StructSpecifier: STRUCT OptTag LC DefList RC //检查结构体是否重复定义
    {
	$$=newast("StructSpecifire",5,$1,$2,$3,$4,$5);
	if(is_struc_define($2))
	{
	    printf("Error type 16 at line %d：Duplicated name '%s'\n",yylineno,$2->content);
	    flag_errfind=1;
	}
	else 
	{//prob struc
	   newstruc(2,$2,$4);
	   $$->scope=$2->content;
	   scopevar($2->content);
	   scopearr($2->content);
	}
	
    }
    | STRUCT Tag //检查是否直接使用为定义的结构体
    {
	$$=newast("StructSpecifire",2,$1,$2);
		
	if(!is_struc_define($2))
	{
	    printf("Error type 17 at line %d：Undefined structure '%s'\n",yylineno,$2->content);
	    flag_errfind=1;
	}
	else $$->scope = $2->content;
    }
;
OptTag: ID {$$=newast("OptTag",1,$1);}
    | {$$=newast("OptTag",0,-1);}
;
Tag: ID {$$=newast("Tag",1,$1);}
;
VarDec: ID {$$=newast("VarDec",1,$1);$$->tag=1;}
    | VarDec LB INT RB {$$=newast("VarDec",4,$1,$2,$3,$4);$$->content = $1->content;$$->tag=4;sz_dx=(int)$3->value;}
;
FunDec: ID LP VarList RP //检查函数是否重复定义
    {
	$$=newast("FunDec",4,$1,$2,$3,$4);
	$$->content = $1->content;
	if(is_func_define($1))
	{
	    printf("Error type 4 at line %d：Redefined function '%s'\n",yylineno,$1->content);
	    flag_errfind=1;
	}
	else
		{
			newfunc(2,$1);
			scopexcvar($1->content);
		}
	    
    }
    | ID LP RP 
    {
	$$=newast("FunDec",3,$1,$2,$3);
	$$->content = $1->content;
	if(is_func_define($1))
	{
	    printf("Error type 4 at line %d：Redefined function '%s'\n",yylineno,$1->content);
	    flag_errfind=1;
	}
	else
	    newfunc(2,$1);
    }
;
VarList: ParamDec COMMA VarList {$$=newast("VarList",3,$1,$2,$3);}
    | ParamDec {$$=newast("VarList",1,$1);}
;
ParamDec: Specifier VarDec {
	$$=newast("ParamDec",2,$1,$2);
	/*printf("%s %s",$1->content,$2->content);getchar();getchar();*/
	newxcvar(2,$1,$2);$2->tag=6;
	}
;
CompSt: LC DefList StmtList RC {$$=newast("Compst",4,$1,$2,$3,$4);}
;
StmtList: Stmt StmtList {$$=newast("StmtList",2,$1,$2);}
    | {$$=newast("StmtList",0,-1);}
;
Stmt: Exp SEMI {$$=newast("Stmt",2,$1,$2);}
    | CompSt {$$=newast("Stmt",1,$1);}
    | RETURN Exp SEMI 
    {
	$$=newast("Stmt",3,$1,$2,$3);
	newfunc(3,$2);
    }
    | IF LP Exp RP Stmt %prec LOWER_ELSE {$$=newast("Stmt",5,$1,$2,$3,$4,$5);}
    | IF LP Exp RP Stmt ELSE Stmt {$$=newast("Stmt",7,$1,$2,$3,$4,$5,$6,$7);}
    | WHILE LP Exp RP Stmt {$$=newast("Stmt",5,$1,$2,$3,$4,$5);}
    | error SEMI {yyerrok;}
;
DefList: Def DefList {$$=newast("DefList",2,$1,$2);}
    | {$$=newast("DefList",0,-1);}
;
Def: Specifier DecList SEMI //检查变量是否重复定义
    {
	$$=newast("Def",3,$1,$2,$3);
	/*if(is_var_define($2))
	{
	    printf("Error type 3 at line %d：Redefined variable '%s'\n",yylineno,$2->content);
	    flag_errfind=1;
	}
	else*/ if($2->tag==1) newvar(2,$1,$2);
	else if($2->tag==4) newarray(3,$1,$2,sz_dx);
    	else newvar(2,$1,$2);
    }
;
DecList: Dec {$$=newast("DecList",1,$1);}
    | Dec COMMA DecList {$$=newast("DecList",3,$1,$2,$3);$$->tag=$3->tag;}
;
Dec: VarDec {$$=newast("Dec",1,$1);}
    | VarDec ASSIGNOP Exp {$$=newast("Dec",3,$1,$2,$3);$$->content=$1->content;}
;
Exp: Exp ASSIGNOP Exp //检查赋值号两边的表达式类型是否一致
    {
	$$=newast("Exp",3,$1,$2,$3);
	//printf("%s   wocalei  %s\n",$1->type,$3->type);
	/*if($1->type == NULL)
	{
	    printf("Error type 5 at line %d：Type mismatched for assignment\n",yylineno);
	    flag_errfind=1;
	 }
	else*/
	if($1->type != NULL&&$3->type!=NULL)
	{
 	if(strcmp($1->type,$3->type))
	{
	    printf("Error type 5 at line %d：Type mismatched for assignment\n",yylineno);
	    flag_errfind=1;
	}
	}
    }
    | Exp AND Exp {$$=newast("Exp",3,$1,$2,$3);}
    | Exp OR Exp {$$=newast("Exp",3,$1,$2,$3);}
    | Exp RELOP Exp {$$=newast("Exp",3,$1,$2,$3);}
    | Exp PLUS Exp //检查操作数类型是否一致
    {   
	$$=newast("Exp",3,$1,$2,$3);
//printf("%s a a %s",$1->type,$3->type);
//getchar();getchar();
	if($1->type==NULL||$3->type==NULL)
	{	
	    //printf("%s a a %s",$1->type,$3->type);
//getchar();getchar();
	    //flag_errfind=1;
	    //printf("Error type 7 at line %d：Type mismatched for operands\n",yylineno);
	}	
	else if(strcmp($1->type,$3->type))
	{	
	    printf("Error type 7 at line %d：Type mismatched for operands\n",yylineno);
	    flag_errfind=1;
	}
    }
    | Exp MINUS Exp 
    {
	$$=newast("Exp",3,$1,$2,$3);
	if($1->type==NULL||$3->type==NULL)
	{
	    //flag_errfind=1;printf("Error type 7 at line %d：Type mismatched for operands\n",yylineno);
	}	
	else if(strcmp($1->type,$3->type))
	{	
	    printf("Error type 7 at line %d：Type mismatched for operands\n",yylineno);
	    flag_errfind=1;
	}
    }
    | Exp STAR Exp 
    {
	$$=newast("Exp",3,$1,$2,$3);
	if($1->type==NULL||$3->type==NULL)
	{
	   // flag_errfind=1;printf("Error type 7 at line %d：Type mismatched for operands\n",yylineno);
	}	
	else if(strcmp($1->type,$3->type))
	{	
	    printf("Error type 7 at line %d：Type mismatched for operands\n",yylineno);
	    flag_errfind=1;
	}
    }
    | Exp DIV Exp 
    {
	$$=newast("Exp",3,$1,$2,$3);
	if($1->type==NULL||$3->type==NULL)
	{
	   // flag_errfind=1;
	  //  printf("Error type 7 at line %d：Type mismatched for operands\n",yylineno);
	}	
	else if(strcmp($1->type,$3->type))
	{	
	    printf("Error type 7 at line %d：Type mismatched for operands\n",yylineno);
	    flag_errfind=1;
	}
    }
    | LP Exp RP {$$=newast("Exp",3,$1,$2,$3);}
    | MINUS Exp {$$=newast("Exp",2,$1,$2);}
    | NOT Exp {$$=newast("Exp",2,$1,$2);}
    | ID LP Args RP //检查函数在调用时是否定义
    {
	$$=newast("Exp",4,$1,$2,$3,$4);
	if(is_var_define($1))
	{
	    printf("Error type 11 at line %d：'%s'is not a function\n",yylineno,$1->content);
	    flag_errfind=1;
	}	
	else if(!is_func_define($1))
	{
	    printf("Error type 2 at line %d：Undefined function '%s'\n",yylineno,$1->content);
	    flag_errfind=1;
	}
    }
    | ID LP RP 
    {
	$$=newast("Exp",3,$1,$2,$3);
	if(is_var_define($1))
	{
	    printf("Error type 11 at line %d：'%s'is not a function\n",yylineno,$1->content);
	    flag_errfind=1;
	}	
	
	if(!is_func_define($1))
	{
	    printf("Error type 2 at line %d：Undefined function '%s'\n",yylineno,$1->content);
	    flag_errfind=1;
	}
    }
    | Exp LB Exp RB //检查数组是否正常使用
    {
	$$=newast("Exp",4,$1,$2,$3,$4);
	if((!is_array_define($1)))
	{
            printf("Also not a array!\n");
 	    flag_errfind=1;
	}
	else if($3->type == NULL)
	{
	    printf("Error type 12 at line %d：'%f' is not an integer\n",yylineno,$3->value);
	    flag_errfind=1;
	}
	else if(strcmp("int",$3->type))
	{	
	    printf("Error type 12 at line %d：'%f' is not an integer\n",yylineno,$3->value);
	    flag_errfind=1;
	}
	else if(is_var_define($1))
	{
	    printf("Error type 11 at line %d：'%s'is not a function\n",yylineno,$1->content);
	    flag_errfind=1;
	}	
	else if(!is_array_define($1))
	{
	    printf("Error type 2 at line %d：Undefined function '%s'\n",yylineno,$1->content);
	    flag_errfind=1;
	}
	//else {add_array_size($1,$3);}
    }
    | Exp DOT Exp //检查结构体是否正常使用
    {
	$$=newast("Exp",3,$1,$2,$3);
	/*if(!is_struc_define($1))
	{
	    printf("Error type 13 at line %d：Illegal use of '.'\n",yylineno);
	    flag_errfind=1;
	}*/
	scope1_stru = var_type($1);
	scope2_var = var_scope($3);
	if(!is_var_define($1)||scope1_stru == NULL)
	{
	    printf("Error type 13 at line %d：Illegal use of '.'\n",yylineno);
	    flag_errfind=1;
	}
	else
	if(scope2_var == NULL)
		{
			 printf("Error type 14 at line %d：Non-existent filed %s\n",yylineno,$3->content);
		}
	else
	{
		
		if(strcmp(scope1_stru,scope2_var))
		{
			printf("Error type 14 at line %d：Non-existent filed %s\n",yylineno,$3->content);
	    		flag_errfind=1;
		}
	}
	
    }
    | ID //检查变量在使用时是否定义
    {
	$$=newast("Exp",1,$1);
	
	if((!is_var_define($1))&&(!is_array_define($1)))
	{
	    printf("Error type 1 at line %d：'%s' Undefined variable\n",yylineno,$1->content);
	    flag_errfind=1;
	}
	else {$$->type = var_type($1);}
    }
    | INT {$$=newast("Exp",1,$1);$$->tag=3;$$->type="int";$$->value=$1->value;}
    | FLOAT {$$=newast("Exp",1,$1);$$->tag=3;$$->type="float";$$->value=$1->value;}
    | error RB {yyerrok;}
    | error RP {yyerrok;}
    | error ASSIGNOP Exp {yyerrok;}
    | error RELOP Exp {yyerrok;}
    | error PLUS Exp {yyerrok;}
    | error MINUS Exp {yyerrok;}
    | error STAR Exp {yyerrok;}
    | error DIV Exp {yyerrok;}
    | Exp ASSIGNOP error {yyerrok;}
    | Exp RELOP error {yyerrok;}
    | Exp PLUS error {yyerrok;}
    | Exp MINUS error {yyerrok;}
    | Exp STAR error {yyerrok;}
    | Exp DIV error {yyerrok;}
;
Args: Exp COMMA Args {$$=newast("Args",3,$1,$2,$3);paramnum=paramnum+1;printf("%d",paramnum);getchar();getchar();}  //记录参数个数
    | Exp {$$=newast("Args",1,$1);paramnum=paramnum+1;printf("%d",paramnum);getchar();getchar();}
;
%%
int yyerror(const char *msg)
{
	flag_errfind=1;
	fprintf(stderr,"error: %s %d %d\n",msg,yylloc.first_line,yylloc.first_column);
	return 0;
}


