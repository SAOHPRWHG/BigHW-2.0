#include "G2048.h"


int G2048::up_align(int c)
{
	int p1 = 0, p2 = 0;//p1ΪĿ��λ�ã�p2Ϊԭλ��
	int tem;
	for (;1; p1++) {
		for (;p1 < row && map[p1][c].value != 0 ; p1++)
			;
		for (p2 = p1; p2 < row && map[p2][c].value == 0; p2++)
			;
		if (p1 == row || p2 == row)
			break;//˵�������ˣ�����˵���ҵ�
		//����ҵ��򽻻�ֵ
		tem = map[p1][c].value;
		map[p1][c].value = map[p2][c].value;
		map[p2][c].value = tem;
		map[p2][c].dcol = c;
		map[p2][c].drow = p1;
	}
	return 0;
}

int G2048::down_align(int c)
{
	int p1 = row-1, p2 = row-1;//p1ΪĿ��λ�ã�p2Ϊԭλ��
	int tem;
	for (; 1; p1--) {
		for (; p1 >= 0 && map[p1][c].value != 0; p1--)
			;
		for (p2 = p1; p2 >= 0 && map[p2][c].value == 0; p2--)
			;
		if (p1 < 0 || p2 < 0)
			break;//˵�������ˣ�����˵���ҵ�
		//����ҵ��򽻻�ֵ
		tem = map[p1][c].value;
		map[p1][c].value = map[p2][c].value;
		map[p2][c].value = tem;
		map[p2][c].dcol = c;
		map[p2][c].drow = p1;
	}
	return 0;
}

int G2048::left_align(int r)
{
	int p1 = 0, p2 = 0;//p1ΪĿ��λ�ã�p2Ϊԭλ��
	int tem;
	for (; 1; p1++) {
		for (; p1 < col && map[r][p1].value != 0; p1++)
			;
		for (p2 = p1; p2 < col && map[r][p2].value == 0; p2++)
			;
		if (p1 == col || p2 == col)
			break;//˵�������ˣ�����˵���ҵ�
		//����ҵ��򽻻�ֵ
		tem = map[r][p1].value;
		map[r][p1].value = map[r][p2].value;
		map[r][p2].value = tem;
		map[r][p2].dcol = p1;
		map[r][p2].drow = r;
	}
	return 0;
}

int G2048::right_align(int r)
{
	int p1 = col-1, p2 = col - 1;//p1ΪĿ��λ�ã�p2Ϊԭλ��
	int tem;
	for (; 1; p1--) {
		for (; p1 >= 0 && map[r][p1].value != 0; p1--)
			;
		for (p2 = p1; p2 >= 0 && map[r][p2].value == 0; p2--)
			;
		if (p1 < 0 || p2 < 0)
			break;//˵�������ˣ�����˵���ҵ�
		//����ҵ��򽻻�ֵ
		tem = map[r][p1].value;
		map[r][p1].value = map[r][p2].value;
		map[r][p2].value = tem;
		map[r][p2].dcol = p1;
		map[r][p2].drow = r;
	}
	return 0;
}

int G2048::up_check(int c)
{
	int p1;
	for (p1 = 0; p1 + 1 < row; p1++) {
		if (map[p1][c].value == 0)
			break;
		if (map[p1 + 1][c].value == map[p1][c].value) {
			
			map[p1][c].value *= 2;
			map[p1][c].status = WAIT_COMBINE;
			
			map[p1 + 1][c].value = 0;
			map[p1 + 1][c].status = COMBINE;
			map[p1 + 1][c].drow = p1;
		}		
	}
	up_align(c);
	return 0;
}

int G2048::down_check(int c)
{
	int p1;
	for (p1 = row - 1; p1 - 1 >= 0; p1--) {
		if (map[p1][c].value == 0)
			break;
		if (map[p1 - 1][c].value == map[p1][c].value) {

			map[p1][c].value *= 2;
			map[p1][c].status = WAIT_COMBINE;

			map[p1 - 1][c].value = 0;
			map[p1 - 1][c].status = COMBINE;
			map[p1 - 1][c].drow = p1;
		}
	}
	down_align(c);
	return 0;
}

int G2048::left_check(int r)
{
	int p1;
	for (p1 = 0; p1 + 1 < col; p1++) {
		if (map[r][p1].value == 0)
			break;
		if (map[r][p1 + 1].value == map[r][p1].value) {

			map[r][p1].value *= 2;
			map[r][p1].status = WAIT_COMBINE;

			map[r][p1 + 1].value = 0;
			map[r][p1 + 1].status = COMBINE;
			map[r][p1 + 1].dcol = p1;
		}
	}
	left_align(r);
	return 0;
}

int G2048::right_check(int r)
{
	int p1;
	for (p1 = col - 1; p1 - 1 >= 0; p1--) {
		if (map[r][p1].value == 0)
			break;
		if (map[r][p1 - 1].value == map[r][p1].value) {

			map[r][p1].value *= 2;
			map[r][p1].status = WAIT_COMBINE;

			map[r][p1 - 1].value = 0;
			map[r][p1 - 1].status = COMBINE;
			map[r][p1 - 1].dcol = p1;//�˴������ص�����
		}
	}
	right_align(r);
	return 0;
}

G2048::G2048(const int Row, const int Col, const int Dst_score, const int Speed, const bool Sep)
{
	score = 0;
	high_score = 0;
	row = Row;
	col = Col;
	dst_score = Dst_score;
	speed = Speed;
	separator = Sep;
	int i = 0;
	map = new BLOCK*[Row];
	for (i = 0; i < Col; i++)
		map[i] = new BLOCK[Col];

}

G2048::~G2048()
{
	int i;
	for (i = 0; i < col; i++)
		delete[]map[i];
	delete[]map;
}

int G2048::set(const int Row, const int Col, const int Dst_score, const int Speed, const bool Sep)
{
	//���ͷ�
	int i;
	for (i = 0; i < col; i++)
		delete[]map[i];
	delete[]map;

	//������
	score = 0;
	row = Row;
	col = Col;
	dst_score = Dst_score;
	speed = Speed;
	separator = Sep;
	map = new BLOCK*[Row];
	for (i = 0; i < Col; i++)
		map[i] = new BLOCK[Col];
	return 0;
}

int G2048::Produce_new_block(const int num, const bool show)
{
	//�������������
	unsigned int t;
	t = (unsigned int)time(0);
	srand(t);

	int i;
	for (i = 0; i < num; i++) {
		int two_or_four = rand() % 10;
		int new_num, new_row, new_col;
		if (two_or_four < 7) //����2
			new_num = 2;
		else
			new_num = 4;
		do {
			new_row = rand() % row;
			new_col = rand() % col;
		} while (map[new_row][new_col].value);

		map[new_row][new_col].value = new_num;
		map[new_row][new_col].status = NORMAL;

		/*��ʾ��ɫ��*/
		if (show) {
			gmw_draw_block(&G2048_CGI, new_row, new_col, new_num, bdi_normal);
		}
	}
	
	return 0;
}

int G2048::Init_map()
{
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			map[i][j].value = 0;
			map[i][j].status = 0;
		}
	}

	Produce_new_block(2, false);//�ڲ�����������ʾɫ��show = false
	return 0;
}

int G2048::Init_frame()
{
	/*��ʼ���߿�*/
	gmw_init(&G2048_CGI);

	gmw_set_color(&G2048_CGI, COLOR_BLACK, COLOR_WHITE);			//����������ɫ
	gmw_set_font(&G2048_CGI, "������", 16);						//����
	gmw_set_frame_style(&G2048_CGI, 10, 5, true);					//��Ϸ��������ÿ��ɫ���10��5���зָ��ߡ�����ɫ����߿򣬿��Ϊ10(�����6λ����)���߶�Ϊ5(Ϊ�˱���ɫ��Ϊ����)��
	gmw_set_frame_color(&G2048_CGI, COLOR_WHITE, COLOR_BLACK);	//��Ϸ��������ɫ
	gmw_set_block_border_switch(&G2048_CGI, true);				//Сɫ����߿�
	gmw_set_rowno_switch(&G2048_CGI, true);						//��ʾ�к�
	gmw_set_colno_switch(&G2048_CGI, true);
	
	/* ��row/col��ֵ������Ϸ���������� */
	gmw_set_rowcol(&G2048_CGI, row, col);

	cls();
	/* ��ʾ��� */
	gmw_draw_frame(&G2048_CGI);

	/*��ʾ��*/
	int i, j;
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++) {
			gmw_draw_block(&G2048_CGI, i, j, map[i][j].value, bdi_normal);
		}

	return 0;
}

int G2048::Game_start()
{
	Init_map();
	Init_frame();

	int loop = 1;
	int maction, mrow, mcol;
	int keycode1, keycode2;
	int ret;
	//char temp[CHAR_LEN];

	while (1) {
		switch (Gameover()) {
		case 0:
			break;
		case 'F':
			gmw_status_line(&G2048_CGI, LOWER_STATUS_LINE, "������δ�ɹ���ͬ־����Ŭ��~", NULL);
			break;
		case 'W':
			gmw_status_line(&G2048_CGI, LOWER_STATUS_LINE, "Winner, You are standing the top of the world!", NULL);
			break;
		}

		ret = gmw_read_keyboard_and_mouse(&G2048_CGI, maction, mrow, mcol, keycode1, keycode2);

		/*���̿���*/
		if (ret == CCT_KEYBOARD_EVENT)  { //CCT_KEYBOARD_EVENT - ������뺬��ȥ�� cmd_console_tools.cpp
			switch (keycode1) {
			case 'R':
			case 'r':
				return 'r';
				break;
			case 'Q':
			case 'q':
				return 'Q';
				break;
			case 0x0D:
				/* ��״̬����ʾ���� */
				//gmw_status_line(&G2048_CGI, LOWER_STATUS_LINE, "[�����س���]", NULL);
				break;
			case 0xE0: //224
				switch (keycode2) {
				case KB_ARROW_UP:
					/* ��״̬����ʾ���� */
					gmw_status_line(&G2048_CGI, LOWER_STATUS_LINE, "[�����ϼ�ͷ]", NULL);
					//Align(DOWN_TO_UP);//����
					score += Check_remove(DOWN_TO_UP);//����������Ʒ�
					Move_animation(DOWN_TO_UP);//��ʾ�ƶ��ϲ�����
					Clear_status();//����״̬
					Produce_new_block(1);//���ɲ���ʾ���򣬼ǵ�����Ҫ����״̬
					break;
				case KB_ARROW_DOWN:
					/* ��״̬����ʾ���� */
					gmw_status_line(&G2048_CGI, LOWER_STATUS_LINE, "[�����¼�ͷ]", NULL);
					//Align(UP_TO_DOWN);//����
					score += Check_remove(UP_TO_DOWN);//����������Ʒ�
					Move_animation(UP_TO_DOWN);//��ʾ�ƶ��ϲ�����
					Clear_status();//����״̬
					Produce_new_block(1);//���ɲ���ʾ���򣬼ǵ�����Ҫ����״̬
					break;
				case KB_ARROW_LEFT:
					/* ��״̬����ʾ���� */
					gmw_status_line(&G2048_CGI, LOWER_STATUS_LINE, "[�������ͷ]", NULL);
					//Align(RIGHT_TO_LEFT);//����
					score += Check_remove(RIGHT_TO_LEFT);//����������Ʒ�
					Move_animation(RIGHT_TO_LEFT);//��ʾ�ƶ��ϲ�����
					Clear_status();//����״̬
					Produce_new_block(1);//���ɲ���ʾ���򣬼ǵ�����Ҫ����״̬
					break;
				case KB_ARROW_RIGHT:
					/* ��״̬����ʾ���� */
					gmw_status_line(&G2048_CGI, LOWER_STATUS_LINE, "[�����Ҽ�ͷ]", NULL);
					//Align(LEFT_TO_RIGHT);//����
					score += Check_remove(LEFT_TO_RIGHT);//����������Ʒ�
					Move_animation(LEFT_TO_RIGHT);//��ʾ�ƶ��ϲ�����
					Clear_status();//����״̬
					Produce_new_block(1);//���ɲ���ʾ���򣬼ǵ�����Ҫ����״̬
					break;
				}
				break;
			}//end of switch
		}//end of else
	}//end of while

	return 0;
}
/***************************************************************************
  �������ƣ�
  ��    �ܣ�����
  ���������const int direction ���뷽��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int G2048::Align(const int direction)
{
	int i;
	switch (direction) {

	case DOWN_TO_UP:
		for (i = 0; i < col; i++) {
			up_align(i);
		}
		break;
	case UP_TO_DOWN:
		for (i = 0; i < col; i++) {
			down_align(i);
		}
		break;
	case RIGHT_TO_LEFT:
		for (i = 0; i < row; i++) {
			left_align(i);
		}
		break;
	case LEFT_TO_RIGHT:
		for (i = 0; i < row; i++) {
			right_align(i);
		}
		break;
	}

	return 0;
}

int G2048::Check_remove(const int direction)
{
	int i;
	switch (direction) {

	case DOWN_TO_UP:
		for (i = 0; i < col; i++) {
			up_check(i);
		}
		break;
	case UP_TO_DOWN:
		for (i = 0; i < col; i++) {
			down_check(i);
		}
		break;
	case RIGHT_TO_LEFT:
		for (i = 0; i < row; i++) {
			left_check(i);
		}
		break;
	case LEFT_TO_RIGHT:
		for (i = 0; i < row; i++) {
			right_check(i);
		}
		break;
	}
	return 0;
}

int G2048::Move_animation(const int direction)
{
	int i, j;
	int dr, dc;
	int value;

	switch (direction) {
		//��ʵֻ���ƶ�����ͷ����й�abs(map[i][j].drow - i)��abs(map[i][j].dcol - i))֮��
	case DOWN_TO_UP:
		for (j = 0; j < col; j++) {
			for (i = 0; i < row; i++) {
				if (!map[i][j].status)
					break;
				dc = map[i][j].dcol;
				dr = map[i][j].drow;
				value = map[dr][dc].value;
				switch (map[i][j].status) {

				case NORMAL:
					g2048_move_block(&G2048_CGI, i, j, value, 0, bdi_normal, direction, abs(dr - i));
					break;
				case WAIT_COMBINE:
					g2048_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dr - i));
					break;
				case COMBINE:
					g2048_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dr - i));
					gmw_draw_block(&G2048_CGI, dr, map[i][j].dcol, value, bdi_normal);
					break;
				}
			}
		}
		break;
	case UP_TO_DOWN:
		for (j = col - 1; j >= 0; j--) {
			for (i = row - 1; i >= 0; i--) {
	
				if (!map[i][j].status)
					break;
				dc = map[i][j].dcol;
				dr = map[i][j].drow;
				value = map[dr][dc].value;
				switch (map[i][j].status) {

				case NORMAL:
					g2048_move_block(&G2048_CGI, i, j, value, 0, bdi_normal, direction, abs(dr - i));
					break;
				case WAIT_COMBINE:
					g2048_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dr - i));
					break;
				case COMBINE:
					g2048_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dr - i));
					gmw_draw_block(&G2048_CGI, dr, map[i][j].dcol, value, bdi_normal);
					break;
				}
			}
		}
		break;
	case RIGHT_TO_LEFT:
		for (i = 0; i < row; i++) {
			for (j = 0; j < col; j++) {
				if (!map[i][j].status)
					break;
				dc = map[i][j].dcol;
				dr = map[i][j].drow;
				value = map[dr][dc].value;
				switch (map[i][j].status) {

				case NORMAL:
					g2048_move_block(&G2048_CGI, i, j, value, 0, bdi_normal, direction, abs(dc - i));
					break;
				case WAIT_COMBINE:
					g2048_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dc - i));
					break;
				case COMBINE:
					g2048_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dc - i));
					gmw_draw_block(&G2048_CGI, map[i][j].drow, dc, value, bdi_normal);
					break;
				}
			}
		}
		break;
	case LEFT_TO_RIGHT:
		for (i = row-1; i >=0; i--) {
			for (j = col-1; j >=0; j--) {
				if (!map[i][j].status)
					break;
				dc = map[i][j].dcol;
				dr = map[i][j].drow;
				value = map[dr][dc].value;
				switch (map[i][j].status) {

				case NORMAL:
					g2048_move_block(&G2048_CGI, i, j, value, 0, bdi_normal, direction, abs(dc - i));
					break;
				case WAIT_COMBINE:
					g2048_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dc - i));
					break;
				case COMBINE:
					g2048_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dc - i));
					gmw_draw_block(&G2048_CGI, map[i][j].drow, dc, value, bdi_normal);
					break;
				}
			}
		}
		break;
	}
	/* ��ʾ��� */
	//gmw_draw_frame(&G2048_CGI);

	/*��ʾ��*/
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++) {
			gmw_draw_block(&G2048_CGI, i, j, map[i][j].value, bdi_normal);
		}
	return 0;
}

int G2048::Clear_status()
{
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			//Ŀ��λ������
			map[i][j].dcol = 0;
			map[i][j].drow = 0;
			if(map[i][j].value == 0)
				map[i][j].status = ZERO;
			else
				map[i][j].status = NORMAL;
		}
	}

	return 0;
}

int G2048::Gameover()
{
	if (score >= dst_score)
		return 'W';//��Ϸʤ��
	
	/*�ж�����*/
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j].status == 0)
				break;
		}
	}
	if (i == row && j == col)
		return 'F';//Full or Fail

	return 0;
}

int G2048::print_map()
{
	cls();
	cout << "�ڲ�����:" << endl;
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++)
			cout << setw(4) << map[i][j].status;
		cout << endl;
	}
	return 0;
}

int G2048::gmw_inner_position_trans(const CONSOLE_GRAPHICS_INFO * const pCGI, POS * const pos)
{
	const int X = pCGI->start_x + 2;//ȥ�����
	const int Y = pCGI->start_y + 1;
	pos->X = X + pos->col * pCGI->CFI.bwidth;
	pos->Y = Y + pos->row * pCGI->CFI.bhigh;
	return 0;
}

int G2048::gmw_inner_draw_block(const int X, const int Y, const CONSOLE_GRAPHICS_INFO * const pCGI, const int bdi_value, const BLOCK_DISPLAY_INFO * const bdi)
{
	Sleep(pCGI->delay_of_draw_block);
	int bg, fg;

	//Ѱ��Ҫ���������͵�ɫ�飺
	int i;
	for (i = 0; bdi[i].value != bdi_value; i++) {
		if (bdi[i].value == BDI_VALUE_END)
			return -1;//����-1˵��û�д�ӡ
	}

	//����-1Ĭ��ֵ
	if (bdi[i].bgcolor == -1)
		bg = pCGI->CFI.bgcolor;
	else
		bg = bdi[i].bgcolor;

	if (bdi[i].fgcolor == -1)
		fg = pCGI->CFI.fgcolor;
	else
		fg = bdi[i].fgcolor;


	//��ʼ��ɫ�飺
	if (bdi[i].content == NULL && bdi[i].value != BDI_VALUE_BLANK) {
		//��ӡɫ��
		int j;
		for (j = 0; j < pCGI->CFI.block_high; j++)
			showstr(X, Y + j, " ", bg, fg, pCGI->CFI.block_width, pCGI->CFI.block_width);

		//��ӡɫ��С�߿�
		if (pCGI->CBI.block_border && pCGI->CFI.block_high > 1 && pCGI->CFI.block_width >= 4) {
			//the first line
			showstr(X, Y, pCGI->CBI.top_left, bg, fg);
			showstr(X + 2, Y, pCGI->CBI.h_normal, bg, fg, pCGI->CFI.block_width / 2 - 2);
			showstr(X + pCGI->CFI.block_width - 2, Y, pCGI->CBI.top_right, bg, fg);

			//body
			int k;
			for (k = 1; k <= pCGI->CFI.block_high - 2; k++) {
				showstr(X, Y + k, pCGI->CBI.v_normal, bg, fg);
				showstr(X + pCGI->CFI.block_width - 2, Y + k, pCGI->CBI.v_normal, bg, fg);
			}

			//the last line
			showstr(X, Y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_left, bg, fg);
			showstr(X + 2, Y + pCGI->CFI.block_high - 1, pCGI->CBI.h_normal, bg, fg, pCGI->CFI.block_width / 2 - 2);
			showstr(X + pCGI->CFI.block_width - 2, Y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_right, bg, fg);
		}

		//��valueת��Ϊchar*��
		char temp[CHAR_LEN];
		sprintf(temp, "%d", bdi[i].value);

		//��ʾvalue�ַ�
		if (pCGI->CFI.block_high % 2)
			showstr(X + pCGI->CFI.block_width / 2 - strlen(temp) / 2 - 1, Y + pCGI->CFI.block_high / 2, temp, bg, fg);
		else
			showstr(X + pCGI->CFI.block_width / 2 - strlen(temp) / 2 - 1, Y + pCGI->CFI.block_high / 2 + 1, temp, bg, fg);
		return 0;
	}

	int j;
	for (j = 0; j < pCGI->CFI.block_high; j++)
		showstr(X, Y + j, " ", bg, fg, pCGI->CFI.block_width, pCGI->CFI.block_width);

	//��ӡɫ��С�߿�
	if (pCGI->CBI.block_border && bdi_value != BDI_VALUE_BLANK && pCGI->CFI.block_high > 1 && pCGI->CFI.block_width >= 4) {
		//the first line
		showstr(X, Y, pCGI->CBI.top_left, bg, fg);
		showstr(X + 2, Y, pCGI->CBI.h_normal, bg, fg, pCGI->CFI.block_width / 2 - 2);
		showstr(X + pCGI->CFI.block_width - 2, Y, pCGI->CBI.top_right, bg, fg);

		//body
		int k;
		for (k = 1; k <= pCGI->CFI.block_high - 2; k++) {
			showstr(X, Y + k, pCGI->CBI.v_normal, bg, fg);
			showstr(X + pCGI->CFI.block_width - 2, Y + k, pCGI->CBI.v_normal, bg, fg);
		}

		//the last line
		showstr(X, Y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_left, bg, fg);
		showstr(X + 2, Y + pCGI->CFI.block_high - 1, pCGI->CBI.h_normal, bg, fg, pCGI->CFI.block_width / 2 - 2);
		showstr(X + pCGI->CFI.block_width - 2, Y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_right, bg, fg);
	}

	//��ʾcontent�ַ�
	if (bdi[i].value != BDI_VALUE_BLANK) {
		if (pCGI->CFI.block_high % 2)
			showstr(X + pCGI->CFI.block_width / 2 - strlen(bdi[i].content) / 2, Y + pCGI->CFI.block_high / 2, bdi[i].content, bg, fg);
		else
			showstr(X + pCGI->CFI.block_width / 2 - strlen(bdi[i].content) / 2, Y + pCGI->CFI.block_high / 2 + 1, bdi[i].content, bg, fg);
	}
	return 0;
}

int G2048::gmw_draw_block(const CONSOLE_GRAPHICS_INFO * const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO * const bdi)
{
	Sleep(pCGI->delay_of_draw_block);
	int bg, fg;

	//���ҵ�ת������
	POS pos = { row_no, col_no, 0, 0 };
	gmw_inner_position_trans(pCGI, &pos);

	//Ѱ��Ҫ���������͵�ɫ�飺
	int i;
	for (i = 0; bdi[i].value != bdi_value; i++) {
		if (bdi[i].value == BDI_VALUE_END)
			return -1;//����-1˵��û�д�ӡ
	}

	//����-1Ĭ��ֵ
	if (bdi[i].bgcolor == -1)
		bg = pCGI->CFI.bgcolor;
	else
		bg = bdi[i].bgcolor;

	if (bdi[i].fgcolor == -1)
		fg = pCGI->CFI.fgcolor;
	else
		fg = bdi[i].fgcolor;


	//��ʼ��ɫ�飺
	if (bdi[i].content == NULL && bdi[i].value != BDI_VALUE_BLANK) {//�տ鲻����
		//��ӡɫ��
		int j;
		for (j = 0; j < pCGI->CFI.block_high; j++)
			showstr(pos.X, pos.Y + j, " ", bg, fg, pCGI->CFI.block_width, pCGI->CFI.block_width);

		//��ӡɫ��С�߿�
		if (pCGI->CBI.block_border && pCGI->CFI.block_high > 1 && pCGI->CFI.block_width >= 4) {
			//the first line
			showstr(pos.X, pos.Y, pCGI->CBI.top_left, bg, fg);
			showstr(pos.X + 2, pos.Y, pCGI->CBI.h_normal, bg, fg, pCGI->CFI.block_width / 2 - 2);
			showstr(pos.X + pCGI->CFI.block_width - 2, pos.Y, pCGI->CBI.top_right, bg, fg);

			//body
			int k;
			for (k = 1; k <= pCGI->CFI.block_high - 2; k++) {
				showstr(pos.X, pos.Y + k, pCGI->CBI.v_normal, bg, fg);
				showstr(pos.X + pCGI->CFI.block_width - 2, pos.Y + k, pCGI->CBI.v_normal, bg, fg);
			}

			//the last line
			showstr(pos.X, pos.Y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_left, bg, fg);
			showstr(pos.X + 2, pos.Y + pCGI->CFI.block_high - 1, pCGI->CBI.h_normal, bg, fg, pCGI->CFI.block_width / 2 - 2);
			showstr(pos.X + pCGI->CFI.block_width - 2, pos.Y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_right, bg, fg);
		}

		//��valueת��Ϊchar*��
		char temp[CHAR_LEN];
		sprintf(temp, "%d", bdi[i].value);

		//��ʾvalue�ַ�
		if (pCGI->CFI.block_high % 2)
			showstr(pos.X + pCGI->CFI.block_width / 2 - strlen(temp) / 2 - 1, pos.Y + pCGI->CFI.block_high / 2, temp, bg, fg);
		else
			showstr(pos.X + pCGI->CFI.block_width / 2 - strlen(temp) / 2 - 1, pos.Y + pCGI->CFI.block_high / 2 + 1, temp, bg, fg);
		return 0;
	}

	int j;
	for (j = 0; j < pCGI->CFI.block_high; j++)
		showstr(pos.X, pos.Y + j, " ", bg, fg, pCGI->CFI.block_width, pCGI->CFI.block_width);

	//��ӡɫ��С�߿�
	//��߲���2�����ӡ��ɫ���ʱ��ֱ������
	if (pCGI->CBI.block_border && bdi_value != BDI_VALUE_BLANK && pCGI->CFI.block_high > 1 && pCGI->CFI.block_width >= 4) {
		//the first line
		showstr(pos.X, pos.Y, pCGI->CBI.top_left, bg, fg);
		showstr(pos.X + 2, pos.Y, pCGI->CBI.h_normal, bg, fg, pCGI->CFI.block_width / 2 - 2);
		showstr(pos.X + pCGI->CFI.block_width - 2, pos.Y, pCGI->CBI.top_right, bg, fg);

		//body
		int k;
		for (k = 1; k <= pCGI->CFI.block_high - 2; k++) {
			showstr(pos.X, pos.Y + k, pCGI->CBI.v_normal, bg, fg);
			showstr(pos.X + pCGI->CFI.block_width - 2, pos.Y + k, pCGI->CBI.v_normal, bg, fg);
		}

		//the last line
		showstr(pos.X, pos.Y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_left, bg, fg);
		showstr(pos.X + 2, pos.Y + pCGI->CFI.block_high - 1, pCGI->CBI.h_normal, bg, fg, pCGI->CFI.block_width / 2 - 2);
		showstr(pos.X + pCGI->CFI.block_width - 2, pos.Y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_right, bg, fg);
	}

	//��ʾcontent�ַ�
	if (bdi[i].value != BDI_VALUE_BLANK) {
		if (pCGI->CFI.block_high % 2)
			showstr(pos.X + pCGI->CFI.block_width / 2 - strlen(bdi[i].content) / 2, pos.Y + pCGI->CFI.block_high / 2, bdi[i].content, bg, fg);
		else
			showstr(pos.X + pCGI->CFI.block_width / 2 - strlen(bdi[i].content) / 2, pos.Y + pCGI->CFI.block_high / 2 + 1, bdi[i].content, bg, fg);

	}

	return 0; //�˾�ɸ�����Ҫ�޸�

}

int G2048::g2048_move_block(const CONSOLE_GRAPHICS_INFO * const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO * const bdi, const int direction, const int distance)
{
	//���ҵ�ת������
	POS pos = { row_no, col_no, 0, 0 };
	gmw_inner_position_trans(pCGI, &pos);

	int X = pos.X;
	int Y = pos.Y;

	int i, j, k;
	for (k = 0; k < distance; k++) {
		switch (direction)
		{
		case DOWN_TO_UP://����
			for (i = 0; i < pCGI->CFI.bhigh; i++) {
				Sleep(pCGI->delay_of_block_moved);
				gmw_inner_draw_block(X, Y - i, pCGI, blank_bdi_value, bdi);
				gmw_inner_draw_block(X, Y - i - 1, pCGI, bdi_value, bdi);
			}
			if (pCGI->CFI.separator) {
				showstr(X, Y - 1, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor, pCGI->CFI.block_width / 2);
			}
			//����λ������
			pos.row--;
			gmw_inner_position_trans(pCGI, &pos);
			X = pos.X;
			Y = pos.Y;
			break;
		case UP_TO_DOWN://����
			for (i = 0; i < pCGI->CFI.bhigh; i++) {
				Sleep(pCGI->delay_of_block_moved);
				gmw_inner_draw_block(X, Y + i, pCGI, blank_bdi_value, bdi);
				gmw_inner_draw_block(X, Y + i + 1, pCGI, bdi_value, bdi);
			}
			if (pCGI->CFI.separator) {
				showstr(X, Y + pCGI->CFI.block_high, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor, pCGI->CFI.block_width / 2);
			}
			//����λ������
			pos.row++;
			gmw_inner_position_trans(pCGI, &pos);
			X = pos.X;
			Y = pos.Y;
			break;
		case RIGHT_TO_LEFT://����
			for (i = 0; i < pCGI->CFI.bwidth; i += 2) {
				Sleep(pCGI->delay_of_block_moved);
				gmw_inner_draw_block(X - i, Y, pCGI, blank_bdi_value, bdi);
				gmw_inner_draw_block(X - i - 2, Y, pCGI, bdi_value, bdi);
			}
			if (pCGI->CFI.separator) {
				for (j = 0; j < pCGI->CFI.block_high; j++) {
					showstr(X - 2, Y + j, pCGI->CFI.v_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				}
			}
			//����λ������
			pos.col--;
			gmw_inner_position_trans(pCGI, &pos);
			X = pos.X;
			Y = pos.Y;
			break;
		case LEFT_TO_RIGHT://����
			for (i = 0; i < pCGI->CFI.bwidth; i += 2) {
				Sleep(pCGI->delay_of_block_moved);
				gmw_inner_draw_block(X + i, Y, pCGI, blank_bdi_value, bdi);
				gmw_inner_draw_block(X + i + 2, Y, pCGI, bdi_value, bdi);
			}
			if (pCGI->CFI.separator) {
				for (j = 0; j < pCGI->CFI.block_high; j++) {
					showstr(X + pCGI->CFI.block_width, Y + j, pCGI->CFI.v_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				}
			}
			//����λ������
			pos.col++;
			gmw_inner_position_trans(pCGI, &pos);
			X = pos.X;
			Y = pos.Y;
			break;
		}
	}

	return 0;
}
