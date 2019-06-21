#include "BplC.h"

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

int BplC::Init_map()
{
	int i, j;
	for (i = 0; i < ROW; i++) 
		for (j = 0; j < COL; j++) 
			map[i][j] = 0;
	return 0;
}

int BplC::Init_frame()
{
	/*初始化边框*/
		/*用缺省值初始化（窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略）*/
	gmw_init(&BplC_CGI);

	//	gmw_set_ext_rowcol(&ColorLinez_CGI, 3, 2, 10, 10);						//注：不设置附加区域，可能导致to_be_continued打印为两行，此问题未处理（后面同）
	gmw_set_color(&BplC_CGI, COLOR_BLACK, COLOR_HWHITE);			//整个窗口颜色
	gmw_set_font(&BplC_CGI, "新宋体", 32);							//字体
//	gmw_set_frame_style(&BplC_CGI);									//游戏主区域风格：每个色块宽2高1，有分隔线
	gmw_set_frame_default_linetype(&BplC_CGI, 2);					//游戏主区域线型：单线
	gmw_set_frame_color(&BplC_CGI, COLOR_HWHITE, COLOR_BLACK);		//游戏主区域颜色
//	gmw_set_block_border_switch(&BplC_CGI);							//每个游戏色块/彩球不需要边框
//	gmw_set_status_line_switch(&BplC_CGI, TOP_STATUS_LINE);				//需要上状态栏
//	gmw_set_status_line_switch(&BplC_CGI, LOWER_STATUS_LINE);			//需要下状态栏
	gmw_set_rowno_switch(&BplC_CGI, true);							//显示行号
	gmw_set_colno_switch(&BplC_CGI, true);							//显示列标
	gmw_set_delay(&BplC_CGI, DELAY_OF_BLOCK_MOVED, BLOCK_MOVED_DELAY_MS * 3);//加大延时
	/* 按row/col的值重设游戏主区域行列 */
	gmw_set_rowcol(&BplC_CGI, ROW, COL);

	cls();
	/* 显示框架 */
	gmw_draw_frame(&BplC_CGI);
	return 0;
}

/***************************************************************************
  函数名称：用鼠标选择打击坐标
  功    能：
  输入参数：
  返 回 值：1和2，1表示模式1,2表示模式2(整机击落)
  说    明：
***************************************************************************/
int BplC::Select_space_by_mouse(char & row, int & col, char & head_row, int & head_col, char & tail_row, int & tail_col)
{
	char temp[CHAR_LEN];
	int loop = 1;
	int maction, mrow, mcol;
	int keycode1, keycode2;
	int ret;

	while (loop) {
		ret = gmw_read_keyboard_and_mouse(&BplC_CGI, maction, mrow, mcol, keycode1, keycode2);
		
		if (ret == CCT_MOUSE_EVENT) {
			switch (maction) {
			case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:
				sprintf(temp, "[单点轰炸] %c行%d列", char('A' + mrow), mcol); //未考虑mrow超过26，mcol超过99的情况，如有需要，请自行处理
				gmw_status_line(&BplC_CGI, TOP_STATUS_LINE, temp);
				Sleep(500);
				row = char('A' + mrow);
				col = mcol;
				return 1;
				break;
			case MOUSE_RIGHT_BUTTON_CLICK:
				head_row = char('A' + mrow);
				head_col = mcol;
				maction = 0;
				//while ((ret = gmw_read_keyboard_and_mouse(&BplC_CGI, maction, mrow, mcol, keycode1, keycode2)) != CCT_MOUSE_EVENT || maction != MOUSE_RIGHT_BUTTON_CLICK)
					//;
				if ((ret = gmw_read_keyboard_and_mouse(&BplC_CGI, maction, mrow, mcol, keycode1, keycode2)) == CCT_MOUSE_EVENT && maction == MOUSE_RIGHT_BUTTON_CLICK)
				{
					tail_row = char('A' + mrow);
					tail_col = mcol;
					sprintf(temp, "[整机轰炸] 头:%c行%d列 - 尾:%c行%d列", head_row, head_col, tail_row, tail_col); //未考虑mrow超过26，mcol超过99的情况，如有需要，请自行处理
					gmw_status_line(&BplC_CGI, TOP_STATUS_LINE, temp);
					Sleep(500);
					return 2;
				}
				else
					continue;
				break;
			}

		}//end of while
	}
	return 0;
}

int BplC::Explode_animation(const char row, const int col, const char head_row, const int head_col, const char tail_row, const int tail_col, const int sel, string spack)
{
	int r = row - 'A';//将行换成int型
	int hr = head_row - 'A';
	int tr = tail_row - 'A';
	/*单点轰炸*/  
	int k;
	if (sel == 1) {
		for (k = 0; k < 5; k++) {
			gmw_draw_block(&BplC_CGI, r, col, 1, bdi_exploded);
			Sleep(50);
			gmw_draw_block(&BplC_CGI, r, col, 0, bdi_normal); //0是空白
			Sleep(50);
		}
		if (spack == "HitFail") {
			map[r][col] = HIT_FAIL;
			gmw_draw_block(&BplC_CGI, r, col, map[r][col], bdi_normal);
		}
		else if (spack == "HitBody") {
			map[r][col] = HIT_BODY;
			gmw_draw_block(&BplC_CGI, r, col, map[r][col], bdi_normal);
		}
		else if (spack == "HitHead") {
			map[r][col] = HIT_HEAD;
			gmw_draw_block(&BplC_CGI, r, col, map[r][col], bdi_normal);
		}
	}
	/*整机轰炸*/
	else if (sel == 2) {
		if (spack == "HitPlane"|| spack == "GameOver") {
			//map[hr][head_col] = HIT_HEAD;
			//gmw_draw_block(&BplC_CGI, r, col, map[hr][head_col], bdi_normal);
			Plane pl(hr, head_col, tr, tail_col);
			int i;
			for (i = 0; i < PLANE_SPACE; i++) {
				if (i == 0)
					map[pl.plane[i].row][pl.plane[i].col] = HIT_HEAD;
				else
					map[pl.plane[i].row][pl.plane[i].col] = HIT_BODY;

				gmw_draw_block(&BplC_CGI, pl.plane[i].row, pl.plane[i].col, map[pl.plane[i].row][pl.plane[i].col], bdi_normal);
			}
		}
	}

	return 0;
}

int BplC::print_map()
{
	cls();
	cout << "内部数组:" << endl;
	int i, j;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++)
			cout << setw(4) << map[i][j];
		cout << endl;
	}
	return 0;
}
