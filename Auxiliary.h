# include<iostream>
# include<vector>
# include<string>
# include<cstring>
# include"MFD.h"
using namespace std;

void help()
{
	cout << "------------------��ӭʹ�������ļ�����ϵͳ--------------just for ypp" << endl;
	cout << "---------------------ϵͳ������˵������-----------------------------" << endl;
	cout << endl;
	cout << "--------------------------�û�����----------------------------------" << endl;
	cout << "1.�û���½ Login username  pwd--------------------------------------" << endl;
	cout << "2.�û�ע�� Register username  pwd-----------------------------------" << endl;
	cout << "3.�û�ע�� Logout --------------------------------------------------" << endl;
	cout << "4.�˳�ϵͳ Exit ----------------------------------------------------" << endl;
	cout << endl;
	cout << "--------------------------�ļ�����----------------------------------" << endl;
	cout << "1.�����ļ�  touch_file  filename mode(0--ֻ��1-��д2--ִ��3--������)" << endl;
	cout << "2.���ļ�  open_file   filename -----------------------------------" << endl;
	cout << "3.�ر��ļ�  close_file  filename -----------------------------------" << endl;
	cout << "4.ɾ���ļ�  delete_file filename -----------------------------------" << endl;
	cout << "5.д���ļ�  write_file  context  [path]/file------------------------" << endl;
	cout << "6.��ȡ�ļ�  read_file   context  [path]/file------------------------" << endl;
	cout << "7.�����ļ�  copy_file   [path1]/file  [path2]/file------------------" << endl;
	cout << endl;
	cout << "--------------------------Ŀ¼����----------------------------------" << endl;
	cout << "1.����Ŀ¼  mk_dir      dirname-------------------------------------" << endl;
	cout << "2.ɾ��Ŀ¼  rv_dir      dirname-------------------------------------" << endl;
	cout << "3.�л�Ŀ¼  cd_dir      path----------------------------------------" << endl;
	cout << "4.��ʾĿ¼  is_dir      [path]--------------------------------------" << endl;
}

//�ָ��ַ������� ���� vector<string>
vector<string> split(const string& str, const string& delim)
{
	vector<string> res;
	if ("" == str) return res;
	//�Ƚ�Ҫ�и���ַ�����string����ת��Ϊchar*����  
	char * strs = new char[str.length() + 1]; //��Ҫ����  
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());
	char *p = strtok(strs, d);
	while (p) {
		string s = p; //�ָ�õ����ַ���ת��Ϊstring����  
		res.push_back(s); //����������  
		p = strtok(NULL, d);
	}
	return res;
}
//����·������
void Path_Deal_With(const string &path, vector<string> &Cur_Absolute_Path)
{
	//����һ��
	Cur_Absolute_Path.clear();
	//�ַ����ָ�
	Cur_Absolute_Path = split(path, "/");
}
//����·��ƥ�亯��  ˳�����þ���·�����һ�㸸�ļ��ڵ�ָ��
bool Absolute_path_is_match(const vector<string>&split_path, const string &original_path)
{
	//1.����ָ����ĸ������� Ŀ¼�ĸ���-1 ��ƥ��
	int count = 0;
	for (int i = 0; i < original_path.size(); i++)if (original_path[i] == '/')count++;
	if (count != (split_path.size() - 1))return false;
	//2.��һ�����û���Ŀ¼ȥ�ȶ� 
	Absolute_fn = Vec_Mfd[CurID].fn;
	for (int i = 0; i < split_path.size() - 1; i++){
		bool flag = false;
		while (Absolute_fn)
		{
			if (Absolute_fn->filename == split_path[i] && Absolute_fn->isdir == true){ flag = true; break; }
			Absolute_fn = Absolute_fn->sibling_next;
		}
		if (flag){ if (i != split_path.size() - 2) Absolute_fn = Absolute_fn->lchild; }
		else return false;
	}
	return true;
}
//��ǰһ��·�����ж�
bool Cur_Path_Is_Match(string &CurPath){
	if (CurPath[0] == '/' || CurPath[CurPath.size() - 1] == '/')return false;
	return  true;
}
//��ǰ�༶·�����ж�
bool Cur_Sub_path_Is_Match(vector<string>&split_path, string original_path)
{
	int count = 0;
	for (int i = 0; i < original_path.size(); i++)if (original_path[i] == '/')count++;
	if (count != (split_path.size() - 1))return false;
	if (split_path.size() == 1){ Absolute_fn = Cur_fn; return true; }
	Absolute_fn = Cur_fn->lchild;
	for (int i = 0; i < split_path.size() - 1; i++){
		bool flag = false;
		while (Absolute_fn)
		{
			if (Absolute_fn->filename == split_path[i] && Absolute_fn->isdir == true){ flag = true; break; }
			Absolute_fn = Absolute_fn->sibling_next;
		}
		if (flag){ if (i != split_path.size() - 2) Absolute_fn = Absolute_fn->lchild; }
		else return false;
	}
	return true;
}
//�ж� Ҫ�������ļ�����Ŀ¼�Ƿ����
bool is_not_exists(FileNode *&CurParentFn, string &new_sub_name, FileNode *&CurPre, bool type)
{
	bool flag = true;
	//1.û�����ļ�
	if (!CurParentFn->lchild){ CurPre = CurParentFn; return true; }
	//2.�����ļ�
	else{
		FileNode *t1;
		t1 = CurParentFn->lchild;
		while (t1){
			if (t1->filename == new_sub_name && t1->isdir == type){ flag = false; CurPre = t1; break; }
			CurPre = t1;
			t1 = t1->sibling_next;
		}
	}
	return flag;
}
//�������ļ�/��Ŀ¼
bool Create_File_Or_Dir_Node(FileNode *&Parent, FileNode *&CurPre, const string filename, bool isdir, string mode)
{
	//����һ���µ����ļ�
	FileNode *source = new FileNode;
	if (!source){ cout << "����ʧ��" << endl; return false; }
	source->filename = filename;
	source->mode = mode[0] - '0';
	source->isdir = isdir;
	//ָ����
	source->parent = Parent;
	//�ǵ�һ��
	if (!Parent->lchild){ Parent->lchild = source; }
	//���ǵ�һ��
	else{ CurPre->sibling_next = source; source->sibling_prev = CurPre; }
	return true;
}
// for_each ʹ�õĴ�ӡ����
void print_path(string &str)
{
	cout << str;
}
//�ͷ��ļ��ڵ�
void Free_fn(FileNode *fn)
{
	fn->lchild = NULL;
	fn->sibling_next = NULL;
	fn->sibling_prev = NULL;
	fn->lchild = NULL;
	//delete fn;
}
//�ͷ��ļ��ڵ�
void Delete_Fn(FileNode *&parenfn, FileNode *&curfn)
{
	//��һ��
	if (parenfn->lchild == curfn)
	{
		if (curfn->sibling_next != NULL)
		{
			parenfn->lchild = curfn->sibling_next;
			curfn->sibling_next->sibling_prev = NULL;
			Free_fn(curfn);
			return;
		}
		if (curfn->sibling_next == NULL)
		{
			parenfn->lchild = NULL;
			Free_fn(curfn);
			return;
		}
	}
	//�м��
	if (parenfn->lchild != curfn)
	{
		if (curfn->sibling_next != NULL)
		{
			curfn->sibling_next->sibling_prev = curfn->sibling_prev;
			curfn->sibling_prev->sibling_next = curfn->sibling_next;
			Free_fn(curfn);
			return;
		}
		if (curfn->sibling_next == NULL)
		{
			curfn->sibling_prev->sibling_next = NULL;
			Free_fn(curfn);
			return;
		}
	}
}
// copy_file �õĸ�������
bool Copy_Create_File_Or_Node(FileNode *&Parent, FileNode *&CurPre, FileNode *&fn_source, string ffilename)
{
	//����һ���µ����ļ�
	FileNode *source = new FileNode;
	if (!source){ cout << "����ʧ��" << endl; return false; }
	//ָ����
	source->parent = Parent;
	//�ǵ�һ��
	if (!Parent->lchild){ Parent->lchild = source; }
	//���ǵ�һ��
	else{ CurPre->sibling_next = source; source->sibling_prev = CurPre; }
	source->filename = ffilename;
	source->isdir = fn_source->isdir;
	source->mode = fn_source->mode;
	source->rwp = fn_source->rwp;
	return true;

}
void Aux_Copy(FileNode *&fn_source)
{
	string str = Vec_Cmd_Line[2];
	if (str[str.size() - 1] == '/')
	{
		string name = fn_source->filename;
		Vec_Cmd_Line[2]+=name;
	}
	Path_Deal_With(Vec_Cmd_Line[2], Vec_Absolute_Path);
	//1.·�����Ǿ���·��
	if (Vec_Absolute_Path.size()>1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0])
	{
		if (Absolute_path_is_match(Vec_Absolute_Path, Vec_Cmd_Line[2]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				cout << "Ŀ���ļ�����,���Ƴɹ�" << endl;

			}
			else
			{
				string name = Vec_Absolute_Path[Vec_Absolute_Path.size() - 1];
				if (Copy_Create_File_Or_Node(Absolute_fn, CurPre, fn_source, name))
				{
					cout << "�ļ������ɹ�" << endl;
				}
				else cout << "�ļ�����ʧ��" << endl;
			}

		}
		else cout << "Ŀ���ļ�·������" << endl;
		return;
	}
	//2.·�����ǵ�ǰ�༶·��	
	if (Vec_Absolute_Path.size()>1 && Vec_Mfd[CurID].fn->filename != Vec_Absolute_Path[0])
	{
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[2]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				cout << "Ŀ���ļ�����,���Ƴɹ�" << endl;

			}
			else
			{
				string name = Vec_Absolute_Path[Vec_Absolute_Path.size() - 1];
				if (Copy_Create_File_Or_Node(Absolute_fn, CurPre, fn_source, name))
				{
					cout << "�ļ������ɹ�" << endl;
				}
				else cout << "�ļ�����ʧ��" << endl;
			}

		}
		else cout << "Ŀ���ļ�·������" << endl;
		return;
	}
	//3.·�����ǵ�ǰ·��
	if (Vec_Absolute_Path.size() == 1)
	{
		if (Cur_Path_Is_Match(Vec_Cmd_Line[2]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Cur_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				cout << "Ŀ���ļ�����,���Ƴɹ�" << endl;

			}
			else
			{
				string name = Vec_Absolute_Path[Vec_Absolute_Path.size() - 1];
				if (Copy_Create_File_Or_Node(Absolute_fn, CurPre, fn_source, name))
				{
					cout << "�ļ������ɹ�" << endl;
				}
				else cout << "�ļ�����ʧ��" << endl;
			}

		}
		else cout << "Ŀ���ļ�·������" << endl;
	}
}