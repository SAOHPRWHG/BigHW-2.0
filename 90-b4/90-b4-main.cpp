/* 1751957 计2班 俞少作 */
#include "90-b4.h"
#include "G2048.h"
using namespace std;

void Input(int * x, int lower, int upper, const char str[])
{
	while (1) {
		cout << str;
		cin >> *x;
		if (!cin.good()) {
			cin.clear();
			cin.ignore(1024, '\n');
		}

		else if (*x <= upper && *x >= lower)
			break;
	}
}

int main()
{
	int col, row;
	int dst_score;//目标分数
	int speed;//延时等级

	G2048 g2048;
	while (1) {
		cls();
		Input(&row, MIN_FRAME, MAX_FRAME, "请输入行数[4-8]：");
		Input(&col, MIN_FRAME, MAX_FRAME, "请输入列数[4-8]：");
		Input(&dst_score, 8192, 65536, "请输入目标分数[8192-65536]：");
		Input(&speed, 0, 5, "请输入动画延时[0-5]：");

		g2048.set(row, col, dst_score, speed);


		/*test
		g2048.Init_map();
		//g2048.print_map();
		//getchar(); getchar(); getchar();
		//g2048.Check_remove(UP_TO_DOWN);
		//g2048.print_map();
		//getchar(); getchar(); getchar();	
		//g2048.Check_remove(DOWN_TO_UP);
		//g2048.print_map();
		//getchar(); getchar(); getchar();
		
		//g2048.print_map();
		//getchar(); getchar(); getchar();
		//g2048.Align(LEFT_TO_RIGHT);
		g2048.print_map();
		getchar(); getchar(); getchar();
		g2048.Check_remove(LEFT_TO_RIGHT);
		g2048.print_map();
		getchar(); getchar(); getchar();
		g2048.Align(RIGHT_TO_LEFT);
		g2048.print_map();
		getchar(); getchar(); getchar();
		g2048.Check_remove(RIGHT_TO_LEFT);
		g2048.print_map();
		getchar(); getchar(); getchar();*/
		
		switch (g2048.Game_start())
		{
			case 'R':
			case 'r':
				continue;
				break;
			case 'Q':
			case 'q':
				return 0;
				break;
		}
	}

	return 0;
}
