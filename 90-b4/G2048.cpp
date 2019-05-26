#include "G2048.h"

int G2048::up_check(int c)
{
	int p1 = 0, p2 = 0, p3 = 0;//p1为目标位置，p2为原位置
	int tem;
	for (; 1; p1++) {
		if (map[p1][c].value!=0) {
			p2 = p1;
		}
		else {
			for (p2 = p1; p2 < row && map[p2][c].value == 0; p2++)
				;
		}

		if (p1 == row || p2 == row)
			break;//说明到底了，否则说明找到
		//如果找到则交换值
		tem = map[p1][c].value;
		map[p1][c].value = map[p2][c].value;
		map[p2][c].value = tem;
		map[p2][c].dcol = c;
		map[p2][c].drow = p1;

		//再向后找一个
		for (p3 = p2 + 1; p3 < row && map[p3][c].value == 0; p3++)
			;
		if (p3 == row)
			break;//说明到底了，否则说明找到
		//上下两个值相等
		if (map[p3][c].value == map[p1][c].value) {
			map[p1][c].value *= 2;
			score += map[p1][c].value;//计分
			map[p3][c].value = 0;
			map[p3][c].dcol = c;
			map[p3][c].drow = p1;

			map[p2][c].status = WAIT_COMBINE;
			map[p3][c].status = COMBINE;
		}

	}
	return 0;
}

int G2048::down_check(int c)
{
	//up换down 1. ++->-- 2.p1 = row - 1; p1 - 1 >= 0; p1--
	int p1 = row - 1, p2 = row - 1, p3 = row - 1;//p1为目标位置，p2为原位置
	int tem;
	for (; 1; p1--) {
		if (map[p1][c].value != 0) {
			p2 = p1;
		}
		else
			for (p2 = p1; p2 >= 0 && map[p2][c].value == 0; p2--)
				;
		if (p1 < 0 || p2 < 0)
			break;//说明到底了，否则说明找到
		//如果找到则交换值
		tem = map[p1][c].value;
		map[p1][c].value = map[p2][c].value;
		map[p2][c].value = tem;
		map[p2][c].dcol = c;
		map[p2][c].drow = p1;

		//再向后找一个
		for (p3 = p2 - 1; p3 >= 0 && map[p3][c].value == 0; p3--)
			;
		if (p3 < 0)
			break;//说明到底了，否则说明找到
		//上下两个值相等
		if (map[p3][c].value == map[p1][c].value) {
			map[p1][c].value *= 2;
			score += map[p1][c].value;//计分
			map[p3][c].value = 0;
			map[p3][c].dcol = c;
			map[p3][c].drow = p1;

			map[p2][c].status = WAIT_COMBINE;
			map[p3][c].status = COMBINE;
		}

	}
	return 0;
	
	/*int p1;
	for (p1 = row - 1; p1 - 1 >= 0; p1--) {
		if (map[p1][c].value == 0)
			break;
		if (map[p1 - 1][c].value == map[p1][c].value) {

			map[p1][c].value *= 2;
			map[p1][c].status = WAIT_COMBINE;

			map[p1 - 1][c].value = 0;
			map[p1 - 1][c].status = COMBINE;
			map[p1 - 1][c].drow = p1;
		}
	}
	down_align(c);
	return 0;*/
}

int G2048::left_check(int r)
{
	int p1 = 0, p2 = 0, p3 = 0;//p1为目标位置，p2为原位置
	int tem;
	for (; 1; p1++) {
		if (map[r][p1].value != 0) {
			p2 = p1;
		}
		else
			for (p2 = p1; p2 < col && map[r][p2].value == 0; p2++)
				;
		if (p1 == col || p2 == col)
			break;//说明到底了，否则说明找到
		//如果找到则交换值
		tem = map[r][p1].value;
		map[r][p1].value = map[r][p2].value;
		map[r][p2].value = tem;
		map[r][p2].dcol = p1;
		map[r][p2].drow = r;

		//再向后找一个
		for (p3 = p2 + 1; p3 < col && map[r][p3].value == 0; p3++)
			;
		if (p3 == col)
			break;//说明到底了，否则说明找到
		//上下两个值相等
		if (map[r][p3].value == map[r][p1].value) {
			map[r][p1].value *= 2;
			score += map[r][p1].value;//计分
			map[r][p3].value = 0;
			map[r][p3].dcol = p1;
			map[r][p3].drow = r;

			map[r][p2].status = WAIT_COMBINE;
			map[r][p3].status = COMBINE;
		}

	}
	return 0;

	/*int p1;
	for (p1 = 0; p1 + 1 < col; p1++) {
		if (map[r][p1].value == 0)
			break;
		if (map[r][p1 + 1].value == map[r][p1].value) {

			map[r][p1].value *= 2;
			map[r][p1].status = WAIT_COMBINE;

			map[r][p1 + 1].value = 0;
			map[r][p1 + 1].status = COMBINE;
			map[r][p1 + 1].dcol = p1;
		}
	}
	left_align(r);
	return 0;*/
}

int G2048::right_check(int r)
{
	//up换down 1. ++->-- 2.p1 = row - 1; p1 - 1 >= 0; p1--
	int p1 = row - 1, p2 = row - 1, p3 = row - 1;//p1为目标位置，p2为原位置
	int tem;
	for (; 1; p1--) {
		if (map[r][p1].value != 0) {
			p2 = p1;
		}
		else
			for (p2 = p1; p2 >= 0 && map[r][p2].value == 0; p2--)
				;
		if (p1 < 0 || p2 < 0)
			break;//说明到底了，否则说明找到
		//如果找到则交换值
		tem = map[r][p1].value;
		map[r][p1].value = map[r][p2].value;
		map[r][p2].value = tem;
		map[r][p2].dcol = p1;
		map[r][p2].drow = r;

		//再向后找一个
		for (p3 = p2 - 1; p3 >= 0 && map[r][p3].value == 0; p3--)
			;
		if (p3 < 0)
			break;//说明到底了，否则说明找到
		//上下两个值相等
		if (map[r][p3].value == map[r][p1].value) {
			map[r][p1].value *= 2;
			score += map[r][p1].value;//计分
			map[r][p3].value = 0;
			map[r][p3].dcol = p1;
			map[r][p3].drow = r;

			map[r][p2].status = WAIT_COMBINE;
			map[r][p3].status = COMBINE;
		}

	}
	return 0;
	
	
	/*int p1;
	for (p1 = col - 1; p1 - 1 >= 0; p1--) {
		if (map[r][p1].value == 0)
			break;
		if (map[r][p1 - 1].value == map[r][p1].value) {

			map[r][p1].value *= 2;
			map[r][p1].status = WAIT_COMBINE;

			map[r][p1 - 1].value = 0;
			map[r][p1 - 1].status = COMBINE;
			map[r][p1 - 1].dcol = p1;//此处有严重的问题
		}
	}
	right_align(r);
	return 0;*/
}

G2048::G2048(const int Row, const int Col, const int Dst_score, const int Speed, const bool Sep)
{
	score = 0;
	high_score = 0;
	row = Row;
	col = Col;
	dst_score = Dst_score;
	speed = Speed;
	separator = Sep;
	int i = 0;
	map = new BLOCK*[Row];
	for (i = 0; i < Col; i++)
		map[i] = new BLOCK[Col];

}

G2048::~G2048()
{
	int i;
	for (i = 0; i < col; i++)
		delete[]map[i];
	delete[]map;
}

int G2048::set(const int Row, const int Col, const int Dst_score, const int Speed, const bool Sep)
{
	//先释放
	int i;
	for (i = 0; i < col; i++)
		delete[]map[i];
	delete[]map;

	//再重置
	score = 0;
	row = Row;
	col = Col;
	dst_score = Dst_score;
	speed = Speed;
	separator = Sep;
	map = new BLOCK*[Row];
	for (i = 0; i < Col; i++)
		map[i] = new BLOCK[Col];
	return 0;
}

int G2048::Produce_new_block(const int num, const bool show)
{
	//生成随机数种子
	unsigned int t;
	t = (unsigned int)time(0);
	srand(t);

	int i;
	for (i = 0; i < num; i++) {
		int two_or_four = rand() % 10;
		int new_num, new_row, new_col;
		if (two_or_four < 7) //生成2
			new_num = 2;
		else
			new_num = 4;
		do {
			new_row = rand() % row;
			new_col = rand() % col;
		} while (map[new_row][new_col].value);

		map[new_row][new_col].value = new_num;
		map[new_row][new_col].status = NORMAL;

		/*显示新色块*/
		if (show) {
			gmw_draw_block(&G2048_CGI, new_row, new_col, new_num, bdi_normal);
		}
	}
	
	return 0;
}

int G2048::Init_map()
{
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			map[i][j].value = 0;
			map[i][j].status = 0;
		}
	}

	Produce_new_block(2, false);//内部操作，不显示色块show = false
	return 0;
}

int G2048::Init_frame()
{
	/*初始化边框*/
	gmw_init(&G2048_CGI);

	gmw_set_color(&G2048_CGI, COLOR_BLACK, COLOR_WHITE);			//整个窗口颜色
	gmw_set_font(&G2048_CGI, "新宋体", 16);						//字体
	gmw_set_frame_style(&G2048_CGI, 10, 5, true);					//游戏主区域风格：每个色块宽10高5，有分隔线【数字色块带边框，宽度为10(放最多6位数字)，高度为5(为了保持色块为方形)】
	gmw_set_frame_color(&G2048_CGI, COLOR_WHITE, COLOR_BLACK);	//游戏主区域颜色
	gmw_set_block_border_switch(&G2048_CGI, true);				//小色块带边框
	gmw_set_rowno_switch(&G2048_CGI, true);						//显示行号
	gmw_set_colno_switch(&G2048_CGI, true);
	gmw_set_delay(&G2048_CGI, DELAY_OF_BLOCK_MOVED, speed*BLOCK_MOVED_DELAY_MS);
	/* 按row/col的值重设游戏主区域行列 */
	gmw_set_rowcol(&G2048_CGI, row, col);

	cls();
	/* 显示框架 */
	gmw_draw_frame(&G2048_CGI);

	/*显示球*/
	int i, j;
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++) {
			gmw_draw_block(&G2048_CGI, i, j, map[i][j].value, bdi_normal);
		}

	return 0;
}

int G2048::Game_start()
{
	Init_map();
	Init_frame();

	int loop = 1;
	int maction, mrow, mcol;
	int keycode1, keycode2;
	int ret;
	char temp[CHAR_LEN];

	while (1) {
		switch (Gameover()) {
		case 0:
			break;
		case 'F':
			gmw_status_line(&G2048_CGI, LOWER_STATUS_LINE, "革命尚未成功，同志仍需努力~", NULL);
			break;
		case 'W':
			gmw_status_line(&G2048_CGI, LOWER_STATUS_LINE, "Winner, You are standing the top of the world!", NULL);
			break;
		}

		ret = gmw_read_keyboard_and_mouse(&G2048_CGI, maction, mrow, mcol, keycode1, keycode2);

		/*键盘控制*/
		if (ret == CCT_KEYBOARD_EVENT)  { //CCT_KEYBOARD_EVENT - 具体键码含义去读 cmd_console_tools.cpp
			switch (keycode1) {
			case 'R':
			case 'r':
				return 'r';
				break;
			case 'Q':
			case 'q':
				return 'Q';
				break;
			case 0x0D:
				/* 下状态栏显示内容 */
				//gmw_status_line(&G2048_CGI, LOWER_STATUS_LINE, "[读到回车键]", NULL);
				break;
			case 0xE0: //224
				switch (keycode2) {
				case KB_ARROW_UP:
					/* 上状态栏显示内容 */
					//Align(DOWN_TO_UP);//对齐
					Check_remove(DOWN_TO_UP);//检查消除，计分
					Move_animation(DOWN_TO_UP);//显示移动合并动画
					Clear_status();//清理状态
					Produce_new_block(1);//生成并显示新球，记得新球要重置状态
					//print_map();
					sprintf(temp, "目标分数：%d 当前得分：%d", dst_score, score);
					gmw_status_line(&G2048_CGI, TOP_STATUS_LINE, temp, NULL);
					break;
				case KB_ARROW_DOWN:
					/* 下状态栏显示内容 */
					//Align(UP_TO_DOWN);//对齐
					Check_remove(UP_TO_DOWN);//检查消除，计分
					Move_animation(UP_TO_DOWN);//显示移动合并动画
					Clear_status();//清理状态
					Produce_new_block(1);//生成并显示新球，记得新球要重置状态
					//print_map();
					sprintf(temp, "目标分数：%d 当前得分：%d", dst_score, score);
					gmw_status_line(&G2048_CGI, TOP_STATUS_LINE, temp, NULL);
					break;
				case KB_ARROW_LEFT:
					/* 下状态栏显示内容 */
				//Align(RIGHT_TO_LEFT);//对齐
					Check_remove(RIGHT_TO_LEFT);//检查消除，计分
					Move_animation(RIGHT_TO_LEFT);//显示移动合并动画
					Clear_status();//清理状态
					Produce_new_block(1);//生成并显示新球，记得新球要重置状态
					//print_map();
					sprintf(temp, "目标分数：%d 当前得分：%d", dst_score, score);
					gmw_status_line(&G2048_CGI, TOP_STATUS_LINE, temp, NULL);
					break;
				case KB_ARROW_RIGHT:
					/* 下状态栏显示内容 */
					//Align(LEFT_TO_RIGHT);//对齐
					Check_remove(LEFT_TO_RIGHT);//检查消除，计分
					Move_animation(LEFT_TO_RIGHT);//显示移动合并动画
					Clear_status();//清理状态
					Produce_new_block(1);//生成并显示新球，记得新球要重置状态
					//print_map();
					sprintf(temp, "目标分数：%d 当前得分：%d", dst_score, score);
					gmw_status_line(&G2048_CGI, TOP_STATUS_LINE, temp, NULL);
					break;
				}
				break;
			}//end of switch
		}//end of else
	}//end of while

	return 0;
}
/***************************************************************************
  函数名称：
  功    能：合并并对齐
  输入参数：const int direction 对齐方向
  返 回 值：
  说    明：
***************************************************************************/
int G2048::Check_remove(const int direction)
{
	int i;
	switch (direction) {

	case DOWN_TO_UP:
		for (i = 0; i < col; i++) {
			up_check(i);
		}
		break;
	case UP_TO_DOWN:
		for (i = 0; i < col; i++) {
			down_check(i);
		}
		break;
	case RIGHT_TO_LEFT:
		for (i = 0; i < row; i++) {
			left_check(i);
		}
		break;
	case LEFT_TO_RIGHT:
		for (i = 0; i < row; i++) {
			right_check(i);
		}
		break;
	}
	return 0;
}

int G2048::Move_animation(const int direction)
{
	int i, j;
	int dr, dc;
	int value;

	switch (direction) {
		//其实只有移动距离和方向有关abs(map[i][j].drow - i)和abs(map[i][j].dcol - i))之别
	case DOWN_TO_UP:
		for (j = 0; j < col; j++) {
			for (i = 0; i < row; i++) {
				if (!map[i][j].status)
					continue;
				dc = map[i][j].dcol;
				dr = map[i][j].drow;
				value = map[dr][dc].value;
				switch (map[i][j].status) {

				case NORMAL:
					gmw_move_block(&G2048_CGI, i, j, value, 0, bdi_normal, direction, abs(dr - i));
					break;
				case WAIT_COMBINE:
					gmw_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dr - i));
					break;
				case COMBINE:
					gmw_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dr - i));
					gmw_draw_block(&G2048_CGI, dr, map[i][j].dcol, value, bdi_normal);
					break;
				}
			}
		}
		break;
	case UP_TO_DOWN:
		for (j = col - 1; j >= 0; j--) {
			for (i = row - 1; i >= 0; i--) {
	
				if (!map[i][j].status)
					continue;
				dc = map[i][j].dcol;
				dr = map[i][j].drow;
				value = map[dr][dc].value;
				switch (map[i][j].status) {

				case NORMAL:
					gmw_move_block(&G2048_CGI, i, j, value, 0, bdi_normal, direction, abs(dr - i));
					break;
				case WAIT_COMBINE:
					gmw_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dr - i));
					break;
				case COMBINE:
					gmw_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dr - i));
					gmw_draw_block(&G2048_CGI, dr, map[i][j].dcol, value, bdi_normal);
					break;
				}
			}
		}
		break;
	case RIGHT_TO_LEFT:
		for (i = 0; i < row; i++) {
			for (j = 0; j < col; j++) {
				if (!map[i][j].status)
					continue;
				dc = map[i][j].dcol;
				dr = map[i][j].drow;
				value = map[dr][dc].value;
				switch (map[i][j].status) {

				case NORMAL:
					gmw_move_block(&G2048_CGI, i, j, value, 0, bdi_normal, direction, abs(dc - j));
					break;
				case WAIT_COMBINE:
					gmw_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dc - j));
					break;
				case COMBINE:
					gmw_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dc - j));
					gmw_draw_block(&G2048_CGI, map[i][j].drow, dc, value, bdi_normal);
					break;
				}
			}
		}
		break;
	case LEFT_TO_RIGHT:
		for (i = row-1; i >=0; i--) {
			for (j = col-1; j >=0; j--) {
				if (!map[i][j].status)
					continue;
				dc = map[i][j].dcol;
				dr = map[i][j].drow;
				value = map[dr][dc].value;
				switch (map[i][j].status) {

				case NORMAL:
					gmw_move_block(&G2048_CGI, i, j, value, 0, bdi_normal, direction, abs(dc - j));
					break;
				case WAIT_COMBINE:
					gmw_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dc - j));
					break;
				case COMBINE:
					gmw_move_block(&G2048_CGI, i, j, value / 2, 0, bdi_normal, direction, abs(dc - j));
					gmw_draw_block(&G2048_CGI, map[i][j].drow, dc, value, bdi_normal);
					break;
				}
			}
		}
		break;
	}
	/* 显示框架 */
	//gmw_draw_frame(&G2048_CGI);

	/*显示球*/
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++) {
			gmw_draw_block(&G2048_CGI, i, j, map[i][j].value, bdi_normal);
		}
	return 0;
}

int G2048::Clear_status()
{
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			//目标位置清零
			map[i][j].dcol = 0;
			map[i][j].drow = 0;
			if(map[i][j].value == 0)
				map[i][j].status = ZERO;
			else
				map[i][j].status = NORMAL;
		}
	}

	return 0;
}

int G2048::Gameover()
{
	if (score >= dst_score)
		return 'W';//游戏胜利
	
	/*判断满格*/
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j].status == 0)
				break;
		}
	}
	if (i == row && j == col)
		return 'F';//Full or Fail

	return 0;
}

int G2048::print_map()
{
	cls();
	cout << "score:" << score << endl;
	cout << "内部数组:" << endl;
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++)
			cout << setw(4) << map[i][j].value;
		cout << endl;
	}
	return 0;
}
