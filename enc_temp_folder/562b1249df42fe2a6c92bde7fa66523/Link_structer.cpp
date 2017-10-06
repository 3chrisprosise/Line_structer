// Link_structer.cpp : 定义控制台应用程序的入口点。
//使用前关闭输入法,输入内容不要包含空格,小数点请不要在数字中出现多次,小数不要表示为 .3 这种形式
//负数请单独放在括号中
//目标表达式      27.52-((-3.25)*2.73/1.73)+(3.22*5.44)^2
// 时间复杂度为O(n^2) 因为调用了 math 函数中的Pow 函数
#include "stdafx.h"
#include "stdio.h"
#include "malloc.h"
#include <conio.h>
#include <math.h>
using namespace std;

char AllowedChar[] = { '1','2','3','4','5','6','7','8','9','0','+','-','*','/','^','(',')','.' };//输入字符必须在限定范围内

char Operator[] = { '+','-','*','/','^','.' }; //操作符不能相连

char HighLevelOperator[] = { '*','/','^' };  //高优先级操作符

char Bracket[] = { '(',')' };//括号就近匹配取值

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

bool IsHighLevelOperator(char p) { //检测是否为高优先级运算符
	
	int i;
	for (i = 0; i < sizeof(HighLevelOperator); i++) {
		if (p == HighLevelOperator[i]) {
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
	for (i = 0; i < 2; i++) {
		if (p == Bracket[i]) {
			return true;
		}
	}
	return false;
}
/*定义运算*/
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

//乘方直接使用 math 函数库中的 pow 函数
typedef struct Expression   // 表达式由字符组成
{
	char c;
	struct Expression *next;
	struct Expression *prev;

}Expression;

typedef struct Calculater {   //计算器双向列表由符号和需要计算的值
	double num = 0;
	char op = NULL;
	struct Calculater *next;
	struct Calculater *prev;
}Calculater;

typedef struct DotNumber {  // 用于小数点之后数字的栈
	double f = 0;
	struct DotNumber *next = NULL;
	struct DotNumber *prev = NULL;
}DotNumber;

int node ;  //用于创建节点时的计数

Expression * Creat() { // 获取输入数据
	Expression *head, *p1, *p2; //p1 用于创建新的结构空间，p2用于存储当前链表末尾空间
	printf("请输入一串表达式，以=结束，第一个数字请正常输入 \n");
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


double GetBarketValue(Calculater **Rbarket) {  // 碰到括号传右括号所在的指针，然后
	
	Calculater *p, *t,*tail;
	int OpRank = 1,PowNumber = 0;
	double result = 0,interesult = 0; // 存储括号内部的结果以及 最总结果的运算
	p = *Rbarket;

	// 遍历一遍，检查是否有 次方计算存在,并且把指针移到最左括号开始的位置
	do {
		if (p->op == '^') { OpRank = 3; PowNumber++; }
		else if ((p->op == '*' || p->op == '/') && OpRank < 3) OpRank = 2;
		p = p->prev;
	} while (p->op != '(');

	t = p;
	//正在处理的括号中的内容
	for (int i = 0; i < PowNumber; i++) {
		do {
			if (t->op == '^') {
				t->prev->num = pow(t->prev->num, t->next->num);
				t->prev->next = t->next->next;
				if (t->next->next) {
					t->next->next->prev = t->prev;//这里还是没有释放没有用的结构体单元，偷懒
				}
				break;
			}
			t = t->next;
		} while (t->op != ')');
	}

	//开始处理乘除法
	t = p;
	do {
		if (t->op == '*') {
			t->prev->num = Myplus(t->prev->num, t->next->num);
			t->prev->next = t->next->next;
			if (t->next->next) {
				t->next->next->prev = t->prev;//这里还是没有释放没有用的结构体单元，偷懒
			}
		}
		else if (t->op == '/') {
			t->prev->num = Mydivide(t->prev->num, t->next->num);
			t->prev->next = t->next->next;
			if (t->next->next) {
				t->next->next->prev = t->prev;//这里还是没有释放没有用的结构体单元，偷懒
			}
		}
		t = t->next;
	} while (t->op != ')');

	// 开始处理加减法
	t = p;
	do {
		if (t->op == '+') {
			t->prev->num = Myadd(t->prev->num, t->next->num);
			t->prev->next = t->next->next;
			if (t->next->next) {
				t->next->next->prev = t->prev;//这里还是没有释放没有用的结构体单元，偷懒
			}
		
		}
		else if (t->op == '-') {
			t->prev->num = Mysubtract(t->prev->num, t->next->num);
			t->prev->next = t->next->next;
			if (t->next->next) {
				t->next->next->prev = t->prev;//这里还是没有释放没有用的结构体单元，偷懒
			}	
		}
		t = t->next;
	} while (t->op != ')');
	//这里计算完成之后应该只剩下两个括号和一组数，接下来去除括号,将右括号变成一个数字
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
	Calculater *cal, *cu, *lat,*ter;// cal 用于存储表头，cu 用于保存中间结果, lat用于free 空结构体
	DotNumber *dot, *numb, *ber; // 用于存放小数后数字的双向链表
	Calculater *calhead, *calp,*calt;
	double RESULT = 0;
	bool dotplus = true;  //  小数点处理时候需要用到
	int pownumber = 0;   // 应用于计算次幂的计数
	int listRank = 0,Lbr=0, Rbr = 0, OpRank = 1;  //用于计数
	head = Creat();  //存放输入字符的栈

	printf("\n");

	p = head; // 开始第一遍遍历
	do{
		listRank++;
		printf("\n当前判断的字符为%c\n", p->c);

		if (!CheckChar(p->c)) 
		{  //这里开始对输入进行检测
			printf("\n输入存在不合法字符,按任意键退出程序\n");
			getchar();
			return 1;
		}
		else if (p ->next) 
		{
			node = p->next;
			if (CheckOperator(p->c, node->c)) { printf("\n输入格式不正确，按任意键退出程序\n"); getchar(); return 2; }
		}   // 操作符不能连续存在
		
		
		 if(IsBracket(p->c)) //对括号进行匹配
		{
			if (p->c == '(')  Lbr++ ;
			else Rbr++;
			if ((Lbr - Rbr) < 0)
			{

				printf("\n输入括号非法！按任意键退出程序\n");
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
		printf("\n输入括号非法！按任意键退出程序\n");
		getchar();
		return 3;
	}
	p = head;  // 将指针放置于链表头

	//简单的显示一下
	printf("\n正在尝试计算值\n");

	do {
		printf(" %c", p->c);
		p = p -> next;
	} while (p);
	p = head;  // 将指针放置于链表头

	/* 开始提取数字*/
	int expn=0,caln = 0; //用于链表计数
	char current_char;

	cal = ((Calculater *)malloc(sizeof(Calculater)));  //创建头节点
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
			if (dotplus) { // 之前没有读到小数点
				cu->num = 10 * int(cu->num) + int(current_char-'0');
				printf("\n当前cu->num 的值为 %lf\n", cu->num);
			}
			else {  //之前读到了小数点,现在又遇到了数字，入链表
				printf("\n之前读到了小数点,现在又遇到了数字，入链表\n");				
				dot->f = int(current_char - '0');
				printf("dot->f的值为%lf", dot->f);
				numb = ((DotNumber *)malloc(sizeof(DotNumber)));
				numb->prev = dot;
				numb->next = NULL;
				numb->f = 0;
				dot->next = numb;
				dot = numb;   //每增加一个数字就把当前指针向后移
			}
		}else {
			if (!dotplus && !IsNumber(current_char)) {  //之前读到了小数点且碰到了下一个字符，优先处理这个问题
				printf("\n之前读到了小数点且碰到了下一个字符，优先处理这个问题\n");
				do {
					cu->num = 0.1 * (dot->f) + 0.1*cu->num;
					dot = dot->prev;
					printf("\n当前cu->num 的值为 %lf\n", cu->num);
				} while (dot);

				dotplus = true;

				free(dot);

				dot = ((DotNumber *)malloc(sizeof(DotNumber)));
				dot->next = NULL;
				dot->prev = NULL;
				dot->f = 0;
			}

			if (current_char == '.') dotplus = false;  // 判断读入字符是否为小数点
			else dotplus = true;
			lat = ((Calculater *)malloc(sizeof(Calculater)));
			cu->next = lat;
			lat->prev = cu;
			lat->op = current_char;
			printf("\n新创建了Calculater结构体，并且将读入的运算符置入\n");
			cu = lat;	
		}

		if (cu->op != NULL) {  //操作符不为NULL ，则刚刚读入了运算符
			printf("\n操作符不为NULL ，则之前读入了运算符\n");
			lat = ((Calculater *)malloc(sizeof(Calculater)));
			cu->next = lat;
			lat->prev = cu;
			lat->op = NULL;
			lat->num = 0;
			cu = lat;  // 当前指针后移
		}

		p = p->next;
	} while (p);
	p = head;  // 将指针置于头结点
	cu->op = NULL;
	cu->next = NULL;
	printf("\n 输出处理过后的字符");
	cu = cal;

	printf("\n开始处理小数点\n");
	do {
		if (cu->op == '.') {
			// lat = cu; ter = cu->next; 这里想了想暂时不free掉两片空间了
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
	printf("\n小数点处理完毕,开始计算括号中内容\n");
	printf("OpRank is %d \n", OpRank);
	
	 calhead = calhead->next;  // calp 为临时指针
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

	 printf("\n开始处理负数\n");
	 calp = calhead;
	 do {
		 if (calp->op == '(' && calp->next->op == '-') {
			 calp->num = -1 * calp->next->next->num;
			 calp->op = NULL;
			 printf("\n处理了负数: %lf\n ", calp->num);

			 calp->next = calp->next->next->next->next;
			 if (calp->next) {
				 calp->next->prev = calp; // 跳过了好多
			 }
			 Lbr--;
			 Rbr--;
		 }
		 calp = calp->next;
	 } while (calp);
	 printf("\n正确处理了输入的表达式的最终值,正在计算结果。。。。。。。。。。。。\n");
	 



	 calp = calhead;
	 dot, numb, ber;  // 这三个变量之后会用到
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
	if (Lbr == 0) { // 无括号，直接计算
		if (OpRank == 3) { // 有次幂的计算，首先计算次幂附近的值，由于没有括号，直接搜索次幂
			//首先计算次幂
			for (int i = 0; i < pownumber; i++) {
				calp = calhead;
				do {
					if (calp->op == '^') {
						calp = calp->prev; // 碰到符号，将当前指针向前移位
						calp->num = pow(calp->num, calp->next->next->num);
						RESULT = calp->num;
						printf("\n中间结果为 ： %lf\n", RESULT);
						printf("\n%计算第%d '^' 的结果为%lf\n", i, calp->num);
						(calp->next) = (calp->next->next->next); 
						if(calp->next){ 
							calp->next->prev = calp;
						}// 第一个next 为符号，第二个next 为数字后，第三个next为应该指向的数字
						
						continue;
					}
					calp = calp->next;
				} while (calp);
			}
			
			
			calp = calhead;

			// 全部化成加减乘除法运算之后，首先计算所有的乘除法
			do {
				if (calp->op == '*' || calp->op == '/') {
					if (calp->op == '*') {
						calp = calp->prev;
						calp->num = Myplus(calp->num, calp->next->next->num);
						RESULT = calp->num;  //放入结果值
						printf("\n中间结果为 ： %lf\n", RESULT);
						(calp->next) = (calp->next->next->next);
						if (calp->next == NULL) {
							break;
						}
						else
							calp->next->prev = calp;
					}else{
						calp = calp->prev;
						calp->num = Mydivide(calp->num, calp->next->next->num);
						RESULT = calp->num;  //放入结果值
						printf("\n中间结果为 ： %lf\n", RESULT);
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

			//重置指针，计算加减后输出
			calp = calhead;
			do {
				if (calp->op == '+' || calp->op == '-') {
					if (calp->op == '+') {
						calp = calp->prev;
						calp->num = Myadd(calp->num, calp->next->next->num);
						RESULT = calp->num;  //放入结果值
						printf("\n中间结果为 ： %lf\n", RESULT);
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
						RESULT = calp->num;  //放入结果值
						printf("\n中间结果为 ： %lf\n", RESULT);
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
		else {  // 无次幂，直接从左到右计算
			printf("\n无次幂运算\n");
			calp = calhead;
			// 全部化成加减乘除法运算之后，首先计算所有的乘除法
			do {
				if (calp->op == '*' || calp->op == '/') {
					if (calp->op == '*') {
						calp = calp->prev;
						calp->num = Myplus(calp->num, calp->next->next->num);
						RESULT = calp->num;  //放入结果值
						printf("\n中间结果为 ： %lf\n", RESULT);
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
						RESULT = calp->num;  //放入结果值
						printf("\n中间结果为 ： %lf\n", RESULT);
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
			//重置指针，计算加减后输出
			calp = calhead;
			do {
				if (calp->op == '+' || calp->op == '-') {
					if (calp->op == '+') {
						calp = calp->prev;
						calp->num = Myadd(calp->num, calp->next->next->num);
						RESULT = calp->num;  //放入结果值
						printf("\n中间结果为 ： %lf\n", RESULT);
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
						RESULT = calp->num;  //放入结果值
						printf("\n中间结果为 ： %lf\n", RESULT);
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

	else {//有括号，首先计算括号

		//  去除所有括号,遇到右括号的时候调用之前的函数即可，最终的结果值保存在表达式第一个数字所对应的结构体中
		calp = calhead;
		// 第一遍遍历，去除所有括号
		/*do {
			if (calp->op == ')') {
				RESULT = GetBarketValue(calp);
				printf("\n括号中的表达式结果为: %lf \n",RESULT);
			}
			calp = calp->next;
		} while (calp);*/

		for (int i = 0; i < Lbr; i++) { // 每次循环去除一个括号后跳出
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
		//第二遍遍历
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
	calp = calhead;// 最终计算

	//计算次幂
	for (int i = 0; i < pownumber; i++) {
		calp = calhead;
		do {
			if (calp->op == '^') {
				calp = calp->prev; // 碰到符号，将当前指针向前移位
				calp->num = pow(calp->num, calp->next->next->num);
				RESULT = calp->num;
				printf("\n中间结果为 ： %lf\n", RESULT);
				printf("\n%计算第%d '^' 的结果为%lf\n", i, calp->num);
				(calp->next) = (calp->next->next->next);
				if (calp->next) {
					calp->next->prev = calp;
				}// 第一个next 为符号，第二个next 为数字后，第三个next为应该指向的数字

				continue;
			}
			calp = calp->next;
		} while (calp);
	}

	calp = calhead;
	// 计算乘除
	do {
		if (calp->op == '*' || calp->op == '/') {
			if (calp->op == '*') {
				calp = calp->prev;
				calp->num = Myplus(calp->num, calp->next->next->num);
				RESULT = calp->num;  //放入结果值
				printf("\n中间结果为 ： %lf\n", RESULT);
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
				RESULT = calp->num;  //放入结果值
				printf("\n中间结果为 ： %lf\n", RESULT);
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

	// 计算加减
	calp = calhead;
	do {
		if (calp->op == '+' || calp->op == '-') {
			if (calp->op == '+') {
				calp = calp->prev;
				calp->num = Myadd(calp->num, calp->next->next->num);
				RESULT = calp->num;  //放入结果值
				printf("\n中间结果为 ： %lf\n", RESULT);
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
				RESULT = calp->num;  //放入结果值
				printf("\n中间结果为 ： %lf\n", RESULT);
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

	printf("最终的结果为： %lf", RESULT);
	getchar();
	return 0;
}
