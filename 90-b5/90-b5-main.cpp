/* 1751957 ��2�� ������ */
#define _CRT_SECURE_NO_WARNINGS
#include "BplC.cpp"
using namespace std;

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ѧ��+MD5(password)����security_str��λ�����γ�Ҫ���͵ġ�����������
  ���������char *reg_str				��������Ҫ���͵Ĵ������أ�
			const char *stu_no			��ѧ��
			const char *stu_password	������ҵϵͳ�еĿ�����룩�����ۿ���೤��ת��ΪMD5��Ϊ32�ֽڵ�hex��
			const char *security_str	����Server���յ��������ܴ�
  �� �� ֵ��
  ˵    ������ѧ�� : 1859999 , ���� : Password����� : f272a9b7422ee1ddec6c4b1abe758cadefc658c2 Ϊ��
			1��Password     �� => MD5 => ��dc647eb65e6711e155375218212b3964��
			2����֤��(ԭʼ) ��1859999+dc647eb65e6711e155375218212b3964
			3����֤��(ԭʼ)����򴮽��а�λ���
				   1859999+dc647eb65e6711e155375218212b3964
				   f272a9b7422ee1ddec6c4b1abe758cadefc658c2
			   ���а�λ��򣬽������֤һ����ͼ��ASCII�ַ���������תΪhex
			   ��֤��(Hex����) ��570a020b58005b1c50510451525406525006005405535450575004020d51505c5757515406015506
			   ���ͣ���1���ֽ� 1��f���� 0x31^0x66 = 0x57
					 ��2���ֽ� 8��2���� 0x38^0x32 = 0x0a
					 ...(��)
***************************************************************************/
int cmd_tcp_socket::make_register_string(char *send_regstr, const char *stu_no, const char *stu_password, const char *security_str)
{
	/* ��Ҫ�Լ�ʵ�� */
	char md5_password[32 + 1];
	MD5(md5_password, stu_password);
	cout << "����(���ܺ�)��" << md5_password << endl;
	char org_regstr[40 + 1];
	strcpy(org_regstr, stu_no);
	strcat(org_regstr, "-");
	strcat(org_regstr, md5_password);
	cout << "��֤��(ԭʼ)��" << org_regstr << endl;
	cout << "���      ��" << security_str << endl;
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
	cout << "��֤��(Hex����) ��" << send_regstr << endl;
	return 0;
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int game_progress(cmd_tcp_socket &client)
{
	char sel;
	char row, head_row, tail_row;
	int col, head_col, tail_col;
	bool recv_startgame = false;

	while (1) {
		/* �ȴ�Server�˵�gameprogress */
		string spack;
		if (client.get_gameprogress_string(spack) < 0) {
			return -1;
		}
		cout << "ServerӦ�� : " << spack << endl;

		if (spack == "StartGame")
			recv_startgame = true;

		/* û�յ�StartGameǰ����������Ϣ����Ϊ���� */
		if (!recv_startgame)
			return -1;

		if (spack == "GameOver") {
			cout << "����GameID : " << client.get_gameid() << endl;
			cout << "���ε÷�   : " << client.get_score() << endl;
			return 0;
		}

		cout << "1.����һ������" << endl;
		cout << "2.�������ܷɻ�����" << endl;
		while (1) {
			sel = _getch();
			if (sel == '1' || sel == '2')
				break;
		}
		switch (sel) {
		case '1':
			cout << "��������(A-J)��(0-9)���� : ";
			cin >> row >> col; //�˴�δ�ж���ȷ��
			if (row >= 'a' && row <= 'j')
				row -= 32;
			client.send_coordinate(row, col);
			break;
		case '2':
			cout << "�������ͷ��(A-J)��(0-9)���� : ";
			cin >> head_row >> head_col; //�˴�δ�ж���ȷ��
			if (head_row >= 'a' && head_row <= 'j')
				head_row -= 32;
			cout << "�������β������(A-J)��(0-9)���� : ";
			cin >> tail_row >> tail_col; //�˴�δ�ж���ȷ��
			if (tail_row >= 'a' && tail_row <= 'j')
				tail_row -= 32;
			client.send_plane_coordinates(head_row, head_col, tail_row, tail_col);
			break;
		}//end of switch
	}//end of while(1)
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
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
			/* �ȴ�Server�˵�gameprogress */
			string spack;
			if (client.get_gameprogress_string(spack) < 0) {
				return -1;
			}

			//cout << "ServerӦ�� : " << spack << endl;
			sprintf(buffer, "ServerӦ�� :  %s", (char*)spack.data());
			gmw_status_line(&bpl.BplC_CGI, TOP_STATUS_LINE, buffer);

			if (spack == "StartGame")
				recv_startgame = true;
			else
				bpl.Explode_animation(row, col, head_row, head_col, tail_row, tail_col, sel, spack);
			/* û�յ�StartGameǰ����������Ϣ����Ϊ���� */
			if (!recv_startgame)
				return -1;

			if (spack == "GameOver") {
				char tem1[CHAR_LEN], tem2[CHAR_LEN];
				sprintf(tem1, "����GameID : %s", client.get_gameid());
				sprintf(tem2, "���ε÷�   :  %d", client.get_score());
				gmw_status_line(&bpl.BplC_CGI, LOWER_STATUS_LINE, tem1);
				//cout << "����GameID : " << client.get_gameid() << endl;
				cout << endl << "���ε÷�   : " << client.get_score() << endl;
				getchar();
				break;
			}

			sel = bpl.Select_space_by_mouse(row, col, head_row, head_col, tail_row, tail_col);
			//cout <<"�����"<< row << col;
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
  �������ƣ�
  ��    �ܣ�
  ���������argv[1] : "-auto"/"-manual"����ʾ�Զ�/�˹���Ϸ
			argv[2] : ѧ��
			argv[3] : ����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int main(int argc, char **argv)
{
	if (argc != 4) {
		cout << "Usage : " << argv[0] << " -auto|-manual  stu_no  stu_pwd" << endl;
		return -1;
	}

	/* ������ȷ��ʾ��demo */
	if (strcmp(argv[1], "-auto") == 0) {
		cout << "Demo��δʵ���Զ���Ϸ" << endl;
		return -1;
	}

	/* ��������Լ��δ������Ҫ�Լ���
	   1��argv[1]�� -auto -manual �������
	   2��ѧ��7λ
	   3��... */

	const int sleep_ms = 5000;
	bool first = true;
	cmd_tcp_socket client;

	/* ��client������е�debug���أ�����ʱ�ɴ򿪣���ͼ�ν���ʱ��رգ� */
	client.set_debug_switch(false);

	while (1) {
		if (!first) { //���κδ�����ʱ5����������������һ�Σ�
			cout << "������������ӶϿ�!" << endl;
			Sleep(sleep_ms);
		}
		else
			first = false;

		/* ���ӷ����� */
		if (client.connect() < 0) {
			/* ����ʧ����Ϣ����û��debug_switch����ӡ */
			cout << "���ӷ�����ʧ��!" << endl;
			continue;//return -1;
		}

		/* ��ȡ���������͹����ļ��ܴ���֤��(��40���ַ�) */
		string s1;
		if (client.get_security_string(s1) < 0) {
			client.close();
			continue; // return -2;
		}
		/* ��ӡ */
		cout << "��ȡ�������ܴ� : " << s1 << endl;

		/* ��ѧ��+MD5�����������ܴ����� ���͸�Server��
		   ע�⣺
		   1������һ����80�ֽڣ���β��81
		   2��make_register_string��Ҫ�Լ�ʵ��
		   3���ɲο�tmake_register_string������ʱ�Ὣ�˺�����lib���Ƴ�   */
		char reg_str[81];
		//client.tmake_register_string(reg_str, "1850000", "185-0*0%0@0", s1.c_str()); //Ԥ�õĲ����û�����ʧЧ��
		client.make_register_string(reg_str, argv[2], argv[3], s1.c_str()); //���Լ���ѧ�ź�����

		/* ����֤�����͹�ȥ */
		client.send_register_string(reg_str);

		/* ������Ϸ��������
		   1�������յ�Server��StartGame�������շ�����
		   2���յ�Server��GameOver�򷵻�0����Ϸ����
		   3���������������-1��������Ϣ����ȷ�ȴ��󣩣��������ٴ��ظ�	*/
		if (game_progress_GUI(client) < 0) {
			client.close();
			continue;
		}
		else {
			/* game_progressֻ���յ� GameOver �ŷ��� 0 */
			break;
		}
	};

	client.close();
	cout << "��Ϸ����" << endl;

	return 0;
}

