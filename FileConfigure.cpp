# include<iostream>
# include<sstream>
# include<stdlib.h>
# include"MFD.h"
# include"Command.h"
using namespace std;
/*
一些全局变量
*/
//相当于MFD表 存储的是 用户名和用户文件目录
vector<MFD> Vec_Mfd;
//文件打开表 记录用户打开文件的个数
vector<vector<string>> Vec_Afd;
//命令行
vector<string>Vec_Cmd_Line;
//命令操作的绝对路径路径
vector<string>Vec_Absolute_Path;
//有时用到第二个路径
vector<string>Vec_Aux_Absolute_Path;
//绝对路径的文件节点 全局可变动
FileNode *Absolute_fn;
//当前路径的文件节点
FileNode *Cur_fn;
//当前用户ID 默认root
int CurID;
//函数调用表
vector<Handler>Vec_Handler_List;
//全局命令
vector<string> show_path;
//初始化
void Init_MFD_Table(){
	string temp_name[Max_User_Num] = { "root", "u1", "u2", "u3", "u4", "u5", "u6", "u7", "u8", "u9", };
	string pwd = "123456";
	for (int i = 0; i<Max_User_Num; i++){
		MFD t_mfd;
		t_mfd.name = temp_name[i];
		t_mfd.pwd = pwd;
		//开辟一个文件节点
		t_mfd.fn = new FileNode;
		if (!t_mfd.fn){ cout << "申请内存失败" << endl; free(t_mfd.fn); return; }
		t_mfd.fn->filename = t_mfd.name;
		t_mfd.fn->isdir = true;
		t_mfd.fn->parent = NULL;
		t_mfd.fn->lchild = NULL;
		t_mfd.fn->sibling_prev = NULL;
		t_mfd.fn->sibling_next = NULL;
		Vec_Mfd.push_back(t_mfd);
	}
	Absolute_fn = NULL;
	//默认为 root 登陆
	CurID = 0;
	Cur_fn = Vec_Mfd[CurID].fn;
	show_path.push_back(Vec_Mfd[CurID].fn->filename);
	show_path.push_back("\\");
}
//初始化 函数调用
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
//命令输入处理
void Cmd_Line_Solve()
{
	string src, temp;
	getline(cin, src);
	stringstream ss(src);
	Vec_Cmd_Line.clear();
	while (ss >> temp)Vec_Cmd_Line.push_back(temp);
	//遍历命令列表
	int i;
	for (i = 0; i < Vec_Handler_List.size(); i++)if (Vec_Cmd_Line[0] == Vec_Handler_List[i].pname)break;
	if (i<Vec_Handler_List.size()){
		//函数处理
		Vec_Handler_List[i].handler();
	}
	else cout << "命令错误" << endl;
}
//主函数
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
