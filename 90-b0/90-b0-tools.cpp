/* 1751957 计2班 俞少作 */
#include "../common/cmd_hdc_tools.h"
#include "90-b0.h"
using namespace std;

void Init_array(int(*a)[MAX_FRAME], int row, int col, int n)
{
	/***************************************************************************
	  函数名称：
	  功    能：初始化数组，生成n个随机彩球
	  输入参数：
	  返 回 值：
	  说    明：颜色1-7之间
	***************************************************************************/

	int i, x, y, color;
	//生成随机数种子
	unsigned int t;
	t = (unsigned int)time(0);
	srand(t);

	for (i = 0; i < n; ) {
		x = rand() % (col)+1;
		y = rand() % (row)+1;
		color = rand() % 7 + 1;
		if (!a[x][y]) {
			a[x][y] = color;
			i++;
		}
	}
}

int* Pre_next(int(*a)[MAX_FRAME], int x, int y, int n, int flag)
{
	/***************************************************************************
	  函数名称：
	  功    能：若flag为真预告下一步n(3)个随机彩球,flag为0，即消除状态，则预告值与上次一样
	  输入参数：
	  返 回 值：数组color[]
	  说    明：颜色1-7之间
	***************************************************************************/
	gotoxy(x, y);
	setcolor(3, 15);
	static int color[MAX_FRAME];
	int i;
	if (flag) {
		//生成随机数种子
		unsigned int t;
		t = (unsigned int)time(0);
		srand(t);

		cout << "下" << n << "个彩球的颜色分别是：";
		for (i = 0; i < n; i++) {
			//x = rand() % 10;
			//y = rand() % 10;
			color[i] = rand() % 7 + 1;
			cout << left << setw(2) << color[i];
			/*if (a[x][y]) {
				a[x][y] = color[i];
				cout << left << setw(2) << color[i];
				i++;

			}*/
		}
		cout << endl;
	}
	else {
		cout << "下" << n << "个彩球的颜色分别是：";
		for (i = 0; i < n; i++) {
			cout << left << setw(2) << color[i];
		}
		cout << endl;
	}

	return color;
}

void Do_next(int(*a)[MAX_FRAME], int row, int col, int *color, int n)
{
	/***************************************************************************
	  函数名称：
	  功    能：将预告的n个小球写入数组
	  输入参数：
	  返 回 值：
	  说    明：
	***************************************************************************/
	int i, x, y;
	for (i = 0; i < n; ) {
		x = rand() % (col)+1;
		y = rand() % (row)+1;
		if (!a[x][y]) {
			a[x][y] = color[i];
			i++;
		}
		if (Gameover(a, row, col))
			break;
	}
}

void Input(int * x, int lower, int upper, const char str[])
{
	while (1) {
		cout << str;
		cin >> *x;
		if (!cin.good()) {
			cin.clear();
			cin.ignore(1024, '\n');
		}

		else if (*x <= upper && *x >= lower)
			break;
	}
}

void Input(char * x, int lower, int upper, const char str[])
{
	while (1) {
		cout << str;
		char ch;
		while ((ch = getchar()) != '\n')
			;
		cin >> *x;
		if (*x >= 'a'&&*x <= 'z')
			*x -= 32;

		if (!cin.good()) {
			cin.clear();
			cin.ignore(1024, '\n');
		}

		else if (*x <= upper && *x >= lower) {
			*x -= ('A' - 1);
			break;
		}
		cout << "输入非法，请重新输入" << endl;
	}
}

void End(int Y)
{
	/***************************************************************************
		  函数名称：
		  功    能：在第Y行开始，输入End返回菜单
		  输入参数：
		  返 回 值：
		  说    明：大小写均可
	***************************************************************************/
	int X;
	const char str[] = "本小题结束，请输入end继续...";
	char x[3];
	char* p = x;
	char ch1[] = "end";
	
	setcolor(3, 15);
	gotoxy(0, Y);
	while (1) {
		cout << str;
		getxy(X, Y);
		cout << "       ";
		gotoxy(X, Y);
		for (p = x; (p - x) < 3; p++) {
			cin >> *p;
		}
		if (!cin.good()) {
			cin.clear();
			cin.ignore(1024, '\n');
		}

		else {
			//统一大小写
			for (p = x; *p; p++) {
				if (*p >= 'A'&&*p <= 'Z')
					*p += 32;
			}

			if (x[0] == ch1[0] && x[1] == ch1[1] && x[2] == ch1[2])
				break;
		}
		cout << "输入错误，请重新输入" << endl;
		gotoxy(0, Y);
	}
	cls();
}

int check_input(int(*a)[MAX_FRAME], int y, int x, int op)
{
	if (op == 0) {
		if (a[y][x])
			return 1;
		else {
			cout << "起始位置为空，请重新输入." << endl;
			return 0;
		}
	}
	else {
		if (!a[y][x])
			return 1;
		else {
			cout << "目标位置非空，请重新输入." << endl;
			return 0;
		}
	}
}

int* queue_push(int *queue, int *rear, int nP)
{
	*rear = nP;
	rear = queue + (rear - queue + 1) % LEN;
	return rear;
}

int queue_pop(int *queue, int **_front)
{
	int P = **_front;
	*_front = queue + (*_front - queue + 1) % LEN;
	return P;
}

int queue_size(const int *front, const int *rear)
{
	return rear - front;
}

int bfs(int path[][MAX_FRAME*MAX_FRAME],int sy, int sx, int ty, int tx, int a[][MAX_FRAME], int row, int col)
{
	/***************************************************************************
	  函数名称：
	  功    能：广度优先搜索找出最短路径，并记录步数，路径和位置在map[][]中
	  输入参数：
	  返 回 值：path[MAX_FRAME*MAXFRAME]==0为搜索到有效路径，path[MAX_FRAME*MAXFRAME]==INF为找不到有效路径
	  说    明：
	***************************************************************************/
	int map[MAX_FRAME][MAX_FRAME] = { INF };
	int repath[2][MAX_FRAME*MAX_FRAME];
	int queue[LEN];//开一个足够大的数组保证环形队列不会队满 
	int *front = &queue[0], *rear = &queue[0];
	//存储方法，先y后x，用一个两位数来表示位置P=10*y+x;
	//取出时，y = P/10;x = P%10; 
	//名词解释：sx,sy,sP->start Position
	//nx,ny,nP = new x,new y, new Position
	//tx,ty,tP = terminal 	
	int dy[4] = { -1,0,0,1 }, dx[4] = { 0,-1,1,0 };

	int i;//记录方向：上左右下 
	int j, k;
	int sP;
	sP = sy * 10 + sx;//转化为初始位置sP 
	rear = queue_push(queue, rear, sP);

	for (j = 0; j < MAX_FRAME*MAX_FRAME; j++) {
		path[1][j] = -1;//非路径记录部分统一为-1；
	}
	//初始化位置信息 
	for (j = 0; j < MAX_FRAME; j++) {
		for (k = 0; k < MAX_FRAME; k++) {
			map[j][k] = INF;
		}
	}
	map[sy][sx] = 0;//初始位置距离为0 
	while (queue_size(front, rear)) {

		int P = queue_pop(queue, &front);
		int y = P / 10;
		int x = P % 10;

		for (i = 0; i < 4; i++) {
			int ny = y + dy[i];
			int nx = x + dx[i];

			//没有越界&&没有彩球&&从未来过 
			if (ny > 0 && ny <= row && nx > 0 && nx <= col && a[ny][nx] == 0 && map[ny][nx] == INF) {
				int nP = 10 * ny + nx;
				rear = queue_push(queue, rear, nP);
				map[ny][nx] = (map[x][y] / 10) * 10 + 10 + i;//储存值为实际距离*10+i(记录由yx->nyx的方向) 
				if (nx == tx && ny == ty)
					break;
			}
		}
		
		if (i != 4) {
			y = ty;
			x = tx;
			for (j = 0; map[y][x]; j++) {
				repath[0][j] = y * 10 + x;
				i = map[y][x] % 10;
				repath[1][j] = i;
				y = y + dy[3 - i];
				x = x + dx[3 - i];
			}
			repath[0][j] = sy * 10 + sx;
			
			for (k = 0; j >= 0;) {
				path[0][k] = repath[0][j];
				path[1][k++] = repath[1][j--];
			}
			return 1;
		}

	}

	return 0;
}

int sweep(int a[][MAX_FRAME], int row, int col, int i, int j, int y, int x, int counter)
{
	/***************************************************************************
	  函数名称：
	  功    能：扫描内部数组，递归出个数
	  输入参数：
	  返 回 值：连珠个数
	  说    明：
	***************************************************************************/

	if (abs(a[i + y][j + x]) == abs(a[i][j]) && (i + y <= row) && (i + y > 0) && (j + x <= col) && (j + x > 0)) {
		counter++;
		counter = sweep(a, row, col, i + y, j + x, y, x, counter);
	}
	return counter;
}

int Mark(int a[][MAX_FRAME], int i, int j, int y, int x, int count, int n)
{
	/***************************************************************************
	  函数名称：
	  功    能：将连珠标记为n，此处取值为-n;
	  输入参数：
	  返 回 值：0
	  说    明：
	***************************************************************************/
	if (a[i][j] > 0) {
		int k;
		for (k = 0; k < count; k++) {
			a[i][j] = n;
			i += y;
			j += x;
		}
		return 0;
	}
	else
		return 0;
}

int Check(int a[][MAX_FRAME], int row, int col)
{
	/***************************************************************************
	  函数名称：
	  功    能：检查内部数组，并消除
	  输入参数：
	  返 回 值：本次得分
	  说    明：双5得80分，单值为n*2,本次消除的颜色和个数以 color*10+count 的方式存储到a[0][0],a[0][1],a[0][2]中
	***************************************************************************/

	int vector_y[] = { -1,-1,-1,0,0,1,1,1 }, vector_x[] = { -1,0,1,-1,1,-1,0,1 };
	int count = 0, count_max = 0, flag = 0;
	int i, j, k,l=0;
	for (i = 1; i <= row; i++) {
		for (j = 1; j <= col; j++) {
			if (a[i][j]>0) {
				for (k = 0; k < COLOR_NUM; k++) {
					int counter = 1;
					count = sweep(a, row, col, i, j, vector_y[k], vector_x[k], counter);
					if ( count >= 5) {
						a[0][l++] = a[i][j] * 10+count;
						if(count > count_max)
							count_max = count;
						Mark(a, i, j, vector_y[k], vector_x[k], count, -a[i][j]);
						//第一次找到,把他们都置为负数
						flag++;
					}
				}
			}
		}
	}
	//清除为0
	for (i = 1; i <= row; i++) {
		for (j = 1; j <= col; j++) {
			if (a[i][j] < 0)
				a[i][j] = 0;
		}
	}

	if (!count_max) {
		a[0][0] = 0;
		a[0][1] = 0;
		a[0][2] = 0;
	}

	return flag * flag*flag*count_max * 2;
}

void static_data(int now[COLOR_NUM],float pro[COLOR_NUM],int del[COLOR_NUM],int a[][MAX_FRAME], int row, int col)
{
	//int now[8] = { 0 }, del[8] = { 0 };
	int del_color, del_num;
	int i,j;
	//统计消除个数
	for (i = 0; i < 3; i++) {
		del_color = a[0][i] / 10;
		del_num = a[0][i] % 10;
		del[del_color] += del_num;
	}
	//统计现存个数
	for (i = 1; i <= row; i++) {
		for (j = 1; j <= col; j++) {
				now[a[i][j]]++;		
		}
	}
	for (i = 0; i <COLOR_NUM; i++) {
		pro[i] = now[i] / float(row*col);
	}
}

void move_which_ball(int a[][MAX_FRAME], int row, int col, int* _sx, int* _sy)
{
	char Sy;
	do {
		Input(&Sy, 'A', 'A' + row - 1, "请以字母+数字形式[例：c2]输入要移动球的矩阵坐标：");
		*_sy = int(Sy);
		Input(_sx, 1, col, "");
		cout << "输入为" << *_sy << "行" << *_sx << "列" << endl;
	} while (!check_input(a, *_sy, *_sx, 0));
}

void move_destin(int a[][MAX_FRAME], int row, int col, int* _tx, int* _ty)
{
	char Ty;
	do {
		Input(&Ty, 'A', 'A' + row - 1, "请以字母+数字形式[例：c2]输入要移动球的目的坐标：");
		*_ty = int(Ty);
		Input(_tx, 1, col, "");
		cout << "输入为" << *_ty << "行" << *_tx << "列" << endl;
	} while (!check_input(a, *_ty, *_tx, 1));
}

void Move_ball(int a[][MAX_FRAME], const int sy, const int sx, const int ty, const int tx)
{
	a[ty][tx] = a[sy][sx];
	a[sy][sx] = 0;
}

int Gameover(int a[][MAX_FRAME], int row, int col)
{
/***************************************************************************
  函数名称：
  功    能：判断游戏结束
  输入参数：
  返 回 值：返回1，判断为真游戏结束，返回0则继续执行
  说    明：
***************************************************************************/
	int i, j;
	for (i = 1; i < row; i++) {
		for (j = 1; j < col; j++) {
			if (a[i][j] == 0)
				return 0;
		}
	}
	return 1;
}


