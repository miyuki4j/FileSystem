# include<iostream>
# include<vector>
# include<string>
# include<cstring>
# include"MFD.h"
using namespace std;

void help()
{
	cout << "------------------欢迎使用树形文件管理系统--------------just for ypp" << endl;
	cout << "---------------------系统操作及说明如下-----------------------------" << endl;
	cout << endl;
	cout << "--------------------------用户操作----------------------------------" << endl;
	cout << "1.用户登陆 Login username  pwd--------------------------------------" << endl;
	cout << "2.用户注册 Register username  pwd-----------------------------------" << endl;
	cout << "3.用户注销 Logout --------------------------------------------------" << endl;
	cout << "4.退出系统 Exit ----------------------------------------------------" << endl;
	cout << endl;
	cout << "--------------------------文件操作----------------------------------" << endl;
	cout << "1.创建文件  touch_file  filename mode(0--只读1-读写2--执行3--不保护)" << endl;
	cout << "2.打开文件  open_file   filename -----------------------------------" << endl;
	cout << "3.关闭文件  close_file  filename -----------------------------------" << endl;
	cout << "4.删除文件  delete_file filename -----------------------------------" << endl;
	cout << "5.写入文件  write_file  context  [path]/file------------------------" << endl;
	cout << "6.读取文件  read_file   context  [path]/file------------------------" << endl;
	cout << "7.复制文件  copy_file   [path1]/file  [path2]/file------------------" << endl;
	cout << endl;
	cout << "--------------------------目录操作----------------------------------" << endl;
	cout << "1.创建目录  mk_dir      dirname-------------------------------------" << endl;
	cout << "2.删除目录  rv_dir      dirname-------------------------------------" << endl;
	cout << "3.切换目录  cd_dir      path----------------------------------------" << endl;
	cout << "4.显示目录  is_dir      [path]--------------------------------------" << endl;
}

//分割字符串函数 返回 vector<string>
vector<string> split(const string& str, const string& delim)
{
	vector<string> res;
	if ("" == str) return res;
	//先将要切割的字符串从string类型转换为char*类型  
	char * strs = new char[str.length() + 1]; //不要忘了  
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());
	char *p = strtok(strs, d);
	while (p) {
		string s = p; //分割得到的字符串转换为string类型  
		res.push_back(s); //存入结果数组  
		p = strtok(NULL, d);
	}
	return res;
}
//绝对路径处理
void Path_Deal_With(const string &path, vector<string> &Cur_Absolute_Path)
{
	//清理一下
	Cur_Absolute_Path.clear();
	//字符串分割
	Cur_Absolute_Path = split(path, "/");
}
//绝对路径匹配函数  顺便设置绝对路径最后一层父文件节点指针
bool Absolute_path_is_match(const vector<string>&split_path, const string &original_path)
{
	//1.如果分隔符的个数不是 目录的个数-1 不匹配
	int count = 0;
	for (int i = 0; i < original_path.size(); i++)if (original_path[i] == '/')count++;
	if (count != (split_path.size() - 1))return false;
	//2.进一步从用户根目录去比对 
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
//当前一级路径的判断
bool Cur_Path_Is_Match(string &CurPath){
	if (CurPath[0] == '/' || CurPath[CurPath.size() - 1] == '/')return false;
	return  true;
}
//当前多级路径的判断
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
//判断 要创建的文件或者目录是否存在
bool is_not_exists(FileNode *&CurParentFn, string &new_sub_name, FileNode *&CurPre, bool type)
{
	bool flag = true;
	//1.没有子文件
	if (!CurParentFn->lchild){ CurPre = CurParentFn; return true; }
	//2.有子文件
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
//创建子文件/子目录
bool Create_File_Or_Dir_Node(FileNode *&Parent, FileNode *&CurPre, const string filename, bool isdir, string mode)
{
	//创建一个新的子文件
	FileNode *source = new FileNode;
	if (!source){ cout << "创建失败" << endl; return false; }
	source->filename = filename;
	source->mode = mode[0] - '0';
	source->isdir = isdir;
	//指针域
	source->parent = Parent;
	//是第一个
	if (!Parent->lchild){ Parent->lchild = source; }
	//不是第一个
	else{ CurPre->sibling_next = source; source->sibling_prev = CurPre; }
	return true;
}
// for_each 使用的打印函数
void print_path(string &str)
{
	cout << str;
}
//释放文件节点
void Free_fn(FileNode *fn)
{
	fn->lchild = NULL;
	fn->sibling_next = NULL;
	fn->sibling_prev = NULL;
	fn->lchild = NULL;
	//delete fn;
}
//释放文件节点
void Delete_Fn(FileNode *&parenfn, FileNode *&curfn)
{
	//第一个
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
	//中间的
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
// copy_file 用的辅助函数
bool Copy_Create_File_Or_Node(FileNode *&Parent, FileNode *&CurPre, FileNode *&fn_source, string ffilename)
{
	//创建一个新的子文件
	FileNode *source = new FileNode;
	if (!source){ cout << "创建失败" << endl; return false; }
	//指针域
	source->parent = Parent;
	//是第一个
	if (!Parent->lchild){ Parent->lchild = source; }
	//不是第一个
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
	//1.路径二是绝对路径
	if (Vec_Absolute_Path.size()>1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0])
	{
		if (Absolute_path_is_match(Vec_Absolute_Path, Vec_Cmd_Line[2]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				cout << "目标文件存在,复制成功" << endl;

			}
			else
			{
				string name = Vec_Absolute_Path[Vec_Absolute_Path.size() - 1];
				if (Copy_Create_File_Or_Node(Absolute_fn, CurPre, fn_source, name))
				{
					cout << "文件拷贝成功" << endl;
				}
				else cout << "文件拷贝失败" << endl;
			}

		}
		else cout << "目标文件路径不对" << endl;
		return;
	}
	//2.路径二是当前多级路径	
	if (Vec_Absolute_Path.size()>1 && Vec_Mfd[CurID].fn->filename != Vec_Absolute_Path[0])
	{
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[2]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				cout << "目标文件存在,复制成功" << endl;

			}
			else
			{
				string name = Vec_Absolute_Path[Vec_Absolute_Path.size() - 1];
				if (Copy_Create_File_Or_Node(Absolute_fn, CurPre, fn_source, name))
				{
					cout << "文件拷贝成功" << endl;
				}
				else cout << "文件拷贝失败" << endl;
			}

		}
		else cout << "目标文件路径不对" << endl;
		return;
	}
	//3.路径二是当前路径
	if (Vec_Absolute_Path.size() == 1)
	{
		if (Cur_Path_Is_Match(Vec_Cmd_Line[2]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Cur_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				cout << "目标文件存在,复制成功" << endl;

			}
			else
			{
				string name = Vec_Absolute_Path[Vec_Absolute_Path.size() - 1];
				if (Copy_Create_File_Or_Node(Absolute_fn, CurPre, fn_source, name))
				{
					cout << "文件拷贝成功" << endl;
				}
				else cout << "文件拷贝失败" << endl;
			}

		}
		else cout << "目标文件路径不对" << endl;
	}
}