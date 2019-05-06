/* 1751957 ��2�� ������ */
#include <iostream>
#include "../common/cmd_console_tools.h"
#include "90-b1.h"
using namespace std;

const int OPT_NUM = 10;

void GameA(struct ball_info map[][MAX_FRAME], int row, int col);
void GameB(struct ball_info map[][MAX_FRAME], int row, int col);
void GameC(struct ball_info map[][MAX_FRAME], int row, int col);
void GameD(struct ball_info map[][MAX_FRAME], int row, int col);
void GameE(struct ball_info map[][MAX_FRAME], int row, int col);
void GameF(struct ball_info map[][MAX_FRAME], int row, int col);
void GameG(struct ball_info map[][MAX_FRAME], int row, int col);
void GameH(struct ball_info map[][MAX_FRAME], int row, int col);
void GameI(struct ball_info map[][MAX_FRAME], int row, int col);

int tj_strchr(const char str[], char ch)
{
	/* ע�⣺�����ڲ��������κ���ʽ�����飨������̬���飩 */
	int i, len, flag = 0;
	len = (int)strlen(str);//�����ʣ��޷�ʶ�������ڲ�'\0'
	for (i = 0; i < len; i++) {
		if (ch == str[i]) {
			flag = 1;
			break;
		}

	}
	if (flag == 1)
		return i + 1;//returnֵ�ɸ�����Ҫ�޸�
	else
		return 0;
}

char menu_choose(const char *tab)
{
	//������ѡ��tab����������
	char m;
	while (1) {
		m = _getche();
		if (m >= 'a'&&m <= 'z')
			m -= 32;
		if (tj_strchr(tab,m)){
			break;
		}
	}
	return m;
}

char menu(const char *array[],const char sepline,const char *tab)
{
	setfontsize("", 14);
	setconsoleborder(120, 60, 600, 600);
	
	int len=0,i;
	char m; 
	//�����ַ�����󳤶�
	for (i = 0; i < OPT_NUM; i++) {
		if ((int)strlen((array)[i]) > len)
			len = (int)strlen(array[i]);
	}
	
	//��ӡ�Ϸָ���
	for (i = 0; i <= len; i++)
		cout << sepline;
	cout << endl;
	//��ӡ����
	for (i = 0; i < OPT_NUM; i++) 
		cout << array[i]<< endl;
	//��ӡ�·ָ���
	for (i = 0; i <= len; i++)
		cout << sepline;
	cout << endl;

	cout << "[��ѡ��]��";

	m = menu_choose(tab);
	return m;
}

int main()
{
	//�ظ���ʾ5��"Hello"
	showstr(5, 5, "90-b1*", COLOR_HYELLOW, COLOR_HBLUE, 5);

	setconsoletitle("Magic Ball");

	const char *op[] =
	{ "A.�ڲ����飬���ɳ�ʼ״̬��Ѱ���Ƿ��г�ʼ��������",
	"B.�ڲ����飬������ʼ����������0�����䲢��0���",
	"C.�ڲ����飬������ʼ������������������ʾ",
	"D.n*n�Ŀ��(�޷ָ���)����ʾ��ʼ״̬",
	"E.n*n�Ŀ��(�зָ���)����ʾ��ʼ״̬",
	"F.n*n�Ŀ��(�зָ���)����ʾ��ʼ״̬����ʼ��������",
	"G.n*n�Ŀ��(�зָ���)��������ʼ�����������ʾ������ʾ",
	"H.cmdͼ�ν���������",
	"I.���ļ��ж�ȡ��������֤����������ʾ���㷨����ȷ��",
	"Q.�˳�"};
	
	while (1) {
		struct ball_info map[MAX_FRAME][MAX_FRAME];
		int col, row;
		int m;
		setcolor(3,15);
		
		m = menu(op, '-',"ABCDEFGHIQ");
		cls();

		Input(&row, MIN_FRAME, MAX_FRAME, "����������(5-9)��");
		Input(&col, MIN_FRAME, MAX_FRAME, "����������(5-9)��");
		
		switch (m) 
		{
			case 'Q':
				return 0;
				break;
			case 'A':
				GameA(map, row, col);
				break;
			case 'B':
				GameB(map, row, col);
				break;
			case 'C':
				GameC(map, row, col);
				break;
			case 'D':
				GameD(map, row, col);
				break;
			case 'E':
				GameE(map, row, col);
				break;
			case 'F':
				GameF(map, row, col);
				break;
			case 'G':
				GameG(map, row, col);
				break;
			case 'H':
				GameH(map, row, col);
				break;
			case 'I':
				GameI(map, row, col);
				break;
		}
	}
	return 0;
}
