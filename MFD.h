# ifndef MFD_H_
# define MFD_H_
# include<string>
# include<cstring>
# include<vector>
# include<iostream>
# include<map>
const int Max_Open_count = 5;
const int Max_User_Num = 10;
const int Max_Cmd_length = 5;
enum r_w_pointer{off,read,write};
enum f_d_flag{file,dir};
enum f_m{readonly,readwrite,execute,unprotected};
using std::string;
using std::vector;
struct FileNode {
	string filename;//文件名/目录名
	int isdir;//目录、文件的识别标志
	int mode;   //0--只读 1--读/写 2--可执行
	int rwp;    // 0--off  1--reading 2--writing
	struct FileNode *parent, *lchild;//指向父亲的指针和指向左孩子的指针
	struct FileNode *sibling_prev, *sibling_next;//指向前一个兄弟的指针和指向后一个兄弟的指针.
	FileNode(string fn="",bool isdir=false,int md=1){
		this->filename = fn;
		this->isdir = isdir;
		this->mode = md;
		this->rwp = r_w_pointer::off;
		this->lchild = NULL;
		this->parent = NULL;
		this->sibling_next = NULL;
		this->sibling_prev = NULL;
	}
	void operator =(const FileNode o)
	{
		this->filename = o.filename;
		this->isdir = o.isdir;
		this->mode = o.mode;
		this->rwp = o.rwp;
		this->lchild = o.lchild;
		this->parent = o.parent;
		this->sibling_next = o.sibling_next;
		this->sibling_prev = o.sibling_prev;
	}
	void show()
	{
		using std::cout;
		using std::endl;
		cout << "\t\t" << this->filename;
		cout << "        " << (this->isdir == false ? "文件":"目录");
		if (!this->isdir)
		{
			cout << "        ";
			if (this->mode == f_m::readonly)cout << "只读";
			if (this->mode == f_m::readwrite)cout << "读/写";
			if(this->mode==f_m::execute) cout << "执行";
			if (this->mode == f_m::unprotected)cout << "不保护" << endl;
		}
		cout << endl;
		
	}
};
//文件主目录vector打表
struct MFD{
	string name;
	string pwd;
	FileNode *fn;
};
//函数调用结构体
typedef void(*FUNC)();
struct Handler
{
	string pname;
	FUNC handler;
};

/*
*一些全局变量 迫于无奈声明为 链接性为外部的变量
*/

//相当于MFD表 存储的是 用户名和用户文件目录
extern vector<MFD> Vec_Mfd;
//文件打开表 记录用户打开文件的个数 和每个文件的路径
extern vector<vector<string>> Vec_Afd;
//命令行
extern vector<string>Vec_Cmd_Line;
//命令操作的绝对路径路径
extern vector<string>Vec_Absolute_Path;
//有时用到第二个路径
extern vector<string>Vec_Aux_Absolute_Path;
//绝对路径的文件节点 全局可变动
extern FileNode *Absolute_fn;
//当前路径的文件节点
extern FileNode *Cur_fn;
//当前用户ID 默认root
extern int CurID;
//全局命令
extern vector<string> show_path;
# endif