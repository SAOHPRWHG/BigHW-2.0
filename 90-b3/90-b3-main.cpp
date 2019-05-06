/* 1751957 ��2�� ������ */
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

/*���������°����̨�±��ֽϺã��ɰ����̨����һ��߿�������⣬�������^_^*/

/*����˵�λ�������*/
const int MX0 = 42;
const int MY0 = 14;
const int MX1 = MX0 + 2;
const int MY1 = MY0 + 1;
const int col = 8;
const int row = 15;
/*�����ı�λ������ܣ�
ÿ��75���֣�������߿��ܿ��78
�߶�20��
*/
const int X0 = 34;
const int Y0 = 11;
const int X1 = X0 + 2;
const int Y1 = Y0 + 1;
const int COL = 38;
const int ROW = 20;
const int NUM_CHAR = 2*COL-1;

const char *frame[] = { "��","����","��","��","��","��","��","����","��" };

/*����˵���������Ϣ*/
const int BOOK_MAX = 100;//�˵��������������

static char buf1[1024 / NUM_CHAR * 1024 * 20][NUM_CHAR + 1];//���л�����������20M
static char bookname[BOOK_MAX][FILENAME_MAX];
static char bookshort[BOOK_MAX][col*2+1];

/*������ʾѡ��λ����Ϣ*/
const int X2 = 11;
const int Y2 = 11+4;

static struct option {
	int pos;//0-9
	int No;//ȫ��
	int sum;//����
}op{0,0};

int menu_keyboard_control();
void hdc_draw_yinyang(int point_x, int point_y, int r);
int split_line_buf(char address[],int mode = 1);
int read_saveinfo(const int linenum);
int Read_keyboard_control(int* pos, int linenum, char*address);
int tj_strstr(const char str[], const char substr[])
{
	/* ע�⣺�����ڲ��������κ���ʽ�����飨������̬���飩 */
	int i, j, len, len_sub, flag = 0;
	len_sub = strlen(substr);
	len = strlen(str);//�����ʣ��޷�ʶ�������ڲ�'\0'
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
		return i + 2 - len_sub;//returnֵ�ɸ�����Ҫ�޸�
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
	// "�X�T�[�U�U�T�a"
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
	showstr(X2, Y2, "[N]�����������Ķ�",bg_color,ft_color);
	showstr(X2, Y2+2, "[Q]���˳��Ķ�", bg_color, ft_color);
	showstr(X2, Y2+4, "[F]����ʽת��", bg_color, ft_color);
	showstr(X2, Y2+6, "[S]����ת�Ķ�����", bg_color, ft_color);
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
	cout << "��ǰλ�ã�";
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
	return !(count % 2);//�����������棬��ͨ�����
}

int main()
{
	const int   win_width = 800, win_high = 800;	//�趨��Ļ���
	const int   win_bgcolor = COLOR_WHITE, win_fgcolor = COLOR_HRED;
	const int   color[] = { 0x00FF0000, 0x0000FF00, 0x000000FF }; //R G B
	const int   STL_X = 0, STL_Y = win_high / 16 - 5;	//״̬�е���ʼ����
	const char *fill_str[] = { "����", "ʵ��" };

	srand((unsigned int)time(0));

	hdc_init(win_bgcolor, win_fgcolor, win_width, win_high);		//��(����ɫ��ǰ��ɫ����ȣ��߶ȣ���ʼ������
	

	/*��ʼ����book*/
	struct _finddata_t file;
	int K;
	long HANDLE; //��¼��һ�δ򿪵��ļ�������ر�ʱ���ô�ֵ		
	/* ���������仯���۲����н��
		   1������*.*����Ϊ��*.dll��/��*.exe������ʽ
		   2������C:\Windows����Ϊ�����ļ�����
	*/
	//Ҳ��дΪ ��C:/Windows/*.*��
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

		//���߿�
		draw_frame(X0, Y0, ROW, COL+1);
		print_remind();

		int linenum = split_line_buf(address);

		int pos = read_saveinfo(linenum);
		print_txt_content(pos);

		int status = Read_keyboard_control(&pos, linenum,address);

		//д�浵
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
  �������ƣ�λ�ü�¼
  ��    �ܣ�����¼�ļ��е�λ��ֵ����
  ������������к���������
  �� �� ֵ���ļ��򿪴�����С������Χ����0�����򷵻�λ��ֵ
  ˵    ����
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
	  �������ƣ��Ķ�ģʽ���̲���
	  ��    �ܣ���ȡ�����������ɶ�����ʾ
	  ���������
	  �� �� ֵ��
	  ˵    ����
***************************************************************************/
int Read_keyboard_control(int* pos,int linenum, char*address) {
	
	int loop = 1;
	int ret, X = 0, Y = 0, maction = 0, keycode1 = 0, keycode2 = 0, KX = 1, KY = 1;

	int flag = 0;//��¼״̬0��δѡ����㣬1����ѡ����㣬δѡ���յ㣬2��ѡ���յ㣬ִ���ƶ�����
	int count = 0;
	float dst;//Ŀ����תλ��
	int mode = 0;
	double posinfo=0;

	while (loop) {
		//Sleep(100);

		ret = read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		if (ret == CCT_MOUSE_EVENT) {

			gotoxy(0, 20);
			setcolor(3, 15);
			//cout << "[��ǰ���λ��] " << X << "��" << " " << Y << " ��";
			switch (maction) {
				case MOUSE_ONLY_MOVED:
					//cout << "�ƶ�          " << endl;
					//showch(X, Y, '*');����ӡ�κ�����
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//�������
					//cout << "�������      " << endl;
					break;
				case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:	//˫�����
					//cout << "˫�����      " << endl;
					//showch(X, Y, '2');			//�����ָ��λ����ʾ2
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//�����Ҽ�
					//cout << "�����Ҽ�      " << endl;
					//showch(X, Y, '3');			//�����ָ��λ����ʾ3
					break;
				case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//˫���Ҽ�
					//cout << "˫���Ҽ�      " << endl;
					//showch(X, Y, '4');	//�����ָ��λ����ʾ4

					break;
				case MOUSE_LEFTRIGHT_BUTTON_CLICK:		//ͬʱ�������Ҽ�
					//cout << "ͬʱ�������Ҽ�" << endl;
					//showch(X, Y, '5');			//�����ָ��λ����ʾ5
					break;
				case MOUSE_WHEEL_CLICK:
					//cout << "���¹���      " << endl;
					break;
				case MOUSE_WHEEL_MOVED_UP:
					//cout << "���������ƶ�  " << endl;
					if (*pos > 0)
						print_txt_content(--*pos);
					break;
				case MOUSE_WHEEL_MOVED_DOWN:
					//cout << "���������ƶ�  " << endl;
					print_txt_content(++*pos);
					break;
				case MOUSE_NO_ACTION:
				default:
					//cout << "��������" << endl;
					//showch(X, Y, '0');			//�����ָ��λ����ʾ0
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			/* ת����9�н��д�ӡ */
			setcolor(3, 15);
			gotoxy(0, 21);
			//cout << "[��ǰ����] : ";

			switch (keycode1) {
				case 'S':
				case 's':
					Input(&dst, 0, 100, 55, Y0-2, "��������תĿ��λ��(0-100.0):");
					*pos = int(linenum / 100.0 * dst);
					print_txt_content(*pos);
					print_pospercent(*pos, linenum);
					break;
				case 'F':
				case 'f':
					posinfo = print_pospercent(*pos, linenum);
					linenum = split_line_buf(address,++mode % 2);//2Ϊmode��Ŀ���˴�ֻ��mode0��mode1
					*pos = int(linenum * posinfo);
					print_txt_content(*pos);
					print_pospercent(*pos, linenum);
					break;

				case 'Q':
				case 'q':
					return -1;//ֱ�ӷ���-1�˳�
					break;
				case 'N':
				case 'n':
					return 0;//ֱ�ӷ���0�˳�
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
				case 27:	//ESC��
					gotoxy(14, 21);
					//cout << "ESC       ";
					break;
				case 32:	//SPACE��
					gotoxy(14, 21);
					//cout << "SPACE     ";
					break;
				case 224:
					switch (keycode2) {
						case KB_ARROW_UP:
							//gotoxy(14, 21);
							//cout << "�ϼ�ͷ    ";
							if (*pos > 0) {
								print_txt_content(--*pos);
								print_pospercent(*pos, linenum);
							}								
							break;
						case KB_ARROW_DOWN:
							//gotoxy(14, 21);
							//cout << "�¼�ͷ    ";
							print_txt_content(++*pos);
							print_pospercent(*pos, linenum);
							break;
						case KB_ARROW_LEFT:
							gotoxy(14, 21);
							//cout << "���ͷ    ";
							break;
						case KB_ARROW_RIGHT:
							gotoxy(14, 21);
							//cout << "�Ҽ�ͷ    ";
							break;
					}
					break;
				default:	//������ֱ�Ӵ�ӡASCIIֵ
					if (keycode1 > 0x20 && keycode1 <= 0x7E) {
						//�ɼ�ASCII�ַ�
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
	return op.No;//loop=0��ʾ�˳�
}

int menu_keyboard_control()
{
	/***************************************************************************
	  �������ƣ�
	  ��    �ܣ���ȡ�����������ɶ�����ʾ
	  ���������
	  �� �� ֵ��˫���Ҽ�����loop=0���ж�Ϊ����Ϸ����������1�����ִ��
	  ˵    ����
	***************************************************************************/

	int loop = 1;
	int ret, X = 0, Y = 0, maction = 0, keycode1 = 0, keycode2 = 0, KX = 1, KY = 1;

	int flag = 0;//��¼״̬0��δѡ����㣬1����ѡ����㣬δѡ���յ㣬2��ѡ���յ㣬ִ���ƶ�����
	int count = 0;

	while (loop) {
		//Sleep(100);

		ret = read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		if (ret == CCT_MOUSE_EVENT ) {

			gotoxy(0, 20);
			setcolor(3, 15);
			//cout << "[��ǰ���λ��] " << X << "��" << " " << Y << " ��";
			switch (maction) {
				case MOUSE_ONLY_MOVED:
					//cout << "�ƶ�          " << endl;
					//showch(X, Y, '*');����ӡ�κ�����
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//�������
					//cout << "�������      " << endl;
					break;
				case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:	//˫�����
					//cout << "˫�����      " << endl;
					//showch(X, Y, '2');			//�����ָ��λ����ʾ2
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//�����Ҽ�
					//cout << "�����Ҽ�      " << endl;
					//showch(X, Y, '3');			//�����ָ��λ����ʾ3
					break;
				case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//˫���Ҽ�
					//cout << "˫���Ҽ�      " << endl;
					//showch(X, Y, '4');	//�����ָ��λ����ʾ4
					
					break;
				case MOUSE_LEFTRIGHT_BUTTON_CLICK:		//ͬʱ�������Ҽ�
					//cout << "ͬʱ�������Ҽ�" << endl;
					//showch(X, Y, '5');			//�����ָ��λ����ʾ5
					break;
				case MOUSE_WHEEL_CLICK:
					//cout << "���¹���      " << endl;
					break;
				case MOUSE_WHEEL_MOVED_UP:
					//cout << "���������ƶ�  " << endl;
					break;
				case MOUSE_WHEEL_MOVED_DOWN:
					//cout << "���������ƶ�  " << endl;
					break;
				case MOUSE_NO_ACTION:
				default:
					//cout << "��������" << endl;
					//showch(X, Y, '0');			//�����ָ��λ����ʾ0
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			/* ת����9�н��д�ӡ */
			setcolor(3, 15);
			gotoxy(0, 21);
			//cout << "[��ǰ����] : ";

			switch (keycode1) {
				case 13:    //Enter	
					gotoxy(14, 21);
					//cout << "ENTER      ";
					loop = 0;
					break;
				case 27:	//ESC��
					gotoxy(14, 21);
					//cout << "ESC       ";
					break;
				case 32:	//SPACE��
					gotoxy(14, 21);
					//cout << "SPACE     ";
					break;
				case 224:
					switch (keycode2) {
						case KB_ARROW_UP:
							gotoxy(14, 21);
							//cout << "�ϼ�ͷ    ";
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
							//cout << "�¼�ͷ    ";
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
							//cout << "���ͷ    ";
							break;
						case KB_ARROW_RIGHT:
							gotoxy(14, 21);
							//cout << "�Ҽ�ͷ    ";
							break;
					}
					break;
				default:	//������ֱ�Ӵ�ӡASCIIֵ
					if (keycode1 > 0x20 && keycode1 <= 0x7E) //�ɼ�ASCII�ַ�
						cout << char(keycode1) << "         ";
					else
						cout << "int(" << keycode1 << ")  ";
					break;
			}//end of swicth(keycode1)
		}//end of else if(ret == CCT_KEYBOARD_EVENT
	}//end of while(loop)
	return op.No;//loop=0��ʾ�˳�
}
/***************************************************************************
  �������ƣ����л���
  ��    �ܣ����ļ���mode0��mode1���ֲ�ͬ��ʽʵ�ַ��У����ֺõ��з���buf1��
  ���������address�ļ�·����
  �� �� ֵ��-1�ļ���ʧ�ܣ����򷵻�������
  ˵    ����
***************************************************************************/
int split_line_buf(char address[],int mode)
{
	ifstream in(address, ios::in | ios::binary);
	if (!in.is_open()) {
		cout << "open error1!" << endl;
		in.close();
		return -1;
	}

	int flag = 0;//���α�Ƿ� 
	int  i, j;
	char ch;
	for (i = 0; !in.eof(); i++) {

		for (j = 0; j < NUM_CHAR; j++) {
			//ģʽһ�����ı����в��Ҳ�tab 
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
				//����û�пո���tab 				
				flag = 0;
				if (ch == '\r') {
					in.get();//skip the '\n'
					buf1[i][j] = '\0';//ͳһ��������ַ�����Ϊj��
					flag = 1;
					break;
				}
				else
					buf1[i][j] = ch;
				//in.getline(buf1[i],sizeof(buf1[i]));
			}
			/*mode2,�����ı�����*/
			else if (mode == 1) {
				ch = in.get();
				//�ų��ļ�����
				if (ch == EOF) {
					flag = -1;
					break;
				}
				flag = 0;
				//��������з�
				if (ch == '\r') {
					flag = 1;//�������з�flag==1
					ch = in.get();//skip the '\n'
					ch = in.get();

					if (ch == '\t' || ch == ' ') {
						buf1[i][j] = '\0';//ͳһ��������ַ�����Ϊj��
						in.seekg(-1, ios::cur);
						break;
					}
					else if (ch == '\r') {
						buf1[i][j] = '\0';//ͳһ��������ַ�����Ϊj��
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
		/*�˴����˼򵥻��������ں���ֻ�漰�����ַ���ʾ��������ں��ִ���Ϊż��ʱ������ȷ��ʾ
		�����������������Ҫ3-4��ASCII���ʾ���ַ��ͻ��������
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