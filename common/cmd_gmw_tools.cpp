/* 1751957 ��2�� ������ */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <climits>
#include <stdlib.h>
#include <conio.h>
#include "cmd_console_tools.h"
#include "cmd_gmw_tools.h"

using namespace std;

/* --------------------------------------------------
		�˴����Ը�����Ҫ�ľ�̬ȫ�ֱ������������٣����û�У�����̬ȫ��ֻ������/�궨�壨�������ޣ���
   -------------------------------------------------- */
const int CHAR_LEN = 256;

struct POS {
	int row;
	int col;
	int X;
	int Y;
};

/* --------------------------------------------------
		�˴����Ը�����Ҫ���ڲ��������ߺ���
		1�����������ޣ�����Ϊ gmw_inner_* 
		2����������
		3��������static������ȷ��ֻ�ڱ�Դ�ļ���ʹ��
   -------------------------------------------------- */

static int gmw_inner_set_start_xy(CONSOLE_GRAPHICS_INFO *const pCGI);
static int gmw_inner_set_block_width_high_ext(CONSOLE_GRAPHICS_INFO *const pCGI);
static int gmw_inner_set_bwidth_bhigh(CONSOLE_GRAPHICS_INFO *const pCGI);
static int gmw_inner_set_main_width_high(CONSOLE_GRAPHICS_INFO *const pCGI);
static int gmw_inner_set_linescols(CONSOLE_GRAPHICS_INFO *const pCGI);
static int ax(int X, const CONSOLE_GRAPHICS_INFO *const pCGI);
static int ay(int Y, const CONSOLE_GRAPHICS_INFO *const pCGI);

/*�ڲ����ز������ú���Ⱥ*/
static int gmw_inner_set_start_xy(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	//���϶���ռ� + ��״̬�� + �б���ʾ
	pCGI->start_y = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no;
	//�������ռ� + �б���ʾ 
	pCGI->start_x = pCGI->extern_left_cols + pCGI->draw_frame_with_row_no;

	//��������״̬��λ��
	if (pCGI->draw_frame_with_row_no) 
		pCGI->SLI.lower_start_x = pCGI->SLI.top_start_x = pCGI->start_x - 1;
	else 
		pCGI->SLI.lower_start_x = pCGI->SLI.top_start_x = pCGI->start_x;

	if (pCGI->draw_frame_with_col_no)
		pCGI->SLI.top_start_y = pCGI->start_y - 2;
	else
		pCGI->SLI.top_start_y = pCGI->start_y - 1;

	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->main_high;

	return 0;
}
/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ɫ�������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ���� ÿ��ɫ��ĸ��ӵĿ�Ⱥ͸߶�(�зָ����Ƿֱ�Ϊ2/1)����separator������� 
			block_width_ext(separator)
			block_high_ext(separator)
***************************************************************************/
static int gmw_inner_set_block_width_high_ext(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	if (pCGI->CFI.separator) {
		pCGI->CFI.block_width_ext = 2;
		pCGI->CFI.block_high_ext = 1;
	}
	else {
		pCGI->CFI.block_width_ext = 0;
		pCGI->CFI.block_high_ext = 0;
	}

	gmw_inner_set_bwidth_bhigh(pCGI);//����ext�ض�����bwidth��bhigh��С
	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ɫ����
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ����1���޸������κ�һ����������Ҫ���µ��ô˺�����������
			������
			block_width
			block_high
			block_width_ext(separator)
			block_high_ext(separator)
***************************************************************************/
static int gmw_inner_set_bwidth_bhigh(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	pCGI->CFI.bwidth = pCGI->CFI.block_width + pCGI->CFI.block_width_ext;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + pCGI->CFI.block_high_ext;

	gmw_inner_set_main_width_high(pCGI);//����bwidth��bhigh�ض������������С
	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ����1���޸������κ�һ����������Ҫ���µ��ô˺�����������
			������
			col_num
			row_num
			bwidth
			bhigh
***************************************************************************/
static int gmw_inner_set_main_width_high(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	if (pCGI->CFI.separator) {
		//��set_rowcol�����ã�main_width = col*bwidth + 2(��߿�)	+ ��һ��
		pCGI->main_width = pCGI->col_num * pCGI->CFI.bwidth + 2 + 1;
		//main_width = row* bhigh+ 2 (�ϱ߿�)
		pCGI->main_high = pCGI->row_num * pCGI->CFI.bhigh + 1;
	}
	else {
		//��set_rowcol�����ã�main_width = col*bwidth + 2*2(���ұ߿�) + ��һ��	
		pCGI->main_width = pCGI->col_num * pCGI->CFI.bwidth + 2 * 2 + 1;
		//main_width = row* bhigh+ 2 (���±߿�)
		pCGI->main_high = pCGI->row_num * pCGI->CFI.bhigh + 2;
	}

	gmw_inner_set_linescols(pCGI);//����������ض����Ĵ��ڴ�С
	gmw_inner_set_start_xy(pCGI);//����������ض���������״̬��λ��
	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������̨���ڵ�������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ����1���޸������κ�һ����������Ҫ���µ��ô˺�����������
			������
			extern
			up_status_line
			lower_status_line
			draw_frame_with_row_no;
			draw_frame_with_col_no;
			main_widthhigh
***************************************************************************/
static int gmw_inner_set_linescols(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	//��\�϶���ռ� + \��״̬�� + \�б���ʾ + \������ + ��״̬�� + �¶���ռ䣩 + 4��1�������뷨��ʾ�� + 3Ԥ�����У�
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->top_status_line + pCGI->lower_status_line + pCGI->draw_frame_with_col_no + pCGI->main_high + 4;
	//��\�����ռ� + \�б���ʾ + \������ + �Ҷ���ռ䣩
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_row_no + pCGI->main_width + 1;
	return 0;
}

/*������ʾ����Ⱥ*/
static int gmw_inner_check_line(char buf[])
{
	int i, count = 0;
	for (i = 0; i < int(strlen(buf)); i++) {
		if (buf[i] < 0)
			count++;
	}
	return !(count % 2);//�����������棬��ͨ�����
}

static int gmw_inner_draw_row_col_signal(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	if (pCGI->draw_frame_with_row_no) {
		int rsign_x = pCGI->start_x - 1;//�б�x����
		int i;
		for (i = 0; i < pCGI->row_num; i++) {
			if (i < 26) {
				showch(rsign_x, pCGI->start_y + (1 + pCGI->CFI.bhigh) / 2 + pCGI->CFI.bhigh*i, 'A' + i, pCGI->area_bgcolor, pCGI->area_fgcolor);
			}
			else if (i < 52) {
				showch(rsign_x, pCGI->start_y + (1 + pCGI->CFI.bhigh) / 2 + pCGI->CFI.bhigh*i, 'a' - 26  + i, pCGI->area_bgcolor, pCGI->area_fgcolor);
			}
			//else���Ǵ���52���ʲô��
			else
				showch(rsign_x, pCGI->start_y + (1 + pCGI->CFI.bhigh) / 2 + pCGI->CFI.bhigh*i, '*', pCGI->area_bgcolor, pCGI->area_fgcolor);
		}
	}
	if (pCGI->draw_frame_with_col_no) {
		int csign_y = pCGI->start_y - 1;//�б�y����
		int i;
		char num[CHAR_LEN];
		for (i = 0; i < pCGI->col_num; i++) {
			if(i<100)
				showstr(pCGI->start_x + 2 + (pCGI->CFI.bwidth - 2) / 2 + pCGI->CFI.bwidth * i, csign_y, _itoa(i,num,10), pCGI->area_bgcolor, pCGI->area_fgcolor);
			else
				showstr(pCGI->start_x + 2 + (pCGI->CFI.bwidth - 2) / 2 + pCGI->CFI.bwidth * i, csign_y, "*", pCGI->area_bgcolor, pCGI->area_fgcolor);
		}
		
	}
	return 0;
}
/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ɫ����ʾ����Ϳ���̨�����ת��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
          row: ��ʾɫ����������е�������
		  col: ��ʾɫ����������е�������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
static int gmw_inner_position_trans(const CONSOLE_GRAPHICS_INFO *const pCGI, POS *const pos)
{
	const int X = pCGI->start_x + 2;//ȥ�����
	const int Y = pCGI->start_y + 1;
	pos->X = X + pos->col * pCGI->CFI.bwidth;
	pos->Y = Y + pos->row * pCGI->CFI.bhigh;
	return 0;
}


/* ----------------------------------------------- 
		ʵ����������ĺ���������������׼����
   ----------------------------------------------- */
/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ����ܵ�������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int row						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
			const int col						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
  �� �� ֵ��
  ˵    ����1��ָ��������Ϸ�ľ������������ֵ
            2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	pCGI->col_num = col;
	pCGI->row_num = row;
	gmw_inner_set_main_width_high(pCGI);

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������������ڣ�����Ϸ�������������ڵ�����cmd���ڣ�����ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int bg_color					��ǰ��ɫ��ȱʡCOLOR_BLACK��
		   const int fg_color					������ɫ��ȱʡCOLOR_WHITE��
		   const bool cascade					���Ƿ�����ȱʡΪtrue-������
  �� �� ֵ��
  ˵    ����1��cascade = trueʱ
				ͬ���޸���Ϸ���������ɫ
				ͬ���޸�����״̬���������ı��ı���ɫ��ǰ��ɫ����Ŀ�ı��ı���ɫ��ǰ��ɫ���䣩
			2���������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15
				    ǰ��ɫ����ɫ��ֵһ�µ����޷���������
					ǰ��ɫ������״̬����Ŀǰ��ɫ�������޷�������Ŀ��ʾ
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	if (cascade) {
		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;
		pCGI->SLI.top_normal_bgcolor = bgcolor;
		pCGI->SLI.top_normal_fgcolor = fgcolor;
		pCGI->SLI.top_catchy_bgcolor = bgcolor;
		pCGI->SLI.lower_normal_bgcolor = bgcolor;
		pCGI->SLI.lower_normal_fgcolor = fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;
	}
	
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ô��ڵ�����
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *fontname					���������ƣ�ֻ����"Terminal"��"������"���֣������򷵻�-1�����ı����壩
		   const int fs_high					������߶ȣ�ȱʡ������Ϊ16�������������ƣ���Ϊ��֤��
		   const int fs_width					������߶ȣ�ȱʡ������Ϊ8�������������ƣ���Ϊ��֤��
  �� �� ֵ��
  ˵    ����1����cmd_console_tools�е�setfontsize���ƣ�Ŀǰֻ֧�֡��������塱�͡������塱
            2������������������ֱ�ӷ��أ�����ԭ�������ò���
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	pCGI->CFT.font_size_high = fs_high;
	pCGI->CFT.font_size_width = fs_width;
	strcpy(pCGI->CFT.font_type, fontname);

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������ʱ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type						����ʱ�����ͣ�ĿǰΪ3�֣�
		   const int delay_ms					����msΪ��λ����ʱ
			   ���߿����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ��ɫ�����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ɫ���ƶ�����ʱ��BLOCK_MOVED_DELAY_MS ~ �������ޣ���Ϊ��֤��ȷ�� <BLOCK_MOVED_DELAY_MS ���� BLOCK_MOVED_DELAY_MS��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	switch (type)
	{
		case DELAY_OF_BLOCK_MOVED:
			if(delay_ms<0)
				pCGI->delay_of_block_moved = 0;
			else
				pCGI->delay_of_block_moved = delay_ms;
			break;
		case DELAY_OF_DRAW_FRAME:
			if (delay_ms < 0)
				pCGI->delay_of_draw_frame = 0;
			else
				pCGI->delay_of_draw_frame = delay_ms;
			break;
		case DELAY_OF_DRAW_BLOCK:
			if (delay_ms < 0)
				pCGI->delay_of_draw_block = BLOCK_MOVED_DELAY_MS;
			else
				pCGI->delay_of_draw_block = delay_ms;
			break;
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ���������������Ϸ����ܽṹ֮����Ҫ�����Ķ�������
  ��    �ܣ�CONSOLE_GRAPHICS_INFO *const pCGI	��
		   const int up_lines					���ϲ�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int down_lines				���²�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int left_cols					����߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int right_cols				���ұ߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
  �� �� ֵ��
  ˵    �����������еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	if(up_lines<0)
		pCGI->extern_up_lines = 0;
	else
		pCGI->extern_up_lines = up_lines;
	if (down_lines < 0)
		pCGI->extern_down_lines = 0;
	else
		pCGI->extern_down_lines = down_lines;
	if (left_cols < 0)
		pCGI->extern_left_cols = 0;
	else
		pCGI->extern_left_cols = left_cols;
	if (right_cols < 0)
		pCGI->extern_right_cols = 0;
	else
		pCGI->extern_right_cols = right_cols;
	
	gmw_inner_set_start_xy(pCGI);
	gmw_inner_set_linescols(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	switch (type)
	{
		case 1:
			strcpy(pCGI->CFI.top_left, "�X");
			strcpy(pCGI->CFI.lower_left, "�^");
			strcpy(pCGI->CFI.top_right, "�[");
			strcpy(pCGI->CFI.lower_right, "�a");
			strcpy(pCGI->CFI.h_normal, "�T");
			strcpy(pCGI->CFI.v_normal, "�U");
			strcpy(pCGI->CFI.h_top_separator, "�j");
			strcpy(pCGI->CFI.h_lower_separator, "�m");
			strcpy(pCGI->CFI.v_left_separator, "�d");
			strcpy(pCGI->CFI.v_right_separator, "�g");
			strcpy(pCGI->CFI.mid_separator, "�p");
			break;
		case 2:
			strcpy(pCGI->CFI.top_left, "��");
			strcpy(pCGI->CFI.lower_left, "��");
			strcpy(pCGI->CFI.top_right, "��");
			strcpy(pCGI->CFI.lower_right, "��");
			strcpy(pCGI->CFI.h_normal, "��");
			strcpy(pCGI->CFI.v_normal, "��");
			strcpy(pCGI->CFI.h_top_separator, "��");
			strcpy(pCGI->CFI.h_lower_separator, "��");
			strcpy(pCGI->CFI.v_left_separator, "��");
			strcpy(pCGI->CFI.v_right_separator, "��");
			strcpy(pCGI->CFI.mid_separator, "��");
			break;
		case 3:
			strcpy(pCGI->CFI.top_left, "�V");
			strcpy(pCGI->CFI.lower_left, "�\");
			strcpy(pCGI->CFI.top_right, "�Y");
			strcpy(pCGI->CFI.lower_right, "�_");
			strcpy(pCGI->CFI.h_normal, "�T");
			strcpy(pCGI->CFI.v_normal, "��");
			strcpy(pCGI->CFI.h_top_separator, "�h");
			strcpy(pCGI->CFI.h_lower_separator, "�k");
			strcpy(pCGI->CFI.v_left_separator, "�b");
			strcpy(pCGI->CFI.v_right_separator, "�e");
			strcpy(pCGI->CFI.mid_separator, "�n");
			break;
		case 4:
			//�W�Z�]�`�c�f�U�o���i�l�o
			strcpy(pCGI->CFI.top_left, "�W");
			strcpy(pCGI->CFI.lower_left, "�]");
			strcpy(pCGI->CFI.top_right, "�Z");
			strcpy(pCGI->CFI.lower_right, "�`");
			strcpy(pCGI->CFI.h_normal, "��");
			strcpy(pCGI->CFI.v_normal, "�U");
			strcpy(pCGI->CFI.h_top_separator, "�i");
			strcpy(pCGI->CFI.h_lower_separator, "�l");
			strcpy(pCGI->CFI.v_left_separator, "�c");
			strcpy(pCGI->CFI.v_right_separator, "�f");
			strcpy(pCGI->CFI.mid_separator, "�o");
			break;
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const char *...						����11�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
	const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
	const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
	//start top_left
	switch (strlen(top_left))
	{
		case 0:
			strcpy(pCGI->CFI.top_left, "  ");
			break;
		case 1:
			strcpy(pCGI->CFI.top_left, top_left);
			strcat(pCGI->CFI.top_left, " ");
			break;
		case 2:
			strcpy(pCGI->CFI.top_left, top_left);
			break;
	}
	//lower_left
	switch (strlen(lower_left))
	{
		case 0:
			strcpy(pCGI->CFI.lower_left, "  ");
			break;
		case 1:
			strcpy(pCGI->CFI.lower_left, lower_left);
			strcat(pCGI->CFI.lower_left, " ");
			break;
		case 2:
			strcpy(pCGI->CFI.lower_left, lower_left);
			break;
	}
	//top_right
	switch (strlen(top_right))
	{
		case 0:
			strcpy(pCGI->CFI.top_right, "  ");
			break;
		case 1:
			strcpy(pCGI->CFI.top_right, top_right);
			strcat(pCGI->CFI.top_right, " ");
			break;
		case 2:
			strcpy(pCGI->CFI.top_right, top_right);
			break;
	}

	//lower_right
	switch (strlen(lower_right))
	{
		case 0:
			strcpy(pCGI->CFI.lower_right, "  ");
			break;
		case 1:
			strcpy(pCGI->CFI.lower_right, lower_right);
			strcat(pCGI->CFI.lower_right, " ");
			break;
		case 2:
			strcpy(pCGI->CFI.lower_right, lower_right);
			break;
	}

	//h_normal
	switch (strlen(h_normal))
	{
		case 0:
			strcpy(pCGI->CFI.h_normal, "  ");
			break;
		case 1:
			strcpy(pCGI->CFI.h_normal, h_normal);
			strcat(pCGI->CFI.h_normal, " ");
			break;
		case 2:
			strcpy(pCGI->CFI.h_normal, h_normal);
			break;
	}

	//v_normal
	switch (strlen(v_normal))
	{
		case 0:
			strcpy(pCGI->CFI.v_normal, "  ");
			break;
		case 1:
			strcpy(pCGI->CFI.v_normal, v_normal);
			strcat(pCGI->CFI.v_normal, " ");
			break;
		case 2:
			strcpy(pCGI->CFI.v_normal, v_normal);
			break;
	}

	//h_top_separator
	switch (strlen(h_top_separator))
	{
		case 0:
			strcpy(pCGI->CFI.h_top_separator, "  ");
			break;
		case 1:
			strcpy(pCGI->CFI.h_top_separator, h_top_separator);
			strcat(pCGI->CFI.h_top_separator, " ");
			break;
		case 2:
			strcpy(pCGI->CFI.h_top_separator, h_top_separator);
			break;
	}

	//h_lower_separator
	switch (strlen(h_lower_separator))
	{
		case 0:
			strcpy(pCGI->CFI.h_lower_separator, "  ");
			break;
		case 1:
			strcpy(pCGI->CFI.h_lower_separator, h_lower_separator);
			strcat(pCGI->CFI.h_lower_separator, " ");
			break;
		case 2:
			strcpy(pCGI->CFI.h_lower_separator, h_lower_separator);
			break;
	}

	//v_left_separator
	switch (strlen(v_left_separator))
	{
		case 0:
			strcpy(pCGI->CFI.v_left_separator, "  ");
			break;
		case 1:
			strcpy(pCGI->CFI.v_left_separator, v_left_separator);
			strcat(pCGI->CFI.v_left_separator, " ");
			break;
		case 2:
			strcpy(pCGI->CFI.v_left_separator, v_left_separator);
			break;
	}

	//v_right_separator
	switch (strlen(v_right_separator))
	{
		case 0:
			strcpy(pCGI->CFI.v_right_separator, "  ");
			break;
		case 1:
			strcpy(pCGI->CFI.v_right_separator, v_right_separator);
			strcat(pCGI->CFI.v_right_separator, " ");
			break;
		case 2:
			strcpy(pCGI->CFI.v_right_separator, v_right_separator);
			break;
	}
	
	//mid_separator
	switch (strlen(mid_separator))
	{
		case 0:
			strcpy(pCGI->CFI.mid_separator, "  ");
			break;
		case 1:
			strcpy(pCGI->CFI.mid_separator, mid_separator);
			strcat(pCGI->CFI.mid_separator, " ");
			break;
		case 2:
			strcpy(pCGI->CFI.mid_separator, mid_separator);
			break;
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�ɫ��������С���Ƿ���Ҫ�ָ��ߵ�
  ������������������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int block_width						����ȣ�����ȱʡ2����ΪԼ�������Ϊ�����Ʊ�����������������Ҫ+1��
			const int block_high						���߶ȣ�����ȱʡ1��
			const bool separator						���Ƿ���Ҫ�ָ��ߣ�ȱʡΪtrue����Ҫ�ָ��ߣ�
  �� �� ֵ��
  ˵    ������ܴ�С/�Ƿ���Ҫ�ָ��ߵȵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	pCGI->CFI.block_width = block_width;
	pCGI->CFI.block_high = block_high;
	pCGI->CFI.separator = separator;

	gmw_inner_set_block_width_high_ext(pCGI);//���ķָ����ᵼ�¸ı�ext
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е���ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int bg_color					������ɫ��ȱʡ -1��ʾ�ô��ڱ���ɫ��
			const int fg_color					��ǰ��ɫ��ȱʡ -1��ʾ�ô���ǰ��ɫ��
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	if (bgcolor == -1)
		pCGI->CFI.bgcolor = pCGI->area_bgcolor;
	else
		pCGI->CFI.bgcolor = bgcolor;
	
	if (fgcolor == -1)
		pCGI->CFI.fgcolor = pCGI->area_fgcolor;
	else
		pCGI->CFI.fgcolor = fgcolor;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	switch (type)
	{
		case 1:
			strcpy(pCGI->CBI.top_left, "�X");
			strcpy(pCGI->CBI.lower_left, "�^");
			strcpy(pCGI->CBI.top_right, "�[");
			strcpy(pCGI->CBI.lower_right, "�a");
			strcpy(pCGI->CBI.h_normal, "�T");
			strcpy(pCGI->CBI.v_normal, "�U");
			break;
		case 2:
			strcpy(pCGI->CBI.top_left, "��");
			strcpy(pCGI->CBI.lower_left, "��");
			strcpy(pCGI->CBI.top_right, "��");
			strcpy(pCGI->CBI.lower_right, "��");
			strcpy(pCGI->CBI.h_normal, "��");
			strcpy(pCGI->CBI.v_normal, "��");
			break;
		case 3:
			strcpy(pCGI->CBI.top_left, "�V");
			strcpy(pCGI->CBI.lower_left, "�\");
			strcpy(pCGI->CBI.top_right, "�Y");
			strcpy(pCGI->CBI.lower_right, "�_");
			strcpy(pCGI->CBI.h_normal, "�T");
			strcpy(pCGI->CBI.v_normal, "��");
			break;
		case 4:
		default:
			//�W�Z�]�`�c�f�U�o���i�l�o
			strcpy(pCGI->CBI.top_left, "�W");
			strcpy(pCGI->CBI.lower_left, "�]");
			strcpy(pCGI->CBI.top_right, "�Z");
			strcpy(pCGI->CBI.lower_right, "�`");
			strcpy(pCGI->CBI.h_normal, "��");
			strcpy(pCGI->CBI.v_normal, "�U");
			break;
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *...					����6�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	//start top_left
	switch (strlen(top_left))
	{
	case 0:
		strcpy(pCGI->CFI.top_left, "  ");
		break;
	case 1:
		strcpy(pCGI->CFI.top_left, top_left);
		strcat(pCGI->CFI.top_left, " ");
		break;
	case 2:
		strcpy(pCGI->CFI.top_left, top_left);
		break;
	default:
		strncpy(pCGI->CFI.top_left, top_left, 2);
		break;
	}
	//lower_left
	switch (strlen(lower_left))
	{
	case 0:
		strcpy(pCGI->CFI.lower_left, "  ");
		break;
	case 1:
		strcpy(pCGI->CFI.lower_left, lower_left);
		strcat(pCGI->CFI.lower_left, " ");
		break;
	case 2:
		strcpy(pCGI->CFI.lower_left, lower_left);
		break;
	default:
		strncpy(pCGI->CFI.lower_left, lower_left, 2);
		break;
	}
	//top_right
	switch (strlen(top_right))
	{
	case 0:
		strcpy(pCGI->CFI.top_right, "  ");
		break;
	case 1:
		strcpy(pCGI->CFI.top_right, top_right);
		strcat(pCGI->CFI.top_right, " ");
		break;
	case 2:
		strcpy(pCGI->CFI.top_right, top_right);
		break;
	default:
		strncpy(pCGI->CFI.top_right, top_right, 2);
		break;

	}

	//lower_right
	switch (strlen(lower_right))
	{
	case 0:
		strcpy(pCGI->CFI.lower_right, "  ");
		break;
	case 1:
		strcpy(pCGI->CFI.lower_right, lower_right);
		strcat(pCGI->CFI.lower_right, " ");
		break;
	case 2:
		strcpy(pCGI->CFI.lower_right, lower_right);
		break;
	default:
		strncpy(pCGI->CFI.lower_right, lower_right, 2);
		break;
	}

	//h_normal
	switch (strlen(h_normal))
	{
	case 0:
		strcpy(pCGI->CFI.h_normal, "  ");
		break;
	case 1:
		strcpy(pCGI->CFI.h_normal, h_normal);
		strcat(pCGI->CFI.h_normal, " ");
		break;
	case 2:
		strcpy(pCGI->CFI.h_normal, h_normal);
		break;
	default:
		strncpy(pCGI->CFI.h_normal, h_normal, 2);
		break;
	}

	//v_normal
	switch (strlen(v_normal))
	{
	case 0:
		strcpy(pCGI->CFI.v_normal, "  ");
		break;
	case 1:
		strcpy(pCGI->CFI.v_normal, v_normal);
		strcat(pCGI->CFI.v_normal, " ");
		break;
	case 2:
		strcpy(pCGI->CFI.v_normal, v_normal);
		break;
	default:
		strncpy(pCGI->CFI.v_normal, v_normal, 2);
		break;
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ÿ����Ϸɫ��(����)�Ƿ���ҪС�߿�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const bool on_off��true - ��Ҫ flase - ����Ҫ��ȱʡfalse��
  �� �� ֵ��
  ˵    �����߿�Լ��Ϊ�����Ʊ����˫��
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->CBI.block_border = on_off;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ����״̬��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡtrue��
  �� �� ֵ��
  ˵    ����1��״̬�������Լ�����£�
			   1.1����״̬��ֻ��һ�У������������Ϸ�����/�б�����棬Ϊ��������ʼһ�У�����������Ͻ����������״̬�������꣩
			   1.2����״̬��ֻ��һ�У������������·����ߵ�����
			   1.3��״̬���Ŀ��Ϊ�������ȣ������Ϣ������ض�
		   2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	switch (type)
	{
		case TOP_STATUS_LINE:
			if(on_off)
				pCGI->top_status_line = pCGI->SLI.is_top_status_line = true;
			break;
		case LOWER_STATUS_LINE:
			if (on_off)
				pCGI->lower_status_line = pCGI->SLI.is_lower_status_line = true;
			break;
	}
	
	gmw_inner_set_start_xy(pCGI);
	gmw_inner_set_linescols(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���������״̬������ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const int normal_bgcolor			�������ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int normal_fgcolor			�������ı�ǰ��ɫ��ȱʡ -1��ʾʹ�ô���ǰ��ɫ��
			const int catchy_bgcolor			����Ŀ�ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int catchy_fgcolor			����Ŀ�ı�ǰ��ɫ��ȱʡ -1��ʾʹ������ɫ��
  ���������
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	switch (type)
	{
		case TOP_STATUS_LINE:
			if (normal_bgcolor == -1)
				pCGI->SLI.top_normal_bgcolor = pCGI->area_bgcolor;
			else
				pCGI->SLI.top_normal_bgcolor = normal_bgcolor;
			if (normal_fgcolor == -1)
				pCGI->SLI.top_normal_fgcolor = pCGI->area_fgcolor;
			else
				pCGI->SLI.top_normal_fgcolor = normal_fgcolor;
			if (catchy_bgcolor == -1)
				pCGI->SLI.top_catchy_bgcolor = pCGI->area_bgcolor;
			else
				pCGI->SLI.top_catchy_bgcolor = catchy_bgcolor;
			if (catchy_fgcolor == -1)
				pCGI->SLI.top_catchy_fgcolor = COLOR_HYELLOW;
			else
				pCGI->SLI.top_catchy_fgcolor = catchy_fgcolor;
		case LOWER_STATUS_LINE:
			if (normal_bgcolor == -1)
				pCGI->SLI.lower_normal_bgcolor = pCGI->area_bgcolor;
			else
				pCGI->SLI.lower_normal_bgcolor = normal_bgcolor;
			if (normal_fgcolor == -1)
				pCGI->SLI.lower_normal_fgcolor = pCGI->area_fgcolor;
			else
				pCGI->SLI.lower_normal_fgcolor = normal_fgcolor;
			if (catchy_bgcolor == -1)
				pCGI->SLI.lower_catchy_bgcolor = pCGI->area_bgcolor;
			else
				pCGI->SLI.lower_catchy_bgcolor = catchy_bgcolor;
			if (catchy_fgcolor == -1)
				pCGI->SLI.lower_catchy_fgcolor = COLOR_HYELLOW;
			else
				pCGI->SLI.lower_catchy_fgcolor = catchy_fgcolor;

	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�к�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���к�Լ��Ϊ��ĸA��ʼ�������У��������26�����a��ʼ������52��ͳһΪ*��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�кŵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	if (on_off) 
		pCGI->draw_frame_with_row_no = true;
	else
		pCGI->draw_frame_with_row_no = false;

	gmw_inner_set_start_xy(pCGI);
	gmw_inner_set_linescols(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�б�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���б�Լ��Ϊ����0��ʼ�������У�����0-99������99ͳһΪ**��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�б�ı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	if (on_off)
		pCGI->draw_frame_with_col_no = true;
	else
		pCGI->draw_frame_with_col_no = false;

	gmw_inner_set_start_xy(pCGI);
	gmw_inner_set_linescols(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡ CONSOLE_GRAPHICS_INFO �ṹ���еĸ���Աֵ
  ���������
  �� �� ֵ��
  ˵    ����1�����������ã���ӡ��ʽ�Զ���
            2������������������δ���ù������Բ�ʵ��
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��� CONSOLE_GRAPHICS_INFO �ṹ����ȱʡֵ���г�ʼ��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI������ṹָ��
		   const int row					����Ϸ����ɫ��������ȱʡ10��
		   const int col					����Ϸ����ɫ��������ȱʡ10��
		   const int bgcolor				���������ڱ���ɫ��ȱʡ COLOR_BLACK��
		   const int fgcolor				���������ڱ���ɫ��ȱʡ COLOR_WHITE��
  �� �� ֵ��
  ˵    �������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ��
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* ������ʼ�����������������ڴ˻����������޸Ĳ������ã��������Ԥ��ֵ
		��Ϸ��������ܣ�Ϊ10*10�����ָ��ߣ�ÿ��ɫ��Ĵ�СΪ2��*1�У�ɫ�鲻���߿�
		���ڱ���ɫ�ڣ�ǰ��ɫ��
		����Ϊ����8*16
		���߿�ɫ��ʱ����ʱ��ɫ���ƶ�ʱ��ʱ3ms
		�������Ҹ�������ȫ��Ϊ0
		��״̬�����������ı���ɫͬ���ڣ���Ŀ�ı�����ͬ���ڣ�ǰ��Ϊ����
		��״̬�����������ı���ɫͬ���ڣ���Ŀ�ı�����ͬ���ڣ�ǰ��Ϊ����
		����ʾ�к�
		����ʾ�б�
		��Ϸ����Ϊ˫�߿򣬴��ָ��ߣ�ɫ���СΪ2�����2��=1�����֣�*1���߶�1�У�����ɫͬ����
		ɫ��Ϊ˫���ߣ���ɫ��δ�꣩		*/
	gmw_set_rowcol(pCGI, row, col);
	gmw_set_color(pCGI, bgcolor, fgcolor);
	gmw_set_font(pCGI);
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, 3);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_BLOCK, 0);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_FRAME, 0);
	gmw_set_ext_rowcol(pCGI);
	gmw_set_frame_default_linetype(pCGI, 1);
	gmw_set_frame_style(pCGI);
	gmw_set_frame_color(pCGI);
	gmw_set_block_default_linetype(pCGI, 1);
	gmw_set_block_border_switch(pCGI, true);
	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE);
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE);
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE);
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE);
	gmw_set_rowno_switch(pCGI);
	gmw_set_colno_switch(pCGI);

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ���
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ��������ɲο�demo��Ч��
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/*���ô��ڹ���*/
	setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	setconsoleborder(pCGI->cols, pCGI->lines,pCGI->cols, pCGI->lines);

	const int X = pCGI->start_x;
	const int Y = pCGI->start_y;
	const int bg = pCGI->CFI.fgcolor;//���涼д���ˣ����ﷴ����
	const int fg = pCGI->CFI.bgcolor;
	int i, j, k, l ;

	//��ӡ���б�
	gmw_inner_draw_row_col_signal(pCGI);

	/*�зָ���*/
	if (pCGI->CFI.separator) {
		//��ʼ����
		//the first line
		showstr(X, Y, pCGI->CFI.top_left, fg, bg);
		for (i = 0; i < pCGI->col_num; i++) {
			showstr(X + i * pCGI->CFI.bwidth + CFI_LEN - 1, Y, pCGI->CFI.h_normal, fg, bg, pCGI->CFI.block_width/2);
			showstr(X + (i + 1)*pCGI->CFI.bwidth, Y, pCGI->CFI.h_top_separator, fg, bg);
		}
		showstr(X + (i)*pCGI->CFI.bwidth, Y, pCGI->CFI.top_right, fg, bg);

		//�м�
		for (i = 0; i < pCGI->row_num; i++) {
			for (j = 1; j <= pCGI->CFI.block_high; j++) {
				for (k = 0; k <= pCGI->col_num; k++) {
					showstr(X + k * pCGI->CFI.bwidth, Y + i * (pCGI->CFI.bhigh) + j, pCGI->CFI.v_normal, fg, bg);
					if(k!= pCGI->col_num)
						showstr(X + k * pCGI->CFI.bwidth + CFI_LEN-1, Y + i * (pCGI->CFI.bhigh) + j, " ", fg, bg, pCGI->CFI.block_width);
				}
				Sleep(pCGI->delay_of_draw_frame);//ÿ��֮��Ӽ��
			}
			//�������ָ���
			showstr(X, Y + (i + 1) * pCGI->CFI.bhigh, pCGI->CFI.v_left_separator, fg, bg);
			for (l = 0; l < pCGI->col_num; l++) {
				showstr( X + l * pCGI->CFI.bwidth + CFI_LEN - 1, Y + (i + 1) * pCGI->CFI.bhigh, pCGI->CFI.h_normal, fg, bg, pCGI->CFI.block_width / 2);
				showstr(  X + (l + 1) * pCGI->CFI.bwidth, Y + (i + 1) * pCGI->CFI.bhigh, pCGI->CFI.mid_separator, fg, bg);
			}
			showstr( X + (l) * pCGI->CFI.bwidth,Y + (i + 1) * pCGI->CFI.bhigh, pCGI->CFI.v_right_separator, fg, bg);//�����һ��"��"�ǵ�
			Sleep(pCGI->delay_of_draw_frame);
		}

		//the last line
		showstr( X ,Y + pCGI->row_num * pCGI->CFI.bhigh, pCGI->CFI.lower_left, fg, bg);
		for (i = 0; i < pCGI->col_num; i++) {
			showstr( X + i * pCGI->CFI.bwidth + CFI_LEN - 1, Y + pCGI->row_num * pCGI->CFI.bhigh, pCGI->CFI.h_normal, fg, bg, pCGI->CFI.block_width / 2);
			showstr( X + (i + 1)*pCGI->CFI.bwidth,Y + pCGI->row_num * pCGI->CFI.bhigh, pCGI->CFI.h_lower_separator, fg, bg);
		}
		showstr( X + (i )*pCGI->CFI.bwidth, Y + pCGI->row_num * pCGI->CFI.bhigh, pCGI->CFI.lower_right, fg, bg);
	}
	
	/*�޷ָ���*/
	else {
		//the first line
		showstr(X, Y, pCGI->CFI.top_left, fg, bg);
		for (i = 0; i < pCGI->col_num; i++) {
			showstr(X + i * pCGI->CFI.bwidth + CFI_LEN - 1, Y, pCGI->CFI.h_normal, fg, bg, pCGI->CFI.block_width / 2);
			//showstr(X + (i + 1)*pCGI->CFI.bwidth, Y, pCGI->CFI.h_top_separator, fg, bg);
		}
		showstr(X + (i)*pCGI->CFI.bwidth + CFI_LEN - 1, Y, pCGI->CFI.top_right, fg, bg);
		//�����ޱ߿���˵bwidthû�ж����ȣ�����+2�����ϱ߿�


		//body
		for (i = 0; i < pCGI->row_num; i++) {
			for (j = 1; j <= pCGI->CFI.block_high; j++) {
				showstr(X , Y + i * (pCGI->CFI.bhigh) + j, pCGI->CFI.v_normal, fg, bg);
				showstr(X + CFI_LEN - 1, Y + i * (pCGI->CFI.bhigh) + j, " ", fg, bg, pCGI->CFI.bwidth*pCGI->col_num);
				showstr(X + pCGI->col_num * pCGI->CFI.bwidth + CFI_LEN - 1, Y + i * (pCGI->CFI.bhigh) + j, pCGI->CFI.v_normal, fg, bg);
				Sleep(pCGI->delay_of_draw_frame);
			}
		}

		//the last line
		showstr(X, Y + pCGI->row_num*pCGI->CFI.bhigh + 1, pCGI->CFI.lower_left, fg, bg);
		for (i = 0; i < pCGI->col_num; i++) {
			showstr(X + i * pCGI->CFI.bwidth + CFI_LEN - 1, Y + pCGI->row_num*pCGI->CFI.bhigh + 1, pCGI->CFI.h_normal, fg, bg, pCGI->CFI.block_width / 2);
			//showstr(X + (i + 1)*pCGI->CFI.bwidth, Y, pCGI->CFI.h_top_separator, fg, bg);
		}
		showstr(X + (i)*pCGI->CFI.bwidth + CFI_LEN - 1, Y + pCGI->row_num*pCGI->CFI.bhigh + 1, pCGI->CFI.lower_right, fg, bg);

	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���״̬������ʾ��Ϣ
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type							��ָ������/��״̬��
		   const char *msg						��������Ϣ
		   const char *catchy_msg					����Ҫ�ر��ע����Ϣ����������Ϣǰ��ʾ��
  �� �� ֵ��
  ˵    ����1���������޶�Ϊ����ܵĿ�ȣ����к��б�λ�ã����������ȥ
            2��������һ���ַ���ĳ���ֵ�ǰ������ᵼ�º������룬Ҫ����
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
	
	int X;
	int Y;
	int WIDTH;
	if (pCGI->draw_frame_with_row_no) {
		X = pCGI->start_x - 1;
		WIDTH = pCGI->main_width + 2;
	}
	else {
		X = pCGI->start_x;
		WIDTH = pCGI->main_width;
	}

	char buf[CHAR_LEN] = "";
	char BUF[CHAR_LEN] = "";
	if(catchy_msg!=NULL)
		strcat(buf, catchy_msg);
	strcat(buf, msg);
	strncpy(BUF, buf, WIDTH);

	switch (type)
	{
		case TOP_STATUS_LINE:
			if (pCGI->draw_frame_with_col_no)
				Y = pCGI->start_y - 2;
			else
				Y = pCGI->start_y - 1;

			//�����ʾ�Ƿ񳬹����
			if (gmw_inner_check_line(BUF)) {
				if (catchy_msg != NULL) {
					showstr(X, Y, catchy_msg, pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor, 1, WIDTH);
					showstr(X + strlen(catchy_msg), Y, msg, pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, 1, WIDTH - strlen(catchy_msg));
				}
				else {
					showstr(X , Y, msg, pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, 1, WIDTH);
				}
			}
			else {
				if (catchy_msg != NULL) {
					showstr(X, Y, catchy_msg, pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor, 1, WIDTH);
					showstr(X + strlen(catchy_msg), Y, msg, pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, 1, WIDTH - strlen(catchy_msg));
				}
				else
					showstr(X , Y, msg, pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, 1, WIDTH-1 );
			}
			break;
		case LOWER_STATUS_LINE:
			Y = pCGI->start_y + pCGI->main_high;

			//�����ʾ�Ƿ񳬹����
			if (gmw_inner_check_line(BUF)) {
				if (catchy_msg != NULL) {
					showstr(X, Y, catchy_msg, pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor, 1, WIDTH);
					showstr(X + strlen(catchy_msg), Y, msg, pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor, 1, WIDTH - strlen(catchy_msg));
				}
				else {
					showstr(X, Y, msg, pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor, 1, WIDTH);
				}
			}
			else {
				if (catchy_msg != NULL) {
					showstr(X, Y, catchy_msg, pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor, 1, WIDTH);
					showstr(X + strlen(catchy_msg), Y, msg, pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, 1, WIDTH - strlen(catchy_msg));
				}
				else
					showstr(X, Y, msg, pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, 1, WIDTH);
			}
			break;
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾĳһ��ɫ��(����Ϊ�ַ���������Ϊrow/col)
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����1��BLOCK_DISPLAY_INFO �ĺ����ͷ�ļ����÷��ο���������
            2��bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
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
	if (bdi[i].content == NULL && bdi[i].value!=BDI_VALUE_BLANK) {//�տ鲻����
		//��ӡɫ��
		int j;
		for (j = 0; j < pCGI->CFI.block_high; j++)
			showstr(pos.X, pos.Y + j, " ", bg, fg, pCGI->CFI.block_width, pCGI->CFI.block_width);

		//��ӡɫ��С�߿�
		if (pCGI->CBI.block_border && pCGI->CFI.block_high>1 && pCGI->CFI.block_width >= 4) {
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
	if (pCGI->CBI.block_border && bdi_value!=BDI_VALUE_BLANK && pCGI->CFI.block_high > 1 && pCGI->CFI.block_width >= 4) {
		//the first line
		showstr(pos.X, pos.Y, pCGI->CBI.top_left, bg, fg);
		showstr(pos.X + 2, pos.Y, pCGI->CBI.h_normal, bg, fg, pCGI->CFI.block_width/2 - 2);
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
/***************************************************************************
  �������ƣ�
  ��    �ܣ�������ʾĳһ��ɫ��(����Ϊ�ַ���������ΪX/Y)
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int X					������̨X����0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int Y					������̨Y����0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����1��BLOCK_DISPLAY_INFO �ĺ����ͷ�ļ����÷��ο���������
			2��bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
***************************************************************************/
int gmw_inner_draw_block(const int X, const int Y, const CONSOLE_GRAPHICS_INFO *const pCGI, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	//Sleep(pCGI->delay_of_draw_block);
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
				showstr( X,  Y + k, pCGI->CBI.v_normal, bg, fg);
				showstr( X + pCGI->CFI.block_width - 2,  Y + k, pCGI->CBI.v_normal, bg, fg);
			}

			//the last line
			showstr( X,  Y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_left, bg, fg);
			showstr( X + 2,  Y + pCGI->CFI.block_high - 1, pCGI->CBI.h_normal, bg, fg, pCGI->CFI.block_width / 2 - 2);
			showstr( X + pCGI->CFI.block_width - 2,  Y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_right, bg, fg);
		}

		//��valueת��Ϊchar*��
		char temp[CHAR_LEN];
		sprintf(temp, "%d", bdi[i].value);

		//��ʾvalue�ַ�
		if (pCGI->CFI.block_high % 2)
			showstr( X + pCGI->CFI.block_width / 2 - strlen(temp) / 2 - 1,  Y + pCGI->CFI.block_high / 2, temp, bg, fg);
		else
			showstr( X + pCGI->CFI.block_width / 2 - strlen(temp) / 2 - 1,  Y + pCGI->CFI.block_high / 2 + 1, temp, bg, fg);
		return 0;
	}

	int j;
	for (j = 0; j < pCGI->CFI.block_high; j++)
		showstr( X,  Y + j, " ", bg, fg, pCGI->CFI.block_width, pCGI->CFI.block_width);

	//��ӡɫ��С�߿�
	if (pCGI->CBI.block_border && bdi_value != BDI_VALUE_BLANK && pCGI->CFI.block_high > 1 && pCGI->CFI.block_width >= 4) {
		//the first line
		showstr( X,  Y, pCGI->CBI.top_left, bg, fg);
		showstr( X + 2,  Y, pCGI->CBI.h_normal, bg, fg, pCGI->CFI.block_width / 2 - 2);
		showstr( X + pCGI->CFI.block_width - 2,  Y, pCGI->CBI.top_right, bg, fg);

		//body
		int k;
		for (k = 1; k <= pCGI->CFI.block_high - 2; k++) {
			showstr( X,  Y + k, pCGI->CBI.v_normal, bg, fg);
			showstr( X + pCGI->CFI.block_width - 2,  Y + k, pCGI->CBI.v_normal, bg, fg);
		}

		//the last line
		showstr( X,  Y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_left, bg, fg);
		showstr( X + 2,  Y + pCGI->CFI.block_high - 1, pCGI->CBI.h_normal, bg, fg, pCGI->CFI.block_width / 2 - 2);
		showstr( X + pCGI->CFI.block_width - 2,  Y + pCGI->CFI.block_high - 1, pCGI->CBI.lower_right, bg, fg);
	}

	//��ʾcontent�ַ�
	if (bdi[i].value != BDI_VALUE_BLANK) {
		if (pCGI->CFI.block_high % 2)
			showstr(X + pCGI->CFI.block_width / 2 - strlen(bdi[i].content) / 2, Y + pCGI->CFI.block_high / 2, bdi[i].content, bg, fg);
		else
			showstr(X + pCGI->CFI.block_width / 2 - strlen(bdi[i].content) / 2, Y + pCGI->CFI.block_high / 2 + 1, bdi[i].content, bg, fg);
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ƶ�ĳһ��ɫ��
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const int blank_bdi_value				���ƶ����������ڶ���Ч����ʾʱ���ڱ�ʾ�հ׵�ֵ��һ��Ϊ0���˴���Ϊ�������룬�ǿ��ǵ����ܳ��ֵ����������
		   const BLOCK_DISPLAY_INFO *const bdi		�������ʾֵ/�հ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
		   const int direction						���ƶ�����һ�����֣������cmd_gmw_tools.h
		   const int distance						���ƶ����루��1��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
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
	
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������̻����
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   int &MAction							��������� CCT_MOUSE_EVENT�����ֵ��Ч��Ϊ MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK ����֮һ
		                                               ������� CCT_KEYBOARD_EVENT�����ֵ��Ч
		   int &MRow								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &MCol								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode1							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ��һ����
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode2							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ�ڶ���������ǵ����룬��Ϊ0��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   const bool update_lower_status_line		������ƶ�ʱ���Ƿ�Ҫ�ڱ���������ʾ"[��ǰ���] *��*��"����Ϣ��true=��ʾ��false=����ʾ��ȱʡΪtrue��
  �� �� ֵ����������Լ��
            1�����������ƶ��������أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] *��*��"
		   2������ǰ������������ҵ�ǰ���ָ��ͣ��������Ϸ�����*��*���ϣ��򷵻� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow Ϊ�кţ�MCol Ϊ�б�
		                          �ҵ�ǰ���ָ��ͣ���ڷǷ����򣨷���Ϸ������Ϸ�����еķָ��ߣ����򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
		   3������ǰ�������Ҽ������ж����ָ��ͣ�������Ƿ�Ϸ���ֱ�ӷ��� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_RIGHT_BUTTON_CLICK, MRow��MCol������
		   4��������¼����ϵ�ĳ������˫���밴��������ֱ�ӷ��� CCT_KEYBOARD_EVENT��KeyCode1/KeyCode2��Ϊ��Ӧ�ļ���ֵ
 ˵    ����ͨ������ cmd_console_tools.cpp �е� read_keyboard_and_mouse ����ʵ��
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	int X = 0, Y = 0;
	int ret ;
	int loop = 1;

	enable_mouse();

	/* ��ӡ��ʼ���λ��[0,0] */
	setcursor(CURSOR_INVISIBLE);	//�رչ��
	//		cout << "[��ǰ���λ��] X:0  Y:0"; //��ӡ��ʼ���λ��

	while (loop) {
		/* �����/���̣�����ֵΪ���������е�ĳһ��, ��ǰ���λ����<X,Y>�� */
		ret = read_keyboard_and_mouse(X, Y, MAction, KeyCode1, KeyCode2);

		if (ret == CCT_MOUSE_EVENT) {
			/* ����״̬����ӡ *///��sprintf��ȥ������ʾ

			MRow = ay(Y, pCGI);
			MCol = ax(X, pCGI);
			if (MCol == -1 || MRow == -1) {
				gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���λ��] λ�÷Ƿ�");
			}
			else {
				if (update_lower_status_line) {
					char buf[CHAR_LEN];
					sprintf(buf, "[��ǰ���λ��] %c�� %d��", MRow + 'A', MCol);
					gmw_status_line(pCGI, LOWER_STATUS_LINE, buf);
				}

				switch (MAction) {
				case MOUSE_ONLY_MOVED:
					//gmw_status_line(pCGI, LOWER_STATUS_LINE, "�ƶ�          ");
					//showch(X, Y, '*');����ӡ�κ�����
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//�������
					//gmw_status_line(pCGI, LOWER_STATUS_LINE, "�������      ");
					//cout << "�������      " << endl;
					//showch(X, Y, '1');			//�����ָ��λ����ʾ1
					return CCT_MOUSE_EVENT;
					break;
				case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:	//˫�����
					//cout << "˫�����      " << endl;
					//showch(X, Y, '2');			//�����ָ��λ����ʾ2
					return CCT_MOUSE_EVENT;
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//�����Ҽ�
					//cout << "�����Ҽ�      " << endl;
					//showch(X, Y, '3');			//�����ָ��λ����ʾ3
					return CCT_MOUSE_EVENT;
					break;
				case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//˫���Ҽ�
					//cout << "˫���Ҽ�      " << endl;
					//showch(X, Y, '4');			//�����ָ��λ����ʾ4
					//loop = 0;
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
				} //end of switch(MAction)

			}

		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT)
			return CCT_KEYBOARD_EVENT;
	} //end of while(1)

	disable_mouse();	//�������
	setcursor(CURSOR_VISIBLE_NORMAL);	//�򿪹��
	return 0; //�˾�ɸ�����Ҫ�޸�
}

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
