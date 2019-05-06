/* 1751957 计2班 俞少作 */
#include <iostream>
#include "../common/cmd_console_tools.h"
#include "90-b1.h"
using namespace std;

void Input(char * x, int lower, int upper,const char except, const char str[])
{
	while (1) {
		cout << str;
		char ch;
		while ((ch = getchar()) != '\n')
			;
		cin >> *x;
		if (*x >= 'a'&&*x <= 'z')
			*x -= 32;

		if (!cin.good()) {
			cin.clear();
			cin.ignore(1024, '\n');
		}

		else if (*x <= upper && *x >= lower|| *x==except) {
			break;
		}
		cout << "输入非法，请重新输入" << endl;
	}
}

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

void Read_enter(int Y, const char str[])
{
	gotoxy(0, Y);
	cout << str;
	while (_getch() != '\r')
		;
}

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

void Init_array(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int i, j;

	//生成随机数种子
	unsigned int t;
	t = (unsigned int)time(0);
	srand(t);

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (!map[i][j].value) {
				map[i][j].value = rand() % 9 + 1;
			}
		}
	}

}

int Check(struct ball_info map[][MAX_FRAME], int row, int col)
{
/***************************************************************************
	函数名称：
	功    能：检查内部数组,返回标记个数
	输入参数：
	返 回 值：
	说    明：将检查出的连子status标记为1
***************************************************************************/
	int i, j, k, count = 0;
	//行检查
	for (i = 0; i < row; i++) {
		for (j = 0; j + 2 < col; j++) {
			if (map[i][j + 1].value == map[i][j].value) {
				if (map[i][j + 2].value == map[i][j + 1].value) {
					map[i][j].status = WILL_REMOVE;
					map[i][j + 1].status = WILL_REMOVE;
					map[i][j + 2].status = WILL_REMOVE;
					for (k = j + 2; map[i][k + 1].value == map[i][k].value && k + 1 < col; k++) {
						map[i][k + 1].status = WILL_REMOVE;
					}	
				}
			}
		}
	}
	//列检查
	for (j = 0; j < col; j++) {
		for (i = 0; i + 2 < row; i++) {
			if (map[i + 1][j].value == map[i][j].value) {
				if (map[i + 2][j].value == map[i + 1][j].value) {
					map[i][j].status = WILL_REMOVE;
					map[i + 1][j].status = WILL_REMOVE;
					map[i + 2][j].status = WILL_REMOVE;
					for (k = i + 2; map[k + 1][j].value == map[k][j].value && k + 1 < row; k++) {
						map[k + 1][j].status = WILL_REMOVE;
					}
				}
			}
		}
	}

	//检查标记数量
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j].status == WILL_REMOVE)
				count++;
		}
	}
	int coef = 2 * (MAX_FRAME + 1) - row - col;
	return coef*count;
}

void Remove_ball(struct ball_info map[][MAX_FRAME], int row, int col)
{
	/***************************************************************************
		  函数名称：
		  功    能：将WILL_REMOVE状态的球的value置为0
		  输入参数：
		  返 回 值：
		  说    明：同时将状态置回NORMAL
	***************************************************************************/

	int i, j;
	//检查标记数量
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j].status == WILL_REMOVE) {
				map[i][j].value = 0;
				map[i][j].status = NORMAL;
			}

		}
	}
}

void Fallen(struct ball_info map[][MAX_FRAME], int row, int col,const int GUI)
{
/***************************************************************************
		函数名称：
		功    能：将上方的球落下填补中间的空位
		输入参数：GUI是否输出图形化，默认为0，不输出
		返 回 值：
		说    明：自下而上扫描，同时将上方待填充的0空位的status置为NEW_FILL
***************************************************************************/

	int i, j,k;
	Remove_ball(map, row, col);

	//自下而上扫描
	for (j = 0; j < col; j++) {
		for (i = row-1; i >= 0; i--) {
			if (map[i][j].value==0) {
				for (k = i - 1; k >= 0 && !map[k][j].value; k--)
					;//找到上方不为零且不出界的球
				if (k >= 0) {
					map[i][j].value = map[k][j].value;
					map[k][j].value = 0;
					if (GUI) {
						Fallen_GUI(k, j, i, j, map);//显示下落轨迹
					}
				}
				//出界则说明上方全为0
			}

		}
	}
	//打上待填充标记
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j].value == 0) {
				map[i][j].status = NEW_FILL;
			}
		}
	}

}

void Fill(struct ball_info map[][MAX_FRAME], int row, int col,const int GUI)
{
	/***************************************************************************
	  函数名称：
	  功    能：填充顶部为0的空位
	  输入参数：
	  返 回 值：
	  说    明：不改变status以便界面显示，之后会用Clear_status将状态清理为0
	***************************************************************************/
	//生成随机数种子
	unsigned int t;
	t = (unsigned int)time(0);
	srand(t);
	
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j].value == 0) {
				map[i][j].value = rand() % 9 + 1;
				map[i][j].bg_color = (map[i][j].value + 4) % 16;
				if (GUI) {
					draw_the_ball(map[i][j]);
					Sleep(100);
				}				
			}
		}
	}
	setcolor(3, 15);
}

void Clear_status(struct ball_info map[][MAX_FRAME], int row, int col)
{
	/***************************************************************************
	  函数名称：
	  功    能：将非0的status清理为NORMAL
	  输入参数：
	  返 回 值：
	  说    明：打印完新填充的base界面之后，清理标记状态
	***************************************************************************/

	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j].status) {
				map[i][j].status = NORMAL;
			}
		}
	}
}

int scanx(int aim_row, struct ball_info map[][MAX_FRAME], int col)
{
	int j,flag=0;
	for (j = 0; j+2 < col; j++) {
		if (map[aim_row][j + 1].value == map[aim_row][j].value) {
			if (map[aim_row][j + 2].value == map[aim_row][j + 1].value) {
				flag++;
			}
		}
	}
	return flag;
}

int scany(int aim_col, struct ball_info map[][MAX_FRAME], int row)
{
	int i, flag = 0;
	for (i = 0; i+2 < row; i++) {
		if (map[i + 1][aim_col].value == map[i][aim_col].value) {
			if (map[i + 2][aim_col].value == map[i + 1][aim_col].value) {
				flag++;
			}
		}
	}
	return flag;
}

void exchange_ball(int r1, int c1, int r2, int c2, struct ball_info map[][MAX_FRAME])
{
	/***************************************************************************
		函数名称：
		功    能：交换map[r1][c1]和map[r2][c2]的value和status
		输入参数：
		返 回 值：0
		说    明：
	***************************************************************************/
	int tem;
	tem = map[r1][c1].value;
	map[r1][c1].value = map[r2][c2].value;
	map[r2][c2].value = tem;

	tem = map[r1][c1].status;
	map[r1][c1].status = map[r2][c2].status;
	map[r2][c2].status = tem;
}

int exchange_check(int aimr, int aimc, struct ball_info map[][MAX_FRAME], int row, int col)
{
/***************************************************************************
	函数名称：
	功    能：目标球分别与右方和下方的球交换，并检查此时的行列是否可达到消除
	输入参数：aimr目标行，aimc目标列
	返 回 值：flag,可交换的组数
	说    明：先判断是否可交换，如果可达成消除则将status标记为PRE_REMIND
***************************************************************************/
	int flag = 0;
	//右侧可交换
	if (aimc + 1 < col) {
		//交换
		exchange_ball(aimr, aimc, aimr, aimc + 1, map);
		//扫描检查
		//只检查交换的两球所在的行列即可，此处一行两列
		if (scanx(aimr, map, col) + scany(aimc, map, row) + scany(aimc + 1, map, row)) {
			map[aimr][aimc].status = PRE_REMIND;
			map[aimr][aimc + 1].status = PRE_REMIND;
			flag++;
		}
		//换回来
		exchange_ball(aimr, aimc, aimr, aimc + 1, map);
	}

	//下方交换
	if (aimr + 1 < row) {
		//交换
		exchange_ball(aimr, aimc, aimr + 1, aimc, map);
		//扫描检查
		//只检查交换的两球所在的行列即可，此处一列两行
		if (scanx(aimr, map, col) + scanx(aimr + 1, map, col) + scany(aimc, map, row)) {
			map[aimr][aimc].status = PRE_REMIND;
			map[aimr + 1][aimc].status = PRE_REMIND;
			flag++;
		}
		//换回来
		exchange_ball(aimr, aimc, aimr + 1, aimc, map);
	}
	return flag;
}

int Pre_reminder(struct ball_info map[][MAX_FRAME], int row, int col,const int GUI)
{
/***************************************************************************
	函数名称：
	功    能：消除提示
	输入参数：
	返 回 值：返回值为0则无可消除球，游戏结束。
	说    明：
***************************************************************************/
	int count = 0;
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			count += exchange_check(i,j,map,row,col);
			if (GUI) {
				draw_the_ball(map[i][j]);
				Sleep(20);
			}
		}
	}
	setcolor(3, 15);
	return count;
}

int Judge_select(int r1,int c1,int r2,int c2,struct ball_info map[][MAX_FRAME], int row, int col)
{
/***************************************************************************
	函数名称：
	功    能：判断选中的球交换后是否能达成消除
	输入参数：
	返 回 值：有值为合法，0为非法不能达成消除
	说    明：先交换，再Check一次，若为真则直接交换两球的值，若非法，则换回来
***************************************************************************/
	int tem,judge;
	tem = map[r1][c1].value;
	map[r1][c1].value = map[r2][c2].value;
	map[r2][c2].value = tem;

	judge = Check(map, row, col);
	if (!judge) {
		tem = map[r1][c1].value;
		map[r1][c1].value = map[r2][c2].value;
		map[r2][c2].value = tem;
	}
	
	return judge;
}


