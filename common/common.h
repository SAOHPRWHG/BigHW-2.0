/* 1751957 计2班 俞少作 */
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
	int separator = 1;//有无分隔符
	int rc_sign = 1;//行列标
	int value = 0;//球的值1-9
	int front_color = 16;//前景色，即球的颜色
	int bg_color = (value + 7) % 16;//背景色，默认为白色
	int status = 0;//球的状态
	/*status:
		0为普通球 NORMAL
		1为形成连子待消除状态 WILL_REMOVE
		2为新填充的球 NEW_FILL
		3为预测提示标记的球 PRE_REMIND*/
};
