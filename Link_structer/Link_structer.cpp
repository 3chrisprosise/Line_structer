// Link_structer.cpp : �������̨Ӧ�ó������ڵ㡣
//ʹ��ǰ�ر����뷨

#include "stdafx.h"
#include "stdio.h"
#include "malloc.h"
#include <iostream>
#include <conio.h>
using namespace std;

char AllowedChar[] = { '1','2','3','4','5','6','7','8','9','0','+','-','*','/','%','^','(',')' };//�����ַ��������޶���Χ��

char Operator[] = { '+','-','*','/','%','^' }; //��������������

char Bracket[] = { '(',')' };//���žͽ�ƥ��ȡֵ

bool CheckChar(char p) { // ��������Ƿ���������
	int i;
	for (i = 0; i < sizeof(AllowedChar); i++) {
		if (p == AllowedChar[i]) {
			return true;
		}
	}
	return false;
}

bool IsOperator(char p) { //����Ƿ�Ϊ������
	int i;
	for (i = 0; i < sizeof(Operator); i++) {
		if (p == Operator[i]) {
			return true;
		}
	}
	return false;
}

bool CheckOperator(char p, char c) { // ����Ƿ��������������˲�����

	if (IsOperator(p) && IsOperator(c)) 
		return true;
	else 
		return false;
}
 
bool CheckPoint(char p) {  // ����Ƿ�������С����
	if (p == '.') {
		return true;
	}
	else
	{
		return false;
	}
}

bool IsBracket(char p) { // ����Ƿ�Ϊ����
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

int node ;  //���ڴ����ڵ�ʱ�ļ���

Exp * Creat() { // ��ȡ��������
	Exp *head, *p1, *p2;
	char get_input;
	printf("������һ�����ʽ����=����\n");
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
	int listRank = 0,Lbr=0, Rbr = 0;  //���ڼ���
	head = Creat();
	p = head;

	do{
		listRank++;
		printf("The Char is %c\n", p->c); //��ʾ���ڴ�����ַ�
		if (!CheckChar(p->c)) 
		{  //���￪ʼ��������м��
			printf("������ڲ��Ϸ��ַ�");
			return 1;
		}
		else if (p ->next) 
		{
			node = p->next;
			if (CheckOperator(p->c, node->c)) { printf("�����ʽ����ȷ"); return 2; }
		}
		else if(IsBracket(p->c)) //�����Ž���ƥ��
		{
			if (p->c == '(') Lbr++;
			else Rbr++;
			if ((Lbr - Rbr) < 0)
			{
				printf("�������ŷǷ���");
				return 3;
			}
		}
		p = p->next;
	} while (p);
	if ((Lbr - Rbr) != 0)
	{
		printf("�������ŷǷ���");
		return 3;
	}
	printf("\n��ȷ����������ı��ʽ������ֵ");
	getchar();
	return 0;
}

