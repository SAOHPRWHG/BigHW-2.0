/* 1751957 ��2�� ������ */
#include <iostream>
#include "../common/cmd_console_tools.h"
#include "90-b1.h"
using namespace std;

const char *Frame[] = { "��","��","��","��","��","��","��","��","��" };
const char *frame[] = { "����","�ש�","��","����","��","�ǩ�","�贈","��","����","��","����","�ߩ�","��"};
                      //"�X�T","�j�T","�[","�U��","�U","�d�T","�p�T","�g","�U��","�U","�^�T","�m�T","�a"

void show_GUI(int Y)
{
	gotoxy(0, Y);
	cout << "���س�����ʾͼ��..." << endl;
	char ch;
	while ((ch = _getch()) != '\r')
		;

	cls();
	setfontsize("������", 26);
	setconsoleborder(120, 60, 600, 600);
	cout << "Ϊ�ζ��������ζ���" << endl;
}

int ax(int x, int separator=1, int rc_sign=1)
{
	//����cmd�������꣬������������
	int ax;
	int X;
	if (separator == 1) {
		if(rc_sign==0)
			X = 0;
		if(rc_sign==1)
			X = 2;	
		if ((x - X) % 2)
			ax = (x + 1 - X) / 4 - 1;
		else
			ax = (x + 2 - X) / 4 - 1;		
	}
	return ax;
}

int ay(int y, int separator=1, int rc_sign=1)
{
	int Y;
	if (separator == 1) {
		if (rc_sign == 0)
			Y = 1;
		if (rc_sign == 1)
			Y = 2;
		y = (y + 1 - Y) / 2 - 1;
	}
	return y;
}

int cx(int ax, int separator=1, int rc_sign=1)
{
	//�����������꣬ת��Ϊcmd�����Ӧ����
	int X;
	if (rc_sign)
		X = 2;
	else
		X = 0;
	if (separator) {
		ax = 4 * ax + 2 + X;
	}
	else {
		ax = 2 * ax + 2 + X;
	}
	return ax;
}

int cy(int ay, int separator=1, int rc_sign=1)
{
	int Y;
	if (rc_sign)
		Y = 2;
	else
		Y = 1;
	if(separator)
		ay = 2 * ay + 1 + Y;
	else
		ay = ay + 1 + Y;
	return ay;
}

void draw_the_ball(struct ball_info ball)
{
	const char *str[] = { "��","��","��","��","��","��" };
	showstr(cx(ball.col, ball.separator , ball.rc_sign), cy(ball.row, ball.separator, ball.rc_sign), str[ball.status], ball.bg_color, ball.front_color);
}

void print_rc_sign(int X, int Y, int separator, int rc_sign,int row,int col, const char c[],const char r[])
{
	int i, gap=1;
	if (rc_sign) {
		cout << "   ";
		if (separator) {
			gap = 2;
		}		
		for (i = 0; i < col; i++) {
			cout << setw(2 * gap) << c[i];
		}
		for (i = 0; i < row; i++) {
			gotoxy(X, Y + gap * i+2);
			cout << left<< setw(2) << r[i];
		}
		gotoxy(X + 2, Y + 1);
	}
}

void Remove_GUI(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j].status == WILL_REMOVE) {
				showstr(cx(map[i][j].col, map[i][j].separator, map[i][j].rc_sign), cy(map[i][j].row, map[i][j].separator, map[i][j].rc_sign), "��", map[i][j].bg_color, map[i][j].front_color);
				Sleep(200);
				showstr(cx(map[i][j].col, map[i][j].separator, map[i][j].rc_sign), cy(map[i][j].row, map[i][j].separator, map[i][j].rc_sign), "��", COLOR_HWHITE);
			}
		}
	}
	setcolor(3, 15);
}

void Fallen_GUI(int sy, int sx, int ty, int tx, struct ball_info map[][MAX_FRAME])
{
	//��ʾ����켣
	for (; sy < ty; sy++) {
		showstr(cx((sx)), cy(sy), "��", COLOR_HWHITE);

		showstr(cx((sx)), cy(sy) + 1, "��", map[sy][sx].bg_color, map[sy][sx].front_color);
		Sleep(100);
		showstr(cx((sx)), cy(sy) + 1, Frame[1], COLOR_HWHITE);

		showstr(cx((sx)), cy(sy) + 2, "��", map[sy][sx].bg_color, map[sy][sx].front_color);
	}
	setcolor(3, 15);
}

void Exchange_GUI(int sy, int sx, int ty, int tx, struct ball_info map[][MAX_FRAME])
{
	int direct = 0;
	if (sy - ty == -1)
		direct = 3;//����
	else if (sy - ty == 1)
		direct = 0;//down
	if (sx - tx == -1)
		direct = 2;//right
	else if (sx - tx == 1)
		direct = 1;//left

	switch (direct) {//�U�T �[�X
		case 0://1�����ϣ�2����
			showstr(cx((sx)), cy(sy), "��", COLOR_HWHITE);

			showstr(cx((sx)), cy(sy) - 1, "��", map[sy][sx].bg_color, map[sy][sx].front_color);
			Sleep(200);

			showstr(cx((tx)), cy(ty), "��", COLOR_HWHITE);

			showstr(cx((tx)), cy(ty) + 1, "��", map[ty][tx].bg_color, map[ty][tx].front_color);
			Sleep(200);


			showstr(cx((sx)), cy(sy) - 1, Frame[1], COLOR_HWHITE);

			showstr(cx((sx)), cy(sy) - 2, "��", map[sy][sx].bg_color, map[sy][sx].front_color);
			Sleep(200);

			showstr(cx((tx)), cy(ty) + 1, Frame[1], COLOR_HWHITE);

			showstr(cx((tx)), cy(ty) + 2, "��", map[ty][tx].bg_color, map[ty][tx].front_color);

			break;
		case 1://����
			showstr(cx((sx)), cy(sy), "��", COLOR_HWHITE);

			showstr(cx((sx)) - 2, cy(sy), "��", map[sy][sx].bg_color, map[sy][sx].front_color);
			Sleep(200);

			showstr(cx((tx)), cy(ty), "��", COLOR_HWHITE);

			showstr(cx((tx)) + 2, cy(ty), "��", map[ty][tx].bg_color, map[ty][tx].front_color);
			Sleep(200);

			showstr(cx((sx)) - 2, cy(sy), Frame[3], COLOR_HWHITE);

			showstr(cx((sx)) - 4, cy(sy), "��", map[sy][sx].bg_color, map[sy][sx].front_color);
			Sleep(200);

			showstr(cx((tx)) + 2, cy(ty), Frame[3], COLOR_HWHITE);

			showstr(cx((tx)) + 4, cy(ty), "��", map[ty][tx].bg_color, map[ty][tx].front_color);

			break;
		case 2://����
			showstr(cx((sx)), cy(sy), "��", COLOR_HWHITE);

			showstr(cx((sx)) + 2, cy(sy), "��", map[sy][sx].bg_color, map[sy][sx].front_color);
			Sleep(200);

			showstr(cx((tx)), cy(ty), "��", COLOR_HWHITE);

			showstr(cx((tx)) - 2, cy(ty), "��", map[ty][tx].bg_color, map[ty][tx].front_color);
			Sleep(200);

			showstr(cx((sx)) + 2, cy(sy), Frame[3], COLOR_HWHITE);

			showstr(cx((sx)) + 4, cy(sy), "��", map[sy][sx].bg_color, map[sy][sx].front_color);
			Sleep(200);

			showstr(cx((tx)) - 2, cy(ty), Frame[3], COLOR_HWHITE);

			showstr(cx((tx)) - 4, cy(ty), "��", map[ty][tx].bg_color, map[ty][tx].front_color);

			break;
		case 3://����
			showstr(cx((sx)), cy(sy), "��", COLOR_HWHITE);

			showstr(cx((sx)), cy(sy) + 1, "��", map[sy][sx].bg_color, map[sy][sx].front_color);
			Sleep(200);

			showstr(cx((tx)), cy(ty), "��", COLOR_HWHITE);

			showstr(cx((tx)), cy(ty) - 1, "��", map[ty][tx].bg_color, map[ty][tx].front_color);
			Sleep(200);

			showstr(cx((sx)), cy(sy) + 1, Frame[1], COLOR_HWHITE);

			showstr(cx((sx)), cy(sy) + 2, "��", map[sy][sx].bg_color, map[sy][sx].front_color);
			Sleep(200);

			showstr(cx((tx)), cy(ty) - 1, Frame[1], COLOR_HWHITE);

			showstr(cx((tx)), cy(ty) - 2, "��", map[ty][tx].bg_color, map[ty][tx].front_color);

			break;
	}
	setcolor(3, 15);
}

void print_GUI(const char *frame[],int separator, int rc_sign,struct ball_info map[][MAX_FRAME], int row, int col, int opt)
{
	gotoxy(0, 0);
	int X, Y;
	int cols, lines, buffer_cols, buffer_lines;
	getconsoleborder(cols, lines, buffer_cols, buffer_lines);
	cout << "��Ļ��" << cols << "��" << lines << "��";
	if (opt)
		cout << "(˫���Ҽ��˳�)";
	cout << endl;
	getxy(X, Y);
	print_rc_sign(X, Y, separator, rc_sign,row,col,"012345678","ABCDEFGHI");
	getxy(X, Y);

	//��ʼ����
	if (!separator) {
		showstr(X, Y, frame[0], COLOR_HWHITE, COLOR_BLACK);
		showstr(X + 2, Y, frame[1], COLOR_HWHITE, COLOR_BLACK, col);
		showstr(X + 2 + col * 2, Y, frame[2], COLOR_HWHITE, COLOR_BLACK);
		// "�X�T�[�U�U�T�a"
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
		//frame_with_sep = {"�X�T","�j�T","�[","�U��","�U","�d�T","�p�T","�g","�U��","�U","�^�T","�m�T","�a"}
		const int b = 4;
		showstr(X, Y, frame[0], COLOR_HWHITE, COLOR_BLACK);
		showstr(X + b, Y, frame[1], COLOR_HWHITE, COLOR_BLACK, col - 1);
		showstr(X + col * b, Y, frame[2], COLOR_HWHITE, COLOR_BLACK);
		showstr(X, Y + 1, frame[3], COLOR_HWHITE, COLOR_BLACK, col);
		showstr(X + col * b, Y + 1, frame[4], COLOR_HWHITE, COLOR_BLACK);

		//�U�T �[�X
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

	//����
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
	setcolor(3,15);
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
	setcolor(3, 15);
}

int mouse_keyboard_control( struct ball_info map[][MAX_FRAME], int row, int col)
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
	int sx = 0, sy = 0, tx, ty;
	int flag = 0;//��¼״̬0��δѡ����㣬1����ѡ����㣬δѡ���յ㣬2��ѡ���յ㣬ִ���ƶ�����
	int count = 0;
	const int UNDER_TABLE = row * 2 + 4;

	while (loop) {
		//Sleep(100);

		ret = read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		if (ret == CCT_MOUSE_EVENT && ax(X) >= 0 && ax(X) < col && ay(Y) >= 0 && ay(Y) < row) {

			gotoxy(0, UNDER_TABLE-1);
			setcolor(3, 15);
			cout << "[��ǰ���λ��] " << char(ay(Y) + 'A') << "��" << " " << ax(X) << " ��";
			switch (maction) {
				case MOUSE_ONLY_MOVED:
					cout << "�ƶ�          " << endl;
					//showch(X, Y, '*');����ӡ�κ�����
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//�������
					cout << "�������      " << endl;
					count++;

					if (map[ay(Y)][ax(X)].status==PRE_REMIND) {
						if (flag == 0) {
							showstr(cx(ax(X)), cy(ay(Y)), "��", map[ay(Y)][ax(X)].bg_color, map[ay(Y)][ax(X)].front_color);
							sx = ax(X);
							sy = ay(Y);
							flag = 2;
						}
						/*else if (flag == 1 || flag == 2) {
							showstr(cx((sx)), cy(sy), "��", map[sy][sx].bg_color, map[sy][sx].front_color);
							showstr(cx(ax(X)), cy(ay(Y)), "��", map[ay(Y)][ax(X)].bg_color, map[ay(Y)][ax(X)].front_color);
							sx = ax(X);
							sy = ay(Y);
							flag = 2;
						}*/
						else {
							if (flag == 1 || flag == 2) {
								tx = ax(X);
								ty = ay(Y);
								flag = 3;
							}
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
	

		if (flag == 3) {
			if (Judge_select(sy,sx,ty,tx,map,row,col)) {
				//��������·��
				Exchange_GUI(sy, sx, ty, tx, map);
			}
			else {
				gotoxy(0, UNDER_TABLE-1);
				cout << "�޷���������                       " << endl;
				showstr(cx((tx)), cy(ty), "��", map[ty][tx].bg_color, map[ty][tx].front_color);
				showstr(cx((sx)), cy(sy), "��", map[sy][sx].bg_color, map[sy][sx].front_color);
				flag = 0;
				continue;
			}
			flag = 0;
			break;
		}
	}//end of while(loop)
	return loop;//loop=0��ʾ�˳�
}

void GameD(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int X, Y;
	Init_array(map, row, col);
	Interface_array(0,5,map,row,col,"��ʼ���飺",1);
	getxy(X, Y);
	show_GUI(Y + 1);
	setconsoleborder(50, 30, 100, 30);
	//frame_with_sep = {"�X�T","�j�T","�[","�U��","�U","�d�T","�p�T","�g","�U��","�U","�^�T","�m�T","�a"}
	print_GUI(Frame,0,1,map, row, col, 0);
	getxy(X, Y);
	End(Y + 2);
}

void GameE(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int X, Y;
	Init_array(map, row, col);
	Interface_array(0, 5, map, row, col, "��ʼ���飺", 1);
	getxy(X, Y);
	show_GUI(Y + 1);
	setconsoleborder(50, 30, 100, 30);
	//frame_with_sep = {"�X�T","�j�T","�[","�U��","�U","�d�T","�p�T","�g","�U��","�U","�^�T","�m�T","�a"}
	print_GUI(frame, 1, 1, map, row, col, 0);
	getxy(X, Y);
	End(Y + 2);
}

void GameF(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int X, Y;
	Init_array(map, row, col);
	Interface_array(0, 5, map, row, col, "��ʼ���飺", 1);
	getxy(X, Y);
	show_GUI(Y + 1);
	setconsoleborder(50, 50, 100, 30);
	const char *frame[] = { "�X�T","�j�T","�[","�U��","�U","�d�T","�p�T","�g","�U��","�U","�^�T","�m�T","�a" };
	//frame_with_sep = {"�X�T","�j�T","�[","�U��","�U","�d�T","�p�T","�g","�U��","�U","�^�T","�m�T","�a"}
	print_GUI(frame, 1, 1, map, row, col, 0);
	getxy(X, Y);
	Read_enter(Y + 2, "���س�������Ѱ�ҳ�ʼ��������Ĳ���...");
	if (Check(map, row, col)) {
		print_GUI(frame, 1, 1, map, row, col, 0);
	}
	else 
		cout << endl << "��ʼ�޿�������";
	End(Y + 2);
}

void GameG(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int X, Y;
	const int UNDER_TABLE = row*2+4;
	Init_array(map, row, col);
	Interface_array(0, 5, map, row, col, "��ʼ���飺", 1);
	getxy(X, Y);
	show_GUI(Y + 1);
	setconsoleborder(50, 50, 100, 30);
	const char *frame[] = { "�X�T","�j�T","�[","�U��","�U","�d�T","�p�T","�g","�U��","�U","�^�T","�m�T","�a" };
	//frame_with_sep = {"�X�T","�j�T","�[","�U��","�U","�d�T","�p�T","�g","�U��","�U","�^�T","�m�T","�a"}
	print_GUI(frame, 1, 1, map, row, col, 0);
	getxy(X, Y);
	Read_enter(UNDER_TABLE, "���س�������Ѱ�ҳ�ʼ��������Ĳ���...");
	
	while (Check(map, row, col)) {
		//Interface_array(X, Y + 2, map, row, col, "��ʼ���������ͬɫ��ʶ����", WILL_REMOVE);
		print_GUI(frame, 1, 1, map, row, col, 0);
		Remove_GUI(map, row, col);

		Read_enter(UNDER_TABLE, "���س����������������0����...        ");
		Fallen(map, row, col, 1);
		//Interface_array(X, Y + 2, map, row, col, "�����0�������(��ͬɫ��ʶ)��", NEW_FILL);
		//print_GUI(frame, 1, 1, map, row, col, 0);
		
		Read_enter(UNDER_TABLE, "���س���������ֵ���...              ");
		Fill(map, row, col, 1);
		//Interface_array(X, Y + 2, map, row, col, "�����0�������(��ͬɫ��ʶ)��", NEW_FILL);
		print_GUI(frame, 1, 1, map, row, col, 0);

		Clear_status(map, row, col);
	}
	gotoxy(0, UNDER_TABLE);
	cout << "��ʼ���޿�������                       ";

	Read_enter(UNDER_TABLE, "���س�����ʾ������ʾ...                ");
	//����Ԥ����ʾ
	if (Pre_reminder(map, row, col, 1));
	else {
		gotoxy(0, UNDER_TABLE);
		cout << "����������ʾ����Ϸ����                      ";
	}
	getxy(X, Y);
	End(UNDER_TABLE);
}

void GameH(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int X, Y;
	int score = 0, sumscore = 0;
	const int UNDER_TABLE = 22;
	Init_array(map, row, col);
	Interface_array(0, 5, map, row, col, "��ʼ���飺", 1);
	getxy(X, Y);
	show_GUI(Y + 1);
	setconsoleborder(60, 50, 100, 30);
	//frame_with_sep = {"�X�T","�j�T","�[","�U��","�U","�d�T","�p�T","�g","�U��","�U","�^�T","�m�T","�a"}
	print_scoreboard(42, 2, sumscore);
	print_GUI(frame, 1, 1, map, row, col, 0);
	getxy(X, Y);
	//��ʼ������
	while ( Check(map, row, col)) {
		print_GUI(frame, 1, 1, map, row, col, 0);
		Remove_GUI(map, row, col);
		Fallen(map, row, col, 1);

		Fill(map, row, col, 1);
		print_GUI(frame, 1, 1, map, row, col, 0);

		Clear_status(map, row, col);
	}
	gotoxy(0, UNDER_TABLE);
	cout << "��ʼ���޿�������                       ";
	
	//��ʽ����
	enable_mouse();
	while (Pre_reminder(map, row, col, 1)) {
		if (mouse_keyboard_control(map,row,col)) {
			while (score = Check(map, row, col)) {
				print_GUI(frame, 1, 1, map, row, col, 0);
				Remove_GUI(map, row, col);
				sumscore += score;
				print_scoreboard(42, 2, sumscore);
				Fallen(map, row, col, 1);

				Fill(map, row, col, 1);
				print_GUI(frame, 1, 1, map, row, col, 0);

				Clear_status(map, row, col);
			}
		}
		else
			break;
	}

	gotoxy(0, UNDER_TABLE-1);
	cout << "���޿���������Ϸ����                      ";
	getxy(X, Y);
	End(UNDER_TABLE);
}


//finish

