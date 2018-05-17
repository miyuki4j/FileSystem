# include<iostream>
# include"MFD.h"
# include"Auxiliary.h"
# include<algorithm>
using namespace std;
/*
* 用户模块
*/
void Login(){
	//检测用户是否存在
	string username = Vec_Cmd_Line[1], pwd = Vec_Cmd_Line[2];
	int i;
	for (i = 0; i<Vec_Mfd.size(); i++){
		if (Vec_Mfd[i].name == username && Vec_Mfd[i].pwd == pwd)break;
	}
	if (i<Vec_Mfd.size()){
		cout << "登陆成功" << endl;
		Cur_fn = Vec_Mfd[i].fn;
		CurID = i;
		//切换用户
		show_path.clear();
		show_path.push_back(Vec_Mfd[CurID].fn->filename);
		show_path.push_back("\\");
	}
	else{ cout << "用户不存在 或 密码错误" << endl; }
}
void Register()
{
	if (CurID != 0){
		cout << "普通用户，无注册权限" << endl;
		return;
	}
	else
	{
		if (Vec_Cmd_Line.size() == 2){
			cout << "密码为空" << endl;
		}
		else if (Vec_Cmd_Line.size() == 1){
			cout << "用户名为空" << endl;
		}
		else{
			string username = Vec_Cmd_Line[1], pwd = Vec_Cmd_Line[2];
			MFD t_mfd;
			t_mfd.name = username; t_mfd.pwd = pwd;
			//开辟一个文件节点
			t_mfd.fn = new FileNode;
			if (!t_mfd.fn){ cout << "申请内存失败" << endl; free(t_mfd.fn); return; }
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
	cout << "\t\t你已退出" << endl;
	CurID = 0;
	Cur_fn = Vec_Mfd[CurID].fn;
	show_path.clear();
	show_path.push_back(Vec_Mfd[CurID].fn->filename);
	show_path.push_back("\\");
}
void do_Exit()
{
	exit(0);
	cout << "\t\t退出成功" << endl;
}

//1.创建文件
void CreateFile(){
	//调用一下路径处理函数
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	//文件权限
	string mode = "1";
	if (Vec_Cmd_Line.size() == 3)
	{
		mode = Vec_Cmd_Line[2];
	}
	//1.绝对路径
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0]){
		//如果父路径匹配
		if (Absolute_path_is_match(Vec_Absolute_Path, Vec_Cmd_Line[1])){
			FileNode *CurPre;
			//如果文件不存在
			if (is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				if (Create_File_Or_Dir_Node(Absolute_fn, CurPre, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], false, mode))
					cout << "文件创建成功 ^_^" << endl;
				else cout << "创建失败 ^_^" << endl;
			}
			else{ cout << "文件已存在 ^_^" << endl; }
		}
		else{ cout << "路径错误 ^_^" << endl; }
		return;
	}
	//2.当前 多级路经
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename != Vec_Absolute_Path[0]){
		//如果父路径匹配
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[1])){
			FileNode *CurPre;
			//如果文件不存在
			if (is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				if (Create_File_Or_Dir_Node(Absolute_fn, CurPre, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], false, mode))
					cout << "文件创建成功 ^_^" << endl;
				else cout << "创建失败 ^_^" << endl;
			}
			else{ cout << "文件已存在 ^_^" << endl; }
		}
		else{ cout << "路径错误 ^_^" << endl; }
		return;
	}
	//3.当前 一级 路径
	if (Vec_Absolute_Path.size() == 1)
	{
		// 文件名是否合法
		if (Cur_Path_Is_Match(Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			//如果可以创建
			if (is_not_exists(Cur_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				if (Create_File_Or_Dir_Node(Cur_fn, CurPre, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], false, mode))
					cout << "文件创建成功 ^_^" << endl;
				else cout << "创建失败 ^_^" << endl;
			}
			else{ cout << "文件已存在 ^_^" << endl; }
		}
		else{ cout << "当前路径下路径错误 ^_^" << endl; }
	}
}
//2.打开文件 open_file filename
void OpenFile()
{
	//如果达到最大打开次数
	if (Vec_Afd.size() == Max_Open_count){ cout << "打开文件已达最大限度，无法再打开" << endl; return; }
	//调用一下路径处理函数
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	//1.绝对路径
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0])
	{
		//检测AFD
		bool flag = false;
		for (int i = 0; i < Vec_Afd.size(); i++)
		{
			if (Vec_Afd[i] == Vec_Absolute_Path){
				flag = true; break;
			}
		}
		if (flag)
		{
			cout << "文件已打开 ^_^" << endl;
			return;
		}
		else{
			if (Absolute_path_is_match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
			{
				FileNode* CurPre;
				if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
				{
					cout << "文件打开成功 ^_^" << endl;
					if (Vec_Afd.size() < Max_Open_count){
						Vec_Afd.push_back(Vec_Absolute_Path);
					}
				}
				else{ cout << "文件不存在" << endl; }
			}
			else{ cout << "路径不匹配" << endl; }
		}
		return;
	}

	//当前多级路径
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename != Vec_Absolute_Path[0])
	{
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[0], CurPre, false))
			{
				//curpre 指向当前存在的那个文件
				//做一个绝对路径
				Vec_Absolute_Path.clear();
				while (CurPre){
					Vec_Absolute_Path.push_back(CurPre->filename);
					CurPre = CurPre->parent;
				}
				//反转
				reverse(Vec_Absolute_Path.begin(), Vec_Absolute_Path.end());
				//检测AFD
				bool flag = false;
				for (int i = 0; i < Vec_Afd.size(); i++)
				{
					if (Vec_Afd[i] == Vec_Absolute_Path){
						flag = true; break;
					}
				}
				if (flag)
				{
					cout << "文件已打开 ^_^" << endl;
					return;
				}
				else
				{
					//加入到afd
					if (Vec_Afd.size() < Max_Open_count){
						Vec_Afd.push_back(Vec_Absolute_Path);
					}
					cout << "文件打开成功" << endl;
				}
			}
			else cout << "不存在该文件" << endl;
		}
		else cout << "路径不匹配" << endl;
		return;
	}

	//3.当前一级路径
	if (Vec_Absolute_Path.size() == 1)
	{
		if (Cur_Path_Is_Match(Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Cur_fn, Vec_Absolute_Path[0], CurPre, false))
			{
				//curpre 指向当前存在的那个文件
				//做一个绝对路径
				Vec_Absolute_Path.clear();
				while (CurPre){
					Vec_Absolute_Path.push_back(CurPre->filename);
					CurPre = CurPre->parent;
				}
				//反转
				reverse(Vec_Absolute_Path.begin(), Vec_Absolute_Path.end());
				//检测AFD
				bool flag = false;
				for (int i = 0; i < Vec_Afd.size(); i++)
				{
					if (Vec_Afd[i] == Vec_Absolute_Path){
						flag = true; break;
					}
				}
				if (flag)
				{
					cout << "文件已打开 ^_^" << endl;
					return;
				}
				else
				{
					//加入到afd
					if (Vec_Afd.size() < Max_Open_count){
						Vec_Afd.push_back(Vec_Absolute_Path);
					}
					cout << "文件打开成功" << endl;
				}
			}
			else cout << "不存在该文件" << endl;
		}
		else cout << "路径不匹配" << endl;
	}

}
//3.关闭文件 close_file filename
void CloseFile()
{
	//调用一下路径处理函数
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	//1.绝对路径
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0])
	{
		bool flag = false;
		int index = -1;
		for (int i = 0; i < Vec_Afd.size(); i++){
			if (Vec_Afd[i] == Vec_Absolute_Path){ flag = true; index = i; break; }
		}
		if (flag)
		{
			//清除文件在 AFD中
			Vec_Afd.erase((Vec_Afd.begin() + index));
			cout << "关闭文件" << endl;
			return;
		}
		else{
			cout << "文件没有打开或其他" << endl;
		}
	}
	//2.当前多级路径
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename != Vec_Absolute_Path[0])
	{
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				//做一个绝对路径
				Vec_Absolute_Path.clear();
				while (CurPre){
					Vec_Absolute_Path.push_back(CurPre->filename);
					CurPre = CurPre->parent;
				}
				//反转
				reverse(Vec_Absolute_Path.begin(), Vec_Absolute_Path.end());
				bool flag = false;
				int index = -1;
				for (int i = 0; i < Vec_Afd.size(); i++){
					if (Vec_Afd[i] == Vec_Absolute_Path){ flag = true; index = i; break; }
				}
				if (flag)
				{
					//清除文件在 AFD中
					Vec_Afd.erase((Vec_Afd.begin() + index));
					cout << "关闭文件成功" << endl;
					return;
				}
				else{
					cout << "文件没有打开或其他" << endl;
				}
			}
		}
		else{
			cout << "路径不匹配" << endl;
		}
	}

	//3.当前1级路径
	if (Vec_Absolute_Path.size() == 1)
	{
		if (Cur_Path_Is_Match(Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Cur_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				//做一个绝对路径
				Vec_Absolute_Path.clear();
				while (CurPre){
					Vec_Absolute_Path.push_back(CurPre->filename);
					CurPre = CurPre->parent;
				}
				//反转
				reverse(Vec_Absolute_Path.begin(), Vec_Absolute_Path.end());
				bool flag = false;
				int index = -1;
				for (int i = 0; i < Vec_Afd.size(); i++){
					if (Vec_Afd[i] == Vec_Absolute_Path){ flag = true; index = i; break; }
				}
				if (flag)
				{
					//清除文件在 AFD中
					Vec_Afd.erase((Vec_Afd.begin() + index));
					cout << "关闭文件成功" << endl;
					return;
				}
				else{
					cout << "文件没有打开或其他" << endl;
				}
			}
		}
		else{
			cout << "路径不匹配" << endl;
		}
	}

}
//4.删除文件 delete_file filename
void DeleteFile(){

	//调用一下路径处理函数
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	//1.绝对路径
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0])
	{
		if (Absolute_path_is_match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				cout << "找到啦" << endl;
				Delete_Fn(Absolute_fn, CurPre);
				cout << "删除成功" << endl;
			}
			else cout << "文件不存在" << endl;
		}
		else cout << "路径不匹配" << endl;
		return;
	}
	//2.当前多级路径
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0])
	{
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				cout << "找到啦" << endl;
				Delete_Fn(Absolute_fn, CurPre);
				cout << "删除成功" << endl;
			}
			else cout << "文件不存在" << endl;
		}
		else cout << "路径不匹配" << endl;
		return;
	}
	//3.当前1级路径
	if (Vec_Absolute_Path.size() == 1)
	{
		if (Cur_Path_Is_Match(Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Cur_fn, Vec_Absolute_Path[0], CurPre, false))
			{
				cout << "找到啦" << endl;
				//做一个绝对路径
				FileNode *CurPre_t = CurPre;
				Vec_Absolute_Path.clear();
				while (CurPre_t){
					Vec_Absolute_Path.push_back(CurPre_t->filename);
					CurPre_t = CurPre_t->parent;
				}
				//反转
				reverse(Vec_Absolute_Path.begin(), Vec_Absolute_Path.end());
				//删除节点
				Delete_Fn(Cur_fn, CurPre);
				cout << "删除成功" << endl;
			}
			else cout << "文件不存在" << endl;
		}
		else cout << "路径不匹配" << endl;
	}
	//检测Afd 表
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
		cout << "在 afd 中删除" << endl;
	}

}
//5.显示文件
void Is_Dir()
{
	//1.当前1级路径
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
	//2.当前多级路径
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
			else cout << "最后一个子目录不存在" << endl;
		}
		else cout << "路径不匹配" << endl;
		return;
	}
	//3.绝对路径
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
			else cout << "最后一个子目录不存在" << endl;
		}
		else cout << "路径不匹配" << endl;
	}
}
//6.创建目录
void Mk_Dir()
{
	//调用一下路径处理函数
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	string mode = "1";
	//1.绝对路径
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0]){
		//如果父路径匹配
		if (Absolute_path_is_match(Vec_Absolute_Path, Vec_Cmd_Line[1])){
			FileNode *CurPre;
			//如果文件不存在
			if (is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
			{
				if (Create_File_Or_Dir_Node(Absolute_fn, CurPre, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], true, mode))
					cout << "目录创建成功 ^_^" << endl;
				else cout << "创建失败 ^_^" << endl;
			}
			else{ cout << "目录已存在 ^_^" << endl; }
		}
		else{ cout << "路径错误 ^_^" << endl; }
		Absolute_fn = NULL;
		return;
	}
	//2.当前 多级 路径
	if (Vec_Absolute_Path.size() > 1 && Vec_Mfd[CurID].fn->filename != Vec_Absolute_Path[0]){
		//如果父路径匹配
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[1])){
			FileNode *CurPre;
			//如果文件不存在
			if (is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
			{
				if (Create_File_Or_Dir_Node(Absolute_fn, CurPre, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], true, mode))
					cout << "目录创建成功 ^_^" << endl;
				else cout << "创建失败 ^_^" << endl;
			}
			else{ cout << "目录已存在 ^_^" << endl; }
		}
		else{ cout << "路径错误 ^_^" << endl; }
		return;
	}

	//3.当前 1级 路径
	if (Vec_Absolute_Path.size() == 1)
	{
		// 文件名是否合法
		if (Cur_Path_Is_Match(Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			//如果可以创建
			if (is_not_exists(Cur_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
			{
				if (Create_File_Or_Dir_Node(Cur_fn, CurPre, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], true, mode))
					cout << "目录创建成功 ^_^" << endl;
				else cout << "创建失败 ^_^" << endl;
			}
			else{ cout << "目录已存在 ^_^" << endl; }
		}
		else{ cout << "当前路径下路径错误 ^_^" << endl; }
	}
}
//7.删除目录
void Rv_Dir()
{
	//调用一下路径处理函数
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	//1.绝对路径
	if (Vec_Absolute_Path.size() > 1 && Vec_Absolute_Path[0] == Vec_Mfd[CurID].fn->filename)
	{
		if (Absolute_path_is_match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
			{
				if (CurPre)
				{
					cout << "提示: 目录不为空，是否删除  （y : 删除 , N : 不删除) , 请选择:";
					char flag;
					cin >> flag;
					if (flag == 'Y' || flag == 'y')
					{
						Delete_Fn(Absolute_fn, CurPre);
						cout << "删除成功" << endl;
						return;
					}
				}
				else
				{
					Delete_Fn(Absolute_fn, CurPre);
					cout << "删除成功" << endl;
					return;
				}

			}
			else cout << "目录不存在" << endl;
		}
		else cout << "路径不匹配" << endl;
		return;
	}
	//2.当前多级路径
	if (Vec_Absolute_Path.size() > 1 && Vec_Absolute_Path[0] != Vec_Mfd[CurID].fn->filename)
	{
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, true))
			{
				if (CurPre)
				{
					cout << "提示: 目录不为空，是否删除  （y : 删除 , N : 不删除) , 请选择:";
					char flag;
					cin >> flag;
					if (flag == 'Y' || flag == 'y')
					{
						Delete_Fn(Absolute_fn, CurPre);
						cout << "删除成功" << endl;
						return;
					}
				}
				else
				{
					Delete_Fn(Absolute_fn, CurPre);
					cout << "删除成功" << endl;
					return;
				}
			}
			else cout << "目录不存在" << endl;
		}
		else cout << "路径不匹配" << endl;
		return;
	}

	//3.当前 1 级路径
	if (Vec_Absolute_Path.size() == 1)
	{
		if (Cur_Path_Is_Match(Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Cur_fn, Vec_Absolute_Path[0], CurPre, true))
			{
				if (CurPre)
				{
					cout << "提示: 目录不为空，是否删除  （y : 删除 , N : 不删除) , 请选择:";
					char flag;
					cin >> flag;
					if (flag == 'Y' || flag == 'y')
					{
						Delete_Fn(Absolute_fn, CurPre);
						cout << "删除成功" << endl;
						return;
					}
				}
				else
				{
					Delete_Fn(Absolute_fn, CurPre);
					cout << "删除成功" << endl;
					return;
				}
			}
			else cout << "文件不存在" << endl;
		}
		else cout << "路径不匹配" << endl;
	}
}
//8.切换目录
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
			cout << "没有父目录" << endl;
			return;
		}
		return;
	}
	//3. cd .
	if (Vec_Cmd_Line.size() == 2 && Vec_Cmd_Line[1] == "."){ return; }
	//路径处理
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);
	string str = Vec_Cmd_Line[1];
	if (str[str.size() - 1] == '/')str.pop_back();

	//3.cd 当前路径一级目录
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
			else{ cout << "路径不匹配" << endl; }
		}
		else{ cout << "路径不匹配" << endl; }
		return;
	}

	if (Vec_Absolute_Path.size() > 1)
	{
		//4.cd 当前路径 多级目录
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
				else cout << "路径不匹配" << endl;
			}
			else cout << "路径不匹配" << endl;
			return;
		}
		//5.cd 绝对路径
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
				else cout << "路径不匹配" << endl;
			}
			else cout << "路径不匹配" << endl;
		}
	}


}

//9.复制文件
void Copy_File()
{
	//1.路径 copy_file  [path1]/file  [path2]/file
	if (Vec_Cmd_Line.size()<3){ cout << "路径不对或语法错误" << endl; return; }
	Path_Deal_With(Vec_Cmd_Line[1], Vec_Absolute_Path);

	//路径1的文件必须存在  路径2的文件存在可不存在 名字为自定义的
	//1.路径一是绝对路径
	if (Vec_Absolute_Path.size()>1 && Vec_Mfd[CurID].fn->filename == Vec_Absolute_Path[0])
	{
		if (Absolute_path_is_match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				Aux_Copy(CurPre);//找到了这个文件
			}
			else cout << "源文件不存在" << endl;
		}
		else cout << "源文件路径不对" << endl;

		return;
	}
	//2.路径一是当前多级路径	
	if (Vec_Absolute_Path.size()>1 && Vec_Mfd[CurID].fn->filename != Vec_Absolute_Path[0])
	{
		if (Cur_Sub_path_Is_Match(Vec_Absolute_Path, Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Absolute_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				Aux_Copy(CurPre);
			}
			else cout << "源文件不存在" << endl;
		}
		else cout << "源文件路径不对" << endl;
		return;
	}
	//3.路径一是当前路径
	if (Vec_Absolute_Path.size() == 1)
	{
		if (Cur_Path_Is_Match(Vec_Cmd_Line[1]))
		{
			FileNode *CurPre;
			if (!is_not_exists(Cur_fn, Vec_Absolute_Path[Vec_Absolute_Path.size() - 1], CurPre, false))
			{
				Aux_Copy(CurPre);
			}
			else cout << "源文件不存在" << endl;
		}
		else cout << "源文件路径不对" << endl;
	}
}
void Read_File()
{
	cout << "逻辑没搞清，没写" << endl;
}
void Write_File()
{
	cout << "逻辑没搞清，没写" << endl;
}