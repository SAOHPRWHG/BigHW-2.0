/* 1751957 ��2�� ������ */
#include "../common/cmd_hdc_tools.h"
#include "90-b0.h"
using namespace std;

void Init_array(int(*a)[MAX_FRAME], int row, int col, int n)
{
	/***************************************************************************
	  �������ƣ�
	  ��    �ܣ���ʼ�����飬����n���������
	  ���������
	  �� �� ֵ��
	  ˵    ������ɫ1-7֮��
	***************************************************************************/

	int i, x, y, color;
	//�������������
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
	  �������ƣ�
	  ��    �ܣ���flagΪ��Ԥ����һ��n(3)���������,flagΪ0��������״̬����Ԥ��ֵ���ϴ�һ��
	  ���������
	  �� �� ֵ������color[]
	  ˵    ������ɫ1-7֮��
	***************************************************************************/
	gotoxy(x, y);
	setcolor(3, 15);
	static int color[MAX_FRAME];
	int i;
	if (flag) {
		//�������������
		unsigned int t;
		t = (unsigned int)time(0);
		srand(t);

		cout << "��" << n << "���������ɫ�ֱ��ǣ�";
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
		cout << "��" << n << "���������ɫ�ֱ��ǣ�";
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
	  �������ƣ�
	  ��    �ܣ���Ԥ���n��С��д������
	  ���������
	  �� �� ֵ��
	  ˵    ����
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
		cout << "����Ƿ�������������" << endl;
	}
}

void End(int Y)
{
	/***************************************************************************
		  �������ƣ�
		  ��    �ܣ��ڵ�Y�п�ʼ������End���ز˵�
		  ���������
		  �� �� ֵ��
		  ˵    ������Сд����
	***************************************************************************/
	int X;
	const char str[] = "��С�������������end����...";
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
			//ͳһ��Сд
			for (p = x; *p; p++) {
				if (*p >= 'A'&&*p <= 'Z')
					*p += 32;
			}

			if (x[0] == ch1[0] && x[1] == ch1[1] && x[2] == ch1[2])
				break;
		}
		cout << "�����������������" << endl;
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
			cout << "��ʼλ��Ϊ�գ�����������." << endl;
			return 0;
		}
	}
	else {
		if (!a[y][x])
			return 1;
		else {
			cout << "Ŀ��λ�÷ǿգ�����������." << endl;
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
	  �������ƣ�
	  ��    �ܣ�������������ҳ����·��������¼������·����λ����map[][]��
	  ���������
	  �� �� ֵ��path[MAX_FRAME*MAXFRAME]==0Ϊ��������Ч·����path[MAX_FRAME*MAXFRAME]==INFΪ�Ҳ�����Ч·��
	  ˵    ����
	***************************************************************************/
	int map[MAX_FRAME][MAX_FRAME] = { INF };
	int repath[2][MAX_FRAME*MAX_FRAME];
	int queue[LEN];//��һ���㹻������鱣֤���ζ��в������ 
	int *front = &queue[0], *rear = &queue[0];
	//�洢��������y��x����һ����λ������ʾλ��P=10*y+x;
	//ȡ��ʱ��y = P/10;x = P%10; 
	//���ʽ��ͣ�sx,sy,sP->start Position
	//nx,ny,nP = new x,new y, new Position
	//tx,ty,tP = terminal 	
	int dy[4] = { -1,0,0,1 }, dx[4] = { 0,-1,1,0 };

	int i;//��¼������������ 
	int j, k;
	int sP;
	sP = sy * 10 + sx;//ת��Ϊ��ʼλ��sP 
	rear = queue_push(queue, rear, sP);

	for (j = 0; j < MAX_FRAME*MAX_FRAME; j++) {
		path[1][j] = -1;//��·����¼����ͳһΪ-1��
	}
	//��ʼ��λ����Ϣ 
	for (j = 0; j < MAX_FRAME; j++) {
		for (k = 0; k < MAX_FRAME; k++) {
			map[j][k] = INF;
		}
	}
	map[sy][sx] = 0;//��ʼλ�þ���Ϊ0 
	while (queue_size(front, rear)) {

		int P = queue_pop(queue, &front);
		int y = P / 10;
		int x = P % 10;

		for (i = 0; i < 4; i++) {
			int ny = y + dy[i];
			int nx = x + dx[i];

			//û��Խ��&&û�в���&&��δ���� 
			if (ny > 0 && ny <= row && nx > 0 && nx <= col && a[ny][nx] == 0 && map[ny][nx] == INF) {
				int nP = 10 * ny + nx;
				rear = queue_push(queue, rear, nP);
				map[ny][nx] = (map[x][y] / 10) * 10 + 10 + i;//����ֵΪʵ�ʾ���*10+i(��¼��yx->nyx�ķ���) 
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
	  �������ƣ�
	  ��    �ܣ�ɨ���ڲ����飬�ݹ������
	  ���������
	  �� �� ֵ���������
	  ˵    ����
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
	  �������ƣ�
	  ��    �ܣ���������Ϊn���˴�ȡֵΪ-n;
	  ���������
	  �� �� ֵ��0
	  ˵    ����
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
	  �������ƣ�
	  ��    �ܣ�����ڲ����飬������
	  ���������
	  �� �� ֵ�����ε÷�
	  ˵    ����˫5��80�֣���ֵΪn*2,������������ɫ�͸����� color*10+count �ķ�ʽ�洢��a[0][0],a[0][1],a[0][2]��
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
						//��һ���ҵ�,�����Ƕ���Ϊ����
						flag++;
					}
				}
			}
		}
	}
	//���Ϊ0
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
	//ͳ����������
	for (i = 0; i < 3; i++) {
		del_color = a[0][i] / 10;
		del_num = a[0][i] % 10;
		del[del_color] += del_num;
	}
	//ͳ���ִ����
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
		Input(&Sy, 'A', 'A' + row - 1, "������ĸ+������ʽ[����c2]����Ҫ�ƶ���ľ������꣺");
		*_sy = int(Sy);
		Input(_sx, 1, col, "");
		cout << "����Ϊ" << *_sy << "��" << *_sx << "��" << endl;
	} while (!check_input(a, *_sy, *_sx, 0));
}

void move_destin(int a[][MAX_FRAME], int row, int col, int* _tx, int* _ty)
{
	char Ty;
	do {
		Input(&Ty, 'A', 'A' + row - 1, "������ĸ+������ʽ[����c2]����Ҫ�ƶ����Ŀ�����꣺");
		*_ty = int(Ty);
		Input(_tx, 1, col, "");
		cout << "����Ϊ" << *_ty << "��" << *_tx << "��" << endl;
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
  �������ƣ�
  ��    �ܣ��ж���Ϸ����
  ���������
  �� �� ֵ������1���ж�Ϊ����Ϸ����������0�����ִ��
  ˵    ����
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


