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
	int Check_remove(const int direction);
	int Move_animation(const int direction);
	int Clear_status();
	int Gameover();
	int print_map();
};

