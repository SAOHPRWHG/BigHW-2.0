/* 1751957 计2班 俞少作 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <math.h>
#include <time.h>
#include "../common/cmd_hdc_tools.h"
using namespace std;

static const double PI = 3.14159;
static HDC hdc = NULL;


void hdc_draw_Mathematical_curve(int point_x, int point_y, int radius_a,int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	double tn = thickness / 2.0;
	double angle;
	double r;
	int x1, y1, x2, y2;
	int x1_max = 0, x1_min, y1_max, y1_min;

	double delta = double(0.25*thickness) / double(radius_a) * 180 / PI;

	x2 = point_x;
	y2 = point_y;
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
		r = radius_a * (1 + sin((angle+rotation_angles)*PI / 180));
		x1 = point_x + int(r * cos(angle * PI / 180));
		y1 = point_y + int(r * sin(angle * PI / 180));

		hdc_point(x1, y1, thickness, RGB_value);
		x2 = x1;
		y2 = y1;
	}

	/*if (filled) {
		int i, j;
		int x_max = max(point_x, x1_max);
		int y_max = max(point_y, y1_max);
		int x_min = min(point_x, x1_min);
		int y_min = min(point_y, y1_min);

		for (i = x_min - 1; i <= x_max + 1; i++) {
			int  xx1, xx2, yy1, yy2;
			int flag = 0;
			for (j = y_min - 1; j <= y_max + 1; j++) {
				if (heart_in_out(point_x, point_y, radius_a, radius_b, rotation_angles, i, j) <= 16) {
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

	}*/




}

