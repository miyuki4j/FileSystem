# include<iostream>
# include<sstream>
# include<stdlib.h>
# include"MFD.h"
# include"Command.h"
using namespace std;
/*
һЩȫ�ֱ���
*/
//�൱��MFD�� �洢���� �û������û��ļ�Ŀ¼
vector<MFD> Vec_Mfd;
//�ļ��򿪱� ��¼�û����ļ��ĸ���
vector<vector<string>> Vec_Afd;
//������
vector<string>Vec_Cmd_Line;
//��������ľ���·��·��
vector<string>Vec_Absolute_Path;
//��ʱ�õ��ڶ���·��
vector<string>Vec_Aux_Absolute_Path;
//����·�����ļ��ڵ� ȫ�ֿɱ䶯
FileNode *Absolute_fn;
//��ǰ·�����ļ��ڵ�
FileNode *Cur_fn;
//��ǰ�û�ID Ĭ��root
int CurID;
//�������ñ�
vector<Handler>Vec_Handler_List;
//ȫ������
vector<string> show_path;
//��ʼ��
void Init_MFD_Table(){
	string temp_name[Max_User_Num] = { "root", "u1", "u2", "u3", "u4", "u5", "u6", "u7", "u8", "u9", };
	string pwd = "123456";
	for (int i = 0; i<Max_User_Num; i++){
		MFD t_mfd;
		t_mfd.name = temp_name[i];
		t_mfd.pwd = pwd;
		//����һ���ļ��ڵ�
		t_mfd.fn = new FileNode;
		if (!t_mfd.fn){ cout << "�����ڴ�ʧ��" << endl; free(t_mfd.fn); return; }
		t_mfd.fn->filename = t_mfd.name;
		t_mfd.fn->isdir = true;
		t_mfd.fn->parent = NULL;
		t_mfd.fn->lchild = NULL;
		t_mfd.fn->sibling_prev = NULL;
		t_mfd.fn->sibling_next = NULL;
		Vec_Mfd.push_back(t_mfd);
	}
	Absolute_fn = NULL;
	//Ĭ��Ϊ root ��½
	CurID = 0;
	Cur_fn = Vec_Mfd[CurID].fn;
	show_path.push_back(Vec_Mfd[CurID].fn->filename);
	show_path.push_back("\\");
}
//��ʼ�� ��������
void Init_Handler()
{
	int handler_count=15;
	Handler handlerlist[] = { { "Login", Login }, { "Register", Register }, { "Logout", Logout }, { "Exit", do_Exit }, 
							  { "touch_file", CreateFile }, { "open_file", OpenFile }, { "close_file", CloseFile }, 
							  { "delete_file", DeleteFile }, { "is_dir", Is_Dir }, { "mk_dir", Mk_Dir }, { "rv_dir", Rv_Dir },
							  { "cd_dir", Cd_Dir }, { "copy_file", Copy_File }, { "read_file", Read_File }, {"write_file",Write_File}
							};
	
	for (int i = 0; i<handler_count; i++)
		Vec_Handler_List.push_back(handlerlist[i]);
}
//�������봦��
void Cmd_Line_Solve()
{
	string src, temp;
	getline(cin, src);
	stringstream ss(src);
	Vec_Cmd_Line.clear();
	while (ss >> temp)Vec_Cmd_Line.push_back(temp);
	//���������б�
	int i;
	for (i = 0; i < Vec_Handler_List.size(); i++)if (Vec_Cmd_Line[0] == Vec_Handler_List[i].pname)break;
	if (i<Vec_Handler_List.size()){
		//��������
		Vec_Handler_List[i].handler();
	}
	else cout << "�������" << endl;
}
//������
int main()
{
	Init_MFD_Table();
	help();
	Init_Handler();
	while (true){
		for (int i = 0; i<show_path.size(); i++)
			cout << show_path[i];
		cout << ">";
		Cmd_Line_Solve();
	} 
	return 0;
}
