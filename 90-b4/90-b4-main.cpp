/* 学号 姓名 班级 */
#include <iostream>
#include "../common/cmd_console_tools.h"
using namespace std;

int main()
{
	int x, y;

	/* 显示16*16的配色框，每个字符之间加一个空格 */
	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++)
			showch(x * 2, y, 'x', y, x, 2);	//在xy位置显示x，用x坐标的值做前景色，y坐标的值做背景色（显示两个x是为了使比例协调，可以换成一个x后观察效果）
		cout << endl; //16个前景色变换完成后，换一行
		Sleep(100);  //延时0.1秒（以毫秒为单位，1000=1秒）
	} //end of for

	//恢复初始颜色
	setcolor();
	cout << endl << "90-b4" << endl << endl;

	return 0;
}
