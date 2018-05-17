# include<iostream>
# include"MFD.h"
# include"Auxiliary.h"
# include<algorithm>
using namespace std;
/*
* �û�ģ��
*/
void Login(){
	//����û��Ƿ����
	string username = Vec_Cmd_Line[1], pwd = Vec_Cmd_Line[2];
	int i;
	for (i = 0; i<Vec_Mfd.size(); i++){
		if (Vec_Mfd[i].name == username && Vec_Mfd[i].pwd == pwd)break;
	}
	if (i<Vec_Mfd.size()){
		cout << "��½�ɹ�" << endl;
		Cur_fn = Vec_Mfd[i].fn;
		CurID = i;
		//�л��û�
		show_path.clear();
		show_path.push_back(Vec_Mfd[CurID].fn->filename);
		show_path.push_back("\\");
	}
	else{ cout << "�û������� �� �������" << endl; }
}
void Register()
{
	if (CurID != 0){
		cout << "��ͨ�û�����ע��Ȩ��" << endl;
		return;
	}
	else
	{
		if (Vec_Cmd_Line.size() == 2){
			cout << "����Ϊ��" << endl;
		}
		else if (Vec_Cmd_Line.size() == 1){
			cout << "�û���Ϊ��" << endl;
		}
		else{
			string username = Vec_Cmd_Line[1], pwd = Vec_Cmd_Line[2];
			MFD t_mfd;
			t_mfd.name = username; t_mfd.pwd = pwd;
			//����һ���ļ��ڵ�
			t_mfd.fn = new FileNode;
			if (!t_mfd.fn){ cout << "�����ڴ�ʧ��" << endl; free(t_mfd.fn); return; }
			t_mfd.fn->filename = t_mfd.name;
			t_mfd.fn->parent = NULL;
			t_mfd.fn->lchild = NULL;
			t_mfd.fn->sibling_prev = NULL;
			t_mfd.fn->sibling_next = NULL;
			Vec_Mfd.push_back(t_mfd);
		}
	}

}
void Logout()
{
	cout << "\t\t�����˳�" << endl;
	CurID = 0;
	Cur_fn = Vec_Mfd[CurID].fn;
	show_path.clear();
	show_path.push_back(Vec_Mfd[CurID].fn->filename);
	show_path.push_back("\\");
}
void do_Exit()
{
	exit(0);
	cout << "\t\t�˳��ɹ�" << endl;
}

//1.�����ļ�
void CreateFile(){
	//����һ��·��������
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	//�ļ�Ȩ��
	string mode = "1";
	if (Vec_Cmd_Line.size() == 3)
	{
		mode = Vec_Cmd_Line[2];
	}
	//1.����·��
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0]){
		//�����·��ƥ��
		if (Absolute_path_is_match(Vec_Absolute_Path, Vec_Cmd_Line[1])){
			FileNode *CurPre;
			//����ļ�������
			if (is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				if (Create_File_Or_Dir_Node(Absolute_fn, CurPre, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], false, mode))
					cout << "�ļ������ɹ� ^_^" << endl;
				else cout << "����ʧ�� ^_^" << endl;
			}
			else{ cout << "�ļ��Ѵ��� ^_^" << endl; }
		}
		else{ cout << "·������ ^_^" << endl; }
		return;
	}
	//2.��ǰ �༶·��
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename != Vec_Absolute_Path[0]){
		//�����·��ƥ��
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[1])){
			FileNode *CurPre;
			//����ļ�������
			if (is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				if (Create_File_Or_Dir_Node(Absolute_fn, CurPre, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], false, mode))
					cout << "�ļ������ɹ� ^_^" << endl;
				else cout << "����ʧ�� ^_^" << endl;
			}
			else{ cout << "�ļ��Ѵ��� ^_^" << endl; }
		}
		else{ cout << "·������ ^_^" << endl; }
		return;
	}
	//3.��ǰ һ�� ·��
	if (Vec_Absolute_Path.size() == 1)
	{
		// �ļ����Ƿ�Ϸ�
		if (Cur_Path_Is_Match(Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			//������Դ���
			if (is_not_exists(Cur_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				if (Create_File_Or_Dir_Node(Cur_fn, CurPre, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], false, mode))
					cout << "�ļ������ɹ� ^_^" << endl;
				else cout << "����ʧ�� ^_^" << endl;
			}
			else{ cout << "�ļ��Ѵ��� ^_^" << endl; }
		}
		else{ cout << "��ǰ·����·������ ^_^" << endl; }
	}
}
//2.���ļ� open_file filename
void OpenFile()
{
	//����ﵽ���򿪴���
	if (Vec_Afd.size() == Max_Open_count){ cout << "���ļ��Ѵ�����޶ȣ��޷��ٴ�" << endl; return; }
	//����һ��·��������
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	//1.����·��
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0])
	{
		//���AFD
		bool flag = false;
		for (int i = 0; i < Vec_Afd.size(); i++)
		{
			if (Vec_Afd[i] == Vec_Absolute_Path){
				flag = true; break;
			}
		}
		if (flag)
		{
			cout << "�ļ��Ѵ� ^_^" << endl;
			return;
		}
		else{
			if (Absolute_path_is_match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
			{
				FileNode* CurPre;
				if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
				{
					cout << "�ļ��򿪳ɹ� ^_^" << endl;
					if (Vec_Afd.size() < Max_Open_count){
						Vec_Afd.push_back(Vec_Absolute_Path);
					}
				}
				else{ cout << "�ļ�������" << endl; }
			}
			else{ cout << "·����ƥ��" << endl; }
		}
		return;
	}

	//��ǰ�༶·��
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename != Vec_Absolute_Path[0])
	{
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[0], CurPre, false))
			{
				//curpre ָ��ǰ���ڵ��Ǹ��ļ�
				//��һ������·��
				Vec_Absolute_Path.clear();
				while (CurPre){
					Vec_Absolute_Path.push_back(CurPre->filename);
					CurPre = CurPre->parent;
				}
				//��ת
				reverse(Vec_Absolute_Path.begin(), Vec_Absolute_Path.end());
				//���AFD
				bool flag = false;
				for (int i = 0; i < Vec_Afd.size(); i++)
				{
					if (Vec_Afd[i] == Vec_Absolute_Path){
						flag = true; break;
					}
				}
				if (flag)
				{
					cout << "�ļ��Ѵ� ^_^" << endl;
					return;
				}
				else
				{
					//���뵽afd
					if (Vec_Afd.size() < Max_Open_count){
						Vec_Afd.push_back(Vec_Absolute_Path);
					}
					cout << "�ļ��򿪳ɹ�" << endl;
				}
			}
			else cout << "�����ڸ��ļ�" << endl;
		}
		else cout << "·����ƥ��" << endl;
		return;
	}

	//3.��ǰһ��·��
	if (Vec_Absolute_Path.size() == 1)
	{
		if (Cur_Path_Is_Match(Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Cur_fn, Vec_Absolute_Path[0], CurPre, false))
			{
				//curpre ָ��ǰ���ڵ��Ǹ��ļ�
				//��һ������·��
				Vec_Absolute_Path.clear();
				while (CurPre){
					Vec_Absolute_Path.push_back(CurPre->filename);
					CurPre = CurPre->parent;
				}
				//��ת
				reverse(Vec_Absolute_Path.begin(), Vec_Absolute_Path.end());
				//���AFD
				bool flag = false;
				for (int i = 0; i < Vec_Afd.size(); i++)
				{
					if (Vec_Afd[i] == Vec_Absolute_Path){
						flag = true; break;
					}
				}
				if (flag)
				{
					cout << "�ļ��Ѵ� ^_^" << endl;
					return;
				}
				else
				{
					//���뵽afd
					if (Vec_Afd.size() < Max_Open_count){
						Vec_Afd.push_back(Vec_Absolute_Path);
					}
					cout << "�ļ��򿪳ɹ�" << endl;
				}
			}
			else cout << "�����ڸ��ļ�" << endl;
		}
		else cout << "·����ƥ��" << endl;
	}

}
//3.�ر��ļ� close_file filename
void CloseFile()
{
	//����һ��·��������
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	//1.����·��
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0])
	{
		bool flag = false;
		int index = -1;
		for (int i = 0; i < Vec_Afd.size(); i++){
			if (Vec_Afd[i] == Vec_Absolute_Path){ flag = true; index = i; break; }
		}
		if (flag)
		{
			//����ļ��� AFD��
			Vec_Afd.erase((Vec_Afd.begin() + index));
			cout << "�ر��ļ�" << endl;
			return;
		}
		else{
			cout << "�ļ�û�д򿪻�����" << endl;
		}
	}
	//2.��ǰ�༶·��
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename != Vec_Absolute_Path[0])
	{
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				//��һ������·��
				Vec_Absolute_Path.clear();
				while (CurPre){
					Vec_Absolute_Path.push_back(CurPre->filename);
					CurPre = CurPre->parent;
				}
				//��ת
				reverse(Vec_Absolute_Path.begin(), Vec_Absolute_Path.end());
				bool flag = false;
				int index = -1;
				for (int i = 0; i < Vec_Afd.size(); i++){
					if (Vec_Afd[i] == Vec_Absolute_Path){ flag = true; index = i; break; }
				}
				if (flag)
				{
					//����ļ��� AFD��
					Vec_Afd.erase((Vec_Afd.begin() + index));
					cout << "�ر��ļ��ɹ�" << endl;
					return;
				}
				else{
					cout << "�ļ�û�д򿪻�����" << endl;
				}
			}
		}
		else{
			cout << "·����ƥ��" << endl;
		}
	}

	//3.��ǰ1��·��
	if (Vec_Absolute_Path.size() == 1)
	{
		if (Cur_Path_Is_Match(Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Cur_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				//��һ������·��
				Vec_Absolute_Path.clear();
				while (CurPre){
					Vec_Absolute_Path.push_back(CurPre->filename);
					CurPre = CurPre->parent;
				}
				//��ת
				reverse(Vec_Absolute_Path.begin(), Vec_Absolute_Path.end());
				bool flag = false;
				int index = -1;
				for (int i = 0; i < Vec_Afd.size(); i++){
					if (Vec_Afd[i] == Vec_Absolute_Path){ flag = true; index = i; break; }
				}
				if (flag)
				{
					//����ļ��� AFD��
					Vec_Afd.erase((Vec_Afd.begin() + index));
					cout << "�ر��ļ��ɹ�" << endl;
					return;
				}
				else{
					cout << "�ļ�û�д򿪻�����" << endl;
				}
			}
		}
		else{
			cout << "·����ƥ��" << endl;
		}
	}

}
//4.ɾ���ļ� delete_file filename
void DeleteFile(){

	//����һ��·��������
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	//1.����·��
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0])
	{
		if (Absolute_path_is_match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				cout << "�ҵ���" << endl;
				Delete_Fn(Absolute_fn, CurPre);
				cout << "ɾ���ɹ�" << endl;
			}
			else cout << "�ļ�������" << endl;
		}
		else cout << "·����ƥ��" << endl;
		return;
	}
	//2.��ǰ�༶·��
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0])
	{
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				cout << "�ҵ���" << endl;
				Delete_Fn(Absolute_fn, CurPre);
				cout << "ɾ���ɹ�" << endl;
			}
			else cout << "�ļ�������" << endl;
		}
		else cout << "·����ƥ��" << endl;
		return;
	}
	//3.��ǰ1��·��
	if (Vec_Absolute_Path.size() == 1)
	{
		if (Cur_Path_Is_Match(Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Cur_fn, Vec_Absolute_Path[0], CurPre, false))
			{
				cout << "�ҵ���" << endl;
				//��һ������·��
				FileNode *CurPre_t = CurPre;
				Vec_Absolute_Path.clear();
				while (CurPre_t){
					Vec_Absolute_Path.push_back(CurPre_t->filename);
					CurPre_t = CurPre_t->parent;
				}
				//��ת
				reverse(Vec_Absolute_Path.begin(), Vec_Absolute_Path.end());
				//ɾ���ڵ�
				Delete_Fn(Cur_fn, CurPre);
				cout << "ɾ���ɹ�" << endl;
			}
			else cout << "�ļ�������" << endl;
		}
		else cout << "·����ƥ��" << endl;
	}
	//���Afd ��
	bool flag = false;
	int index = -1;
	for (int i = 0; i < Vec_Afd.size(); i++)
	{
		if (Vec_Afd[i] == Vec_Absolute_Path)
		{
			flag = true;
			index = i;
			break;
		}
	}
	if (flag)
	{
		Vec_Afd.erase(Vec_Afd.begin() + index);
		cout << "�� afd ��ɾ��" << endl;
	}

}
//5.��ʾ�ļ�
void Is_Dir()
{
	//1.��ǰ1��·��
	if (Vec_Cmd_Line.size() == 1)
	{
		FileNode *child = Cur_fn->lchild;
		while (child)
		{
			child->show();
			child = child->sibling_next;
		}
		return;
	}
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	string temp = Vec_Cmd_Line[1];
	if (temp[temp.size() - 1] == '/')temp.pop_back();
	//2.��ǰ�༶·��
	if (Vec_Cmd_Line.size() == 2 && Vec_Absolute_Path[0] != Vec_Mfd[CurID].fn->filename)
	{
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, temp))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
			{
				FileNode *child = CurPre->lchild;
				while (child)
				{
					child->show();
					child = child->sibling_next;
				}
			}
			else cout << "���һ����Ŀ¼������" << endl;
		}
		else cout << "·����ƥ��" << endl;
		return;
	}
	//3.����·��
	if (Vec_Cmd_Line.size() > 1 && Vec_Absolute_Path[0] == Vec_Mfd[CurID].fn->filename)
	{
		if (Absolute_path_is_match(Vec_Absolute_Path, temp))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
			{
				FileNode *child = CurPre->lchild;
				while (child)
				{
					child->show();
					child = child->sibling_next;
				}
			}
			else cout << "���һ����Ŀ¼������" << endl;
		}
		else cout << "·����ƥ��" << endl;
	}
}
//6.����Ŀ¼
void Mk_Dir()
{
	//����һ��·��������
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	string mode = "1";
	//1.����·��
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0]){
		//�����·��ƥ��
		if (Absolute_path_is_match(Vec_Absolute_Path, Vec_Cmd_Line[1])){
			FileNode *CurPre;
			//����ļ�������
			if (is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
			{
				if (Create_File_Or_Dir_Node(Absolute_fn, CurPre, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], true, mode))
					cout << "Ŀ¼�����ɹ� ^_^" << endl;
				else cout << "����ʧ�� ^_^" << endl;
			}
			else{ cout << "Ŀ¼�Ѵ��� ^_^" << endl; }
		}
		else{ cout << "·������ ^_^" << endl; }
		Absolute_fn = NULL;
		return;
	}
	//2.��ǰ �༶ ·��
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename != Vec_Absolute_Path[0]){
		//�����·��ƥ��
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[1])){
			FileNode *CurPre;
			//����ļ�������
			if (is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
			{
				if (Create_File_Or_Dir_Node(Absolute_fn, CurPre, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], true, mode))
					cout << "Ŀ¼�����ɹ� ^_^" << endl;
				else cout << "����ʧ�� ^_^" << endl;
			}
			else{ cout << "Ŀ¼�Ѵ��� ^_^" << endl; }
		}
		else{ cout << "·������ ^_^" << endl; }
		return;
	}

	//3.��ǰ 1�� ·��
	if (Vec_Absolute_Path.size() == 1)
	{
		// �ļ����Ƿ�Ϸ�
		if (Cur_Path_Is_Match(Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			//������Դ���
			if (is_not_exists(Cur_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
			{
				if (Create_File_Or_Dir_Node(Cur_fn, CurPre, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], true, mode))
					cout << "Ŀ¼�����ɹ� ^_^" << endl;
				else cout << "����ʧ�� ^_^" << endl;
			}
			else{ cout << "Ŀ¼�Ѵ��� ^_^" << endl; }
		}
		else{ cout << "��ǰ·����·������ ^_^" << endl; }
	}
}
//7.ɾ��Ŀ¼
void Rv_Dir()
{
	//����һ��·��������
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	//1.����·��
	if (Vec_Absolute_Path.size() > 1 && Vec_Absolute_Path[0] == Vec_Mfd[CurID].fn->filename)
	{
		if (Absolute_path_is_match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
			{
				if (CurPre)
				{
					cout << "��ʾ: Ŀ¼��Ϊ�գ��Ƿ�ɾ��  ��y : ɾ�� , N : ��ɾ��) , ��ѡ��:";
					char flag;
					cin >> flag;
					if (flag == 'Y' || flag == 'y')
					{
						Delete_Fn(Absolute_fn, CurPre);
						cout << "ɾ���ɹ�" << endl;
						return;
					}
				}
				else
				{
					Delete_Fn(Absolute_fn, CurPre);
					cout << "ɾ���ɹ�" << endl;
					return;
				}

			}
			else cout << "Ŀ¼������" << endl;
		}
		else cout << "·����ƥ��" << endl;
		return;
	}
	//2.��ǰ�༶·��
	if (Vec_Absolute_Path.size() > 1 && Vec_Absolute_Path[0] != Vec_Mfd[CurID].fn->filename)
	{
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
			{
				if (CurPre)
				{
					cout << "��ʾ: Ŀ¼��Ϊ�գ��Ƿ�ɾ��  ��y : ɾ�� , N : ��ɾ��) , ��ѡ��:";
					char flag;
					cin >> flag;
					if (flag == 'Y' || flag == 'y')
					{
						Delete_Fn(Absolute_fn, CurPre);
						cout << "ɾ���ɹ�" << endl;
						return;
					}
				}
				else
				{
					Delete_Fn(Absolute_fn, CurPre);
					cout << "ɾ���ɹ�" << endl;
					return;
				}
			}
			else cout << "Ŀ¼������" << endl;
		}
		else cout << "·����ƥ��" << endl;
		return;
	}

	//3.��ǰ 1 ��·��
	if (Vec_Absolute_Path.size() == 1)
	{
		if (Cur_Path_Is_Match(Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Cur_fn, Vec_Absolute_Path[0], CurPre, true))
			{
				if (CurPre)
				{
					cout << "��ʾ: Ŀ¼��Ϊ�գ��Ƿ�ɾ��  ��y : ɾ�� , N : ��ɾ��) , ��ѡ��:";
					char flag;
					cin >> flag;
					if (flag == 'Y' || flag == 'y')
					{
						Delete_Fn(Absolute_fn, CurPre);
						cout << "ɾ���ɹ�" << endl;
						return;
					}
				}
				else
				{
					Delete_Fn(Absolute_fn, CurPre);
					cout << "ɾ���ɹ�" << endl;
					return;
				}
			}
			else cout << "�ļ�������" << endl;
		}
		else cout << "·����ƥ��" << endl;
	}
}
//8.�л�Ŀ¼
void Cd_Dir(){
	//1. cd_dir 
	if (Vec_Cmd_Line.size() == 1)
	{
		for_each(show_path.begin(), show_path.end(), print_path);
		cout << endl;
		return;
	}
	//2. cd ..
	if (Vec_Cmd_Line.size() == 2 && Vec_Cmd_Line[1] == "..")
	{
		if (Cur_fn->parent)
		{
			Cur_fn = Cur_fn->parent;
			show_path.pop_back();
			show_path.pop_back();
		}
		else
		{
			cout << "û�и�Ŀ¼" << endl;
			return;
		}
		return;
	}
	//3. cd .
	if (Vec_Cmd_Line.size() == 2 && Vec_Cmd_Line[1] == "."){ return; }
	//·������
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	string str = Vec_Cmd_Line[1];
	if (str[str.size() - 1] == '/')str.pop_back();

	//3.cd ��ǰ·��һ��Ŀ¼
	if (Vec_Absolute_Path.size() == 1)
	{
		if (Cur_Path_Is_Match(str))
		{
			FileNode *CurPre;
			if (!is_not_exists(Cur_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
			{
				Cur_fn = CurPre;
				for (int i = 0; i < Vec_Absolute_Path.size(); i++)
				{
					show_path.push_back(Vec_Absolute_Path[i]);
					show_path.push_back("\\");
				}
			}
			else{ cout << "·����ƥ��" << endl; }
		}
		else{ cout << "·����ƥ��" << endl; }
		return;
	}

	if (Vec_Absolute_Path.size() > 1)
	{
		//4.cd ��ǰ·�� �༶Ŀ¼
		if (Vec_Absolute_Path[0] != Vec_Mfd[CurID].fn->filename)
		{
			if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, str)){
				FileNode *CurPre;
				if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
				{
					Cur_fn = CurPre;
					for (int i = 0; i < Vec_Absolute_Path.size(); i++)
					{
						show_path.push_back(Vec_Absolute_Path[i]);
						show_path.push_back("\\");
					}
				}
				else cout << "·����ƥ��" << endl;
			}
			else cout << "·����ƥ��" << endl;
			return;
		}
		//5.cd ����·��
		if (Vec_Absolute_Path[0] == Vec_Mfd[CurID].fn->filename)
		{
			if (Absolute_path_is_match(Vec_Absolute_Path, str)){
				FileNode *CurPre;
				if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
				{
					Cur_fn = CurPre;
					show_path.clear();
					for (int i = 0; i < Vec_Absolute_Path.size(); i++)
					{
						show_path.push_back(Vec_Absolute_Path[i]);
						show_path.push_back("\\");
					}
				}
				else cout << "·����ƥ��" << endl;
			}
			else cout << "·����ƥ��" << endl;
		}
	}


}

//9.�����ļ�
void Copy_File()
{
	//1.·�� copy_file  [path1]/file  [path2]/file
	if (Vec_Cmd_Line.size()<3){ cout << "·�����Ի��﷨����" << endl; return; }
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);

	//·��1���ļ��������  ·��2���ļ����ڿɲ����� ����Ϊ�Զ����
	//1.·��һ�Ǿ���·��
	if (Vec_Absolute_Path.size()>1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0])
	{
		if (Absolute_path_is_match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				Aux_Copy(CurPre);//�ҵ�������ļ�
			}
			else cout << "Դ�ļ�������" << endl;
		}
		else cout << "Դ�ļ�·������" << endl;

		return;
	}
	//2.·��һ�ǵ�ǰ�༶·��	
	if (Vec_Absolute_Path.size()>1 && Vec_Mfd[CurID].fn->filename != Vec_Absolute_Path[0])
	{
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				Aux_Copy(CurPre);
			}
			else cout << "Դ�ļ�������" << endl;
		}
		else cout << "Դ�ļ�·������" << endl;
		return;
	}
	//3.·��һ�ǵ�ǰ·��
	if (Vec_Absolute_Path.size() == 1)
	{
		if (Cur_Path_Is_Match(Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Cur_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				Aux_Copy(CurPre);
			}
			else cout << "Դ�ļ�������" << endl;
		}
		else cout << "Դ�ļ�·������" << endl;
	}
}
void Read_File()
{
	cout << "�߼�û���壬ûд" << endl;
}
void Write_File()
{
	cout << "�߼�û���壬ûд" << endl;
}