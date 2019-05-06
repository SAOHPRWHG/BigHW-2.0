/* 1751957 计2班 俞少作 */
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
	/* 注意：函数内不允许定义任何形式的数组（包括静态数组） */
	int i, len, flag = 0;
	len = (int)strlen(str);//有疑问，无法识别并跳过内部'\0'
	for (i = 0; i < len; i++) {
		if (ch == str[i]) {
			flag = 1;
			break;
		}

	}
	if (flag == 1)
		return i + 1;//return值可根据需要修改
	else
		return 0;
}

char menu_choose(const char *tab)
{
	//负责解决选项tab的输入问题
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
	//计算字符串最大长度
	for (i = 0; i < OPT_NUM; i++) {
		if ((int)strlen((array)[i]) > len)
			len = (int)strlen(array[i]);
	}
	
	//打印上分割线
	for (i = 0; i <= len; i++)
		cout << sepline;
	cout << endl;
	//打印内容
	for (i = 0; i < OPT_NUM; i++) 
		cout << array[i]<< endl;
	//打印下分割线
	for (i = 0; i <= len; i++)
		cout << sepline;
	cout << endl;

	cout << "[请选择]：";

	m = menu_choose(tab);
	return m;
}

int main()
{
	//重复显示5次"Hello"
	showstr(5, 5, "90-b1*", COLOR_HYELLOW, COLOR_HBLUE, 5);

	setconsoletitle("Magic Ball");

	const char *op[] =
	{ "A.内部数组，生成初始状态，寻找是否有初始可消除项",
	"B.内部数组，消除初始可消除项后非0项下落并用0填充",
	"C.内部数组，消除初始可消除项后查找消除提示",
	"D.n*n的框架(无分隔线)，显示初始状态",
	"E.n*n的框架(有分隔线)，显示初始状态",
	"F.n*n的框架(有分隔线)，显示初始状态及初始可消除项",
	"G.n*n的框架(有分隔线)，消除初始可消除项后显示消除提示",
	"H.cmd图形界面完整版",
	"I.从文件中读取数据以验证查找消除提示的算法的正确性",
	"Q.退出"};
	
	while (1) {
		struct ball_info map[MAX_FRAME][MAX_FRAME];
		int col, row;
		int m;
		setcolor(3,15);
		
		m = menu(op, '-',"ABCDEFGHIQ");
		cls();

		Input(&row, MIN_FRAME, MAX_FRAME, "请输入行数(5-9)：");
		Input(&col, MIN_FRAME, MAX_FRAME, "请输入列数(5-9)：");
		
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
