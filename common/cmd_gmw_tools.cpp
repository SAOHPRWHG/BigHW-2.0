/* 1751957 计2班 俞少作 */
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
		此处可以给出需要的静态全局变量（尽可能少，最好没有）、静态全局只读变量/宏定义（个数不限）等
   -------------------------------------------------- */
const int CHAR_LEN = 256;

struct POS {
	int row;
	int col;
	int X;
	int Y;
};

/* --------------------------------------------------
		此处可以给出需要的内部辅助工具函数
		1、函数名不限，建议为 gmw_inner_* 
		2、个数不限
		3、必须是static函数，确保只在本源文件中使用
   -------------------------------------------------- */

static int gmw_inner_set_start_xy(CONSOLE_GRAPHICS_INFO *const pCGI);
static int gmw_inner_set_block_width_high_ext(CONSOLE_GRAPHICS_INFO *const pCGI);
static int gmw_inner_set_bwidth_bhigh(CONSOLE_GRAPHICS_INFO *const pCGI);
static int gmw_inner_set_main_width_high(CONSOLE_GRAPHICS_INFO *const pCGI);
static int gmw_inner_set_linescols(CONSOLE_GRAPHICS_INFO *const pCGI);
static int ax(int X, const CONSOLE_GRAPHICS_INFO *const pCGI);
static int ay(int Y, const CONSOLE_GRAPHICS_INFO *const pCGI);

/*内部隐藏参数设置函数群*/
static int gmw_inner_set_start_xy(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	//（上额外空间 + 上状态栏 + 列标显示
	pCGI->start_y = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no;
	//（左额外空间 + 行标显示 
	pCGI->start_x = pCGI->extern_left_cols + pCGI->draw_frame_with_row_no;

	//设置上下状态栏位置
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
  函数名称：
  功    能：计算色块额外宽高
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明： 每个色块的附加的宽度和高度(有分隔线是分别为2/1)，由separator计算而来 
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

	gmw_inner_set_bwidth_bhigh(pCGI);//更改ext必定更改bwidth、bhigh大小
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：计算色块宽高
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：1、修改其中任何一个变量都需要重新调用此函数更改设置
			包括：
			block_width
			block_high
			block_width_ext(separator)
			block_high_ext(separator)
***************************************************************************/
static int gmw_inner_set_bwidth_bhigh(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	pCGI->CFI.bwidth = pCGI->CFI.block_width + pCGI->CFI.block_width_ext;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + pCGI->CFI.block_high_ext;

	gmw_inner_set_main_width_high(pCGI);//更改bwidth、bhigh必定更改主区域大小
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：计算主区域宽高
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：1、修改其中任何一个变量都需要重新调用此函数更改设置
			包括：
			col_num
			row_num
			bwidth
			bhigh
***************************************************************************/
static int gmw_inner_set_main_width_high(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	if (pCGI->CFI.separator) {
		//在set_rowcol中设置：main_width = col*bwidth + 2(左边框)	+ 留一行
		pCGI->main_width = pCGI->col_num * pCGI->CFI.bwidth + 2 + 1;
		//main_width = row* bhigh+ 2 (上边框)
		pCGI->main_high = pCGI->row_num * pCGI->CFI.bhigh + 1;
	}
	else {
		//在set_rowcol中设置：main_width = col*bwidth + 2*2(左右边框) + 留一行	
		pCGI->main_width = pCGI->col_num * pCGI->CFI.bwidth + 2 * 2 + 1;
		//main_width = row* bhigh+ 2 (上下边框)
		pCGI->main_high = pCGI->row_num * pCGI->CFI.bhigh + 2;
	}

	gmw_inner_set_linescols(pCGI);//更改主区域必定更改窗口大小
	gmw_inner_set_start_xy(pCGI);//更改主区域必定更改上下状态栏位置
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：计算控制台窗口的行列数
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：1、修改其中任何一个变量都需要重新调用此函数更改设置
			包括：
			extern
			up_status_line
			lower_status_line
			draw_frame_with_row_no;
			draw_frame_with_col_no;
			main_widthhigh
***************************************************************************/
static int gmw_inner_set_linescols(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	//（\上额外空间 + \上状态栏 + \列标显示 + \主区域 + 下状态栏 + 下额外空间） + 4（1中文输入法提示行 + 3预留空行）
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->top_status_line + pCGI->lower_status_line + pCGI->draw_frame_with_col_no + pCGI->main_high + 4;
	//（\左额外空间 + \行标显示 + \主区域 + 右额外空间）
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_row_no + pCGI->main_width + 1;
	return 0;
}

/*辅助显示函数群*/
static int gmw_inner_check_line(char buf[])
{
	int i, count = 0;
	for (i = 0; i < int(strlen(buf)); i++) {
		if (buf[i] < 0)
			count++;
	}
	return !(count % 2);//若整除返回真，即通过检查
}

static int gmw_inner_draw_row_col_signal(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	if (pCGI->draw_frame_with_row_no) {
		int rsign_x = pCGI->start_x - 1;//行标x不变
		int i;
		for (i = 0; i < pCGI->row_num; i++) {
			if (i < 26) {
				showch(rsign_x, pCGI->start_y + (1 + pCGI->CFI.bhigh) / 2 + pCGI->CFI.bhigh*i, 'A' + i, pCGI->area_bgcolor, pCGI->area_fgcolor);
			}
			else if (i < 52) {
				showch(rsign_x, pCGI->start_y + (1 + pCGI->CFI.bhigh) / 2 + pCGI->CFI.bhigh*i, 'a' - 26  + i, pCGI->area_bgcolor, pCGI->area_fgcolor);
			}
			//else忘记大于52输出什么了
			else
				showch(rsign_x, pCGI->start_y + (1 + pCGI->CFI.bhigh) / 2 + pCGI->CFI.bhigh*i, '*', pCGI->area_bgcolor, pCGI->area_fgcolor);
		}
	}
	if (pCGI->draw_frame_with_col_no) {
		int csign_y = pCGI->start_y - 1;//列标y不变
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
  函数名称：
  功    能：计算色块显示坐标和控制台坐标的转换
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
          row: 显示色块在主框架中的行坐标
		  col: 显示色块在主框架中的列坐标
  返 回 值：
  说    明：
***************************************************************************/
static int gmw_inner_position_trans(const CONSOLE_GRAPHICS_INFO *const pCGI, POS *const pos)
{
	const int X = pCGI->start_x + 2;//去除框架
	const int Y = pCGI->start_y + 1;
	pos->X = X + pos->col * pCGI->CFI.bwidth;
	pos->Y = Y + pos->row * pCGI->CFI.bhigh;
	return 0;
}


/* ----------------------------------------------- 
		实现下面给出的函数（函数声明不准动）
   ----------------------------------------------- */
/***************************************************************************
  函数名称：
  功    能：设置游戏主框架的行列数
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int row						：行数(错误则为0，不设上限，人为保证正确性)
			const int col						：列数(错误则为0，不设上限，人为保证正确性)
  返 回 值：
  说    明：1、指消除类游戏的矩形区域的行列值
            2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	pCGI->col_num = col;
	pCGI->row_num = row;
	gmw_inner_set_main_width_high(pCGI);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置整个窗口（含游戏区、附加区在内的整个cmd窗口）的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int bg_color					：前景色（缺省COLOR_BLACK）
		   const int fg_color					：背景色（缺省COLOR_WHITE）
		   const bool cascade					：是否级联（缺省为true-级联）
  返 回 值：
  说    明：1、cascade = true时
				同步修改游戏主区域的颜色
				同步修改上下状态栏的正常文本的背景色和前景色，醒目文本的背景色（前景色不变）
			2、不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15
				    前景色背景色的值一致导致无法看到内容
					前景色正好是状态栏醒目前景色，导致无法看到醒目提示
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
	
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置窗口的字体
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *fontname					：字体名称（只能是"Terminal"和"新宋体"两种，错误则返回-1，不改变字体）
		   const int fs_high					：字体高度（缺省及错误为16，不设其它限制，人为保证）
		   const int fs_width					：字体高度（缺省及错误为8，不设其它限制，人为保证）
  返 回 值：
  说    明：1、与cmd_console_tools中的setfontsize相似，目前只支持“点阵字体”和“新宋体”
            2、若设置其它字体则直接返回，保持原字体设置不变
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	pCGI->CFT.font_size_high = fs_high;
	pCGI->CFT.font_size_width = fs_width;
	strcpy(pCGI->CFT.font_type, fontname);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置延时
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type						：延时的类型（目前为3种）
		   const int delay_ms					：以ms为单位的延时
			   画边框的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   画色块的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   色块移动的延时：BLOCK_MOVED_DELAY_MS ~ 不设上限，人为保证正确（ <BLOCK_MOVED_DELAY_MS 则置 BLOCK_MOVED_DELAY_MS）
  返 回 值：
  说    明：
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  输入参数：设置游戏主框架结构之外需要保留的额外区域
  功    能：CONSOLE_GRAPHICS_INFO *const pCGI	：
		   const int up_lines					：上部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int down_lines				：下部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int left_cols					：左边额外的列（缺省及错误为0，不设上限，人为保证）
		   const int right_cols				：右边额外的列（缺省及错误为0，不设上限，人为保证）
  返 回 值：
  说    明：额外行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
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
	return 0; //此句可根据需要修改

}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	switch (type)
	{
		case 1:
			strcpy(pCGI->CFI.top_left, "╔");
			strcpy(pCGI->CFI.lower_left, "╚");
			strcpy(pCGI->CFI.top_right, "╗");
			strcpy(pCGI->CFI.lower_right, "╝");
			strcpy(pCGI->CFI.h_normal, "═");
			strcpy(pCGI->CFI.v_normal, "║");
			strcpy(pCGI->CFI.h_top_separator, "╦");
			strcpy(pCGI->CFI.h_lower_separator, "╩");
			strcpy(pCGI->CFI.v_left_separator, "╠");
			strcpy(pCGI->CFI.v_right_separator, "╣");
			strcpy(pCGI->CFI.mid_separator, "╬");
			break;
		case 2:
			strcpy(pCGI->CFI.top_left, "┏");
			strcpy(pCGI->CFI.lower_left, "┗");
			strcpy(pCGI->CFI.top_right, "┓");
			strcpy(pCGI->CFI.lower_right, "┛");
			strcpy(pCGI->CFI.h_normal, "━");
			strcpy(pCGI->CFI.v_normal, "┃");
			strcpy(pCGI->CFI.h_top_separator, "┳");
			strcpy(pCGI->CFI.h_lower_separator, "┻");
			strcpy(pCGI->CFI.v_left_separator, "┣");
			strcpy(pCGI->CFI.v_right_separator, "┫");
			strcpy(pCGI->CFI.mid_separator, "╋");
			break;
		case 3:
			strcpy(pCGI->CFI.top_left, "╒");
			strcpy(pCGI->CFI.lower_left, "╘");
			strcpy(pCGI->CFI.top_right, "╕");
			strcpy(pCGI->CFI.lower_right, "╛");
			strcpy(pCGI->CFI.h_normal, "═");
			strcpy(pCGI->CFI.v_normal, "│");
			strcpy(pCGI->CFI.h_top_separator, "╤");
			strcpy(pCGI->CFI.h_lower_separator, "╧");
			strcpy(pCGI->CFI.v_left_separator, "╞");
			strcpy(pCGI->CFI.v_right_separator, "╡");
			strcpy(pCGI->CFI.mid_separator, "╪");
			break;
		case 4:
			//╓╖╙╜╟╢║╫─╥╨╫
			strcpy(pCGI->CFI.top_left, "╓");
			strcpy(pCGI->CFI.lower_left, "╙");
			strcpy(pCGI->CFI.top_right, "╖");
			strcpy(pCGI->CFI.lower_right, "╜");
			strcpy(pCGI->CFI.h_normal, "─");
			strcpy(pCGI->CFI.v_normal, "║");
			strcpy(pCGI->CFI.h_top_separator, "╥");
			strcpy(pCGI->CFI.h_lower_separator, "╨");
			strcpy(pCGI->CFI.v_left_separator, "╟");
			strcpy(pCGI->CFI.v_right_separator, "╢");
			strcpy(pCGI->CFI.mid_separator, "╫");
			break;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const char *...						：共11种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
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

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的色块数量大小、是否需要分隔线等
  输入参数：输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int block_width						：宽度（错误及缺省2，因为约定表格线为中文制表符，如果给出奇数，要+1）
			const int block_high						：高度（错误及缺省1）
			const bool separator						：是否需要分隔线（缺省为true，需要分隔线）
  返 回 值：
  说    明：框架大小/是否需要分隔线等的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	pCGI->CFI.block_width = block_width;
	pCGI->CFI.block_high = block_high;
	pCGI->CFI.separator = separator;

	gmw_inner_set_block_width_high_ext(pCGI);//更改分隔符会导致改变ext
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int bg_color					：背景色（缺省 -1表示用窗口背景色）
			const int fg_color					：前景色（缺省 -1表示用窗口前景色）
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	switch (type)
	{
		case 1:
			strcpy(pCGI->CBI.top_left, "╔");
			strcpy(pCGI->CBI.lower_left, "╚");
			strcpy(pCGI->CBI.top_right, "╗");
			strcpy(pCGI->CBI.lower_right, "╝");
			strcpy(pCGI->CBI.h_normal, "═");
			strcpy(pCGI->CBI.v_normal, "║");
			break;
		case 2:
			strcpy(pCGI->CBI.top_left, "┏");
			strcpy(pCGI->CBI.lower_left, "┗");
			strcpy(pCGI->CBI.top_right, "┓");
			strcpy(pCGI->CBI.lower_right, "┛");
			strcpy(pCGI->CBI.h_normal, "━");
			strcpy(pCGI->CBI.v_normal, "┃");
			break;
		case 3:
			strcpy(pCGI->CBI.top_left, "╒");
			strcpy(pCGI->CBI.lower_left, "╘");
			strcpy(pCGI->CBI.top_right, "╕");
			strcpy(pCGI->CBI.lower_right, "╛");
			strcpy(pCGI->CBI.h_normal, "═");
			strcpy(pCGI->CBI.v_normal, "│");
			break;
		case 4:
		default:
			//╓╖╙╜╟╢║╫─╥╨╫
			strcpy(pCGI->CBI.top_left, "╓");
			strcpy(pCGI->CBI.lower_left, "╙");
			strcpy(pCGI->CBI.top_right, "╖");
			strcpy(pCGI->CBI.lower_right, "╜");
			strcpy(pCGI->CBI.h_normal, "─");
			strcpy(pCGI->CBI.v_normal, "║");
			break;
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *...					：共6种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置每个游戏色块(彩球)是否需要小边框
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const bool on_off：true - 需要 flase - 不需要（缺省false）
  返 回 值：
  说    明：边框约定为中文制表符，双线
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->CBI.block_border = on_off;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示上下状态栏
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const bool on_off					：true - 需要 flase - 不需要（缺省true）
  返 回 值：
  说    明：1、状态栏的相关约定如下：
			   1.1、上状态栏只能一行，在主区域最上方框线/列标的上面，为主区域的最开始一行（主区域的左上角坐标就是上状态栏的坐标）
			   1.2、下状态栏只能一行，在主区域最下方框线的下面
			   1.3、状态栏的宽度为主区域宽度，如果信息过长则截断
		   2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置上下状态栏的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const int normal_bgcolor			：正常文本背景色（缺省 -1表示使用窗口背景色）
			const int normal_fgcolor			：正常文本前景色（缺省 -1表示使用窗口前景色）
			const int catchy_bgcolor			：醒目文本背景色（缺省 -1表示使用窗口背景色）
			const int catchy_fgcolor			：醒目文本前景色（缺省 -1表示使用亮黄色）
  输入参数：
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示行号
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、行号约定为字母A开始连续排列（如果超过26，则从a开始，超过52的统一为*，实际应用不可能）
            2、是否显示行号的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	if (on_off) 
		pCGI->draw_frame_with_row_no = true;
	else
		pCGI->draw_frame_with_row_no = false;

	gmw_inner_set_start_xy(pCGI);
	gmw_inner_set_linescols(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示列标
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、列标约定为数字0开始连续排列（数字0-99，超过99统一为**，实际应用不可能）
            2、是否显示列标的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	if (on_off)
		pCGI->draw_frame_with_col_no = true;
	else
		pCGI->draw_frame_with_col_no = false;

	gmw_inner_set_start_xy(pCGI);
	gmw_inner_set_linescols(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：打印 CONSOLE_GRAPHICS_INFO 结构体中的各成员值
  输入参数：
  返 回 值：
  说    明：1、仅供调试用，打印格式自定义
            2、本函数测试用例中未调用过，可以不实现
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：将 CONSOLE_GRAPHICS_INFO 结构体用缺省值进行初始化
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI：整体结构指针
		   const int row					：游戏区域色块行数（缺省10）
		   const int col					：游戏区域色块列数（缺省10）
		   const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
		   const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
  返 回 值：
  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* 先做初始化，这样后续可以在此基础上任意修改部分设置，其余采用预设值
		游戏区域（主框架）为10*10，带分隔线，每个色块的大小为2列*1行，色块不带边框
		窗口背景色黑，前景色白
		字体为点阵，8*16
		画边框及色块时无延时，色块移动时延时3ms
		上下左右辅助区域全部为0
		上状态栏开，正常文本颜色同窗口，醒目文本背景同窗口，前景为亮黄
		下状态栏开，正常文本颜色同窗口，醒目文本背景同窗口，前景为亮黄
		不显示行号
		不显示列标
		游戏区域为双线框，带分隔线，色块大小为2（宽度2列=1个汉字）*1（高度1行），颜色同窗口
		色块为双框线，颜色（未完）		*/
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

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：画主游戏框架
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：具体可参考demo的效果
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/*重置窗口工作*/
	setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	setconsoleborder(pCGI->cols, pCGI->lines,pCGI->cols, pCGI->lines);

	const int X = pCGI->start_x;
	const int Y = pCGI->start_y;
	const int bg = pCGI->CFI.fgcolor;//下面都写反了，这里反过来
	const int fg = pCGI->CFI.bgcolor;
	int i, j, k, l ;

	//打印行列标
	gmw_inner_draw_row_col_signal(pCGI);

	/*有分隔符*/
	if (pCGI->CFI.separator) {
		//开始画框
		//the first line
		showstr(X, Y, pCGI->CFI.top_left, fg, bg);
		for (i = 0; i < pCGI->col_num; i++) {
			showstr(X + i * pCGI->CFI.bwidth + CFI_LEN - 1, Y, pCGI->CFI.h_normal, fg, bg, pCGI->CFI.block_width/2);
			showstr(X + (i + 1)*pCGI->CFI.bwidth, Y, pCGI->CFI.h_top_separator, fg, bg);
		}
		showstr(X + (i)*pCGI->CFI.bwidth, Y, pCGI->CFI.top_right, fg, bg);

		//中间
		for (i = 0; i < pCGI->row_num; i++) {
			for (j = 1; j <= pCGI->CFI.block_high; j++) {
				for (k = 0; k <= pCGI->col_num; k++) {
					showstr(X + k * pCGI->CFI.bwidth, Y + i * (pCGI->CFI.bhigh) + j, pCGI->CFI.v_normal, fg, bg);
					if(k!= pCGI->col_num)
						showstr(X + k * pCGI->CFI.bwidth + CFI_LEN-1, Y + i * (pCGI->CFI.bhigh) + j, " ", fg, bg, pCGI->CFI.block_width);
				}
				Sleep(pCGI->delay_of_draw_frame);//每行之间加间隔
			}
			//单独画分隔行
			showstr(X, Y + (i + 1) * pCGI->CFI.bhigh, pCGI->CFI.v_left_separator, fg, bg);
			for (l = 0; l < pCGI->col_num; l++) {
				showstr( X + l * pCGI->CFI.bwidth + CFI_LEN - 1, Y + (i + 1) * pCGI->CFI.bhigh, pCGI->CFI.h_normal, fg, bg, pCGI->CFI.block_width / 2);
				showstr(  X + (l + 1) * pCGI->CFI.bwidth, Y + (i + 1) * pCGI->CFI.bhigh, pCGI->CFI.mid_separator, fg, bg);
			}
			showstr( X + (l) * pCGI->CFI.bwidth,Y + (i + 1) * pCGI->CFI.bhigh, pCGI->CFI.v_right_separator, fg, bg);//把最后一个"┼"盖掉
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
	
	/*无分隔符*/
	else {
		//the first line
		showstr(X, Y, pCGI->CFI.top_left, fg, bg);
		for (i = 0; i < pCGI->col_num; i++) {
			showstr(X + i * pCGI->CFI.bwidth + CFI_LEN - 1, Y, pCGI->CFI.h_normal, fg, bg, pCGI->CFI.block_width / 2);
			//showstr(X + (i + 1)*pCGI->CFI.bwidth, Y, pCGI->CFI.h_top_separator, fg, bg);
		}
		showstr(X + (i)*pCGI->CFI.bwidth + CFI_LEN - 1, Y, pCGI->CFI.top_right, fg, bg);
		//对于无边框来说bwidth没有额外宽度，必须+2（左，上边框）


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

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：在状态栏上显示信息
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type							：指定是上/下状态栏
		   const char *msg						：正常信息
		   const char *catchy_msg					：需要特别标注的信息（在正常信息前显示）
  返 回 值：
  说    明：1、输出宽度限定为主框架的宽度（含行号列标位置），超出则截去
            2、如果最后一个字符是某汉字的前半个，会导致后面乱码，要处理
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

			//检查显示是否超过宽度
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

			//检查显示是否超过宽度
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

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
            2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	Sleep(pCGI->delay_of_draw_block);
	int bg, fg;

	//先找到转换坐标
	POS pos = { row_no, col_no, 0, 0 };
	gmw_inner_position_trans(pCGI, &pos);

	//寻找要画哪种类型的色块：
	int i;
	for (i = 0; bdi[i].value != bdi_value; i++) {
		if (bdi[i].value == BDI_VALUE_END) 
			return -1;//返回-1说明没有打印
	}

	//处理-1默认值
	if (bdi[i].bgcolor == -1)
		bg = pCGI->CFI.bgcolor;
	else
		bg = bdi[i].bgcolor;

	if (bdi[i].fgcolor == -1)
		fg = pCGI->CFI.fgcolor;
	else
		fg = bdi[i].fgcolor;


	//开始画色块：
	if (bdi[i].content == NULL && bdi[i].value!=BDI_VALUE_BLANK) {//空块不画框
		//打印色块
		int j;
		for (j = 0; j < pCGI->CFI.block_high; j++)
			showstr(pos.X, pos.Y + j, " ", bg, fg, pCGI->CFI.block_width, pCGI->CFI.block_width);

		//打印色块小边框
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

		//把value转化为char*型
		char temp[CHAR_LEN];
		sprintf(temp, "%d", bdi[i].value);

		//显示value字符
		if (pCGI->CFI.block_high % 2)
			showstr(pos.X + pCGI->CFI.block_width / 2 - strlen(temp) / 2 - 1, pos.Y + pCGI->CFI.block_high / 2, temp, bg, fg);
		else
			showstr(pos.X + pCGI->CFI.block_width / 2 - strlen(temp) / 2 - 1, pos.Y + pCGI->CFI.block_high / 2 + 1, temp, bg, fg);
		return 0;
	}

	int j;
	for (j = 0; j < pCGI->CFI.block_high; j++)
		showstr(pos.X, pos.Y + j, " ", bg, fg, pCGI->CFI.block_width, pCGI->CFI.block_width);
	
	//打印色块小边框
	//宽高不足2，或打印空色块的时候直接跳过
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

	//显示content字符
	if (bdi[i].value != BDI_VALUE_BLANK) {
		if (pCGI->CFI.block_high % 2)
			showstr(pos.X + pCGI->CFI.block_width / 2 - strlen(bdi[i].content) / 2, pos.Y + pCGI->CFI.block_high / 2, bdi[i].content, bg, fg);
		else
			showstr(pos.X + pCGI->CFI.block_width / 2 - strlen(bdi[i].content) / 2, pos.Y + pCGI->CFI.block_high / 2 + 1, bdi[i].content, bg, fg);

	}

	return 0; //此句可根据需要修改
}
/***************************************************************************
  函数名称：
  功    能：重载显示某一个色块(内容为字符串，坐标为X/Y)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int X					：控制台X（从0开始，人为保证正确性，程序不检查）
		   const int Y					：控制台Y（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
			2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
int gmw_inner_draw_block(const int X, const int Y, const CONSOLE_GRAPHICS_INFO *const pCGI, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	//Sleep(pCGI->delay_of_draw_block);
	int bg, fg;

	//寻找要画哪种类型的色块：
	int i;
	for (i = 0; bdi[i].value != bdi_value; i++) {
		if (bdi[i].value == BDI_VALUE_END)
			return -1;//返回-1说明没有打印
	}

	//处理-1默认值
	if (bdi[i].bgcolor == -1)
		bg = pCGI->CFI.bgcolor;
	else
		bg = bdi[i].bgcolor;

	if (bdi[i].fgcolor == -1)
		fg = pCGI->CFI.fgcolor;
	else
		fg = bdi[i].fgcolor;


	//开始画色块：
	if (bdi[i].content == NULL && bdi[i].value != BDI_VALUE_BLANK) {
		//打印色块
		int j;
		for (j = 0; j < pCGI->CFI.block_high; j++)
			showstr(X, Y + j, " ", bg, fg, pCGI->CFI.block_width, pCGI->CFI.block_width);

		//打印色块小边框
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

		//把value转化为char*型
		char temp[CHAR_LEN];
		sprintf(temp, "%d", bdi[i].value);

		//显示value字符
		if (pCGI->CFI.block_high % 2)
			showstr( X + pCGI->CFI.block_width / 2 - strlen(temp) / 2 - 1,  Y + pCGI->CFI.block_high / 2, temp, bg, fg);
		else
			showstr( X + pCGI->CFI.block_width / 2 - strlen(temp) / 2 - 1,  Y + pCGI->CFI.block_high / 2 + 1, temp, bg, fg);
		return 0;
	}

	int j;
	for (j = 0; j < pCGI->CFI.block_high; j++)
		showstr( X,  Y + j, " ", bg, fg, pCGI->CFI.block_width, pCGI->CFI.block_width);

	//打印色块小边框
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

	//显示content字符
	if (bdi[i].value != BDI_VALUE_BLANK) {
		if (pCGI->CFI.block_high % 2)
			showstr(X + pCGI->CFI.block_width / 2 - strlen(bdi[i].content) / 2, Y + pCGI->CFI.block_high / 2, bdi[i].content, bg, fg);
		else
			showstr(X + pCGI->CFI.block_width / 2 - strlen(bdi[i].content) / 2, Y + pCGI->CFI.block_high / 2 + 1, bdi[i].content, bg, fg);
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：移动某一个色块
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值（一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
		   const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
		   const int direction						：移动方向，一共四种，具体见cmd_gmw_tools.h
		   const int distance						：移动距离（从1开始，人为保证正确性，程序不检查）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
	//先找到转换坐标
	POS pos = { row_no, col_no, 0, 0 };
	gmw_inner_position_trans(pCGI, &pos);

	int X = pos.X;
	int Y = pos.Y;

	int i, j, k;
	for (k = 0; k < distance; k++) {
		switch (direction)
		{
		case DOWN_TO_UP://向上
			for (i = 0; i < pCGI->CFI.bhigh; i++) {
				Sleep(pCGI->delay_of_block_moved);
				gmw_inner_draw_block(X, Y - i, pCGI, blank_bdi_value, bdi);
				gmw_inner_draw_block(X, Y - i - 1, pCGI, bdi_value, bdi);
			}
			if (pCGI->CFI.separator) {
				showstr(X, Y - 1, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor, pCGI->CFI.block_width / 2);
			}
			//更新位置坐标
			pos.row--;
			gmw_inner_position_trans(pCGI, &pos);
			X = pos.X;
			Y = pos.Y;
			break;
		case UP_TO_DOWN://向下
			for (i = 0; i < pCGI->CFI.bhigh; i++) {
				Sleep(pCGI->delay_of_block_moved);
				gmw_inner_draw_block(X, Y + i, pCGI, blank_bdi_value, bdi);
				gmw_inner_draw_block(X, Y + i + 1, pCGI, bdi_value, bdi);
			}
			if (pCGI->CFI.separator) {
				showstr(X, Y + pCGI->CFI.block_high, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor, pCGI->CFI.block_width / 2);
			}
			//更新位置坐标
			pos.row++;
			gmw_inner_position_trans(pCGI, &pos);
			X = pos.X;
			Y = pos.Y;
			break;
		case RIGHT_TO_LEFT://向左
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
			//更新位置坐标
			pos.col--;
			gmw_inner_position_trans(pCGI, &pos);
			X = pos.X;
			Y = pos.Y;
			break;
		case LEFT_TO_RIGHT://向右
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
			//更新位置坐标
			pos.col++;
			gmw_inner_position_trans(pCGI, &pos);
			X = pos.X;
			Y = pos.Y;
			break;
		}
	}
	
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：读键盘或鼠标
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   int &MAction							：如果返回 CCT_MOUSE_EVENT，则此值有效，为 MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK 两者之一
		                                               如果返回 CCT_KEYBOARD_EVENT，则此值无效
		   int &MRow								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的行号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &MCol								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的列号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode1							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第一个）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode2							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第二个，如果是单键码，则为0）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   const bool update_lower_status_line		：鼠标移动时，是否要在本函数中显示"[当前光标] *行*列"的信息（true=显示，false=不显示，缺省为true）
  返 回 值：函数返回约定
            1、如果是鼠标移动，不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] *行*列"
		   2、如果是按下鼠标左键，且当前鼠标指针停留在主游戏区域的*行*列上，则返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow 为行号，MCol 为列标
		                          且当前鼠标指针停留在非法区域（非游戏区域，游戏区域中的分隔线），则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
		   3、如果是按下鼠标右键，则不判断鼠标指针停留区域是否合法，直接返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_RIGHT_BUTTON_CLICK, MRow、MCol不可信
		   4、如果按下键盘上的某键（含双键码按键），则直接返回 CCT_KEYBOARD_EVENT，KeyCode1/KeyCode2中为对应的键码值
 说    明：通过调用 cmd_console_tools.cpp 中的 read_keyboard_and_mouse 函数实现
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	int X = 0, Y = 0;
	int ret ;
	int loop = 1;

	enable_mouse();

	/* 打印初始光标位置[0,0] */
	setcursor(CURSOR_INVISIBLE);	//关闭光标
	//		cout << "[当前鼠标位置] X:0  Y:0"; //打印初始鼠标位置

	while (loop) {
		/* 读鼠标/键盘，返回值为下述操作中的某一种, 当前鼠标位置在<X,Y>处 */
		ret = read_keyboard_and_mouse(X, Y, MAction, KeyCode1, KeyCode2);

		if (ret == CCT_MOUSE_EVENT) {
			/* 在下状态栏打印 *///先sprintf进去，再显示

			MRow = ay(Y, pCGI);
			MCol = ax(X, pCGI);
			if (MCol == -1 || MRow == -1) {
				gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前鼠标位置] 位置非法");
			}
			else {
				if (update_lower_status_line) {
					char buf[CHAR_LEN];
					sprintf(buf, "[当前鼠标位置] %c行 %d列", MRow + 'A', MCol);
					gmw_status_line(pCGI, LOWER_STATUS_LINE, buf);
				}

				switch (MAction) {
				case MOUSE_ONLY_MOVED:
					//gmw_status_line(pCGI, LOWER_STATUS_LINE, "移动          ");
					//showch(X, Y, '*');不打印任何内容
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					//gmw_status_line(pCGI, LOWER_STATUS_LINE, "按下左键      ");
					//cout << "按下左键      " << endl;
					//showch(X, Y, '1');			//在鼠标指针位置显示1
					return CCT_MOUSE_EVENT;
					break;
				case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:	//双击左键
					//cout << "双击左键      " << endl;
					//showch(X, Y, '2');			//在鼠标指针位置显示2
					return CCT_MOUSE_EVENT;
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
					//cout << "按下右键      " << endl;
					//showch(X, Y, '3');			//在鼠标指针位置显示3
					return CCT_MOUSE_EVENT;
					break;
				case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//双击右键
					//cout << "双击右键      " << endl;
					//showch(X, Y, '4');			//在鼠标指针位置显示4
					//loop = 0;
					break;
				case MOUSE_LEFTRIGHT_BUTTON_CLICK:		//同时按下左右键
					//cout << "同时按下左右键" << endl;
					//showch(X, Y, '5');			//在鼠标指针位置显示5
					break;
				case MOUSE_WHEEL_CLICK:
					//cout << "按下滚轮      " << endl;
					break;
				case MOUSE_WHEEL_MOVED_UP:
					//cout << "滚轮向上移动  " << endl;
					break;
				case MOUSE_WHEEL_MOVED_DOWN:
					//cout << "滚轮向下移动  " << endl;
					break;
				case MOUSE_NO_ACTION:
				default:
					//cout << "其它操作" << endl;
					//showch(X, Y, '0');			//在鼠标指针位置显示0
					break;
				} //end of switch(MAction)

			}

		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT)
			return CCT_KEYBOARD_EVENT;
	} //end of while(1)

	disable_mouse();	//禁用鼠标
	setcursor(CURSOR_VISIBLE_NORMAL);	//打开光标
	return 0; //此句可根据需要修改
}

static int ax(int X, const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	const int X0 = pCGI->start_x + 2;
	int flag, col;
	flag = (X - X0) % pCGI->CFI.bwidth;

	//余数介于block_width和bwidth之间说明在边框上
	if (flag < pCGI->CFI.block_width) {
		col = (X - X0) / pCGI->CFI.bwidth;
		if (col >= 0 && col < pCGI->col_num)
			return col;
		else
			return -1;
	}
	else 
		return -1;//-1表示停留区域非法
}

static int ay(int Y, const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	const int Y0 = pCGI->start_y + 1;
	int flag, row;
	flag = (Y - Y0) % pCGI->CFI.bhigh;

	//余数介于block_width和bwidth之间说明在边框上
	if (flag < pCGI->CFI.block_high) {
		row = (Y - Y0) / pCGI->CFI.bhigh;
		if (row >= 0 && row < pCGI->row_num)
			return row;
		else
			return -1;
	}
	else
		return -1;//-1表示停留区域非法
}
