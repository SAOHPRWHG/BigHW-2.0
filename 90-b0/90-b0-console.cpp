/* 1751957 ��2�� ������ */
#include "../common/cmd_hdc_tools.h"
#include "90-b0.h"
using namespace std;

void show_GUI(int Y)
{
	gotoxy(0, Y );
	cout << "���س�����ʾͼ��..." << endl;
	char ch;
	while ((ch = _getch()) != '\r')
		;

	cls();
	setfontsize("������", 26);
	cout << "Ϊ�ζ��������ζ���" << endl;
}

void print_GUI_without(int a[][MAX_FRAME], int row,int col,int opt)
{
	int X, Y;
	int cols, lines, buffer_cols, buffer_lines;
	getconsoleborder(cols, lines, buffer_cols, buffer_lines);
	cout << "��Ļ��" << cols << "��" << lines << "��";
	if (opt)
		cout << "(�Ҽ��˳�)";
	cout << endl;
	getxy(X, Y);
	showstr(X, Y, "�X", COLOR_HWHITE,COLOR_BLACK);
	showstr(X+2, Y, "�T", COLOR_HWHITE, COLOR_BLACK, col);
	showstr(X+2+col*2, Y, "�[", COLOR_HWHITE, COLOR_BLACK);
	//�U�T �[�X
	cout << endl;
	for (int i = 1; i <= row; i++) {
		showstr(X, Y+i, "�U", COLOR_HWHITE, COLOR_BLACK);
		showstr(X + 2, Y+i, "��", COLOR_HWHITE, COLOR_BLACK, col);
		showstr(X + 2 + col * 2, Y+i, "�U", COLOR_HWHITE, COLOR_BLACK);
		cout << endl;
	}
	showstr(X, Y + 1 + row, "�^", COLOR_HWHITE, COLOR_BLACK);
	showstr(X + 2, Y + 1 + row, "�T", COLOR_HWHITE, COLOR_BLACK, col);
	showstr(X + 2 + col * 2, Y + 1 + row, "�a", COLOR_HWHITE, COLOR_BLACK);
	
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			if (a[i][j]) {
				showstr(X + 2*j, Y + i, "��", 7+a[i][j], 8-a[i][j]);
			}
			else {
				showstr(X + 2*j, Y + i, "��", COLOR_HWHITE, COLOR_BLACK);
			}
		}
	}
	setcolor();
}

void print_GUI_with(int a[][MAX_FRAME], int row, int col, int opt)
{
	setcolor(3, 15);
	gotoxy(0, 0);
	int X, Y;
	int cols, lines, buffer_cols, buffer_lines;
	getconsoleborder(cols, lines, buffer_cols, buffer_lines);
	cout << "��Ļ��" << lines << "��" << cols<< "��";
	if (opt)
		cout << "(˫���Ҽ��˳�)";
	cout << endl;

	const int b = 4;
	getxy(X, Y);
	showstr(X, Y, "�X�T", COLOR_HWHITE, COLOR_BLACK);
	showstr(X + b, Y, "�j�T", COLOR_HWHITE, COLOR_BLACK, col-1);
	showstr(X + col * b, Y, "�[", COLOR_HWHITE, COLOR_BLACK);
	showstr(X, Y + 1, "�U��", COLOR_HWHITE, COLOR_BLACK,col);
	showstr(X+col*b, Y + 1 , "�U", COLOR_HWHITE, COLOR_BLACK);
	
	//�U�T �[�X
	cout << endl;
	for (int i = 1; i < row; i++) {
		showstr(X, Y + 2 * i, "�d�T", COLOR_HWHITE, COLOR_BLACK);
		showstr(X + b, Y + 2 * i, "�p�T", COLOR_HWHITE, COLOR_BLACK, col-1);
		showstr(X + col * b, Y + 2 * i, "�g", COLOR_HWHITE, COLOR_BLACK);
		showstr(X, Y + 2 * i + 1, "�U��", COLOR_HWHITE, COLOR_BLACK, col);
		showstr(X + col * b, Y + 2 * i + 1, "�U", COLOR_HWHITE, COLOR_BLACK);
		cout << endl;
	}
	showstr(X, Y + 2*row, "�^�T", COLOR_HWHITE, COLOR_BLACK);
	showstr(X + b, Y + 2*row, "�m�T", COLOR_HWHITE, COLOR_BLACK, col-1);
	showstr(X + col * b, Y + 2*row, "�a", COLOR_HWHITE, COLOR_BLACK);
	
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			if (a[i][j]) {
				showstr(X + b * j-2, Y + 2 * i - 1 , "��", 7 + a[i][j], 8 - a[i][j]);
			}
			else {
				showstr(X + b * j-2, Y + 2 * i - 1, "��", COLOR_HWHITE, COLOR_BLACK);
			}
		}
	}
	setcolor();
}

void cmd_to_array(int &x, int &y, const int Y = 1, const int X = 0)
{
	//����Ļ����תΪ�����ڲ�����
	y = (y + 1 - Y)/2;
	
	if ((x - X) % 2)
		x = (x + 1 - X)/4;
	else
		x = (x + 2 - X)/4;
}

void array_to_cmd(int &x, int &y, const int Y = 1, const int X = 0)
{
	//�������ڲ�����תΪ��Ļ����
	y = 2 * y - 1 + Y;
	x = 4 * x - 2 + X;

}

int ax(int x, const int X = 0)
{
	int ax;
	if ((x - X) % 2)
		ax = (x + 1 - X) / 4;
	else
		ax = (x + 2 - X) / 4;
	return ax;
}

int ay(int y, const int Y = 1)
{
	y = (y + 1 - Y) / 2;
	return y;
}

int cx(int ax, const int X = 0)
{
	ax = 4 * ax - 2 + X;
	return ax;
}

int cy(int ay, const int Y = 1)
{
	ay = 2 * ay - 1 + Y;
	return ay;
}

void show_path_GUI(int path[][MAX_FRAME*MAX_FRAME], int sy, int sx, int ty, int tx, int a[][MAX_FRAME])
{
	int i = 0;//��㷽��Ϊ0�����Է����ȡ��1��ʼ
	int SX = sx, SY = sy;
	while (path[1][i] != -1) {
		switch (path[1][i++]) {//�U�T �[�X
			case 0://����
				showstr(cx((sx)), cy(sy), "��", COLOR_HWHITE, 8 - a[SY][SX]);
		
				showstr(cx((sx)), cy(sy) - 1, "��", 7 + a[SY][SX], 8 - a[SY][SX]);
				Sleep(200);
				showstr(cx((sx)), cy(sy) - 1, "�T", COLOR_HWHITE, 8 - a[SY][SX]);
			
				showstr(cx((sx)), cy(sy) - 2, "��", 7 + a[SY][SX], 8 - a[SY][SX]);
				sy--;
				break;
			case 1://����
				showstr(cx((sx)), cy(sy), "��", COLOR_HWHITE, 8 - a[SY][SX]);
				
				showstr(cx((sx)) - 2, cy(sy), "��", 7 + a[SY][SX], 8 - a[SY][SX]);
				Sleep(200);
				showstr(cx((sx)) - 2, cy(sy), "�U", COLOR_HWHITE, 8 - a[SY][SX]);
				
				showstr(cx((sx)) - 4, cy(sy), "��", 7 + a[SY][SX], 8 - a[SY][SX]);
				sx--;
				break;
			case 2://����
				showstr(cx((sx)), cy(sy), "��", COLOR_HWHITE, 8 - a[SY][SX]);
				
				showstr(cx((sx)) + 2, cy(sy), "��", 7 + a[SY][SX], 8 - a[SY][SX]);
				Sleep(200);
				showstr(cx((sx)) + 2, cy(sy), "�U", COLOR_HWHITE, 8 - a[SY][SX]);
				
				showstr(cx((sx)) + 4, cy(sy), "��", 7 + a[SY][SX], 8 - a[SY][SX]);
				sx++;
				break;
			case 3://����
				showstr(cx((sx)), cy(sy), "��", COLOR_HWHITE, 8 - a[SY][SX]);
				
				showstr(cx((sx)), cy(sy) + 1, "��", 7 + a[SY][SX], 8 - a[SY][SX]);
				Sleep(200);
				showstr(cx((sx)), cy(sy) + 1, "�T", COLOR_HWHITE, 8 - a[SY][SX]);
				
				showstr(cx((sx)), cy(sy) + 2, "��", 7 + a[SY][SX], 8 - a[SY][SX]);
				sy++;
				break;
		}
	}
}

int mouse_keyboard_control(int path[][MAX_FRAME*MAX_FRAME],int a[][MAX_FRAME],int row,int col)
{
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ȡ�����������ɶ�����ʾ
  ���������
  �� �� ֵ��˫���Ҽ�����loop=0���ж�Ϊ����Ϸ����������1�����ִ��
  ˵    ����
***************************************************************************/
	
	int loop=1;
	int ret, X = 0, Y = 0, maction = 0, keycode1 = 0, keycode2 = 0, KX = 1, KY = 1;
	int sx=0, sy=0, tx, ty;
	int flag = 0;//��¼״̬0��δѡ����㣬1����ѡ����㣬δѡ���յ㣬2��ѡ���յ㣬ִ���ƶ�����
	int count=0;

	if (a[1][1]) 
		showstr(cx(1), cy(1), "��", 7 + a[1][1], COLOR_BLACK);		
	else
		showstr(cx(1), cy(1), "��", COLOR_HWHITE, COLOR_BLACK);

	while (loop) {
		//Sleep(100);
		
		ret = read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);	
		if (ret == CCT_MOUSE_EVENT&& ax(X)>0 && ax(X) <= col && ay(Y)>0 &&ay(Y)<=row) {
			
			gotoxy(0, 20);
			setcolor(3,15);
			cout << "[��ǰ���λ��] " << char(ay(Y) + 'A' - 1)<<"��" << " " << ax(X) << " ��";
			switch (maction) {
				case MOUSE_ONLY_MOVED:
					cout << "�ƶ�          " << endl;
					//showch(X, Y, '*');����ӡ�κ�����
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//�������
					cout << "�������      " << endl;		
					count++;
					if (count == 1) {
						if (a[1][1]) {
							flag = 1;
							sx = 1;
							sy = 1;
						}
						else
							showstr(cx(1), cy(1), "��", COLOR_HWHITE, COLOR_BLACK);
					}
									
					if (a[ay(Y)][ax(X)]) {
						if (flag == 0) {
							showstr(cx(ax(X)), cy(ay(Y)), "��", 7 + a[ay(Y)][ax(X)], 8 - a[ay(Y)][ax(X)]);
							sx = ax(X);
							sy = ay(Y);
							flag = 2;
						}
						else if (flag == 1||flag==2) {
							showstr(cx((sx)), cy(sy), "��", 7 + a[sy][sx], 8 - a[sy][sx]);
							showstr(cx(ax(X)), cy(ay(Y)), "��", 7 + a[ay(Y)][ax(X)], 8 - a[ay(Y)][ax(X)]);	
							sx = ax(X);
							sy = ay(Y);
							flag = 2;
						}
					}
					else {
						if (flag == 1||flag==2) {
							tx = ax(X);
							ty = ay(Y);
							flag = 3;
						}
					}
					break;
				case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:	//˫�����
					cout << "˫�����      " << endl;
					//showch(X, Y, '2');			//�����ָ��λ����ʾ2
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//�����Ҽ�
					cout << "�����Ҽ�      " << endl;
					//showch(X, Y, '3');			//�����ָ��λ����ʾ3
					break;
				case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//˫���Ҽ�
					cout << "˫���Ҽ�      " << endl;
					//showch(X, Y, '4');	//�����ָ��λ����ʾ4
					loop = 0;
					break;
				case MOUSE_LEFTRIGHT_BUTTON_CLICK:		//ͬʱ�������Ҽ�
					cout << "ͬʱ�������Ҽ�" << endl;
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
					cout << "��������" << endl;
					//showch(X, Y, '0');			//�����ָ��λ����ʾ0
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			/* ת����9�н��д�ӡ */
			setcolor(3, 15);
			gotoxy(0, 21);
			cout << "[��ǰ����] : ";

			switch (keycode1) {
				case 13:    //Enter	
					if (flag == 0) {
						if (a[KY][KX]) {
							showstr(cx(KX), cy(KY), "��", 7 + a[KY][KX], 8 - a[KY][KX]);
							flag = 1;
							sx = KX;
							sy = KY;
						}
					}
					else if (flag == 1) {
						showstr(cx((sx)), cy(sy), "��", 7 + a[sy][sx], 8 - a[sy][sx]);
						showstr(cx(KX), cy(KY), "��", 7 + a[KY][KX], 8 - a[KY][KX]);
						flag = 1;
						sx = KX;
						sy = KY;
					}
					else if (flag == 2) {
						if (!a[KY][KX]) {
							flag = 3;
							tx = KX;
							ty = KY;
						}
						else {
							showstr(cx((sx)), cy(sy), "��", 7 + a[sy][sx], 8 - a[sy][sx]);
							showstr(cx(KX), cy(KY), "��", 7 + a[KY][KX], 8 - a[KY][KX]);
							flag = 1;
							sx = KX;
							sy = KY;
						}
					}
					gotoxy(14, 21);
					cout << "ENTER      ";
					break;	
				case 27:	//ESC��
					if (flag == 1 || flag == 2) {
						showstr(cx((sx)), cy(sy), "��", 7 + a[sy][sx], 8 - a[sy][sx]);
						flag = 0;
					}
					gotoxy(14, 21);
					cout << "ESC       ";
					break;
				case 32:	//SPACE��
					gotoxy(14, 21);
					cout << "SPACE     ";
					break;
				case 224:
					switch (keycode2) {
						case KB_ARROW_UP:
							if (KY > 1 && KY <= row && KX > 0 && KX <= col) {
								if (flag == 0 || flag == 2) {
									if (a[KY][KX])
										showstr(cx(KX), cy(KY), "��", 7 + a[KY][KX], 8 - a[KY][KX]);
									else
										showstr(cx(KX), cy(KY), "��", COLOR_HWHITE, 8 - a[KY][KX]);
								}
								else if (flag == 1)
									flag = 2;
								KY--;
								if (a[KY][KX])
									showstr(cx(KX), cy(KY), "��", 7 + a[KY][KX], 8 - a[KY][KX]);
								else
									showstr(cx(KX), cy(KY), "��", COLOR_HWHITE, 8 - a[KY][KX]);
							}
							gotoxy(14, 21);
							cout << "�ϼ�ͷ    ";
							break;
						case KB_ARROW_DOWN:
							if (KY > 0 && KY < row && KX > 0 && KX <= col) {
								if (flag == 0 || flag == 2) {
									if (a[KY][KX])
										showstr(cx(KX), cy(KY), "��", 7 + a[KY][KX], 8 - a[KY][KX]);
									else
										showstr(cx(KX), cy(KY), "��", COLOR_HWHITE, 8 - a[KY][KX]);
								}
								else if (flag == 1)
									flag = 2;
								KY++;
								if (a[KY][KX])
									showstr(cx(KX), cy(KY), "��", 7 + a[KY][KX], 8 - a[KY][KX]);
								else
									showstr(cx(KX), cy(KY), "��", COLOR_HWHITE, 8 - a[KY][KX]);
							}			
							gotoxy(14, 21);
							cout << "�¼�ͷ    ";
							break;
						case KB_ARROW_LEFT:
							if (KY > 0 && KY <= row && KX > 1 && KX <= col) {
								if (flag == 0 || flag == 2) {
									if (a[KY][KX])
										showstr(cx(KX), cy(KY), "��", 7 + a[KY][KX], 8 - a[KY][KX]);
									else
										showstr(cx(KX), cy(KY), "��", COLOR_HWHITE, 8 - a[KY][KX]);
								}
								else if (flag == 1)
									flag = 2;
								KX--;
								if (a[KY][KX])
									showstr(cx(KX), cy(KY), "��", 7 + a[KY][KX], 8 - a[KY][KX]);
								else
									showstr(cx(KX), cy(KY), "��", COLOR_HWHITE, 8 - a[KY][KX]);
							}
							gotoxy(14, 21);
							cout << "���ͷ    ";
							break;
						case KB_ARROW_RIGHT:
							if (KY > 0 && KY <= row && KX > 0 && KX < col) {
								if (flag == 0 || flag == 2) {
									if (a[KY][KX])
										showstr(cx(KX), cy(KY), "��", 7 + a[KY][KX], 8 - a[KY][KX]);
									else
										showstr(cx(KX), cy(KY), "��", COLOR_HWHITE, 8 - a[KY][KX]);
								}
								else if (flag == 1)
									flag = 2;
								KX++;
								if (a[KY][KX])
									showstr(cx(KX), cy(KY), "��", 7 + a[KY][KX], 8 - a[KY][KX]);
								else
									showstr(cx(KX), cy(KY), "��", COLOR_HWHITE, 8 - a[KY][KX]);
							}
							gotoxy(14, 21);
							cout << "�Ҽ�ͷ    ";
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
		
		if (flag == 3) {
			if (bfs(path, sy, sx, ty, tx, a, row, col)) {
				//����·��
				show_path_GUI(path, sy, sx, ty, tx,a);
				Move_ball(a, sy, sx, ty, tx);
			}
			else {
				gotoxy(0, 21);
				cout << "�޷��ҵ���Ч·��" << endl;
				flag = 0;
				continue;
			}
			flag = 0;
			break;
		}
	}//end of while(loop)
	return loop;//loop=0��ʾ�˳�
}

void print_scoreboard(int X, int Y, int sumscore)
{
	setcolor(COLOR_HWHITE, COLOR_BLACK);
	gotoxy(X, Y);
	cout << "�X�T�T�T�T�T�[";
	gotoxy(X, Y + 1);
	cout << "�U�÷֣�" << setw(3) << sumscore << " �U";
	gotoxy(X, Y + 2);
	cout << "�^�T�T�T�T�T�a";
}

void print_preboard(int a[][MAX_FRAME],int X,int Y,int last_score)
{
	int *color;
	color = Pre_next(a, 0, 0, 3, last_score);
	showstr(X, Y , "�X�T�j�T�j�T�[", COLOR_HWHITE, COLOR_BLACK);
	for (int i = 0; i < 3; i++) {
		showstr(X+4*i, Y + 1, "�U", COLOR_HWHITE, COLOR_BLACK);
		showstr(X + 2+4*i, Y + 1, "��", 7 + color[i], 8 - color[i]);
	}
	showstr(X+12, Y + 1, "�U", COLOR_HWHITE, COLOR_BLACK);
	showstr(X, Y + 2, "�^�T�m�T�m�T�a", COLOR_HWHITE, COLOR_BLACK);
}

void print_staticinfo(int X,int Y,int a[][MAX_FRAME], int row, int col)
{
	static int del[COLOR_NUM] = { 0 };
	int now[COLOR_NUM] = { 0 };
	float pro[COLOR_NUM] = {0};
	static_data(now, pro, del, a, row, col);
	showstr(X, Y, "�X�T�T�T�T�T�T�T�T�T�T�T�[", COLOR_HWHITE, COLOR_BLACK);
	
	showstr(X, Y + 1 , "�U", COLOR_HWHITE, COLOR_BLACK);
	showstr(X + 2, Y + 1 , "��", COLOR_HWHITE, COLOR_BLACK);
	setcolor(COLOR_HWHITE, COLOR_BLACK);
	cout << ":" << right << setw(2) << now[0] << "/(";
	cout << setiosflags(ios::fixed) << setprecision(2) << pro[0] << "%) del- 0";
	cout << "  �U";

	for (int i = 1; i < 8; i++) {
		showstr(X , Y + 1 + i, "�U", COLOR_HWHITE, COLOR_BLACK);
		showstr(X+2, Y + 1 + i, "��", 7+i, 8-i);
		setcolor(COLOR_HWHITE, COLOR_BLACK);
		cout << ":" << right << setw(2) << now[i] << "/(";
		cout << setiosflags(ios::fixed) <<setprecision(2)<< pro[i] << "%) del-" <<right<<setw(2)<< del[i];
		cout << "  �U";
	}
	showstr(X, Y+9, "�^�T�T�T�T�T�T�T�T�T�T�T�a", COLOR_HWHITE, COLOR_BLACK);
}

void Game4(int a[][MAX_FRAME], int row, int col)
{
	int X, Y, opt = 0;
	Init_array(a, row, col, 5);
	Interface_array(0, 5, row, col, a, opt++);
	getxy(X, Y);
	show_GUI(Y + 1);
	setconsoleborder(30, 30,100,30);
	print_GUI_without(a, row, col, 0);
	getxy(X, Y);
	End(Y + 2);
}

void Game5(int a[][MAX_FRAME], int row, int col)
{
	int X, Y, opt = 0;
	Init_array(a, row, col, 5);
	Interface_array(0, 5, row, col, a, opt++);
	getxy(X, Y);
	show_GUI(Y + 1);
	print_GUI_with(a, row, col, 0);
	getxy(X, Y);
	//setconsoleborder(35, 25, 35, 25);
	End(Y + 3);
}

void Game6(int a[][MAX_FRAME], int row, int col)
{
	int X, Y;
	int  sy = 0, ty = 0, sx = 0, tx = 0;
	int opt = 0;
	int path[2][MAX_FRAME*MAX_FRAME];
	Init_array(a, row, col, int(row*col*0.6));
	//Interface_array(0, 5, row, col, a, opt++);
	getxy(X, Y);
	show_GUI(Y+1);
	setconsoleborder(40, 30, 100, 30);
	print_GUI_with(a, row, col, 0);
	//Pre_next(a, X, Y + 1, 3, 1);
	//���ѡ��Ŀ������ƶ�����Ŀ��
	enable_mouse();
	mouse_keyboard_control(path, a, row, col);
	End(21);
}

void Game7(int a[][MAX_FRAME], int row, int col)
{
	int X, Y;
	int path[2][MAX_FRAME*MAX_FRAME];
	int *color;
	int score = 0, sumscore = 0;
	Init_array(a, row, col, 5);
	//Interface_array(0, 5, row, col, a, opt++);
	getxy(X, Y);
	show_GUI(Y + 1);
	setconsoleborder(70, 40, 100, 100);
	print_scoreboard(40,1,sumscore);
	print_preboard(a, 40, 5, 1);
	print_GUI_with(a, row, col, 1);
	print_staticinfo(40, 10, a, row, col);
	//���ѡ��Ŀ������ƶ�����Ŀ��
	enable_mouse();
	while (!Gameover(a,row,col)) {
		if (mouse_keyboard_control(path, a, row, col)) {//��������� 	
			score = Check(a, row, col);
			sumscore += score;
			color = Pre_next(a, 0, 20, 3, 0);
			if(!score)
				Do_next(a, row, col, color, 3);
			print_scoreboard(40, 1, sumscore);
			print_preboard(a, 40, 5, 1);
			print_GUI_with(a, row, col, 1);
			print_staticinfo(40, 10, a, row, col);
		/*	else {
			Move_ball(a, sy, sx, ty, tx);
			Do_next(a, row, col, color, 3);
			getxy(X, Y);
			Interface_array(0, Y + 2, row, col, a, -1);
			score = Check(a, row, col);
			sumscore += score;
			cout << "���ε÷֣�" << score << " �ܵ÷֣�" << sumscore << endl;
		}*/

		}
		else
			break;
	}
	End(21);
}

void Game8(int a[][MAX_FRAME], int row, int col)
{
	int X, Y;
	int path[2][MAX_FRAME*MAX_FRAME];
	int *color;
	int score = 0, sumscore = 0;
	Init_array(a, row, col, 5);
	//Interface_array(0, 5, row, col, a, opt++);
	getxy(X, Y);
	show_GUI(Y + 1);
	setconsoleborder(70, 40, 100, 100);
	print_scoreboard(40, 1, sumscore);
	print_preboard(a, 40, 5, 1);
	print_GUI_with(a, row, col, 1);
	print_staticinfo(40, 10, a, row, col);
	//���ѡ��Ŀ������ƶ�����Ŀ��
	enable_mouse();
	while (!Gameover(a, row, col)) {
		if (mouse_keyboard_control(path, a, row, col)) {//��������� 	
			score = Check(a, row, col);
			sumscore += score;
			color = Pre_next(a, 0, 20, 3, 0);
			if (!score)
				Do_next(a, row, col, color, 3);
			print_scoreboard(40, 1, sumscore);
			print_preboard(a, 40, 5, 1);
			print_GUI_with(a, row, col, 1);
			print_staticinfo(40, 10, a, row, col);
		}
		else
			break;
	}
	End(21);


}