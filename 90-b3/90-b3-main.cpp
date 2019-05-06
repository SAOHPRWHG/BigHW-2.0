/* 1751957 计2班 俞少作 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include "../common/cmd_console_tools.h"
#include "../common/cmd_hdc_tools.h"
using namespace std;

/*本程序在新版控制台下表现较好，旧版控制台存在一点边框绘制问题，望您理解^_^*/

/*定义菜单位置坐标架*/
const int MX0 = 42;
const int MY0 = 14;
const int MX1 = MX0 + 2;
const int MY1 = MY0 + 1;
const int col = 8;
const int row = 15;
/*定义文本位置坐标架：
每行75个字，待两侧边框总宽度78
高度20行
*/
const int X0 = 34;
const int Y0 = 11;
const int X1 = X0 + 2;
const int Y1 = Y0 + 1;
const int COL = 38;
const int ROW = 20;
const int NUM_CHAR = 2*COL-1;

const char *frame[] = { "┏","━━","┓","┃","　","┃","┗","━━","┛" };

/*定义菜单行数等信息*/
const int BOOK_MAX = 100;//菜单最多容纳书数量

static char buf1[1024 / NUM_CHAR * 1024 * 20][NUM_CHAR + 1];//分行缓冲变量，最大20M
static char bookname[BOOK_MAX][FILENAME_MAX];
static char bookshort[BOOK_MAX][col*2+1];

/*定义提示选项位置信息*/
const int X2 = 11;
const int Y2 = 11+4;

static struct option {
	int pos;//0-9
	int No;//全部
	int sum;//总数
}op{0,0};

int menu_keyboard_control();
void hdc_draw_yinyang(int point_x, int point_y, int r);
int split_line_buf(char address[],int mode = 1);
int read_saveinfo(const int linenum);
int Read_keyboard_control(int* pos, int linenum, char*address);
int tj_strstr(const char str[], const char substr[])
{
	/* 注意：函数内不允许定义任何形式的数组（包括静态数组） */
	int i, j, len, len_sub, flag = 0;
	len_sub = strlen(substr);
	len = strlen(str);//有疑问，无法识别并跳过内部'\0'
	for (i = len - 1; i >= 0; i--) {
		if (substr[len_sub - 1] == str[i]) {
			for (j = 1; j < len_sub; j++) {
				if (substr[len_sub - 1 - j] != str[i - j]) {
					flag = 0;
					break;
				}

			}
			if (j == len_sub) {
				flag = 1;
				break;
			}
		}
	}
	if (flag == 1)
		return i + 2 - len_sub;//return值可根据需要修改
	else
		return 0;
}

void Input(float * x, int lower, int upper,int pos_x,int pos_y, const char str[])
{
	const int LEN = 50;
	while (1) {
		showstr(pos_x, pos_y, str,COLOR_WHITE,COLOR_BLACK);
		cin >> *x;
		if (!cin.good()) {
			cin.clear();
			cin.ignore(1024, '\n');
		}

		else if (*x <= upper && *x >= lower)
			break;
	}
	showstr(pos_x, pos_y, " ", COLOR_WHITE, COLOR_BLACK, 1, LEN);
}

void draw_frame(int X, int Y, int row, int col, const int bg_color = COLOR_HWHITE,const int ft_color = COLOR_BLACK)
{
	showstr(X, Y, frame[0], bg_color, ft_color);
	showstr(X + 1, Y, frame[1], bg_color, ft_color, col);
	showstr(X + 1 + col * 2, Y, frame[2], bg_color, ft_color);
	// "╔═╗║║═╝"
	cout << endl;
	for (int i = 1; i <= row; i++) {
		showstr(X, Y + i, frame[3], bg_color, ft_color);
		showstr(X + 1, Y + i, frame[4], bg_color, ft_color, col);
		showstr(X + 1 + col * 2, Y + i, frame[5], bg_color, ft_color);
		cout << endl;
	}
	showstr(X, Y + 1 + row, frame[6], bg_color, ft_color);
	showstr(X + 1, Y + 1 + row, frame[7], bg_color, ft_color, col);
	showstr(X + 1 + col * 2, Y + 1 + row, frame[8], bg_color, ft_color);
}

void print_remind(int bg_color = COLOR_WHITE,int ft_color = COLOR_BLACK)
{
	showstr(X2, Y2, "[N]：结束本次阅读",bg_color,ft_color);
	showstr(X2, Y2+2, "[Q]：退出阅读", bg_color, ft_color);
	showstr(X2, Y2+4, "[F]：格式转换", bg_color, ft_color);
	showstr(X2, Y2+6, "[S]：跳转阅读进度", bg_color, ft_color);
}

void print_menu(const int No)
{
	int i = 0;
	for(;i<row;i++)
		showstr(MX1, MY1+i, bookshort[No+i],COLOR_HWHITE,COLOR_BLACK,1,col*2);
}

void print_txt_content(const int cur)
{
	int i;
	for (i = 0; i < ROW; i++) {
		showstr(X1, Y1 + i, buf1[cur+i], COLOR_HWHITE, COLOR_BLACK,1,NUM_CHAR);
	}
}

double print_pospercent(const int pos, const int linenum,const int pos_x = X2, const int pos_y = Y2+8)
{
	double cur = pos / double(linenum);
	gotoxy(pos_x, pos_y);
	cout << "当前位置：";
	printf("%.2f%%", 100*cur);
	return cur;
}

void newline(int X,int Y)
{
	getxy(X, Y);
	gotoxy(X, Y + 1);
}

int check_line(char buf[])
{
	int i, count = 0;
	for (i = 0; i < int(strlen(buf)); i++) {
		if (buf[i] < 0)
			count++;
	}
	return !(count % 2);//若整除返回真，即通过检查
}

int main()
{
	const int   win_width = 800, win_high = 800;	//设定屏幕宽度
	const int   win_bgcolor = COLOR_WHITE, win_fgcolor = COLOR_HRED;
	const int   color[] = { 0x00FF0000, 0x0000FF00, 0x000000FF }; //R G B
	const int   STL_X = 0, STL_Y = win_high / 16 - 5;	//状态行的起始坐标
	const char *fill_str[] = { "空心", "实心" };

	srand((unsigned int)time(0));

	hdc_init(win_bgcolor, win_fgcolor, win_width, win_high);		//用(背景色，前景色，宽度，高度）初始化窗口
	

	/*开始遍历book*/
	struct _finddata_t file;
	int K;
	long HANDLE; //记录第一次打开的文件句柄，关闭时需用此值		
	/* 将参数做变化，观察运行结果
		   1、将“*.*”换为“*.dll”/“*.exe”等形式
		   2、将“C:\Windows”换为其它文件夹名
	*/
	//也可写为 “C:/Windows/*.*”
	K = HANDLE = _findfirst("./book/*.txt", &file);
	int i;
	for (i=0; K != -1; K = _findnext(HANDLE, &file),i++) {

		strcpy(bookname[i], file.name);
		int n;
		n = tj_strstr(bookname[i], ".txt")-1;
		if (n > 2*col)
			n = 2*col;
		strncpy(bookshort[i], bookname[i], n);
		bookshort[i][n] = '\0';
		if (!check_line(bookshort[i])) {
			strncpy(bookshort[i], bookname[i], n-1);
			bookshort[i][n-1] = '\0';
		}
	}
	op.sum = i-1;

	enable_mouse();
	while (1) {
		cls();
		draw_frame(MX0, MY0, row, col);
		print_menu(0);
		showstr(MX1, MY1 + op.pos, bookshort[op.No], 3, 15, 1, col * 2 - 1);
		menu_keyboard_control();

		char address[FILENAME_MAX] = "./book/";
		char savepath[FILENAME_MAX] = "./book/position.saving/";
		strcat(address, bookname[op.No]);
		strcat(savepath, bookname[op.No]);
		cout << address << endl;

		setcolor(COLOR_WHITE);
		cls();
		setconsoleborder(150, 40);

		hdc_draw_yinyang(590, 350, 350);

		//画边框
		draw_frame(X0, Y0, ROW, COL+1);
		print_remind();

		int linenum = split_line_buf(address);

		int pos = read_saveinfo(linenum);
		print_txt_content(pos);

		int status = Read_keyboard_control(&pos, linenum,address);

		//写存档
		double posinfo = print_pospercent(pos, linenum);

		ofstream out(savepath, ios::out | ios::binary);
		if (!out.is_open()) {
			cout << "savepath: saving fail!" << endl;
			out.close();
			return -1;
		}
		out << posinfo;
		out.close();


		if (status == -1) {
			gotoxy(0, 0);
			return 0;
		}
			
	}

}
/***************************************************************************
  函数名称：位置记录
  功    能：将记录文件中的位置值读出
  输入参数：分行后的最大行数
  返 回 值：文件打开错误或大小超出范围返回0，否则返回位置值
  说    明：
***************************************************************************/
int read_saveinfo(const int linenum)
{
	const int BUF_MAX = 100;
	char path[FILENAME_MAX] = "./book/position.saving/";
	strcat(path, bookname[op.No]);
	ifstream in(path, ios::in | ios::binary);
	if (!in.is_open()) {
		gotoxy(0, 0);
		//cout << "position_save: saving file open fail!" << endl;
		in.close();
		return 0;
	}

	char buf[BUF_MAX+1];
	in.getline(buf, BUF_MAX);
	in.close();
	int pos;
	double percent;
	sscanf(buf, "%lf", &percent);
	pos = int(percent*linenum);
	return pos;
}

/***************************************************************************
	  函数名称：阅读模式键盘操作
	  功    能：读取键鼠操作并完成动画演示
	  输入参数：
	  返 回 值：
	  说    明：
***************************************************************************/
int Read_keyboard_control(int* pos,int linenum, char*address) {
	
	int loop = 1;
	int ret, X = 0, Y = 0, maction = 0, keycode1 = 0, keycode2 = 0, KX = 1, KY = 1;

	int flag = 0;//记录状态0：未选定起点，1：已选定起点，未选定终点，2：选定终点，执行移动操作
	int count = 0;
	float dst;//目标跳转位置
	int mode = 0;
	double posinfo=0;

	while (loop) {
		//Sleep(100);

		ret = read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		if (ret == CCT_MOUSE_EVENT) {

			gotoxy(0, 20);
			setcolor(3, 15);
			//cout << "[当前鼠标位置] " << X << "行" << " " << Y << " 列";
			switch (maction) {
				case MOUSE_ONLY_MOVED:
					//cout << "移动          " << endl;
					//showch(X, Y, '*');不打印任何内容
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					//cout << "按下左键      " << endl;
					break;
				case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:	//双击左键
					//cout << "双击左键      " << endl;
					//showch(X, Y, '2');			//在鼠标指针位置显示2
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
					//cout << "按下右键      " << endl;
					//showch(X, Y, '3');			//在鼠标指针位置显示3
					break;
				case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//双击右键
					//cout << "双击右键      " << endl;
					//showch(X, Y, '4');	//在鼠标指针位置显示4

					break;
				case MOUSE_LEFTRIGHT_BUTTON_CLICK:		//同时按下左右键
					//cout << "同时按下左右键" << endl;
					//showch(X, Y, '5');			//在鼠标指针位置显示5
					break;
				case MOUSE_WHEEL_CLICK:
					//cout << "按下滚轮      " << endl;
					break;
				case MOUSE_WHEEL_MOVED_UP:
					//cout << "滚轮向上移动  " << endl;
					if (*pos > 0)
						print_txt_content(--*pos);
					break;
				case MOUSE_WHEEL_MOVED_DOWN:
					//cout << "滚轮向下移动  " << endl;
					print_txt_content(++*pos);
					break;
				case MOUSE_NO_ACTION:
				default:
					//cout << "其它操作" << endl;
					//showch(X, Y, '0');			//在鼠标指针位置显示0
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			/* 转到第9行进行打印 */
			setcolor(3, 15);
			gotoxy(0, 21);
			//cout << "[当前按键] : ";

			switch (keycode1) {
				case 'S':
				case 's':
					Input(&dst, 0, 100, 55, Y0-2, "请输入跳转目标位置(0-100.0):");
					*pos = int(linenum / 100.0 * dst);
					print_txt_content(*pos);
					print_pospercent(*pos, linenum);
					break;
				case 'F':
				case 'f':
					posinfo = print_pospercent(*pos, linenum);
					linenum = split_line_buf(address,++mode % 2);//2为mode数目，此处只有mode0和mode1
					*pos = int(linenum * posinfo);
					print_txt_content(*pos);
					print_pospercent(*pos, linenum);
					break;

				case 'Q':
				case 'q':
					return -1;//直接返回-1退出
					break;
				case 'N':
				case 'n':
					return 0;//直接返回0退出
					break;

				case 33:  //page up				
					*pos -= ROW;
					if (*pos >= 0) {
						print_txt_content(*pos);
						print_pospercent(*pos, linenum);
					}					
					else
						*pos += ROW;
					break;
				case 34:  //page down
					*pos += ROW;
					print_txt_content(*pos);
					print_pospercent(*pos, linenum);
					break;
				case 13:    //Enter	
					gotoxy(14, 21);
					//cout << "ENTER      ";
					//loop = 0;
					break;
				case 27:	//ESC键
					gotoxy(14, 21);
					//cout << "ESC       ";
					break;
				case 32:	//SPACE键
					gotoxy(14, 21);
					//cout << "SPACE     ";
					break;
				case 224:
					switch (keycode2) {
						case KB_ARROW_UP:
							//gotoxy(14, 21);
							//cout << "上箭头    ";
							if (*pos > 0) {
								print_txt_content(--*pos);
								print_pospercent(*pos, linenum);
							}								
							break;
						case KB_ARROW_DOWN:
							//gotoxy(14, 21);
							//cout << "下箭头    ";
							print_txt_content(++*pos);
							print_pospercent(*pos, linenum);
							break;
						case KB_ARROW_LEFT:
							gotoxy(14, 21);
							//cout << "左箭头    ";
							break;
						case KB_ARROW_RIGHT:
							gotoxy(14, 21);
							//cout << "右箭头    ";
							break;
					}
					break;
				default:	//其他键直接打印ASCII值
					if (keycode1 > 0x20 && keycode1 <= 0x7E) {
						//可见ASCII字符
						gotoxy(0, 0);
						cout << char(keycode1) << "         ";
					}
					else {
						gotoxy(0, 0);
						cout << "int(" << keycode1 << ")  ";
					}					
					break;
			}//end of swicth(keycode1)
		}//end of else if(ret == CCT_KEYBOARD_EVENT
	}//end of while(loop)
	return op.No;//loop=0表示退出
}

int menu_keyboard_control()
{
	/***************************************************************************
	  函数名称：
	  功    能：读取键鼠操作并完成动画演示
	  输入参数：
	  返 回 值：双击右键返回loop=0，判断为假游戏结束，返回1则继续执行
	  说    明：
	***************************************************************************/

	int loop = 1;
	int ret, X = 0, Y = 0, maction = 0, keycode1 = 0, keycode2 = 0, KX = 1, KY = 1;

	int flag = 0;//记录状态0：未选定起点，1：已选定起点，未选定终点，2：选定终点，执行移动操作
	int count = 0;

	while (loop) {
		//Sleep(100);

		ret = read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		if (ret == CCT_MOUSE_EVENT ) {

			gotoxy(0, 20);
			setcolor(3, 15);
			//cout << "[当前鼠标位置] " << X << "行" << " " << Y << " 列";
			switch (maction) {
				case MOUSE_ONLY_MOVED:
					//cout << "移动          " << endl;
					//showch(X, Y, '*');不打印任何内容
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					//cout << "按下左键      " << endl;
					break;
				case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:	//双击左键
					//cout << "双击左键      " << endl;
					//showch(X, Y, '2');			//在鼠标指针位置显示2
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
					//cout << "按下右键      " << endl;
					//showch(X, Y, '3');			//在鼠标指针位置显示3
					break;
				case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//双击右键
					//cout << "双击右键      " << endl;
					//showch(X, Y, '4');	//在鼠标指针位置显示4
					
					break;
				case MOUSE_LEFTRIGHT_BUTTON_CLICK:		//同时按下左右键
					//cout << "同时按下左右键" << endl;
					//showch(X, Y, '5');			//在鼠标指针位置显示5
					break;
				case MOUSE_WHEEL_CLICK:
					//cout << "按下滚轮      " << endl;
					break;
				case MOUSE_WHEEL_MOVED_UP:
					//cout << "滚轮向上移动  " << endl;
					break;
				case MOUSE_WHEEL_MOVED_DOWN:
					//cout << "滚轮向下移动  " << endl;
					break;
				case MOUSE_NO_ACTION:
				default:
					//cout << "其它操作" << endl;
					//showch(X, Y, '0');			//在鼠标指针位置显示0
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			/* 转到第9行进行打印 */
			setcolor(3, 15);
			gotoxy(0, 21);
			//cout << "[当前按键] : ";

			switch (keycode1) {
				case 13:    //Enter	
					gotoxy(14, 21);
					//cout << "ENTER      ";
					loop = 0;
					break;
				case 27:	//ESC键
					gotoxy(14, 21);
					//cout << "ESC       ";
					break;
				case 32:	//SPACE键
					gotoxy(14, 21);
					//cout << "SPACE     ";
					break;
				case 224:
					switch (keycode2) {
						case KB_ARROW_UP:
							gotoxy(14, 21);
							//cout << "上箭头    ";
							if (op.pos == 0 && op.No != 0) {
								op.No--;
								print_menu(op.No);
								showstr(MX1, MY1 + op.pos, bookshort[op.No], 3, 15, 1, col * 2 - 1);
							}
							else if(op.pos>0){					
								showstr(MX1, MY1 + op.pos, bookshort[op.No], COLOR_HWHITE, COLOR_BLACK, 1, col * 2-1);
								op.pos--;
								op.No--;
								showstr(MX1, MY1 + op.pos, bookshort[op.No], 3, 15, 1, col * 2 - 1);
							}

							break;
						case KB_ARROW_DOWN:
							gotoxy(14, 21);
							//cout << "下箭头    ";
							if (op.pos == row-1 && op.No != op.sum) {
								op.No++;
								print_menu(op.No-row+1);
								showstr(MX1, MY1 + op.pos, bookshort[op.No], 3, 15, 1, col * 2-1);
							}
							else if (op.pos < row-1) {
								showstr(MX1, MY1 + op.pos, bookshort[op.No], COLOR_HWHITE, COLOR_BLACK, 1, col * 2-1);
								op.pos++;
								op.No++;
								showstr(MX1, MY1 + op.pos, bookshort[op.No], 3, 15, 1, col * 2-1);
							}


							break;
						case KB_ARROW_LEFT:
							gotoxy(14, 21);
							//cout << "左箭头    ";
							break;
						case KB_ARROW_RIGHT:
							gotoxy(14, 21);
							//cout << "右箭头    ";
							break;
					}
					break;
				default:	//其他键直接打印ASCII值
					if (keycode1 > 0x20 && keycode1 <= 0x7E) //可见ASCII字符
						cout << char(keycode1) << "         ";
					else
						cout << "int(" << keycode1 << ")  ";
					break;
			}//end of swicth(keycode1)
		}//end of else if(ret == CCT_KEYBOARD_EVENT
	}//end of while(loop)
	return op.No;//loop=0表示退出
}
/***************************************************************************
  函数名称：分行缓冲
  功    能：将文件按mode0和mode1两种不同方式实现分行，将分好的行放入buf1中
  输入参数：address文件路径，
  返 回 值：-1文件打开失败，否则返回总行数
  说    明：
***************************************************************************/
int split_line_buf(char address[],int mode)
{
	ifstream in(address, ios::in | ios::binary);
	if (!in.is_open()) {
		cout << "open error1!" << endl;
		in.close();
		return -1;
	}

	int flag = 0;//换段标记符 
	int  i, j;
	char ch;
	for (i = 0; !in.eof(); i++) {

		for (j = 0; j < NUM_CHAR; j++) {
			//模式一，按文本换行并且补tab 
			if (mode == 0) {
				ch = in.get();
				if (ch == EOF) {
					flag = -1;
					break;
				}

				if (flag &&j == 0 && !(ch == '\t' || ch == ' ')) {
					strcpy(buf1[i], "    ");
					j += 3;
				}
				//行首没有空格则补tab 				
				flag = 0;
				if (ch == '\r') {
					in.get();//skip the '\n'
					buf1[i][j] = '\0';//统一本行输出字符数量为j个
					flag = 1;
					break;
				}
				else
					buf1[i][j] = ch;
				//in.getline(buf1[i],sizeof(buf1[i]));
			}
			/*mode2,不按文本换行*/
			else if (mode == 1) {
				ch = in.get();
				//排除文件结束
				if (ch == EOF) {
					flag = -1;
					break;
				}
				flag = 0;
				//如果遇换行符
				if (ch == '\r') {
					flag = 1;//遇到换行符flag==1
					ch = in.get();//skip the '\n'
					ch = in.get();

					if (ch == '\t' || ch == ' ') {
						buf1[i][j] = '\0';//统一本行输出字符数量为j个
						in.seekg(-1, ios::cur);
						break;
					}
					else if (ch == '\r') {
						buf1[i][j] = '\0';//统一本行输出字符数量为j个
						in.seekg(-1, ios::cur);
						buf1[++i][0] = 0;
						break;
					}
					else {
						buf1[i][j] = ch;
					}
				}
				else {
					buf1[i][j] = ch;
				}

			}//end of mode2
 		}
		if (flag == 0)
			buf1[i][j] = 0;
		else if (flag == -1)
			break;
		/*此处做了简单化处理，由于汉字只涉及两个字符表示，因此行内汉字代码为偶数时即可正确表示
		但如果遇到更复杂需要3-4个ASCII码表示的字符就会出现问题
		*/
		if (!check_line(buf1[i]) && flag!=1) {
			buf1[i][j - 1] = '\0';
			in.seekg(-1, ios::cur);
		}
	}

	if (i < ROW) {
		for (j = i; j < ROW; j++)
			strcpy(buf1[j], "");
		i = ROW;
	}

	in.close();
	return i;
}