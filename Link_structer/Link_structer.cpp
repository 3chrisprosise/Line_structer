// Link_structer.cpp : 定义控制台应用程序的入口点。
//使用前关闭输入法

#include "stdafx.h"
#include "stdio.h"
#include "malloc.h"
#include <iostream>
#include <conio.h>
using namespace std;

char AllowedChar[] = { '1','2','3','4','5','6','7','8','9','0','+','-','*','/','%','^','(',')' };//输入字符必须在限定范围内

char Operator[] = { '+','-','*','/','%','^' }; //操作符不能相连

char Bracket[] = { '(',')' };//括号就近匹配取值

bool CheckChar(char p) { // 检测输入是否满足条件
	int i;
	for (i = 0; i < sizeof(AllowedChar); i++) {
		if (p == AllowedChar[i]) {
			return true;
		}
	}
	return false;
}

bool IsOperator(char p) { //检测是否为操作符
	int i;
	for (i = 0; i < sizeof(Operator); i++) {
		if (p == Operator[i]) {
			return true;
		}
	}
	return false;
}

bool CheckOperator(char p, char c) { // 检测是否连续两侧输入了操作符

	if (IsOperator(p) && IsOperator(c)) 
		return true;
	else 
		return false;
}
 
bool CheckPoint(char p) {  // 检测是否输入了小数点
	if (p == '.') {
		return true;
	}
	else
	{
		return false;
	}
}

bool IsBracket(char p) { // 检测是否为括号
	int i;
	for (i = 0; i < sizeof(Bracket); i++) {
		if (p == Bracket[i]) {
			return true;
		}
	}
	return false;
}

typedef struct Exp
{
	char c;
	struct Exp *next;
}Exp;

int node ;  //用于创建节点时的计数

Exp * Creat() { // 获取输入数据
	Exp *head, *p1, *p2;
	char get_input;
	printf("请输入一串表达式，以=结束\n");
	node = 0;
	p1 = p2 = (Exp *)malloc(sizeof(Exp));
	p1->c = getche();
	p1->next = NULL;
	while (p1->c != '=') {
		node++;
		if (node == 1) {
			head = p1;
		}
		else {
			p2->next = p1;
		}
		p2 = p1;
		p1 = (Exp *)malloc(sizeof(Exp));
		p1->c = getche();
		p1->next = NULL;
		
	}
	return head;
}

int main()
{	
	Exp *head, *p, *node;
	int listRank = 0,Lbr=0, Rbr = 0;  //用于计数
	head = Creat();
	p = head;

	do{
		listRank++;
		printf("The Char is %c\n", p->c); //显示正在处理的字符
		if (!CheckChar(p->c)) 
		{  //这里开始对输入进行检测
			printf("输入存在不合法字符");
			return 1;
		}
		else if (p ->next) 
		{
			node = p->next;
			if (CheckOperator(p->c, node->c)) { printf("输入格式不正确"); return 2; }
		}
		else if(IsBracket(p->c)) //对括号进行匹配
		{
			if (p->c == '(') Lbr++;
			else Rbr++;
			if ((Lbr - Rbr) < 0)
			{
				printf("输入括号非法！");
				return 3;
			}
		}
		p = p->next;
	} while (p);
	if ((Lbr - Rbr) != 0)
	{
		printf("输入括号非法！");
		return 3;
	}
	printf("\n正确处理了输入的表达式的最终值");
	getchar();
	return 0;
}

