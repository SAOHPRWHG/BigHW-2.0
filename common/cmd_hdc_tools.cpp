/* 1751957 计2班 俞少作 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

/* 允许添加需要的头文件 */
#include <math.h>
#include "cmd_hdc_tools.h"
using namespace std;

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();

/* 已定义的静态全局变量 */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;

/* 允许适度使用静态全局变量，但仅限static，即外部不可见 */


/* 此处允许添加自定义函数，但仅限static，即外部不可见 */
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
  函数名称：
  功    能：返回将一组x,y代入直线x1y1x2y2方程后解的正负
  输入参数：
  返 回 值：1和-1,0(在直线上)
  说    明：注意x1,y1,x2,y2顺序不要反
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
  函数名称：
  功    能：判断点（x,y）在圆内还是圆外
  输入参数：
  返 回 值：flag<0在圆内，flag>0在圆外
  说    明：
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


/* 下面给出了几个基本函数的完整实现，不要改动 */
/***************************************************************************
  函数名称：
  功    能：初始化
  输入参数：const int bgcolor：背景色
            const int fgcolor：前景色
			const int width  ：屏幕宽度（点阵）
			const int high   ：屏幕高度（点阵）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* 先释放，防止不release而再次init（hdc_release可重入） */
	hdc_release();

	/* 窗口init后，用一个静态全局量记录，后续hdc_cls()会用到 */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	setcursor(CURSOR_INVISIBLE);
	setcolor(bgcolor, fgcolor);
	setfontsize("点阵字体", 16);
	setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16) , 120, 999); //将点阵的宽度及高度转换为特定字体的行列数，!!的含义：如果不是8/16的倍数，行列多+1
	cls();
}

/***************************************************************************
  函数名称：
  功    能：释放画图资源
  输入参数：
  返 回 值：
  说    明：可重入
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
  函数名称：
  功    能：设置画笔颜色，传入RGB值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB三色，值0-255
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  函数名称：
  功    能：清除屏幕上现有的图形
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_cls()
{
    /* 发现一定要换一种颜色初始化才能清除像素点，找到更简便方法的同学可以通知我 */
	hdc_init(_BgColor_, (_FgColor_ +1)%16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* 部分机器上运行demo时，会出现hdc_cls()后第一根线有缺线的情况，加延时即可
	   如果部分机器运行还有问题，调高此延时值 */
	Sleep(30);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
            const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x1：x1坐标，左上角为(0,0)
			const int y1：y1坐标，左上角为(0,0)
			const int x2：x2坐标，左上角为(0,0)
			const int y2：y2坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画一个指定粗细的点(用画实心圆来模拟)
  输入参数：const int x			：x坐标，左上角为(0,0)
            const int y			：y坐标，左上角为(0,0)
			const int thickness	：点的粗细，下限位1，上限不限(如过大会导致不完全填充)（有缺省值）
			const int RGB_value	：点的颜色（有缺省值）
  返 回 值：
  说    明：改进运行效率
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

	const int tn_end = (tn <= 1) ? 1 : tn/2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* 用画圆的方式模拟粗点 */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle ++) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* 放开此注释，可以看到一个粗点用了几个像素点组成 */
	gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	给出下列函数的实现，函数名及参数表不准改动
	1、不需要调用系统的其他有关图形操作的函数
	2、下列函数的实现都基于hdc_point函数的组合
	3、想一想，是不是所有函数都需要给出独立的实现过程？应该先实现哪些函数？
	4、填充是实现中最复杂的部分
	5、系统的角度与函数参数中的角度含义相差180°
   ------------------------------------------------------------------------- */
/***************************************************************************
  函数名称：
  功    能：画线段
  输入参数：const int x1		：起点的x
            const int y1		：起点的y
			const int x2		：终点的x
			const int y2		：终点的y
			const int thickness	：线段的粗细（有缺省值）1-5 缺省2
			const int RGB_value	：线段的颜色（有缺省值）0-255 
  返 回 值：
  说    明：
***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */
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
	/* 用画圆的方式模拟粗点 */
	for (level = - tn_end; level <= tn_end; level++) {	
		xx1 = x1 + (int)(level * delta_x);	
		/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
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
		/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
		if (yy2 != old_yy2) {
			old_yy2 = yy2;
			hdc_base_line(xx1, yy1, xx2, yy2);
			++count;
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：给出三点的坐标，画一个三角形
  输入参数：const int x1		：第1个点的x
            const int y1		：第1个点的y
			const int x2		：第2个点的x
			const int y2		：第2个点的y
			const int x3		：第3个点的x
			const int y3		：第3个点的y
			bool filled			：是否需要填充（有缺省值）
			const int thickness	：边的粗细（有缺省值）
			const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：不判断三点是否共线，如果共线，划出一条直线即可
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
  函数名称：
  功    能：给出左上角坐标及宽度、高度，画出一个长方形
  输入参数：const int left_up_x			：左上角x
            const int left_up_y			：左上角y
			const int width				：宽度
			const int high				：高度
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）
			bool filled					：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
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

	/*画线*/
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
  函数名称：
  功    能：给出左上角坐标及边长，画出一个长方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int length			：边长
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：
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

	/*画线*/
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
  函数名称：
  功    能：按照给出的参数画一段圆弧
  输入参数：const int point_x		：圆心的x
            const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
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
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x1 = point_x + (int)((radius ) * sin(angle * PI / 180));
			y1 = point_y - (int)((radius ) * cos(angle * PI / 180));			
			hdc_point(x1, y1, thickness, RGB_value);
		}

}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个扇形
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：当起始/结束角度差值为360的倍数时，不画两条边
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
		
		/* 注意，系统的坐标轴，0°在圆的最下方 */
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
  函数名称：
  功    能：按照给出的参数画一个圆
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{

	double tn = thickness / 2.0;
	double angle;
	int x1, y1;
	double delta = double(0.25*thickness) / double(radius) * 180 / PI;

	for (angle = 0; angle <= 360; angle += delta) {
		/* 注意，系统的坐标轴，0°在圆的最下方 */
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
  函数名称：
  功    能：按照给出的参数画一个椭圆
  输入参数：const int point_x			：圆心的x
			const int point_y			：圆心的y
			const int radius_a			：平行于X轴的半径
			const int radius_b			：平行于Y轴的半径
			const int rotation_angles	：以圆心为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
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

		/* 注意，系统的坐标轴，0°在圆的最下方 */
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


