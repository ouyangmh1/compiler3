#include <stdio.h>
#include "data_structure.h"
#include "compile.h"

using namespace std;

const string INIT_NAME = "initmainabcdehahaha";
int indent_cnt;
inline void indent(std :: string& s){for(int i = 0; i < indent_cnt; ++i) s += "\t";}

void pushdown(struct TreeNode* root) {
	for (int i = 0; i < NODE_MAX_CHILD_NUMBER; ++i)
		if (root->child[i]) {
			if (root->funcbh) root->child[i]->funcbh = root->funcbh;
			pushdown(root->child[i]);
		}
	for (struct TreeNode* t : root->childlist)
		if (t) {
			if (root->funcbh) t->funcbh = root->funcbh;
			pushdown(t);
		}
}

int findTindex(struct TreeNode* root) {
	if (T_map.find((string)root->name + "@func" + to_string(root->funcbh)) != T_map.end())
		return T_map[(string)root->name + "@func" + to_string(root->funcbh)];
	return T_map[(string)root->name + "@func0"];
}

int T_map_insert(string s, int funcbh) {
	return T_map[s + "@func" + to_string(funcbh)] = T_map_cnt++;
}

int T_map_insert(struct TreeNode* root) {
	return T_map[root->name + "@func" + to_string(root->funcbh)] = T_map_cnt++;
}

void flush() {
	outputstring += decl_buffer;
	outputstring += prog_buffer;
	decl_buffer = prog_buffer = "";
}

void write_initfunc() {
	indent(outputstring); outputstring += "f_" + INIT_NAME + " [0]\n"; //fprintf(fout, ("f_" + INIT_NAME + " [0]\n").c_str());
	++indent_cnt;
	for (struct TreeNode* tnode : init_main_list) {
		if (tnode->type == NODE_CONSTDEF) {
			int tempval;
			indent(decl_buffer); decl_buffer += "var t" + to_string(tempval = t_cnt++) + "\n"; //fprintf(fout, "var t%d\n", tempval = t_cnt++);
			compile(tnode->child[1]);
			indent(prog_buffer); prog_buffer += "T" + to_string(findTindex(tnode)) + " = t" + to_string(tempval) + "\n"; //fprintf(fout, "T%d = t%d\n",findTindex(tnode) ,tempval);
		}
		if (tnode->type == NODE_VARDEF_2) {
			int tempval;
			indent(decl_buffer); decl_buffer += "var t" + to_string(tempval = t_cnt++) + "\n";  //fprintf(fout, "var t%d\n", tempval = t_cnt++);
			compile(tnode->child[1]);
			indent(prog_buffer); prog_buffer += "T" + to_string(findTindex(tnode)) + " = t" + to_string(tempval) + "\n"; //fprintf(fout, "T%d = t%d\n", findTindex(tnode), tempval);
		}
	}
	flush();
	indent(outputstring); outputstring += "return 0\n"; //fprintf(fout, "return 0\n");
	--indent_cnt; indent(outputstring); outputstring += "end f_" + INIT_NAME + "\n"; //fprintf(fout, ("end f_" + INIT_NAME + "\n").c_str());
}

void compile(struct TreeNode* root) {
	int tempval1, tempval2, tempval3;
	struct TreeNode* tempnode;
	switch(root->type) {
		case NODE_COMPUNIT:
			outputstring += "//Eeyore Code:\n"; //fprintf(fout, "//Eeyore Code:\n");
			for (struct TreeNode* t : root->childlist)
				compile(t);
			break;
		case NODE_CONSTDECL:
			compile(root->child[1]);
			break;
		case NODE_VARDECL:
			compile(root->child[1]);
			break;
		case NODE_CONSTDEFS:
			for (struct TreeNode* t : root->childlist)
				compile(t);
			break;
		case NODE_CONSTDEF:
			T_map[(string)root->name + "@func" + to_string(root->funcbh)] = T_map_cnt++;
			indent(decl_buffer); decl_buffer += "var T" + to_string(T_map[(std :: string)root->name + "@func" + to_string(root->funcbh)]) + "\n"; //fprintf(fout, "var T%d\n", T_map[(std :: string)root->name + "@func" + to_string(root->funcbh)]);
			if (!root->funcbh)
				init_main_list.push_back(root);
			else {
				int tempval;
				indent(decl_buffer); decl_buffer += "var t" + to_string(tempval = t_cnt++) + "\n";  //fprintf(fout, "var t%d\n", tempval = t_cnt++);
				compile(root->child[1]);
				indent(prog_buffer); prog_buffer += "T" + to_string(findTindex(root)) + " = t" + to_string(tempval) + "\n"; //fprintf(fout, "T%d = t%d\n", findTindex(root), tempval);
			}
			break;
		case NODE_VARDEFS:
			for (struct TreeNode* t : root->childlist)
				compile(t);
			break;
		case NODE_VARDEF_1:
			T_map[(string)root->name + "@func" + to_string(root->funcbh)] = T_map_cnt++;
			indent(decl_buffer); decl_buffer += "var T" + to_string(T_map[(std :: string)root->name + "@func" + to_string(root->funcbh)]) + "\n"; //fprintf(fout, "var T%d\n", T_map[(std :: string)root->name + "@func" + to_string(root->funcbh)]);
			break;
		case NODE_VARDEF_2:
			T_map[(string)root->name + "@func" + to_string(root->funcbh)] = T_map_cnt++;
			indent(decl_buffer); decl_buffer += "var T" + to_string(T_map[(std :: string)root->name + "@func" + to_string(root->funcbh)]) + "\n"; //fprintf(fout, "var T%d\n", T_map[(std :: string)root->name + "@func" + to_string(root->funcbh)]);
			if (!root->funcbh)
				init_main_list.push_back(root);
			else {
				int tempval;
				indent(decl_buffer); decl_buffer += "var t" + to_string(tempval = t_cnt++) + "\n";  //fprintf(fout, "var t%d\n", tempval = t_cnt++);
				compile(root->child[1]);
				indent(prog_buffer); prog_buffer += "T" + to_string(findTindex(root)) + " = t" + to_string(tempval) + "\n"; //fprintf(fout, "T%d = t%d\n", findTindex(root), tempval);
			}
			break;
		case NODE_FUNCDEF_1:
			indent(outputstring); outputstring += "f_" + root->name + " [" + to_string(root->child[1]->childlist.size()) + "]\n"; //fprintf(fout, "f_%s [%d]\n", root->name.c_str(), root->child[1]->childlist.size());
			++indent_cnt;
				tempnode = root->child[1];
				tempval1 = 0;
				for (struct TreeNode* tnode : tempnode->childlist) {
					T_map_insert(tnode->name, root->funcbh);
					indent(decl_buffer); decl_buffer += "T" + to_string(findTindex(tnode)) + " = p" + to_string(tempval1) + "\n"; //fprintf(fout, "T%d = p%d\n", findTindex(tnode), tempval1);
					++tempval1;
				}
				compile(root->child[2]);
				indent(prog_buffer); prog_buffer += "return 0\n";//fprintf(fout, "return 0\n");
				flush();
			--indent_cnt;
			indent(outputstring); outputstring += "end f_" + root->name + "\n"; //fprintf(fout, "end f_%s\n", root->name.c_str());
			break;
		case NODE_FUNCDEF_2:
			indent(outputstring); outputstring += "f_" + root->name + " [0]\n"; //fprintf(fout, "f_%s [0]\n", root->name.c_str());
			++indent_cnt;
				if (root->name == "main") {
					indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
					indent(prog_buffer); prog_buffer += "t" + to_string(t_cnt++) + " = call f_" + INIT_NAME + "\n"; //fprintf(fout, ("t%d = call f_" + INIT_NAME + "\n").c_str(), t_cnt++);
				}
				compile(root->child[2]);
				flush();
				indent(outputstring); outputstring += "return 0\n"; //fprintf(fout, "return 0\n");
			--indent_cnt;
			indent(outputstring); outputstring += "end f_" + root->name + "\n"; //fprintf(fout, "end f_%s\n", root->name.c_str());
			break;
		case NODE_BLOCKITEMS:
			for (struct TreeNode* t : root->childlist)
				compile(t);
			break;
		case NODE_STMT_1:
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval1 = t_cnt++;
			compile(root->child[1]);
			indent(prog_buffer); prog_buffer += "T" + to_string(findTindex(root->child[0])) + " = t" + to_string(tempval1) + "\n"; //fprintf(fout, "T%d = t%d\n", findTindex(root->child[0]), tempval1);
			break;
		case NODE_STMT_2:
			if (root->child[0]->type == NODE_UNARYEXP_1) {
				indent(prog_buffer); prog_buffer += "call f_" + root->child[0]->name + "\n"; //fprintf(fout, "call f_%s\n", root->child[0]->name.c_str());
			}
			if (root->child[0]->type == NODE_UNARYEXP_2) {
				tempnode = root->child[0]->child[0];
				for (struct TreeNode* tnode : tempnode->childlist) {
					tempval2 = t_cnt;
					indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt++) + "\n"; //fprintf(fout, "var t%d\n", t_cnt++);
					compile(tnode);
					indent(prog_buffer); prog_buffer += "param t" + to_string(tempval2) + "\n"; //fprintf(fout, "param t%d\n", tempval2);
				}
				indent(prog_buffer); prog_buffer += "call f_" + root->child[0]->name + "\n"; //fprintf(fout, "call f_%s\n", root->child[0]->name.c_str());
			}
			break;
		case NODE_STMT_4:
			compile(root->child[0]);
			break;
		case NODE_STMT_5:
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval1 = t_cnt++;
			compile(root->child[0]);
			tempval2 = goto_cnt++;
			indent(prog_buffer); prog_buffer += "if t" + to_string(tempval1) + " == 0 goto l" + to_string(tempval2) + "\n"; //fprintf(fout, "if t%d == 0 goto l%d\n", tempval1, tempval2);
			++indent_cnt; compile(root->child[1]); --indent_cnt;
			indent(prog_buffer); prog_buffer += "l" + to_string(tempval2) + ":\n"; //fprintf(fout, "l%d:\n", tempval2);
			break;
		case NODE_STMT_6:
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval1 = t_cnt++;
			compile(root->child[0]);
			tempval2 = goto_cnt++;
			indent(prog_buffer); prog_buffer += "if t" + to_string(tempval1) + " == 0 goto l" + to_string(tempval2) + "\n"; //fprintf(fout, "if t%d == 0 goto l%d\n", tempval1, tempval2);
			++indent_cnt; compile(root->child[1]);
			tempval3 = goto_cnt++;
			indent(prog_buffer); prog_buffer += "goto l" + to_string(tempval3) + "\n"; //fprintf(fout, "goto l%d\n", tempval3);
			--indent_cnt; indent(prog_buffer); prog_buffer += "l" + to_string(tempval2) + ":\n"; //fprintf(fout, "l%d:\n", tempval2);
			++indent_cnt; compile(root->child[2]);
			--indent_cnt; indent(prog_buffer); prog_buffer += "l" + to_string(tempval3) + ":\n"; //fprintf(fout, "l%d:\n", tempval3);
			break;
		case NODE_STMT_7:
			tempval1 = goto_cnt++;
		       	indent(prog_buffer); prog_buffer += "l" + to_string(tempval1) + ":\n"; //fprintf(fout, "l%d:\n", tempval1);
			++indent_cnt;
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval2 = t_cnt++;
			compile(root->child[0]);
			tempval3 = goto_cnt++;
			root->childbh[0] = tempval1; root->childbh[1] = tempval3;
			--indent_cnt; indent(prog_buffer); prog_buffer += "if t" + to_string(tempval2) + " == 0 goto l" + to_string(tempval3) + "\n"; //fprintf(fout, "if t%d == 0 goto l%d\n", tempval2, tempval3);
			++indent_cnt; compile(root->child[1]);
			indent(prog_buffer); prog_buffer += "goto l" + to_string(tempval1) + "\n"; //fprintf(fout, "goto l%d\n", tempval1);
			--indent_cnt;indent(prog_buffer); prog_buffer += "l" + to_string(tempval3) + ":\n"; //fprintf(fout, "l%d:\n", tempval3);
			break;
		case NODE_STMT_8:
			tempnode = root->parent;
			while(tempnode->type != NODE_STMT_7) {
				tempnode = tempnode->parent;
				if (!tempnode) yyerror("NODE_STMT_8 hits NULL!");
			}
			indent(prog_buffer); prog_buffer += "goto l" + to_string(tempnode->childbh[1]) + "\n"; //fprintf(fout, "goto l%d\n", tempnode->childbh[1]);
			break;
		case NODE_STMT_9:
			tempnode = root->parent;
			while(tempnode->type != NODE_STMT_7) {
				tempnode = tempnode->parent;
				if (!tempnode) yyerror("NODE_STMT_8 hits NULL!");
			}
			indent(prog_buffer); prog_buffer += "goto l" + to_string(tempnode->childbh[0]) + "\n"; //fprintf(fout, "goto l%d\n", tempnode->childbh[0]);
			break;
		case NODE_STMT_10:
			indent(prog_buffer); prog_buffer += "return 0\n"; //fprintf(fout, "return 0\n");
			break;
		case NODE_STMT_11:
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval1 = t_cnt++;
			compile(root->child[0]);
			indent(prog_buffer); prog_buffer += "return t" + to_string(tempval1) + "\n"; //fprintf(fout, "return t%d\n", tempval1);
			break;
		case NODE_ADDEXP:
			tempval1 = t_cnt-1;
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval2 = t_cnt++;
			compile(root->child[0]);
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval3 = t_cnt++;
			compile(root->child[1]);
			indent(prog_buffer); prog_buffer += "t" + to_string(tempval1) + " = t" + to_string(tempval2) + " " + root->name + " t" + to_string(tempval3) + "\n"; //fprintf(fout, "t%d = t%d %s t%d\n", tempval1, tempval2, root->name.c_str(), tempval3);
			break;
		case NODE_MULEXP:
			tempval1 = t_cnt-1;
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval2 = t_cnt++;
			compile(root->child[0]);
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval3 = t_cnt++;
			compile(root->child[1]);
			indent(prog_buffer); prog_buffer += "t" + to_string(tempval1) + " = t" + to_string(tempval2) + " " + root->name + " t" + to_string(tempval3) + "\n"; //fprintf(fout, "t%d = t%d %s t%d\n", tempval1, tempval2, root->name.c_str(), tempval3);
			break;
		case NODE_NUMBER:
			indent(prog_buffer); prog_buffer += "t" + to_string(t_cnt-1) + " = " + to_string(root->val) + "\n"; //fprintf(fout, "t%d = %d\n", t_cnt-1, root->val);
			break;
		case NODE_LVAL:
			indent(prog_buffer); prog_buffer += "t" + to_string(t_cnt-1) + " = T" + to_string(findTindex(root)) + "\n"; //fprintf(fout, "t%d = T%d\n", t_cnt-1, findTindex(root));
			break;
		case NODE_PRIMARYEXP_1:
			compile(root->child[0]);
			break;
		case NODE_LOREXP:
			tempval1 = t_cnt-1;
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval2 = t_cnt++;
			compile(root->child[0]);
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval3 = t_cnt++;
			compile(root->child[1]);
			indent(prog_buffer); prog_buffer += "t" + to_string(tempval1) + " = t" + to_string(tempval2) + " " + root->name + " t" + to_string(tempval3) + "\n"; //fprintf(fout, "t%d = t%d %s t%d\n", tempval1, tempval2, root->name.c_str(), tempval3);
			break;
		case NODE_LANDEXP:
			tempval1 = t_cnt-1;
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval2 = t_cnt++;
			compile(root->child[0]);
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval3 = t_cnt++;
			compile(root->child[1]);
			indent(prog_buffer); prog_buffer += "t" + to_string(tempval1) + " = t" + to_string(tempval2) + " " + root->name + " t" + to_string(tempval3) + "\n"; //fprintf(fout, "t%d = t%d %s t%d\n", tempval1, tempval2, root->name.c_str(), tempval3);
			break;
		case NODE_EQEXP:
			tempval1 = t_cnt-1;
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval2 = t_cnt++;
			compile(root->child[0]);
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval3 = t_cnt++;
			compile(root->child[1]);
			indent(prog_buffer); prog_buffer += "t" + to_string(tempval1) + " = t" + to_string(tempval2) + " " + root->name + " t" + to_string(tempval3) + "\n"; //fprintf(fout, "t%d = t%d %s t%d\n", tempval1, tempval2, root->name.c_str(), tempval3);
			break;
		case NODE_RELEXP:
			tempval1 = t_cnt-1;
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval2 = t_cnt++;
			compile(root->child[0]);
			indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt) + "\n"; //fprintf(fout, "var t%d\n", t_cnt);
			tempval3 = t_cnt++;
			compile(root->child[1]);
			indent(prog_buffer); prog_buffer += "t" + to_string(tempval1) + " = t" + to_string(tempval2) + " " + root->name + " t" + to_string(tempval3) + "\n"; //fprintf(fout, "t%d = t%d %s t%d\n", tempval1, tempval2, root->name.c_str(), tempval3);
			break;
		case NODE_UNARYEXP_1:
			indent(prog_buffer); prog_buffer += "t" + to_string(t_cnt-1) + " = call f_" + root->name + "\n"; //fprintf(fout, "t%d = call f_%s\n", t_cnt-1, root->name.c_str());
			break;
		case NODE_UNARYEXP_2:
			tempval1 = t_cnt-1;
			tempnode = root->child[0];
			for (struct TreeNode* tnode : tempnode->childlist) {
				tempval2 = t_cnt;
				indent(decl_buffer); decl_buffer += "var t" + to_string(t_cnt++) + "\n"; //fprintf(fout, "var t%d\n", t_cnt++);
				compile(tnode);
				indent(prog_buffer); prog_buffer += "param t" + to_string(tempval2) + "\n"; //fprintf(fout, "param t%d\n", tempval2);
			}
			indent(prog_buffer); prog_buffer += "t" + to_string(tempval1) + " = call f_" + root->name + "\n"; //fprintf(fout, "t%d = call f_%s\n", tempval1, root->name.c_str());
			break;
		case NODE_UNARYEXP_3:
			tempval1 = t_cnt-1;
			indent(decl_buffer); decl_buffer += "var t" + to_string(tempval2 = t_cnt++) + "\n"; //fprintf(fout, "var t%d\n", tempval2 = t_cnt++);
			compile(root->child[0]);
			indent(prog_buffer); prog_buffer += "t" + to_string(tempval1) + " = " + root->name + " t" + to_string(tempval2) + "\n"; //fprintf(fout, "t%d = %s t%d\n", tempval1, root->name.c_str(), tempval2);
			break;
	}
	
	if (!root->funcbh) {
		flush();	
	}
}
