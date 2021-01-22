%{
#include <stdio.h>
#include <string>
#include <map>
#include <string>
#include "data_structure.h"
#include "compile.h"

extern FILE* yyin;
extern FILE* yyout;
extern int yylineno;

int yylex();
void yyerror(char*);

int tree_node_count;
struct TreeNode* root;
std :: map <std :: string, int> T_map;
std :: map <std :: string, int> p_map;
int T_map_cnt;
int p_map_cnt;
int t_cnt;
int goto_cnt;
int eeyore_cnt;
int func_cnt = 1;
std :: string outputstring;
std :: string decl_buffer;
std :: string prog_buffer;

std :: vector <struct TreeNode*> init_main_list;

FILE* checkfile = fopen("checkfile.txt", "w");
FILE* fout = fopen("output.eeyore", "w");
%}

%union {
	int value;
	char* name;
	struct TreeNode* node;
	/*struct CompUnit* compunit;
	struct Defs* defs;
	struct Def* def;
	struct IdentParam* identparam;
	struct InitVal* initval;
	struct Func* func;
	struct FuncFParam* funcfparam;
	std :: vector <Block*> block;*/
};

%token <value> INTCONST
%token <name> IDENT
%token <name> CONST INT VOID IF ELSE WHILE BREAK CONTINUE RETURN
%token <name> PRN_L PRN_R ARR_L ARR_R BRA_L BRA_R COMMA EOL

%type <name> UnaryOp

%type <node> CompUnit
%type <node> Decl ConstDecl VarDecl
%type <node> ConstDefs VarDefs
%type <node> ConstDef VarDef
%type <node> ConstIdentParam VarIdentParam
%type <node> ConstInitVal ConstInitVals InitVal InitVals

%type <node> FuncDef
%type <node> FuncFParams
%type <node> Block BlockItems

%type <node> Number

%type <node> ConstExp Exp
%type <node> Type FuncFParam BlockItem Stmt LVal Cond AddExp LOrExp PrimaryExp UnaryExp FuncRParams ArrExps
%type <node> MulExp RelExp EqExp LAndExp

%right <name> OP_1 OP_8
%left <name> OP_2 OP_3 OP_4 OP_5 OP_6 OP_7

%start CompUnit

%%

CompUnit : CompUnit Decl {root = $$ = $1; $$->childlist.push_back($2); $2->parent=$$;}
	| CompUnit FuncDef {root = $$ = $1; $$->childlist.push_back($2); $2->parent=$$;}
	| {root = $$ = new TreeNode(NODE_COMPUNIT);}
	;

Decl : 	ConstDecl {$$ = $1;}
     	| VarDecl {$$ = $1;}
	;

ConstDecl : CONST Type ConstDefs EOL {$$ = new TreeNode(NODE_CONSTDECL); $$->child[0]=$2; $$->child[1]=$3; $2->parent=$$; $3->parent=$$;}
	  ;

ConstDefs : ConstDefs COMMA ConstDef {$$ = $1; $$->childlist.push_back($3); $3->parent=$$;}
	  | ConstDef {$$ = new TreeNode(NODE_CONSTDEFS); $$->childlist.push_back($1); $1->parent=$$;}
	;

ConstDef : IDENT ConstIdentParam OP_8 ConstInitVal {$$ = new TreeNode(NODE_CONSTDEF); $$->name=$1; $$->child[0]=$2; $$->child[1]=$4;}
	 ;

ConstIdentParam : ConstIdentParam ARR_L ConstExp ARR_R {$$=$1; $$->childlist.push_back($3);}
	   | {$$ = new TreeNode(NODE_CONSTIDENTPARAM);}
	;

ConstInitVal : ConstExp {$$=$1;}
	     | BRA_L ConstInitVals BRA_R
	     | BRA_L BRA_R
	;

ConstInitVals : ConstInitVal				//To be modified
	      | ConstInitVals COMMA ConstInitVal
	;

VarDecl : Type VarDefs EOL {$$ = new TreeNode(NODE_VARDECL); $$->child[0]=$1; $$->child[1]=$2; $1->parent=$$; $2->parent=$$;}
	;

VarDefs : VarDefs COMMA VarDef {$$ = $1; $$->childlist.push_back($3); $3->parent=$$;}
	| VarDef {$$ = new TreeNode(NODE_VARDEFS); $$->childlist.push_back($1); $1->parent=$$;}
	;

VarDef : IDENT VarIdentParam {$$ = new TreeNode(NODE_VARDEF_1); $$->name=$1; $$->child[0]=$2;}
       | IDENT VarIdentParam OP_8 InitVal {$$ = new TreeNode(NODE_VARDEF_2); $$->name=$1; $$->child[0]=$2; $$->child[1]=$4;}
	; 

VarIdentParam : VarIdentParam ARR_L ConstExp ARR_R {$$=NULL;}
	      | {$$=NULL;}
	;

InitVal : Exp
	| BRA_L BRA_R
	| BRA_L InitVals BRA_R
	;

InitVals : InitVals COMMA InitVal
	 | InitVal
	;

Type : VOID {$$ = new TreeNode(NODE_TYPE); $$->name="void";}
     | INT {$$ = new TreeNode(NODE_TYPE); $$->name="int";}
	;

FuncDef : Type IDENT PRN_L FuncFParams PRN_R Block {$$ = new TreeNode(NODE_FUNCDEF_1); $$->name=$2; $$->child[0]=$1; $$->child[1]=$4; $$->child[2]=$6; $$->funcbh=$4->funcbh=$6->funcbh=func_cnt++;}
	| Type IDENT PRN_L PRN_R Block {$$ = new TreeNode(NODE_FUNCDEF_2); $$->name=$2; $$->child[0]=$1; $$->child[2]=$5; $$->funcbh=$5->funcbh=func_cnt++;}
	;

FuncFParams : FuncFParams COMMA FuncFParam {$$=$1; $$->childlist.push_back($3);}
	    | FuncFParam {$$ = new TreeNode(NODE_FUNCFPARAMS); $$->childlist.push_back($1);}
	;

FuncFParam : Type IDENT {$$ = new TreeNode(NODE_FUNCFPARAM_1); $$->child[0]=$1; $$->name=$2;}
	   | Type IDENT ARR_L ARR_R ArrExps {$$ = new TreeNode(NODE_FUNCFPARAM_2); $$->child[0]=$1; $$->child[1]=$5; $$->name=$2;}
	;

ArrExps : ArrExps ARR_L Exp ARR_R {$$=$1; $$->childlist.push_back($3);}
       | {$$ = new TreeNode(NODE_ARREXPS);}
	;

Block : BRA_L BlockItems BRA_R {$$ = $2;}
      ;

BlockItems : BlockItems BlockItem {$$ = $1; $$->childlist.push_back($2); $2->parent=$$;}
	   | {$$ = new TreeNode(NODE_BLOCKITEMS);}
	;

BlockItem : Decl {$$ = $1;}
	  | Stmt {$$ = $1;}
	;

Stmt : LVal OP_8 Exp EOL {$$ = new TreeNode(NODE_STMT_1); $$->child[0]=$1; $$->child[1]=$3; $1->parent=$$; $3->parent=$$;}
     	| Exp EOL {$$ = new TreeNode(NODE_STMT_2); $$->child[0]=$1; $1->parent=$$;}
	| EOL {$$ = new TreeNode(NODE_STMT_3);}
	| Block {$$ = new TreeNode(NODE_STMT_4); $$->child[0]=$1; $1->parent=$$;}
	| IF PRN_L Cond PRN_R Stmt {$$ = new TreeNode(NODE_STMT_5); $$->child[0]=$3; $$->child[1]=$5; $3->parent=$$; $5->parent=$$;}
	| IF PRN_L Cond PRN_R Stmt ELSE Stmt {$$ = new TreeNode(NODE_STMT_6); $$->child[0]=$3; $$->child[1]=$5; $$->child[2]=$7; $3->parent=$$; $5->parent=$$; $7->parent=$$;}
	| WHILE PRN_L Cond PRN_R Stmt {$$ = new TreeNode(NODE_STMT_7); $$->child[0]=$3; $$->child[1]=$5; $3->parent=$$; $5->parent=$$;}
	| BREAK EOL {$$ = new TreeNode(NODE_STMT_8);}
	| CONTINUE EOL {$$ = new TreeNode(NODE_STMT_9);}
	| RETURN EOL {$$ = new TreeNode(NODE_STMT_10);}
	| RETURN Exp EOL {$$ = new TreeNode(NODE_STMT_11); $$->child[0]=$2;}
	;

Exp : AddExp {$$ = $1;}
    ;

Cond : LOrExp {$$ = $1;}
     ;

LVal : IDENT ArrExps {$$ = new TreeNode(NODE_LVAL); $$->name=$1; $$->child[0]=$2;}
     ;

PrimaryExp : PRN_L Exp PRN_R {$$ = new TreeNode(NODE_PRIMARYEXP_1); $$->child[0]=$2;}
	| LVal {$$=$1;}
	| Number {$$=$1;}
	;

Number : INTCONST {$$ = new TreeNode(NODE_NUMBER); $$->val=$1;}
       ;

UnaryExp : PrimaryExp {$$ = $1;}
	 | IDENT PRN_L PRN_R {$$ = new TreeNode(NODE_UNARYEXP_1); $$->name=$1;}
	 | IDENT PRN_L FuncRParams PRN_R {$$ = new TreeNode(NODE_UNARYEXP_2); $$->name=$1; $$->child[0]=$3;}
	| UnaryOp UnaryExp {$$ = new TreeNode(NODE_UNARYEXP_3); $$->name=$1; $$->child[0]=$2;}
	;

UnaryOp : OP_3 {$$=$1;}
	| OP_1 {$$=$1;}
	;

FuncRParams : FuncRParams COMMA Exp {$$=$1; $$->childlist.push_back($3);}
	   | Exp {$$ = new TreeNode(NODE_FUNCRPARAMS); $$->childlist.push_back($1);}
	;

MulExp : UnaryExp {$$ = $1;}
       | MulExp OP_2 UnaryExp {$$ = new TreeNode(NODE_MULEXP); $$->name=$2; $$->child[0]=$1; $$->child[1]=$3;}
	;

AddExp : MulExp {$$ = $1;}
       | AddExp OP_3 MulExp {$$ = new TreeNode(NODE_ADDEXP); $$->name=$2; $$->child[0]=$1; $$->child[1]=$3;}
	;

RelExp : AddExp {$$=$1;}
       | RelExp OP_4 AddExp {$$ = new TreeNode(NODE_RELEXP); $$->name=$2; $$->child[0]=$1; $$->child[1]=$3;}
	;

EqExp : RelExp {$$=$1;}
      | EqExp OP_5 RelExp {$$ = new TreeNode(NODE_EQEXP); $$->name=$2; $$->child[0]=$1; $$->child[1]=$3;}
	;

LAndExp : EqExp {$$ = $1;}
	| LAndExp OP_7 EqExp {$$ = new TreeNode(NODE_LANDEXP); $$->name=$2; $$->child[0]=$1; $$->child[1]=$3;}
	;

LOrExp : LAndExp {$$ = $1;}
       | LOrExp OP_6 LAndExp {$$ = new TreeNode(NODE_LOREXP); $$->name=$2; $$->child[0]=$1; $$->child[1]=$3;}
	;

ConstExp : AddExp {$$=$1;}
	 ;

%%

void yyerror (char* s) {
	fprintf(stderr, ">> ERROR: %s, in line %d\n", s, yylineno);
	exit(0);
}

int main(int argc, char** argv) {
	if (argc == 6) {
		yyin = fopen(argv[3], "r");
		fout = fopen(argv[5], "w");
	}else {
		yyin = fopen(argv[1], "r");
		fout = fopen("output.e", "w");
	}
	yyparse();
	pushdown(root);
	compile(root);
	write_initfunc();
	
	//printf(outputstring.c_str());
	fputs(outputstring.c_str(), fout);
	
	fclose(yyin);
	fclose(yyout);
	fclose(fout);
	return 0;
}
