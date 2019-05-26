#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "90-b4.h"
class G2048
{
private:
	CONSOLE_GRAPHICS_INFO G2048_CGI; //����һ��CGI����
	BLOCK **map;//��άBLOCK�ṹ��ָ�룬��̬�����ά����
	int row;
	int col;
	int score;//ʵ�ʷ���
	int dst_score;//Ŀ�����
	int high_score;//��߷�
	int speed;
	bool separator;
	/*private functions*/
	int up_align(int c);//c:Ŀ����
	int down_align(int c);
	int left_align(int r);
	int right_align(int r);
	int up_check(int c);//c:Ŀ����
	int down_check(int c);
	int left_check(int r);
	int right_check(int r);
public:
	G2048(const int Row = 8 ,const int Col = 8,const int Dst_score = 8192,const int Speed = 0,const bool Sep = true);
	~G2048();
	int set(const int Row = 8, const int Col = 8, const int Dst_score = 8192, const int Speed = 0, const bool Sep = true);
	int Produce_new_block(const int num = 1, const bool show = true);
	int Init_map();//��ʼ���ڲ����飬
	int Init_frame();//��ʼ������
	int Game_start();//���ĺ�����������Ϸ����ȫ����������
	int Align(const int direction);
	int Check_remove(const int direction);
	int Move_animation(const int direction);
	int Clear_status();
	int Gameover();
	int print_map();
	int gmw_inner_position_trans(const CONSOLE_GRAPHICS_INFO *const pCGI, POS *const pos);
	int gmw_inner_draw_block(const int X, const int Y, const CONSOLE_GRAPHICS_INFO *const pCGI, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi);
	int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi);
	int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance);
};

