/* 1751957 计2班 俞少作 */
#include "../common/cmd_hdc_tools.h"
#include "../common/cmd_gmw_tools.h"
#include "90-b0.h"
using namespace std;
/*敬爱的沈老师：
     非常抱歉，本次大作业由于种种原因未能完全实现对90-b0及90-b1内容的重构，对不起您*/

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
		Input(&row, MIN_FRAME, MAX_FRAME, "请输入行数(7-9)：");
		Input(&col, MIN_FRAME, MAX_FRAME, "请输入列数(7-9)：");
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

//显示菜单，返回菜单选项值
int menu()
{
	int m;
	/*
--------------------------------------------
1.内部数组，随机生成初始5个球
2.内部数组，随机生成60%的球，寻找移动路径
3.内部数组，完整版
4.画出n*n的框架（无分隔线），随机显示5个球
5.画出n*n的框架（有分隔线），随机显示5个球
6.n*n的框架，60%的球，支持鼠标，完成一次移动
7.cmd图形界面完整版
8.cmd图形界面完整版-支持同时读键（额外加分）
0.退出
--------------------------------------------
[请选择0-8]
	*/
	setfontsize("", 14);
	setconsoleborder(120, 60, 600, 600);
	setconsoletitle("Color linez");
	cout << "---------------------------------" << endl;
	cout << "1.内部数组，随机生成初始5个球" << endl;
	cout << "2.内部数组，随机生成60%的球，寻找移动路径" << endl;
	cout << "3.内部数组，完整版 " << endl;
	cout << "4.画出n*n的框架（无分隔线），随机显示5个球" << endl;
	cout << "5.画出n*n的框架（有分隔线），随机显示5个球" << endl;
	cout << "6.n*n的框架，60%的球，支持鼠标，完成一次移动" << endl;
	cout << "7.cmd图形界面完整版" << endl;
	cout << "8.cmd图形界面完整版-支持同时读键（额外加分）" << endl;
	cout << "0.退出" << endl;
	cout << "---------------------------------" << endl;
	cout << "[请选择:]";
	while (1) {
		m = _getche();
		if (m >= '0' && m <= '8')
			break;
	}
	return m - '0';
}