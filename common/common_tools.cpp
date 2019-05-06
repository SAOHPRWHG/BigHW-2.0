/* 1751957 计2班 俞少作 */
#include <iostream>
#include "../common/cmd_console_tools.h"
#include "common.h"
using namespace std;


//菜单函数
char menu_choose(const char *tab)
{
	//负责解决选项tab的输入问题
	char m;
	while (1) {
		m = _getche();
		if (m >= 'a'&&m <= 'z')
			m -= 32;
		if (tj_strchr(tab, m)) {
			break;
		}
	}
	return m;
}

char menu(const char *array[], const char sepline, const char *tab)
{
	setfontsize("", 14);
	setconsoleborder(120, 60, 600, 600);

	unsigned int len = 0, i;
	char m;
	//计算字符串最大长度
	for (i = 0; i < OPT_NUM; i++) {
		if (strlen((array)[i]) > len)
			len = strlen(array[i]);
	}

	//打印上分割线
	for (i = 0; i <= len; i++)
		cout << sepline;
	cout << endl;
	//打印内容
	for (i = 0; i < OPT_NUM; i++)
		cout << array[i] << endl;
	//打印下分割线
	for (i = 0; i <= len; i++)
		cout << sepline;
	cout << endl;

	cout << "[请选择]：";

	m = menu_choose(tab);
	return m;
}

//打印内部数组界面函数
void Interface_array(int x, int y, struct ball_info map[][MAX_FRAME], int row, int col, const char str[], int sign_status)
{
	/***************************************************************************
	  函数名称：
	  功    能：显示数组状态下的界面
	  输入参数：x,y为打印位置，row，col为行列,opt为选择头文字
	  返 回 值：
	  说    明：sign_status为本次高亮标记的球的状态
	***************************************************************************/
	int X, Y;//用于获取的辅助位置坐标
	int i, j;
	gotoxy(x, y);

	cout << str << endl;
	/*if (opt == 1)
		cout << "初始可消除项（不同色标识）：" << endl;
	else if (opt == -1)
		cout << "移动后数组：" << endl;
	else if (opt == 2)
		cout << "下落除0后的数组(不同色标识)：" << endl;
	else if(opt==3)
	else
		cout << "初始数组：" << endl;*/
		//开始画表·
	cout << "  |";
	for (i = 0; i < col; i++) {
		cout << right << setw(3) << i;
	}
	getxy(X, Y);
	cout << endl << "--+";
	for (i = 3; i <= X; i++)
		cout << "-";
	cout << endl;
	for (i = 0; i < row; i++) {
		cout << left << setw(2) << char('A' + i) << "|";
		for (j = 0; j < col; j++) {
			if (map[i][j].status == sign_status) {
				cout << "  ";
				getxy(X, Y);
				showch(X, Y, char(map[i][j].value + '0'), COLOR_HYELLOW, COLOR_BLACK, 1);
				setcolor(3, 15);
			}
			else
				cout << right << setw(3) << map[i][j].value;
		}
		cout << endl;
	}


}

//打印行列标函数
void print_rc_sign(int X, int Y, int separator, int rc_sign, int row, int col, const char c[], const char r[])
{
	int i, gap = 1;
	if (rc_sign) {
		cout << "   ";
		if (separator) {
			gap = 2;
		}
		for (i = 0; i < col; i++) {
			cout << setw(2 * gap) << c[i];
		}
		for (i = 0; i < row; i++) {
			gotoxy(X, Y + gap * i + 2);
			cout << left << setw(2) << r[i];
		}
		gotoxy(X + 2, Y + 1);
	}
}

//显示外框架函数
void print_GUI(const char *frame[], int separator, int rc_sign, struct ball_info map[][MAX_FRAME], int row, int col, int opt)
{
	gotoxy(0, 0);
	int X, Y;
	int cols, lines, buffer_cols, buffer_lines;
	getconsoleborder(cols, lines, buffer_cols, buffer_lines);
	cout << "屏幕：" << cols << "行" << lines << "列";
	if (opt)
		cout << "(双击右键退出)";
	cout << endl;
	getxy(X, Y);
	print_rc_sign(X, Y, separator, rc_sign, row, col, "012345678", "ABCDEFGHI");
	getxy(X, Y);

	//开始画框
	if (!separator) {
		showstr(X, Y, frame[0], COLOR_HWHITE, COLOR_BLACK);
		showstr(X + 2, Y, frame[1], COLOR_HWHITE, COLOR_BLACK, col);
		showstr(X + 2 + col * 2, Y, frame[2], COLOR_HWHITE, COLOR_BLACK);
		// "╔═╗║║═╝"
		cout << endl;
		for (int i = 1; i <= row; i++) {
			showstr(X, Y + i, frame[3], COLOR_HWHITE, COLOR_BLACK);
			showstr(X + 2, Y + i, frame[4], COLOR_HWHITE, COLOR_BLACK, col);
			showstr(X + 2 + col * 2, Y + i, frame[5], COLOR_HWHITE, COLOR_BLACK);
			cout << endl;
		}
		showstr(X, Y + 1 + row, frame[6], COLOR_HWHITE, COLOR_BLACK);
		showstr(X + 2, Y + 1 + row, frame[7], COLOR_HWHITE, COLOR_BLACK, col);
		showstr(X + 2 + col * 2, Y + 1 + row, frame[8], COLOR_HWHITE, COLOR_BLACK);
	}
	else {
		//frame_with_sep = {"╔═","╦═","╗","║　","║","╠═","╬═","╣","║　","║","╚═","╩═","╝"}
		const int b = 4;
		showstr(X, Y, frame[0], COLOR_HWHITE, COLOR_BLACK);
		showstr(X + b, Y, frame[1], COLOR_HWHITE, COLOR_BLACK, col - 1);
		showstr(X + col * b, Y, frame[2], COLOR_HWHITE, COLOR_BLACK);
		showstr(X, Y + 1, frame[3], COLOR_HWHITE, COLOR_BLACK, col);
		showstr(X + col * b, Y + 1, frame[4], COLOR_HWHITE, COLOR_BLACK);

		//║═ ╗╔
		cout << endl;
		for (int i = 1; i < row; i++) {
			showstr(X, Y + 2 * i, frame[5], COLOR_HWHITE, COLOR_BLACK);
			showstr(X + b, Y + 2 * i, frame[6], COLOR_HWHITE, COLOR_BLACK, col - 1);
			showstr(X + col * b, Y + 2 * i, frame[7], COLOR_HWHITE, COLOR_BLACK);
			showstr(X, Y + 2 * i + 1, frame[8], COLOR_HWHITE, COLOR_BLACK, col);
			showstr(X + col * b, Y + 2 * i + 1, frame[9], COLOR_HWHITE, COLOR_BLACK);
			cout << endl;
		}
		showstr(X, Y + 2 * row, frame[10], COLOR_HWHITE, COLOR_BLACK);
		showstr(X + b, Y + 2 * row, frame[11], COLOR_HWHITE, COLOR_BLACK, col - 1);
		showstr(X + col * b, Y + 2 * row, frame[12], COLOR_HWHITE, COLOR_BLACK);

	}

	//画球
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			map[i][j].row = i;
			map[i][j].col = j;
			map[i][j].separator = separator;
			map[i][j].rc_sign = rc_sign;
			map[i][j].bg_color = (map[i][j].value + 4) % 16;
			draw_the_ball(map[i][j]);
		}
	}
	setcolor(3, 15);
}

//显示一个小球函数
void draw_the_ball(struct ball_info ball)
{
	const char *str[] = { "○","◎","★","●","○","○" };
	showstr(cx(ball.col, ball.separator, ball.rc_sign), cy(ball.row, ball.separator, ball.rc_sign), str[ball.status], ball.bg_color, ball.front_color);
}

//通用输入函数
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

//End函数
void End(int Y)
{
	/***************************************************************************
		  函数名称：
		  功    能：在第Y行开始，输入End返回菜单
		  输入参数：
		  返 回 值：
		  说    明：大小写均可
	***************************************************************************/
	int X;
	const char str[] = "本小题结束，请输入end继续...";
	char x[3];
	char* p = x;
	char ch1[] = "end";
	char ch;

	setcolor(3, 15);
	gotoxy(0, Y);
	while (1) {
		cout << str;
		getxy(X, Y);
		cout << "       ";
		gotoxy(X, Y);
		while ((ch = getchar()) != '\n')
			;
		for (p = x; (p - x) < 3; p++) {
			cin >> *p;
		}
		if (!cin.good()) {
			cin.clear();
			cin.ignore(1024, '\n');
		}

		else {
			//统一大小写
			for (p = x; *p; p++) {
				if (*p >= 'A'&&*p <= 'Z')
					*p += 32;
			}

			if (x[0] == ch1[0] && x[1] == ch1[1] && x[2] == ch1[2])
				break;
		}
		cout << "输入错误，请重新输入" << endl;
		gotoxy(0, Y);
	}
	cls();
}

/*
亲爱的沈老师：
	由于时间来不及，公共函数未来得及在Color Linez中完成实现细节的彻底修改，故提交了之前的版本，
但所有的公共函数都考虑了兼容性并加以改进，望您谅解。
*/
