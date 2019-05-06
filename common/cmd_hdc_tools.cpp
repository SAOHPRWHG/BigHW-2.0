/* 1751957 ��2�� ������ */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

/* ���������Ҫ��ͷ�ļ� */
#include <math.h>
#include "cmd_hdc_tools.h"
using namespace std;

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();

/* �Ѷ���ľ�̬ȫ�ֱ��� */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;

/* �����ʶ�ʹ�þ�̬ȫ�ֱ�����������static�����ⲿ���ɼ� */


/* �˴���������Զ��庯����������static�����ⲿ���ɼ� */
int Max(int a, int b, int c) 
{
	if (a >= b) {
		if (a >= c)
			return a;
		else
			return c;
	}
	else {
		if (b >= c)
			return b;
		else
			return c;
	}
}

int Max(int a, int b, int c, int d)
{
	return max(a, b) >= max(c, d) ? max(a, b) : max(c, d);
}

int Min(int a, int b, int c) {
	if (a >= b) {
		if (b >= c)
			return c;
		else
			return b;
	}
	else {
		if (a >= c)
			return c;
		else
			return a;
	}
}

int Min(int a, int b, int c, int d)
{
	return min(a, b) <= min(c, d) ? min(a, b) : min(c, d);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ؽ�һ��x,y����ֱ��x1y1x2y2���̺�������
  ���������
  �� �� ֵ��1��-1,0(��ֱ����)
  ˵    ����ע��x1,y1,x2,y2˳��Ҫ��
***************************************************************************/
int line_up_down(int x1,int y1,int x2,int y2,int x,int y)
{
	int flag;
	flag = (y2 - y1)*(x - x1) - (x2 - x1)*(y - y1);
	if (flag > 0)
		return 1;
	else if (flag < 0)
		return -1;
	else
		return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��жϵ㣨x,y����Բ�ڻ���Բ��
  ���������
  �� �� ֵ��flag<0��Բ�ڣ�flag>0��Բ��
  ˵    ����
***************************************************************************/
int circle_in_out(int point_x, int point_y, int r, int x, int y)
{
	int flag;
	flag = (x - point_x)*(x - point_x) + (y - point_y)*(y - point_y) - r * r;
	
	return flag;
}

double ellipse_in_out(int point_x, int point_y, int a, int b, int rot,int x,int y)
{
	double r = (360-rot) * PI / 180;
	double flag;
	double s1 = (a*a*sin(r)*sin(r) + b * b*cos(r)*cos(r))*(x - point_x)*(x - point_x);
	double s2 = (a*a*cos(r)*cos(r) + b * b*sin(r)*sin(r))*(y - point_y)*(y - point_y);
	double s3 = 2 * (a*a - b * b)*sin(r)*cos(r)*(x - point_x)*(y - point_y);
	long long s4 = a*b*b;
	flag = s1 + s2 + s3 - a*s4;
	return flag;
}


/* ��������˼�����������������ʵ�֣���Ҫ�Ķ� */
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʼ��
  ���������const int bgcolor������ɫ
            const int fgcolor��ǰ��ɫ
			const int width  ����Ļ��ȣ�����
			const int high   ����Ļ�߶ȣ�����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* ���ͷţ���ֹ��release���ٴ�init��hdc_release�����룩 */
	hdc_release();

	/* ����init����һ����̬ȫ������¼������hdc_cls()���õ� */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	setcursor(CURSOR_INVISIBLE);
	setcolor(bgcolor, fgcolor);
	setfontsize("��������", 16);
	setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16) , 120, 999); //������Ŀ�ȼ��߶�ת��Ϊ�ض��������������!!�ĺ��壺�������8/16�ı��������ж�+1
	cls();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ͷŻ�ͼ��Դ
  ���������
  �� �� ֵ��
  ˵    ����������
***************************************************************************/
void hdc_release()
{
	if (hdc) {
		ReleaseDC(hWnd, hdc);
		hdc = NULL;
		setcursor(CURSOR_VISIBLE_NORMAL);
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGBֵ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGB��ɫ��ֵ0-255
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ļ�����е�ͼ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_cls()
{
    /* ����һ��Ҫ��һ����ɫ��ʼ������������ص㣬�ҵ�����㷽����ͬѧ����֪ͨ�� */
	hdc_init(_BgColor_, (_FgColor_ +1)%16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* ���ֻ���������demoʱ�������hdc_cls()���һ������ȱ�ߵ����������ʱ����
	   ������ֻ������л������⣬���ߴ���ʱֵ */
	Sleep(30);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x��x���꣬���Ͻ�Ϊ(0,0)
            const int y��y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x1��x1���꣬���Ͻ�Ϊ(0,0)
			const int y1��y1���꣬���Ͻ�Ϊ(0,0)
			const int x2��x2���꣬���Ͻ�Ϊ(0,0)
			const int y2��y2���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ��һ��ָ����ϸ�ĵ�(�û�ʵ��Բ��ģ��)
  ���������const int x			��x���꣬���Ͻ�Ϊ(0,0)
            const int y			��y���꣬���Ͻ�Ϊ(0,0)
			const int thickness	����Ĵ�ϸ������λ1�����޲���(�����ᵼ�²���ȫ���)����ȱʡֵ��
			const int RGB_value	�������ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �����Ľ�����Ч��
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������
	/* ������ֱ�������ޣ����ǵ����޳���һ����С�ǣ�������Բ����λ��δʵ�� */

	const int tn_end = (tn <= 1) ? 1 : tn/2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* �û�Բ�ķ�ʽģ��ֵ� */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle ++) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* �ſ���ע�ͣ����Կ���һ���ֵ����˼������ص���� */
	gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	�������к�����ʵ�֣���������������׼�Ķ�
	1������Ҫ����ϵͳ�������й�ͼ�β����ĺ���
	2�����к�����ʵ�ֶ�����hdc_point���������
	3����һ�룬�ǲ������к�������Ҫ����������ʵ�ֹ��̣�Ӧ����ʵ����Щ������
	4�������ʵ������ӵĲ���
	5��ϵͳ�ĽǶ��뺯�������еĽǶȺ������180��
   ------------------------------------------------------------------------- */
/***************************************************************************
  �������ƣ�
  ��    �ܣ����߶�
  ���������const int x1		������x
            const int y1		������y
			const int x2		���յ��x
			const int y2		���յ��y
			const int thickness	���߶εĴ�ϸ����ȱʡֵ��1-5 ȱʡ2
			const int RGB_value	���߶ε���ɫ����ȱʡֵ��0-255 
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������
	/* ������ֱ�������ޣ����ǵ����޳���һ����С�ǣ�������Բ����λ��δʵ�� */
	double k1, k2, delta_x, delta_y;
	if (x2 == x1) {
		delta_x = 0;
		delta_y = 1;
	}
	else if (y2 == y1) {
		delta_x = 1;
		delta_y = 0;
	}
	else {
		k1 = double(y2 - y1) / double(x2 - x1);
		k2 = -1 / (k1);
		delta_x = 1 / sqrt(1 + k2 * k2);
		delta_y = fabs(k2) / sqrt(1 + k2 * k2);
	}
	


	const int tn_end = (tn <= 1) ? 1 : tn / 2;
	
	int level;
	int old_xx1 = INT_MAX, old_yy1 = INT_MAX, old_xx2 = INT_MAX, old_yy2 = INT_MAX, xx1, xx2, yy1, yy2;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	
	xx1 = x1;
	yy1 = y1;
	xx2 = x2;
	yy2 = y2;
	/* �û�Բ�ķ�ʽģ��ֵ� */
	for (level = - tn_end; level <= tn_end; level++) {	
		xx1 = x1 + (int)(level * delta_x);	
		/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
		if (xx1 != old_xx1 ) {
			old_xx1 = xx1;
			hdc_base_line(xx1, yy1, xx2, yy2);
			++count;			
		}
		yy1 = y1 + (int)(level * delta_y);
		if (yy1 != old_yy1) {
			old_yy1 = yy1;
			hdc_base_line(xx1, yy1, xx2, yy2);
			++count;
		}
		xx2 = x2 + (int)(level * delta_x);
		if (xx2 != old_xx2 ) {
			old_xx2 = xx2;
			hdc_base_line(xx1, yy1, xx2, yy2);
			++count;
		}
		yy2 = y2 + (int)(level * delta_y);
		/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
		if (yy2 != old_yy2) {
			old_yy2 = yy2;
			hdc_base_line(xx1, yy1, xx2, yy2);
			++count;
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������������꣬��һ��������
  ���������const int x1		����1�����x
            const int y1		����1�����y
			const int x2		����2�����x
			const int y2		����2�����y
			const int x3		����3�����x
			const int y3		����3�����y
			bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness	���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value	����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �������ж������Ƿ��ߣ�������ߣ�����һ��ֱ�߼���
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x3, y3, x2, y2, thickness, RGB_value);
	hdc_line(x1, y1, x3, y3, thickness, RGB_value);
	
	if (filled) {
		int i,j;
		double gpx = (x1 + x2 + x3) / 3.00;
		double gpy = (y1 + y2 + y3) / 3.00;
		int x_max = Max(x1, x2, x3);
		int y_max = Max(y1, y2, y3);
		int x_min = Min(x1, x2, x3);
		int y_min = Min(y1, y2, y3);
		int flag1 = line_up_down(x1, y1, x2, y2, int(gpx), int(gpy));
		int flag2 = line_up_down(x3, y3, x2, y2, int(gpx), int(gpy));
		int flag3 = line_up_down(x3, y3, x1, y1, int(gpx), int(gpy));

		for (i = x_min; i <= x_max; i++) {
			int  xx1, xx2, yy1, yy2;
			int flag = 0;
			for (j = y_min; j <= y_max; j++) {
				if (line_up_down(x1, y1, x2, y2, i, j)*flag1 >= 0 && line_up_down(x3, y3, x2, y2, i, j)*flag2 >= 0 && line_up_down(x3, y3, x1, y1, i, j)*flag3 >= 0) {
					if (flag == 0) {
						xx1 = i;
						yy1 = j;
						flag = 1;
					}
					if (j == y_max) {
						xx2 = i;
						yy2 = j;
						hdc_base_line(xx1, yy1, xx2, yy2);
					}
				}
				else if (flag == 1) {
					xx2 = i;
					yy2 = j;
					flag = 0;
					hdc_base_line(xx1, yy1, xx2, yy2);
				}
			}

		}//end of for(x)

	}//end of if(filled)
		
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰��ȡ��߶ȣ�����һ��������
  ���������const int left_up_x			�����Ͻ�x
            const int left_up_y			�����Ͻ�y
			const int width				�����
			const int high				���߶�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ����
			bool filled					���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	int x1, x2, x3, x4, y1, y2, y3, y4;
	x1 = left_up_x;
	y1 = left_up_y;

	x2 = x1 + (int)(width*cos(rotation_angles* PI / 180));
	y2 = y1 + (int)(width*sin(rotation_angles* PI / 180));

	x3 = x1 + (int)(high*cos((rotation_angles+90) * PI / 180));
	y3 = y1 + (int)(high*sin((rotation_angles+90) * PI / 180));

	x4 = x3 + (int)(width*cos(rotation_angles* PI / 180));
	y4 = y3 + (int)(width*sin(rotation_angles* PI / 180));

	/*����*/
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x1, y1, x3, y3, thickness, RGB_value);
	hdc_line(x4, y4, x2, y2, thickness, RGB_value);
	hdc_line(x4, y4, x3, y3, thickness, RGB_value);

	if (filled) {
		int i, j;
		double gpx = (x1 + x4) / 2.00;
		double gpy = (y1 + y4) / 2.00;
		int x_max = Max(x1, x2, x3, x4);
		int y_max = Max(y1, y2, y3, y4);
		int x_min = Min(x1, x2, x3, x4);
		int y_min = Min(y1, y2, y3, y4);
		int flag1 = line_up_down(x1, y1, x2, y2, int(gpx), int(gpy));
		int flag2 = line_up_down(x4, y4, x2, y2, int(gpx), int(gpy));
		int flag3 = line_up_down(x1, y1, x3, y3, int(gpx), int(gpy));
		int flag4 = line_up_down(x4, y4, x3, y3, int(gpx), int(gpy));

		for (i = x_min; i <= x_max; i++) {
			int  xx1, xx2, yy1, yy2;
			int flag = 0;
			for (j = y_min; j <= y_max; j++) {
				if (line_up_down(x1, y1, x2, y2, i, j)*flag1 >= 0 && line_up_down(x4, y4, x2, y2, i, j)*flag2 >= 0 && line_up_down(x1, y1, x3, y3, i, j)*flag3 >= 0 && line_up_down(x4, y4, x3, y3, i, j)*flag4 >= 0) {
					if (flag == 0) {
						xx1 = i;
						yy1 = j;
						flag = 1;
					}
					if (j == y_max) {
						xx2 = i;
						yy2 = j;
						hdc_base_line(xx1, yy1, xx2, yy2);
					}
				}
				else if (flag == 1) {
					xx2 = i;
					yy2 = j;
					flag = 0;
					hdc_base_line(xx1, yy1, xx2, yy2);
				}
			}
		}
	}
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰�߳�������һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int length			���߳�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	int x1, x2, x3, x4, y1, y2, y3, y4;
	x1 = left_up_x;
	y1 = left_up_y;

	x2 = x1 + (int)(length*cos(rotation_angles* PI / 180));
	y2 = y1 + (int)(length*sin(rotation_angles* PI / 180));

	x3 = x1 + (int)(length*cos((rotation_angles + 90) * PI / 180));
	y3 = y1 + (int)(length*sin((rotation_angles + 90) * PI / 180));

	x4 = x3 + (int)(length*cos(rotation_angles* PI / 180));
	y4 = y3 + (int)(length*sin(rotation_angles* PI / 180));

	/*����*/
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x1, y1, x3, y3, thickness, RGB_value);
	hdc_line(x4, y4, x2, y2, thickness, RGB_value);
	hdc_line(x4, y4, x3, y3, thickness, RGB_value);

	if (filled) {
		int i, j;
		double gpx = (x1 + x4) / 2.00;
		double gpy = (y1 + y4) / 2.00;
		int x_max = Max(x1, x2, x3, x4);
		int y_max = Max(y1, y2, y3, y4);
		int x_min = Min(x1, x2, x3, x4);
		int y_min = Min(y1, y2, y3, y4);
		int flag1 = line_up_down(x1, y1, x2, y2, int(gpx), int(gpy));
		int flag2 = line_up_down(x4, y4, x2, y2, int(gpx), int(gpy));
		int flag3 = line_up_down(x1, y1, x3, y3, int(gpx), int(gpy));
		int flag4 = line_up_down(x4, y4, x3, y3, int(gpx), int(gpy));

		
		for (i = x_min; i <= x_max; i++) {
			int  xx1, xx2, yy1, yy2;
			int flag = 0;
			for (j = y_min; j <= y_max; j++) {			
				if (line_up_down(x1, y1, x2, y2, i, j)*flag1 >= 0 && line_up_down(x4, y4, x2, y2, i, j)*flag2 >= 0 && line_up_down(x1, y1, x3, y3, i, j)*flag3 >= 0 && line_up_down(x4, y4, x3, y3, i, j)*flag4 >= 0) {
					if (flag==0) {
						xx1 = i;
						yy1 = j;
						flag = 1;
					}
					if (j == y_max) {
						xx2 = i;
						yy2 = j;
						hdc_base_line(xx1, yy1, xx2, yy2);
					}
				}
				else if (flag == 1) {
					xx2 = i;
					yy2 = j;
					flag = 0;
					hdc_base_line(xx1, yy1, xx2, yy2);
				}			
			}					
		}

	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ��
  ���������const int point_x		��Բ�ĵ�x
            const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)

{
	double tn = thickness / 2.0;
	double angle;
	int x1,y1;
	double delta = double(0.25*thickness) / double(radius) * 180 / PI;

	int end = angle_end;

	if (angle_begin > angle_end) {
		for (; end < angle_begin; end += 360)
			;
	}
		for (angle = angle_begin; angle <= end; angle+=delta) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			x1 = point_x + (int)((radius ) * sin(angle * PI / 180));
			y1 = point_y - (int)((radius ) * cos(angle * PI / 180));			
			hdc_point(x1, y1, thickness, RGB_value);
		}

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ������
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ��������ʼ/�����ǶȲ�ֵΪ360�ı���ʱ������������
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	double tn = thickness / 2.0;
	double angle;
	int x1,y1,x2,y2;
	int x1_max=0, x1_min, y1_max, y1_min;
	int sx, sy, tx, ty;
	sx = point_x + (int)((radius)* sin(angle_begin * PI / 180));
	sy = point_y - (int)((radius)* cos(angle_begin * PI / 180));
	tx = point_x + (int)((radius)* sin(angle_end * PI / 180));
	ty = point_y - (int)((radius)* cos(angle_end * PI / 180));

	double delta = double(0.25*thickness) / double(radius) * 180 / PI;

	int end = angle_end;

	if (angle_begin > angle_end) {
		for (; end < angle_begin; end += 360)
			;
	}

	x2 = point_x + (int)((radius)* sin(angle_begin * PI / 180));
	y2 = point_y - (int)((radius)* cos(angle_begin * PI / 180));
	x1_max = x2;
	x1_min = x2;
	y1_max = y2;
	y1_min = y2;


	hdc_line(point_x, point_y, sx, sy, thickness, RGB_value);
	for (angle = angle_begin; angle <= end; angle += delta) {
		x1_max = max(x1_max, x2);
		x1_min = min(x1_min, x2);
		y1_max = max(y1_max, y2);
		y1_min = min(y1_min, y2);
		
		/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
		x1 = point_x + (int)((radius)* sin(angle * PI / 180));
		y1 = point_y - (int)((radius)* cos(angle * PI / 180));
		hdc_point(x1, y1, thickness, RGB_value);
		x2 = x1;
		y2 = y1;
	}
	hdc_line(tx, ty, point_x, point_y, thickness, RGB_value);

	if (filled) {
		int i, j;
		double gpx = point_x + 0.5*((radius)* sin(0.5*(end + angle_begin) * PI / 180));
		double gpy = point_y - 0.5*((radius)* cos(0.5*(end + angle_begin) * PI / 180));
		int x_max = max(point_x, x1_max);
		int y_max = max(point_y, y1_max);
		int x_min = min(point_x, x1_min);
		int y_min = min(point_y, y1_min);
		int flag2 = line_up_down(point_x, point_y, sx, sy, int(gpx), int(gpy));
		int flag3 = line_up_down(point_x, point_y, tx, ty, int(gpx), int(gpy));



		for (i = x_min; i <= x_max; i++) {
			int  xx1, xx2, yy1, yy2;
			int flag = 0;
			for (j = y_min; j <= y_max; j++) {
				
				if (end - angle_begin <= 180) {
					if (circle_in_out(point_x, point_y, radius, i, j) <= 0 && line_up_down(point_x, point_y, sx, sy, i, j)*flag2 >= 0 && line_up_down(point_x, point_y, tx, ty, i, j)*flag3 >= 0) {
						if (flag == 0) {
							xx1 = i;
							yy1 = j;
							flag = 1;
						}
						if (j == y_max) {
							xx2 = i;
							yy2 = j;
							hdc_base_line(xx1, yy1, xx2, yy2);
						}
					}
					else if (flag == 1) {
						xx2 = i;
						yy2 = j;
						flag = 0;
						hdc_base_line(xx1, yy1, xx2, yy2);
					}
				}
				
				else if (end - angle_begin > 180) {
					if (circle_in_out(point_x, point_y, radius, i, j) <= 0 && !(line_up_down(point_x, point_y, sx, sy, i, j)*flag2 < 0 && line_up_down(point_x, point_y, tx, ty, i, j)*flag3 < 0)) {
						if (flag == 0) {
							xx1 = i;
							yy1 = j;
							flag = 1;
						}
						if (j == y_max) {
							xx2 = i;
							yy2 = j;
							hdc_base_line(xx1, yy1, xx2, yy2);
						}
					}
					else if (flag == 1) {
						xx2 = i;
						yy2 = j;
						flag = 0;
						hdc_base_line(xx1, yy1, xx2, yy2);
					}

				}
			}
		}

	}
	
	

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{

	double tn = thickness / 2.0;
	double angle;
	int x1, y1;
	double delta = double(0.25*thickness) / double(radius) * 180 / PI;

	for (angle = 0; angle <= 360; angle += delta) {
		/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
		x1 = point_x + (int)((radius)* sin(angle * PI / 180));
		y1 = point_y - (int)((radius)* cos(angle * PI / 180));
		hdc_point(x1, y1, thickness, RGB_value);
	}

	if (filled) {
		int i, j;
		int x_max = point_x + radius;
		int y_max = point_y + radius;
		int x_min = point_x - radius;
		int y_min = point_y - radius;

		for (i = x_min; i <= x_max; i++) {
			int  xx1, xx2, yy1, yy2;
			int flag = 0;
			for (j = y_min; j <= y_max; j++) {
				if (circle_in_out(point_x,point_y,radius,i,j)<=0) {
					if (flag == 0) {
						xx1 = i;
						yy1 = j;
						flag = 1;
					}
					if (j == y_max) {
						xx2 = i;
						yy2 = j;
						hdc_base_line(xx1, yy1, xx2, yy2);
					}
				}
				else if (flag == 1) {
					xx2 = i;
					yy2 = j;
					flag = 0;
					hdc_base_line(xx1, yy1, xx2, yy2);
				}
			}
		}

	}

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ����Բ
  ���������const int point_x			��Բ�ĵ�x
			const int point_y			��Բ�ĵ�y
			const int radius_a			��ƽ����X��İ뾶
			const int radius_b			��ƽ����Y��İ뾶
			const int rotation_angles	����Բ��Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			����ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	double tn = thickness / 2.0;
	double angle;
	double x0, y0;
	int x1, y1, x2, y2;
	int x1_max = 0, x1_min, y1_max, y1_min;

	double delta = double(0.33*thickness) / double(radius_a) * 180 / PI;

	x2 = point_x + int((radius_a) * cos(rotation_angles*PI / 180) - 0 * sin(rotation_angles*PI / 180));
	y2 = point_y + int((radius_a) * sin(rotation_angles*PI / 180) + 0 * cos(rotation_angles*PI / 180));
	x1_max = x2;
	x1_min = x2;
	y1_max = y2;
	y1_min = y2;

	for (angle = 0; angle <= 360; angle += delta) {
		x1_max = max(x1_max, x2);
		x1_min = min(x1_min, x2);
		y1_max = max(y1_max, y2);
		y1_min = min(y1_min, y2);

		/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
		x0 =  ((radius_a)* cos(angle * PI / 180));
		y0 =  ((radius_b)* sin(angle * PI / 180));
		x1 = point_x + int(x0 * cos(rotation_angles*PI / 180) - y0 * sin(rotation_angles*PI / 180));
		y1 = point_y + int(x0 * sin(rotation_angles*PI / 180) + y0 * cos(rotation_angles*PI / 180));
		
		hdc_point(x1, y1, thickness, RGB_value);
		x2 = x1;
		y2 = y1;
	}

	if (filled) {
		int i, j;
		int x_max = max(point_x, x1_max);
		int y_max = max(point_y, y1_max);
		int x_min = min(point_x, x1_min);
		int y_min = min(point_y, y1_min);

		for (i = x_min-1; i <= x_max+1; i++) {
			int  xx1, xx2, yy1, yy2;
			int flag = 0;
			for (j = y_min-1; j <= y_max+1; j++) {
				if (ellipse_in_out(point_x,point_y,radius_a,radius_b,rotation_angles,i,j) <= 16) {
					if (flag == 0) {
						xx1 = i;
						yy1 = j;
						flag = 1;
					}
					if (j == y_max) {
						xx2 = i;
						yy2 = j;
						hdc_base_line(xx1, yy1, xx2, yy2);
					}
				}
				else if (flag == 1) {
					xx2 = i;
					yy2 = j;
					flag = 0;
					hdc_base_line(xx1, yy1, xx2, yy2);
				}
			}
		}

	}


}


