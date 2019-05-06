/* 学号 姓名 班级 */
#include <iostream>
#include "../common/cmd_console_tools.h"
using namespace std;

int main()
{
	const int MAX_X = 50;
	const int Y = 2;
	const int bg_color = COLOR_HYELLOW;	//背景为亮黄色
	const int fg_color = COLOR_HBLUE;		//前景为亮蓝色
	const char ch = '*';        //字符为*(注意：如果是空格，则前景色无法显示)
	const int LENGTH = 10;
	int x;

	/* 将一串字符从左向右移动 */
	for (x = 0; x < MAX_X; x++) {
		/* 在坐标(x,2)位置处连续打印10个字符 */
		showch(x, Y, ch, bg_color, fg_color, LENGTH);

		/* 延时0.1秒 */
		Sleep(100);

		if (x < MAX_X - 1) {
			/* 清除显示(最后一次保留)，清除方法为用正常颜色+空格重画一遍刚才的位置 */
			showch(x, Y, ' ', COLOR_BLACK, COLOR_WHITE, LENGTH);
		}
	} //end of for

	//恢复初始颜色
	setcolor();
	cout << endl << "90-b5" << endl << endl;

	return 0;
}
