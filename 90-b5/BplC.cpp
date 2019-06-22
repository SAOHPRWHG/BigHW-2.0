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
	gmw_set_color(&BplC_CGI, COLOR_BLACK, COLOR_HWHITE);			//整个窗口颜色//
	//gmw_set_font(&BplC_CGI, "新宋体", 32);		//字体
	gmw_set_font(&BplC_CGI, "新宋体", 20);							//字体
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

int BplC::Init_Airport()
{
	int i, j, k = 0;
	//←
	for (i = 0 + 2; i + 2 < ROW; i++) {
		for (j = 0 ; j + 3 < COL; j++) {
			Airport[k++].set(Point(i, j), Point(i, j + 3));
		}
	}
	//→
	for (i = 0 + 2; i + 2 < ROW; i++) {
		for (j = 0 + 3; j < COL; j++) {
			Airport[k++].set(Point(i, j), Point(i, j - 3));
		}
	}
	//↑
	for (i = 0; i + 3 < ROW; i++) {
		for (j = 0 + 2; j + 2 < COL; j++) {
			Airport[k++].set(Point(i, j), Point(i + 3, j));
		}
	}
	//↓
	for (i = 0 + 3; i < ROW; i++) {
		for (j = 0 + 2; j + 2 < COL; j++) {
			Airport[k++].set(Point(i, j), Point(i - 3, j));
		}
	}
	cout << "KKK:" << endl;
	return 0;
}

int BplC::Init_possible_map()
{
	int i, j;
	for (i = 0; i < ROW; i++) 
		for (j = 0; j < COL; j++) 
			possible_map[i][j] = 0;
	return 0;
}

int BplC::Update_possible(const string spack, const int sel, const char _row, const int col, const char _head_row, const int head_col, const char _tail_row, const int tail_col)
{
	const int row = _row - 'A';
	const int head_row = _head_row - 'A';
	const int tail_row = _tail_row - 'A';

	if (spack == "StartGame") {
		//将所有的possible置为0.001
		int k;
		for (k = 0; k < PLANE_NUMBER; k++) {
			Airport[k].possible = 0.001;
		}
	}
	else if (sel == 1) {
		//HitBody将对应飞机概率增加0.1
		if (spack == "HitBody") {
			int i, k;
			for (k = 0; k < PLANE_NUMBER; k++) {
				for (i = 0; i < PLANE_SIZE; i++) {
					if (Airport[k].possible > 0 && Airport[k].possible < 2 && Airport[k].plane[i].row == row && Airport[k].plane[i].col == col)
						Airport[k].possible += 0.1;
				}
			}
		}
		//HitFail将所有覆盖飞机置0
		else if (spack == "HitFail") {
			int i, k;
			for (k = 0; k < PLANE_NUMBER; k++) {
				for (i = 0; i < PLANE_SIZE; i++) {
					if (Airport[k].plane[i].row == row && Airport[k].plane[i].col == col)
						Airport[k].possible = 0;
				}
			}
		}
		//HitHead将对应飞机概率增加0.5，其余Body覆盖的飞机置0
		else if (spack == "HitHead") {
			int i, k;
			//头
			for (k = 0; k < PLANE_NUMBER; k++) {
				if (Airport[k].possible > 0 && Airport[k].possible < 1.99 && Airport[k].head.row == row && Airport[k].head.col == col) {
					//Airport[k].possible += 0.5;
					Airport[k].possible += 0.6;
					//break;
				}
			}
			//身体
			for (k = 0; k < PLANE_NUMBER; k++) {
				for (i = 1; i < PLANE_SIZE; i++) {//i从1开始，不含头，即身体坐标在此位置
					if (Airport[k].plane[i].row == row && Airport[k].plane[i].col == col)
						Airport[k].possible = 0;
				}
			}
		}
	}
	else if (sel == 2) {
		//HitPlane和GameOver将对应Plane的飞机possible置为2
		if (spack == "HitPlane" || spack == "GameOver") {
			int i, j, k;
			for (k = 0; k < PLANE_NUMBER; k++) {
				if (Airport[k].possible > 0 && Airport[k].possible < 1.99 && Airport[k].head.row == head_row && Airport[k].head.col == head_col && Airport[k].tail.row == tail_row && Airport[k].tail.col == tail_col){
					Airport[k].possible = 2;
					break;
				}
			}
			//把所有身体覆盖的飞机置零
			int K = k;
			int flag;
			for (k = 0; k < PLANE_NUMBER; k++) {
				flag = 0;
				if (k == K)
					continue;
				for (i = 0; i < PLANE_SIZE; i++) {
					for (j = 0; j < PLANE_SIZE; j++) {
						if ( Airport[k].plane[i].row == Airport[K].plane[j].row && Airport[k].plane[i].col == Airport[K].plane[j].col) {
							Airport[k].possible = 0;
							flag = 1;
							break;
						}
					}
					if (flag)
						break;
				}
			}
		}
		else if (spack == "HitFail") {
			int k;
			for (k = 0; k < PLANE_NUMBER; k++) {
				if (Airport[k].head.row == head_row && Airport[k].head.col == head_col && Airport[k].tail.row == tail_row && Airport[k].tail.col == tail_col)
					Airport[k].possible = 0;
			}
		}
	}
	return 0;
}

int BplC::Predict(char & row, int & col, char & head_row, int & head_col, char & tail_row, int & tail_col)
{
	int i, j, k;
	//清零概率地图
	Init_possible_map();

	//重新计算概率地图
	for (k = 0; k < PLANE_NUMBER; k++) {
		if (fabs(Airport[k].possible-2)<1e-5)//确定的飞机不计算在内
			continue;
		for (i = 0; i < PLANE_SIZE; i++) {
			if(map[Airport[k].plane[i].row][Airport[k].plane[i].col]==0)
				possible_map[Airport[k].plane[i].row][Airport[k].plane[i].col] += Airport[k].possible;
		}
	}
	//是否增加打头策略
	for (k = 0; k < PLANE_NUMBER; k++) {
		if (Airport[k].possible > 0.5 && Airport[k].possible < 1.999 && map[Airport[k].head.row][Airport[k].head.col] == 0) {
			row = Airport[k].head.row + 'A';
			col = Airport[k].head.col;
			return 1;//sel=1，直接打头
		}
	}

	//寻找超过0.9的项
	for (k = 0; k < PLANE_NUMBER; k++) {
		if (Airport[k].possible > 0.91 && Airport[k].possible < 1.999) {
			head_row = Airport[k].head.row + 'A';
			head_col = Airport[k].head.col;
			tail_row = Airport[k].tail.row + 'A';
			tail_col = Airport[k].tail.col;
			return 2;//sel=2，直接打飞机
		}
	}

	//否则计算地图上概率最大的点
	int max_row = 0, max_col = 0;
	double maxP = 0;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			if (possible_map[i][j] > maxP && map[i][j] == 0) {
				maxP = possible_map[i][j];
				max_row = i;
				max_col = j;//记录最大概率的位置信息
			}
		}
	}

	row = max_row + 'A';
	col = max_col;
	return 1;//sel=1,单点打击
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
				sprintf(temp, "[单点打击] %c行%d列", char('A' + mrow), mcol); //未考虑mrow超过26，mcol超过99的情况，如有需要，请自行处理
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

int BplC::Explode_animation(const char row, const int col, const char head_row, const int head_col, const char tail_row, const int tail_col, const int sel, string spack,const int show_explode, const int fast_mode)
{
	int r = row - 'A';//将行换成int型
	int hr = head_row - 'A';
	int tr = tail_row - 'A';
	/*单点轰炸*/  
	int k;
	if (sel == 1) {
		if (show_explode) {
			for (k = 0; k < 5; k++) {
				gmw_draw_block(&BplC_CGI, r, col, 1, bdi_exploded);
				Sleep(50);
				gmw_draw_block(&BplC_CGI, r, col, 0, bdi_normal); //0是空白
				Sleep(50);
			}
		}
		if (spack == "HitFail") {
			map[r][col] = HIT_FAIL;
			if(!fast_mode)
				gmw_draw_block(&BplC_CGI, r, col, map[r][col], bdi_normal);
		}
		else if (spack == "HitBody") {
			map[r][col] = HIT_BODY;
			if (!fast_mode)
				gmw_draw_block(&BplC_CGI, r, col, map[r][col], bdi_normal);
		}
		else if (spack == "HitHead") {
			map[r][col] = HIT_HEAD;
			if (!fast_mode)
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
			for (i = 0; i < PLANE_SIZE; i++) {
				if (i == 0)
					map[pl.plane[i].row][pl.plane[i].col] = HIT_HEAD;
				else
					map[pl.plane[i].row][pl.plane[i].col] = HIT_BODY;
				if (!fast_mode)
					gmw_draw_block(&BplC_CGI, pl.plane[i].row, pl.plane[i].col, map[pl.plane[i].row][pl.plane[i].col], bdi_normal);
			}
		}
	}
	return 0;
}

int BplC::print_map()
{
	setcolor(3, 15);
	cls();
	cout << "map:" << endl;
	int i, j;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++)
			cout << setw(4) << map[i][j];
		cout << endl;
	}
	return 0;
}

int BplC::print_possible_map()
{
	setcolor(3, 15);
	cls();
	cout << "possible_map:" << endl;
	int i, j;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++)
			cout << setw(8) << possible_map[i][j];
		cout << endl;
	}
	return 0;
}

int BplC::print_Airport()
{
	setcolor(3, 15);
	cls();
	int k;
	for (k = 0; k < PLANE_NUMBER; k++) {
		cout << "No：" << k << "  Head:(" << Airport[k].head.row << " ," << Airport[k].head.col<<")";
		cout<<"  Tail:("<< Airport[k].tail.row << " ," << Airport[k].tail.col<<")";
		cout << "  Possible: " << Airport[k].possible << endl;
	}

	return 0;
}
