/* 1751957 ��2�� ������ */
#pragma once
#include <iostream>
#include<iomanip>
#include <cstdio>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <string.h>

struct ball_info {
	int row;//Y
	int col;//X
	int separator = 1;//���޷ָ���
	int rc_sign = 1;//���б�
	int value = 0;//���ֵ1-9
	int front_color = 16;//ǰ��ɫ���������ɫ
	int bg_color = (value + 7) % 16;//����ɫ��Ĭ��Ϊ��ɫ
	int status = 0;//���״̬
	/*status:
		0Ϊ��ͨ�� NORMAL
		1Ϊ�γ����Ӵ�����״̬ WILL_REMOVE
		2Ϊ�������� NEW_FILL
		3ΪԤ����ʾ��ǵ��� PRE_REMIND*/
};
