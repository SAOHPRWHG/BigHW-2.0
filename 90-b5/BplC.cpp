#include "BplC.h"

static int ax(int X, const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	const int X0 = pCGI->start_x + 2;
	int flag, col;
	flag = (X - X0) % pCGI->CFI.bwidth;

	//��������block_width��bwidth֮��˵���ڱ߿���
	if (flag < pCGI->CFI.block_width) {
		col = (X - X0) / pCGI->CFI.bwidth;
		if (col >= 0 && col < pCGI->col_num)
			return col;
		else
			return -1;
	}
	else
		return -1;//-1��ʾͣ������Ƿ�
}

static int ay(int Y, const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	const int Y0 = pCGI->start_y + 1;
	int flag, row;
	flag = (Y - Y0) % pCGI->CFI.bhigh;

	//��������block_width��bwidth֮��˵���ڱ߿���
	if (flag < pCGI->CFI.block_high) {
		row = (Y - Y0) / pCGI->CFI.bhigh;
		if (row >= 0 && row < pCGI->row_num)
			return row;
		else
			return -1;
	}
	else
		return -1;//-1��ʾͣ������Ƿ�
}

int BplC::Init_map()
{
	int i, j;
	for (i = 0; i < ROW; i++) 
		for (j = 0; j < COL; j++) 
			map[i][j] = 0;
	return 0;
}

int BplC::Init_frame()
{
	/*��ʼ���߿�*/
		/*��ȱʡֵ��ʼ�������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ�ԣ�*/
	gmw_init(&BplC_CGI);

	//	gmw_set_ext_rowcol(&ColorLinez_CGI, 3, 2, 10, 10);						//ע�������ø������򣬿��ܵ���to_be_continued��ӡΪ���У�������δ��������ͬ��
	gmw_set_color(&BplC_CGI, COLOR_BLACK, COLOR_HWHITE);			//����������ɫ
	gmw_set_font(&BplC_CGI, "������", 32);							//����
//	gmw_set_frame_style(&BplC_CGI);									//��Ϸ��������ÿ��ɫ���2��1���зָ���
	gmw_set_frame_default_linetype(&BplC_CGI, 2);					//��Ϸ���������ͣ�����
	gmw_set_frame_color(&BplC_CGI, COLOR_HWHITE, COLOR_BLACK);		//��Ϸ��������ɫ
//	gmw_set_block_border_switch(&BplC_CGI);							//ÿ����Ϸɫ��/������Ҫ�߿�
//	gmw_set_status_line_switch(&BplC_CGI, TOP_STATUS_LINE);				//��Ҫ��״̬��
//	gmw_set_status_line_switch(&BplC_CGI, LOWER_STATUS_LINE);			//��Ҫ��״̬��
	gmw_set_rowno_switch(&BplC_CGI, true);							//��ʾ�к�
	gmw_set_colno_switch(&BplC_CGI, true);							//��ʾ�б�
	gmw_set_delay(&BplC_CGI, DELAY_OF_BLOCK_MOVED, BLOCK_MOVED_DELAY_MS * 3);//�Ӵ���ʱ
	/* ��row/col��ֵ������Ϸ���������� */
	gmw_set_rowcol(&BplC_CGI, ROW, COL);

	cls();
	/* ��ʾ��� */
	gmw_draw_frame(&BplC_CGI);
	return 0;
}

/***************************************************************************
  �������ƣ������ѡ��������
  ��    �ܣ�
  ���������
  �� �� ֵ��1��2��1��ʾģʽ1,2��ʾģʽ2(��������)
  ˵    ����
***************************************************************************/
int BplC::Select_space_by_mouse(char & row, int & col, char & head_row, int & head_col, char & tail_row, int & tail_col)
{
	char temp[CHAR_LEN];
	int loop = 1;
	int maction, mrow, mcol;
	int keycode1, keycode2;
	int ret;

	while (loop) {
		ret = gmw_read_keyboard_and_mouse(&BplC_CGI, maction, mrow, mcol, keycode1, keycode2);
		
		if (ret == CCT_MOUSE_EVENT) {
			switch (maction) {
			case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:
				sprintf(temp, "[�����ը] %c��%d��", char('A' + mrow), mcol); //δ����mrow����26��mcol����99�������������Ҫ�������д���
				gmw_status_line(&BplC_CGI, TOP_STATUS_LINE, temp);
				Sleep(500);
				row = char('A' + mrow);
				col = mcol;
				return 1;
				break;
			case MOUSE_RIGHT_BUTTON_CLICK:
				head_row = char('A' + mrow);
				head_col = mcol;
				maction = 0;
				//while ((ret = gmw_read_keyboard_and_mouse(&BplC_CGI, maction, mrow, mcol, keycode1, keycode2)) != CCT_MOUSE_EVENT || maction != MOUSE_RIGHT_BUTTON_CLICK)
					//;
				if ((ret = gmw_read_keyboard_and_mouse(&BplC_CGI, maction, mrow, mcol, keycode1, keycode2)) == CCT_MOUSE_EVENT && maction == MOUSE_RIGHT_BUTTON_CLICK)
				{
					tail_row = char('A' + mrow);
					tail_col = mcol;
					sprintf(temp, "[������ը] ͷ:%c��%d�� - β:%c��%d��", head_row, head_col, tail_row, tail_col); //δ����mrow����26��mcol����99�������������Ҫ�������д���
					gmw_status_line(&BplC_CGI, TOP_STATUS_LINE, temp);
					Sleep(500);
					return 2;
				}
				else
					continue;
				break;
			}

		}//end of while
	}
	return 0;
}

int BplC::Explode_animation(const char row, const int col, const char head_row, const int head_col, const char tail_row, const int tail_col, const int sel, string spack)
{
	int r = row - 'A';//���л���int��
	int hr = head_row - 'A';
	int tr = tail_row - 'A';
	/*�����ը*/  
	int k;
	if (sel == 1) {
		for (k = 0; k < 5; k++) {
			gmw_draw_block(&BplC_CGI, r, col, 1, bdi_exploded);
			Sleep(50);
			gmw_draw_block(&BplC_CGI, r, col, 0, bdi_normal); //0�ǿհ�
			Sleep(50);
		}
		if (spack == "HitFail") {
			map[r][col] = HIT_FAIL;
			gmw_draw_block(&BplC_CGI, r, col, map[r][col], bdi_normal);
		}
		else if (spack == "HitBody") {
			map[r][col] = HIT_BODY;
			gmw_draw_block(&BplC_CGI, r, col, map[r][col], bdi_normal);
		}
		else if (spack == "HitHead") {
			map[r][col] = HIT_HEAD;
			gmw_draw_block(&BplC_CGI, r, col, map[r][col], bdi_normal);
		}
	}
	/*������ը*/
	else if (sel == 2) {
		if (spack == "HitPlane"|| spack == "GameOver") {
			//map[hr][head_col] = HIT_HEAD;
			//gmw_draw_block(&BplC_CGI, r, col, map[hr][head_col], bdi_normal);
			Plane pl(hr, head_col, tr, tail_col);
			int i;
			for (i = 0; i < PLANE_SPACE; i++) {
				if (i == 0)
					map[pl.plane[i].row][pl.plane[i].col] = HIT_HEAD;
				else
					map[pl.plane[i].row][pl.plane[i].col] = HIT_BODY;

				gmw_draw_block(&BplC_CGI, pl.plane[i].row, pl.plane[i].col, map[pl.plane[i].row][pl.plane[i].col], bdi_normal);
			}
		}
	}

	return 0;
}

int BplC::print_map()
{
	cls();
	cout << "�ڲ�����:" << endl;
	int i, j;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++)
			cout << setw(4) << map[i][j];
		cout << endl;
	}
	return 0;
}
