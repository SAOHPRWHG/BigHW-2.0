/* 1751957 ��2�� ������ */
#include "../common/cmd_hdc_tools.h"
#include "../common/cmd_gmw_tools.h"
#include "90-b0.h"
using namespace std;
/*����������ʦ��
     �ǳ���Ǹ�����δ���ҵ��������ԭ��δ����ȫʵ�ֶ�90-b0��90-b1���ݵ��ع����Բ�����*/

int menu();
void Game1(int a[][MAX_FRAME], int row, int col);
void Game2(int a[][MAX_FRAME], int row, int col);
void Game3(int a[][MAX_FRAME], int row, int col);
void Game4(int a[][MAX_FRAME], int row, int col);
void Game5(int a[][MAX_FRAME], int row, int col);
void Game6(int a[][MAX_FRAME], int row, int col);
void Game7(int a[][MAX_FRAME], int row, int col);
void Game8(int a[][MAX_FRAME], int row, int col);

int main()
{
	showstr(10, 10, "This is 90-b0\n\n");
	while (1) {
		int a[MAX_FRAME][MAX_FRAME] = { 0 };
		int col, row;
		int m;
		setcolor(3,15);

		m = menu();
		cls();
		Input(&row, MIN_FRAME, MAX_FRAME, "����������(7-9)��");
		Input(&col, MIN_FRAME, MAX_FRAME, "����������(7-9)��");
		switch (m)
		{
			case 0:
				return 0;
				break;
			case 1:
				Game1(a, row, col);
				break;
			case 2:
				Game2(a, row, col);
				break;
			case 3:
				Game3(a, row, col);
				break;
			case 4:
				Game4(a, row, col);
				break;
			case 5:
				Game5(a, row, col);
				break;
			case 6:
				Game6(a, row, col);
				break;
			case 7:
				Game7(a, row, col);
				break;
			case 8:
				Game8(a, row, col);
				break;
		}
	}
	return 0;
}

//��ʾ�˵������ز˵�ѡ��ֵ
int menu()
{
	int m;
	/*
--------------------------------------------
1.�ڲ����飬������ɳ�ʼ5����
2.�ڲ����飬�������60%����Ѱ���ƶ�·��
3.�ڲ����飬������
4.����n*n�Ŀ�ܣ��޷ָ��ߣ��������ʾ5����
5.����n*n�Ŀ�ܣ��зָ��ߣ��������ʾ5����
6.n*n�Ŀ�ܣ�60%����֧����꣬���һ���ƶ�
7.cmdͼ�ν���������
8.cmdͼ�ν���������-֧��ͬʱ����������ӷ֣�
0.�˳�
--------------------------------------------
[��ѡ��0-8]
	*/
	setfontsize("", 14);
	setconsoleborder(120, 60, 600, 600);
	setconsoletitle("Color linez");
	cout << "---------------------------------" << endl;
	cout << "1.�ڲ����飬������ɳ�ʼ5����" << endl;
	cout << "2.�ڲ����飬�������60%����Ѱ���ƶ�·��" << endl;
	cout << "3.�ڲ����飬������ " << endl;
	cout << "4.����n*n�Ŀ�ܣ��޷ָ��ߣ��������ʾ5����" << endl;
	cout << "5.����n*n�Ŀ�ܣ��зָ��ߣ��������ʾ5����" << endl;
	cout << "6.n*n�Ŀ�ܣ�60%����֧����꣬���һ���ƶ�" << endl;
	cout << "7.cmdͼ�ν���������" << endl;
	cout << "8.cmdͼ�ν���������-֧��ͬʱ����������ӷ֣�" << endl;
	cout << "0.�˳�" << endl;
	cout << "---------------------------------" << endl;
	cout << "[��ѡ��:]";
	while (1) {
		m = _getche();
		if (m >= '0' && m <= '8')
			break;
	}
	return m - '0';
}