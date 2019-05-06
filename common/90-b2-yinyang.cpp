/* 1751957 ¼Æ2°à ÓáÉÙ×÷ */
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

void hdc_draw_yinyang(int point_x, int point_y, int r)
{
	hdc_circle(point_x, point_y, r, 1, 2, RGB(255, 255, 255));
	//Sleep(1000);

	hdc_sector(point_x, point_y, r, 180, 360, 1, 2, RGB(0, 0, 0));

	hdc_sector(point_x, point_y + r / 2, r / 2, 180, 360, 1, 2, RGB(255, 255, 255));

	hdc_sector(point_x, point_y - r / 2, r / 2, 0, 180, 1, 2, RGB(0, 0, 0));
	//Sleep(1000);

	hdc_circle(point_x, point_y - int(0.50*r), int(0.11*r), 1, 2, RGB(255, 255, 255));
	//Sleep(1000);
	hdc_circle(point_x, point_y + int(0.50*r), int(0.11*r), 1, 2, COLOR_BLACK);
}
