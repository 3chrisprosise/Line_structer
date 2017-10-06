// Link_structer.cpp : �������̨Ӧ�ó������ڵ㡣
//ʹ��ǰ�ر����뷨,�������ݲ�Ҫ�����ո�,С�����벻Ҫ�������г��ֶ��,С����Ҫ��ʾΪ .3 ������ʽ
//�����뵥������������
//Ŀ����ʽ      27.52-((-3.25)*2.73/1.73)+(3.22*5.44)^2
// ʱ�临�Ӷ�ΪO(n^2) ��Ϊ������ math �����е�Pow ����
#include "stdafx.h"
#include "stdio.h"
#include "malloc.h"
#include <conio.h>
#include <math.h>
using namespace std;

char AllowedChar[] = { '1','2','3','4','5','6','7','8','9','0','+','-','*','/','^','(',')','.' };//�����ַ��������޶���Χ��

char Operator[] = { '+','-','*','/','^','.' }; //��������������

char HighLevelOperator[] = { '*','/','^' };  //�����ȼ�������

char Bracket[] = { '(',')' };//���žͽ�ƥ��ȡֵ

char Number[] = { '1','2','3','4','5','6','7','8','9','0' };

bool IsNumber(char c) {
	int i;
	for (i = 0; i < sizeof(Number); i++) {
		if (c == Number[i])
		{
			return true;
		}
	}
	return false;
}

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

bool IsHighLevelOperator(char p) { //����Ƿ�Ϊ�����ȼ������
	
	int i;
	for (i = 0; i < sizeof(HighLevelOperator); i++) {
		if (p == HighLevelOperator[i]) {
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
	for (i = 0; i < 2; i++) {
		if (p == Bracket[i]) {
			return true;
		}
	}
	return false;
}
/*��������*/
double Myadd(double a, double b) {
	return a + b;
}
double Mysubtract(double a, double b) {
	return a - b;
}
double Myplus(double a, double b) {
	return a*b;
}
double Mydivide(double a, double b) {
	return a / b;
}

//�˷�ֱ��ʹ�� math �������е� pow ����
typedef struct Expression   // ���ʽ���ַ����
{
	char c;
	struct Expression *next;
	struct Expression *prev;

}Expression;

typedef struct Calculater {   //������˫���б��ɷ��ź���Ҫ�����ֵ
	double num = 0;
	char op = NULL;
	struct Calculater *next;
	struct Calculater *prev;
}Calculater;

typedef struct DotNumber {  // ����С����֮�����ֵ�ջ
	double f = 0;
	struct DotNumber *next = NULL;
	struct DotNumber *prev = NULL;
}DotNumber;

int node ;  //���ڴ����ڵ�ʱ�ļ���

Expression * Creat() { // ��ȡ��������
	Expression *head, *p1, *p2; //p1 ���ڴ����µĽṹ�ռ䣬p2���ڴ洢��ǰ����ĩβ�ռ�
	printf("������һ�����ʽ����=��������һ���������������� \n");
	node = 0;

	p1 = p2 = (Expression *)malloc(sizeof(Expression));

	p1->c = getche();
	p1->next = NULL;
	p1->prev = NULL;

	while (p1->c != '=') {

		node++;

		if (node == 1) {
			head = p1;
		}
		else {
			p2->next = p1;
			p1->prev = p2;
		}

		p2 = p1;
		p1 = (Expression *)malloc(sizeof(Expression));
		p1->c = getche();
		p1->next = NULL;
		
	}
	return head;
}


double GetBarketValue(Calculater **Rbarket) {  // �������Ŵ����������ڵ�ָ�룬Ȼ��
	
	Calculater *p, *t,*tail;
	int OpRank = 1,PowNumber = 0;
	double result = 0,interesult = 0; // �洢�����ڲ��Ľ���Լ� ���ܽ��������
	p = *Rbarket;

	// ����һ�飬����Ƿ��� �η��������,���Ұ�ָ���Ƶ��������ſ�ʼ��λ��
	do {
		if (p->op == '^') { OpRank = 3; PowNumber++; }
		else if ((p->op == '*' || p->op == '/') && OpRank < 3) OpRank = 2;
		p = p->prev;
	} while (p->op != '(');

	t = p;
	//���ڴ���������е�����
	for (int i = 0; i < PowNumber; i++) {
		do {
			if (t->op == '^') {
				t->prev->num = pow(t->prev->num, t->next->num);
				t->prev->next = t->next->next;
				if (t->next->next) {
					t->next->next->prev = t->prev;//���ﻹ��û���ͷ�û���õĽṹ�嵥Ԫ��͵��
				}
				break;
			}
			t = t->next;
		} while (t->op != ')');
	}

	//��ʼ����˳���
	t = p;
	do {
		if (t->op == '*') {
			t->prev->num = Myplus(t->prev->num, t->next->num);
			t->prev->next = t->next->next;
			if (t->next->next) {
				t->next->next->prev = t->prev;//���ﻹ��û���ͷ�û���õĽṹ�嵥Ԫ��͵��
			}
		}
		else if (t->op == '/') {
			t->prev->num = Mydivide(t->prev->num, t->next->num);
			t->prev->next = t->next->next;
			if (t->next->next) {
				t->next->next->prev = t->prev;//���ﻹ��û���ͷ�û���õĽṹ�嵥Ԫ��͵��
			}
		}
		t = t->next;
	} while (t->op != ')');

	// ��ʼ����Ӽ���
	t = p;
	do {
		if (t->op == '+') {
			t->prev->num = Myadd(t->prev->num, t->next->num);
			t->prev->next = t->next->next;
			if (t->next->next) {
				t->next->next->prev = t->prev;//���ﻹ��û���ͷ�û���õĽṹ�嵥Ԫ��͵��
			}
		
		}
		else if (t->op == '-') {
			t->prev->num = Mysubtract(t->prev->num, t->next->num);
			t->prev->next = t->next->next;
			if (t->next->next) {
				t->next->next->prev = t->prev;//���ﻹ��û���ͷ�û���õĽṹ�嵥Ԫ��͵��
			}	
		}
		t = t->next;
	} while (t->op != ')');
	//����������֮��Ӧ��ֻʣ���������ź�һ������������ȥ������,�������ű��һ������
	t->num = t->prev->num;
	t->prev = p->prev;
	p->prev->next = t;
	t->op = NULL;
	result = t->num;
	return result;
}
int main()
{	
	Expression *head, *p, *node;
	Calculater *cal, *cu, *lat,*ter;// cal ���ڴ洢��ͷ��cu ���ڱ����м���, lat����free �սṹ��
	DotNumber *dot, *numb, *ber; // ���ڴ��С�������ֵ�˫������
	Calculater *calhead, *calp,*calt;
	double RESULT = 0;
	bool dotplus = true;  //  С���㴦��ʱ����Ҫ�õ�
	int pownumber = 0;   // Ӧ���ڼ�����ݵļ���
	int listRank = 0,Lbr=0, Rbr = 0, OpRank = 1;  //���ڼ���
	head = Creat();  //��������ַ���ջ

	printf("\n");

	p = head; // ��ʼ��һ�����
	do{
		listRank++;
		printf("\n��ǰ�жϵ��ַ�Ϊ%c\n", p->c);

		if (!CheckChar(p->c)) 
		{  //���￪ʼ��������м��
			printf("\n������ڲ��Ϸ��ַ�,��������˳�����\n");
			getchar();
			return 1;
		}
		else if (p ->next) 
		{
			node = p->next;
			if (CheckOperator(p->c, node->c)) { printf("\n�����ʽ����ȷ����������˳�����\n"); getchar(); return 2; }
		}   // ������������������
		
		
		 if(IsBracket(p->c)) //�����Ž���ƥ��
		{
			if (p->c == '(')  Lbr++ ;
			else Rbr++;
			if ((Lbr - Rbr) < 0)
			{

				printf("\n�������ŷǷ�����������˳�����\n");
				getchar();
				return 3;
			}
		}
		if (IsHighLevelOperator(p->c) && OpRank <= 2) OpRank = 2;
		if (p->c == '^') OpRank = 3;
		p = p->next;
	} while (p);

	if ((Lbr - Rbr) != 0)
	{
		printf("\n�������ŷǷ�����������˳�����\n");
		getchar();
		return 3;
	}
	p = head;  // ��ָ�����������ͷ

	//�򵥵���ʾһ��
	printf("\n���ڳ��Լ���ֵ\n");

	do {
		printf(" %c", p->c);
		p = p -> next;
	} while (p);
	p = head;  // ��ָ�����������ͷ

	/* ��ʼ��ȡ����*/
	int expn=0,caln = 0; //�����������
	char current_char;

	cal = ((Calculater *)malloc(sizeof(Calculater)));  //����ͷ�ڵ�
	cal->op = NULL;
	cal->prev = NULL;
	cal->next = NULL;
	cal->num = 0;
	cu = cal;

	dot = ((DotNumber *)malloc(sizeof(DotNumber)));
	dot->next = NULL;
	dot->prev = NULL;
	dot->f = 0;
	do {
		current_char = p->c;
		if (IsNumber(current_char)) {
			if (dotplus) { // ֮ǰû�ж���С����
				cu->num = 10 * int(cu->num) + int(current_char-'0');
				printf("\n��ǰcu->num ��ֵΪ %lf\n", cu->num);
			}
			else {  //֮ǰ������С����,���������������֣�������
				printf("\n֮ǰ������С����,���������������֣�������\n");				
				dot->f = int(current_char - '0');
				printf("dot->f��ֵΪ%lf", dot->f);
				numb = ((DotNumber *)malloc(sizeof(DotNumber)));
				numb->prev = dot;
				numb->next = NULL;
				numb->f = 0;
				dot->next = numb;
				dot = numb;   //ÿ����һ�����־Ͱѵ�ǰָ�������
			}
		}else {
			if (!dotplus && !IsNumber(current_char)) {  //֮ǰ������С��������������һ���ַ������ȴ����������
				printf("\n֮ǰ������С��������������һ���ַ������ȴ����������\n");
				do {
					cu->num = 0.1 * (dot->f) + 0.1*cu->num;
					dot = dot->prev;
					printf("\n��ǰcu->num ��ֵΪ %lf\n", cu->num);
				} while (dot);

				dotplus = true;

				free(dot);

				dot = ((DotNumber *)malloc(sizeof(DotNumber)));
				dot->next = NULL;
				dot->prev = NULL;
				dot->f = 0;
			}

			if (current_char == '.') dotplus = false;  // �ж϶����ַ��Ƿ�ΪС����
			else dotplus = true;
			lat = ((Calculater *)malloc(sizeof(Calculater)));
			cu->next = lat;
			lat->prev = cu;
			lat->op = current_char;
			printf("\n�´�����Calculater�ṹ�壬���ҽ���������������\n");
			cu = lat;	
		}

		if (cu->op != NULL) {  //��������ΪNULL ����ոն����������
			printf("\n��������ΪNULL ����֮ǰ�����������\n");
			lat = ((Calculater *)malloc(sizeof(Calculater)));
			cu->next = lat;
			lat->prev = cu;
			lat->op = NULL;
			lat->num = 0;
			cu = lat;  // ��ǰָ�����
		}

		p = p->next;
	} while (p);
	p = head;  // ��ָ������ͷ���
	cu->op = NULL;
	cu->next = NULL;
	printf("\n ������������ַ�");
	cu = cal;

	printf("\n��ʼ����С����\n");
	do {
		if (cu->op == '.') {
			// lat = cu; ter = cu->next; ������������ʱ��free����Ƭ�ռ���
			cu->prev->num = cu->prev->num + cu->next->num;
			cu->prev->next = cu->next->next;
		}
		cu = cu->next;
	} while (cu);

	cu = cal;
	calhead = ((Calculater *)malloc(sizeof(Calculater)));
	calhead->next = NULL;
	calhead->num = 0;
	calhead->prev = NULL;
	calhead->op = NULL;
	calp = calhead;
	do {
		if (cu->op) {
			if (cu->op == '^') pownumber++;
			printf("%c",cu->op);
			calt=((Calculater *)malloc(sizeof(Calculater)));
			calt->next = NULL;
			calt->num = 0;
			calt->prev = NULL;
			calt->op = cu->op;
			calt->prev = calp;
			calp->next = calt;
			
			calp = calt;
			
		}
		else if(!cu->num == float(0)){
			printf("%lf", cu->num);
			calt = ((Calculater *)malloc(sizeof(Calculater)));
			calt->next = NULL;
			calt->num = cu->num;
			calt->prev = NULL;
			calt->op = NULL;
			calt->prev = calp;
			calp->next = calt;

			calp = calt;
		}
		cu = cu->next;
	} while (cu);
	printf("\nС���㴦�����,��ʼ��������������\n");
	printf("OpRank is %d \n", OpRank);
	
	 calhead = calhead->next;  // calp Ϊ��ʱָ��
	 calhead->prev = NULL;
	 calp = calhead;

	 do {
		 if (calp->op) {
			 printf("%c", calp->op);
		 }
		 else
		 {
			 printf("%lf", calp->num);
		 }
		 calp = calp->next;
	 } while (calp);

	 printf("\n��ʼ������\n");
	 calp = calhead;
	 do {
		 if (calp->op == '(' && calp->next->op == '-') {
			 calp->num = -1 * calp->next->next->num;
			 calp->op = NULL;
			 printf("\n�����˸���: %lf\n ", calp->num);

			 calp->next = calp->next->next->next->next;
			 if (calp->next) {
				 calp->next->prev = calp; // �����˺ö�
			 }
			 Lbr--;
			 Rbr--;
		 }
		 calp = calp->next;
	 } while (calp);
	 printf("\n��ȷ����������ı��ʽ������ֵ,���ڼ�����������������������������\n");
	 



	 calp = calhead;
	 dot, numb, ber;  // ����������֮����õ�
	 do {
		 if (calp->op) {
			 printf("%c", calp->op);
		 }
		 else
		 {
			 printf("%lf", calp->num);
		 }
		 calp = calp->next;
	 } while (calp);


	 calp = calhead;
	if (Lbr == 0) { // �����ţ�ֱ�Ӽ���
		if (OpRank == 3) { // �д��ݵļ��㣬���ȼ�����ݸ�����ֵ������û�����ţ�ֱ����������
			//���ȼ������
			for (int i = 0; i < pownumber; i++) {
				calp = calhead;
				do {
					if (calp->op == '^') {
						calp = calp->prev; // �������ţ�����ǰָ����ǰ��λ
						calp->num = pow(calp->num, calp->next->next->num);
						RESULT = calp->num;
						printf("\n�м���Ϊ �� %lf\n", RESULT);
						printf("\n%�����%d '^' �Ľ��Ϊ%lf\n", i, calp->num);
						(calp->next) = (calp->next->next->next); 
						if(calp->next){ 
							calp->next->prev = calp;
						}// ��һ��next Ϊ���ţ��ڶ���next Ϊ���ֺ󣬵�����nextΪӦ��ָ�������
						
						continue;
					}
					calp = calp->next;
				} while (calp);
			}
			
			
			calp = calhead;

			// ȫ�����ɼӼ��˳�������֮�����ȼ������еĳ˳���
			do {
				if (calp->op == '*' || calp->op == '/') {
					if (calp->op == '*') {
						calp = calp->prev;
						calp->num = Myplus(calp->num, calp->next->next->num);
						RESULT = calp->num;  //������ֵ
						printf("\n�м���Ϊ �� %lf\n", RESULT);
						(calp->next) = (calp->next->next->next);
						if (calp->next == NULL) {
							break;
						}
						else
							calp->next->prev = calp;
					}else{
						calp = calp->prev;
						calp->num = Mydivide(calp->num, calp->next->next->num);
						RESULT = calp->num;  //������ֵ
						printf("\n�м���Ϊ �� %lf\n", RESULT);
						(calp->next) = (calp->next->next->next);
						if (calp->next == NULL) {
							break;
						}
						else
							calp->next->prev = calp;
					}
				}
				calp = calp->next;
			} while (calp);

			//����ָ�룬����Ӽ������
			calp = calhead;
			do {
				if (calp->op == '+' || calp->op == '-') {
					if (calp->op == '+') {
						calp = calp->prev;
						calp->num = Myadd(calp->num, calp->next->next->num);
						RESULT = calp->num;  //������ֵ
						printf("\n�м���Ϊ �� %lf\n", RESULT);
						(calp->next) = (calp->next->next->next);
						if (calp->next == NULL) {
							break;
						}
						else
							calp->next->prev = calp;
					}
					else {
						calp = calp->prev;
						calp->num = Mysubtract(calp->num, calp->next->next->num);
						RESULT = calp->num;  //������ֵ
						printf("\n�м���Ϊ �� %lf\n", RESULT);
						(calp->next) = (calp->next->next->next);
						if (calp->next == NULL) {
							break;
						}
						else
							calp->next->prev = calp;
					}
				}
				
				calp = calp->next;
			} while (calp);
		}
		else {  // �޴��ݣ�ֱ�Ӵ����Ҽ���
			printf("\n�޴�������\n");
			calp = calhead;
			// ȫ�����ɼӼ��˳�������֮�����ȼ������еĳ˳���
			do {
				if (calp->op == '*' || calp->op == '/') {
					if (calp->op == '*') {
						calp = calp->prev;
						calp->num = Myplus(calp->num, calp->next->next->num);
						RESULT = calp->num;  //������ֵ
						printf("\n�м���Ϊ �� %lf\n", RESULT);
						(calp->next) = (calp->next->next->next);
						if (calp->next == NULL) {
							break;
						}
						else
							calp->next->prev = calp;
					}
					else {
						calp = calp->prev;
						calp->num = Mydivide(calp->num, calp->next->next->num);
						RESULT = calp->num;  //������ֵ
						printf("\n�м���Ϊ �� %lf\n", RESULT);
						(calp->next) = (calp->next->next->next);
						if (calp->next == NULL) {
							break;
						}
						else
							calp->next->prev = calp;
					}
				}
				calp = calp->next;
			} while (calp);
			//����ָ�룬����Ӽ������
			calp = calhead;
			do {
				if (calp->op == '+' || calp->op == '-') {
					if (calp->op == '+') {
						calp = calp->prev;
						calp->num = Myadd(calp->num, calp->next->next->num);
						RESULT = calp->num;  //������ֵ
						printf("\n�м���Ϊ �� %lf\n", RESULT);
						(calp->next) = (calp->next->next->next);
						if (calp->next == NULL) {
							break;
						}
						else
							calp->next->prev = calp;
					}
					else {
						calp = calp->prev;
						calp->num = Mysubtract(calp->num, calp->next->next->num);
						RESULT = calp->num;  //������ֵ
						printf("\n�м���Ϊ �� %lf\n", RESULT);
						(calp->next) = (calp->next->next->next);
						if (calp->next == NULL) {
							break;
						}else
							calp->next->prev = calp;
					}
				}
				
				calp = calp->next;
			} while (calp);
		}
	}

	else {//�����ţ����ȼ�������

		//  ȥ����������,���������ŵ�ʱ�����֮ǰ�ĺ������ɣ����յĽ��ֵ�����ڱ��ʽ��һ����������Ӧ�Ľṹ����
		calp = calhead;
		// ��һ�������ȥ����������
		/*do {
			if (calp->op == ')') {
				RESULT = GetBarketValue(calp);
				printf("\n�����еı��ʽ���Ϊ: %lf \n",RESULT);
			}
			calp = calp->next;
		} while (calp);*/

		for (int i = 0; i < Lbr; i++) { // ÿ��ѭ��ȥ��һ�����ź�����
			calp = calhead;

			do {
				if (calp->op == ')') {
					RESULT = GetBarketValue(&calp);
					break;
				}
				calp = calp->next;
			} while (calp);
		}


		printf("\n");
		//�ڶ������
		calp = calhead;
		do {
			if (calp->op == NULL) {
				printf("%lf",calp->num);
			}
			else
			{
				printf("%c",calp->op );
			}
			calp = calp->next;
		} while (calp);
	}
	calp = calhead;// ���ռ���

	//�������
	for (int i = 0; i < pownumber; i++) {
		calp = calhead;
		do {
			if (calp->op == '^') {
				calp = calp->prev; // �������ţ�����ǰָ����ǰ��λ
				calp->num = pow(calp->num, calp->next->next->num);
				RESULT = calp->num;
				printf("\n�м���Ϊ �� %lf\n", RESULT);
				printf("\n%�����%d '^' �Ľ��Ϊ%lf\n", i, calp->num);
				(calp->next) = (calp->next->next->next);
				if (calp->next) {
					calp->next->prev = calp;
				}// ��һ��next Ϊ���ţ��ڶ���next Ϊ���ֺ󣬵�����nextΪӦ��ָ�������

				continue;
			}
			calp = calp->next;
		} while (calp);
	}

	calp = calhead;
	// ����˳�
	do {
		if (calp->op == '*' || calp->op == '/') {
			if (calp->op == '*') {
				calp = calp->prev;
				calp->num = Myplus(calp->num, calp->next->next->num);
				RESULT = calp->num;  //������ֵ
				printf("\n�м���Ϊ �� %lf\n", RESULT);
				(calp->next) = (calp->next->next->next);
				if (calp->next == NULL) {
					break;
				}
				else
					calp->next->prev = calp;
			}
			else {
				calp = calp->prev;
				calp->num = Mydivide(calp->num, calp->next->next->num);
				RESULT = calp->num;  //������ֵ
				printf("\n�м���Ϊ �� %lf\n", RESULT);
				(calp->next) = (calp->next->next->next);
				if (calp->next == NULL) {
					break;
				}
				else
					calp->next->prev = calp;
			}
		}
		calp = calp->next;
	} while (calp);

	// ����Ӽ�
	calp = calhead;
	do {
		if (calp->op == '+' || calp->op == '-') {
			if (calp->op == '+') {
				calp = calp->prev;
				calp->num = Myadd(calp->num, calp->next->next->num);
				RESULT = calp->num;  //������ֵ
				printf("\n�м���Ϊ �� %lf\n", RESULT);
				(calp->next) = (calp->next->next->next);
				if (calp->next == NULL) {
					break;
				}
				else
					calp->next->prev = calp;
			}
			else {
				calp = calp->prev;
				calp->num = Mysubtract(calp->num, calp->next->next->num);
				RESULT = calp->num;  //������ֵ
				printf("\n�м���Ϊ �� %lf\n", RESULT);
				(calp->next) = (calp->next->next->next);
				if (calp->next == NULL) {
					break;
				}
				else
					calp->next->prev = calp;
			}
		}

		calp = calp->next;
	} while (calp);

	printf("���յĽ��Ϊ�� %lf", RESULT);
	getchar();
	return 0;
}
