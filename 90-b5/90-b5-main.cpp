/* 1751957 计2班 俞少作 */
#define _CRT_SECURE_NO_WARNINGS
#include "BplC.cpp"
using namespace std;

/***************************************************************************
  函数名称：
  功    能：将学号+MD5(password)再用security_str按位异或后形成要发送的“报道”数据
  输入参数：char *reg_str				：异或后需要发送的串（返回）
			const char *stu_no			：学号
			const char *stu_password	：交作业系统中的口令（明码），无论口令多长，转换为MD5后为32字节的hex串
			const char *security_str	：从Server端收到的异或加密串
  返 回 值：
  说    明：以学号 : 1859999 , 密码 : Password，异或串 : f272a9b7422ee1ddec6c4b1abe758cadefc658c2 为例
			1、Password     ： => MD5 => “dc647eb65e6711e155375218212b3964”
			2、认证串(原始) ：1859999+dc647eb65e6711e155375218212b3964
			3、认证串(原始)和异或串进行按位异或
				   1859999+dc647eb65e6711e155375218212b3964
				   f272a9b7422ee1ddec6c4b1abe758cadefc658c2
			   进行按位异或，结果不保证一定是图形ASCII字符，所以再转为hex
			   认证串(Hex发送) ：570a020b58005b1c50510451525406525006005405535450575004020d51505c5757515406015506
			   解释：第1个字节 1和f，即 0x31^0x66 = 0x57
					 第2个字节 8和2，即 0x38^0x32 = 0x0a
					 ...(略)
***************************************************************************/
int cmd_tcp_socket::make_register_string(char *send_regstr, const char *stu_no, const char *stu_password, const char *security_str)
{
	/* 需要自己实现 */
	char md5_password[32 + 1];
	MD5(md5_password, stu_password);
	cout << "口令(加密后)：" << md5_password << endl;
	char org_regstr[40 + 1];
	strcpy(org_regstr, stu_no);
	strcat(org_regstr, "-");
	strcat(org_regstr, md5_password);
	cout << "认证串(原始)：" << org_regstr << endl;
	cout << "异或串      ：" << security_str << endl;
	char mid_regstr[40 + 1];
	int i;
	for (i = 0; org_regstr[i]; i++) {
		mid_regstr[i] = org_regstr[i] ^ security_str[i];
	}
	/*cout << "mid_regstr:"  ;
	for (i = 0; org_regstr[i]; i++)
		printf("%02x ", mid_regstr[i]);
	cout << endl;*/
	for (i = 0; org_regstr[i]; i++)
		sprintf(send_regstr + 2 * i, "%02x", mid_regstr[i]);
	cout << "认证串(Hex发送) ：" << send_regstr << endl;
	return 0;
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int game_progress(cmd_tcp_socket &client)
{
	char sel;
	char row, head_row, tail_row;
	int col, head_col, tail_col;
	bool recv_startgame = false;

	while (1) {
		/* 等待Server端的gameprogress */
		string spack;
		if (client.get_gameprogress_string(spack) < 0) {
			return -1;
		}
		cout << "Server应答 : " << spack << endl;

		if (spack == "StartGame")
			recv_startgame = true;

		/* 没收到StartGame前所有其他信息均认为错误 */
		if (!recv_startgame)
			return -1;

		if (spack == "GameOver") {
			cout << "本次GameID : " << client.get_gameid() << endl;
			cout << "本次得分   : " << client.get_score() << endl;
			return 0;
		}

		cout << "1.传送一个坐标" << endl;
		cout << "2.传送整架飞机坐标" << endl;
		while (1) {
			sel = _getch();
			if (sel == '1' || sel == '2')
				break;
		}
		switch (sel) {
		case '1':
			cout << "请输入行(A-J)列(0-9)坐标 : ";
			cin >> row >> col; //此处未判断正确性
			if (row >= 'a' && row <= 'j')
				row -= 32;
			client.send_coordinate(row, col);
			break;
		case '2':
			cout << "请输入机头行(A-J)列(0-9)坐标 : ";
			cin >> head_row >> head_col; //此处未判断正确性
			if (head_row >= 'a' && head_row <= 'j')
				head_row -= 32;
			cout << "请输入机尾正中行(A-J)列(0-9)坐标 : ";
			cin >> tail_row >> tail_col; //此处未判断正确性
			if (tail_row >= 'a' && tail_row <= 'j')
				tail_row -= 32;
			client.send_plane_coordinates(head_row, head_col, tail_row, tail_col);
			break;
		}//end of switch
	}//end of while(1)
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int game_progress_GUI(cmd_tcp_socket &client)
{
	while (2) {
		int sel;
		char row, head_row, tail_row;
		int col, head_col, tail_col;
		bool recv_startgame = false;
		char buffer[CHAR_LEN];

		BplC bpl;

		bpl.Init_map();
		bpl.Init_frame();

		while (1) {
			/* 等待Server端的gameprogress */
			string spack;
			if (client.get_gameprogress_string(spack) < 0) {
				return -1;
			}

			//cout << "Server应答 : " << spack << endl;
			sprintf(buffer, "Server应答 :  %s", (char*)spack.data());
			gmw_status_line(&bpl.BplC_CGI, TOP_STATUS_LINE, buffer);

			if (spack == "StartGame")
				recv_startgame = true;
			else
				bpl.Explode_animation(row, col, head_row, head_col, tail_row, tail_col, sel, spack);
			/* 没收到StartGame前所有其他信息均认为错误 */
			if (!recv_startgame)
				return -1;

			if (spack == "GameOver") {
				char tem1[CHAR_LEN], tem2[CHAR_LEN];
				sprintf(tem1, "本次GameID : %s", client.get_gameid());
				sprintf(tem2, "本次得分   :  %d", client.get_score());
				gmw_status_line(&bpl.BplC_CGI, LOWER_STATUS_LINE, tem1);
				//cout << "本次GameID : " << client.get_gameid() << endl;
				cout << endl << "本次得分   : " << client.get_score() << endl;
				getchar();
				break;
			}

			sel = bpl.Select_space_by_mouse(row, col, head_row, head_col, tail_row, tail_col);
			//cout <<"看这里："<< row << col;
			switch (sel) {
			case 1:
				client.send_coordinate(row, col);
				break;
			case 2:
				client.send_plane_coordinates(head_row, head_col, tail_row, tail_col);
				break;
			}

		}//end of while(1)
	}//end of while(2)
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：argv[1] : "-auto"/"-manual"，表示自动/人工游戏
			argv[2] : 学号
			argv[3] : 密码
  返 回 值：
  说    明：
***************************************************************************/
int main(int argc, char **argv)
{
	if (argc != 4) {
		cout << "Usage : " << argv[0] << " -auto|-manual  stu_no  stu_pwd" << endl;
		return -1;
	}

	/* 给个明确提示的demo */
	if (strcmp(argv[1], "-auto") == 0) {
		cout << "Demo中未实现自动游戏" << endl;
		return -1;
	}

	/* 其余合理性检查未做，需要自己补
	   1、argv[1]除 -auto -manual 外均错误
	   2、学号7位
	   3、... */

	const int sleep_ms = 5000;
	bool first = true;
	cmd_tcp_socket client;

	/* 打开client类对象中的debug开关（调试时可打开，到图形界面时需关闭） */
	client.set_debug_switch(false);

	while (1) {
		if (!first) { //出任何错误，延时5秒重连（不包括第一次）
			cout << "与服务器的连接断开!" << endl;
			Sleep(sleep_ms);
		}
		else
			first = false;

		/* 连接服务器 */
		if (client.connect() < 0) {
			/* 连接失败信息，有没有debug_switch都打印 */
			cout << "连接服务器失败!" << endl;
			continue;//return -1;
		}

		/* 读取服务器发送过来的加密串认证串(共40个字符) */
		string s1;
		if (client.get_security_string(s1) < 0) {
			client.close();
			continue; // return -2;
		}
		/* 打印 */
		cout << "读取的异或加密串 : " << s1 << endl;

		/* 将学号+MD5密码用异或加密串异或后 发送给Server端
		   注意：
		   1、长度一定是80字节，含尾零81
		   2、make_register_string需要自己实现
		   3、可参考tmake_register_string，测试时会将此函数从lib中移除   */
		char reg_str[81];
		//client.tmake_register_string(reg_str, "1850000", "185-0*0%0@0", s1.c_str()); //预置的测试用户（已失效）
		client.make_register_string(reg_str, argv[2], argv[3], s1.c_str()); //传自己的学号和密码

		/* 将认证串发送过去 */
		client.send_register_string(reg_str);

		/* 进入游戏交互环节
		   1、必须收到Server的StartGame，才能收发后续
		   2、收到Server的GameOver则返回0，游戏结束
		   3、其它错误均返回-1（报文信息不正确等错误），重连，再次重复	*/
		if (game_progress_GUI(client) < 0) {
			client.close();
			continue;
		}
		else {
			/* game_progress只有收到 GameOver 才返回 0 */
			break;
		}
	};

	client.close();
	cout << "游戏结束" << endl;

	return 0;
}

