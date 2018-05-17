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
	string filename;//�ļ���/Ŀ¼��
	int isdir;//Ŀ¼���ļ���ʶ���־
	int mode;   //0--ֻ�� 1--��/д 2--��ִ��
	int rwp;    // 0--off  1--reading 2--writing
	struct FileNode *parent, *lchild;//ָ���׵�ָ���ָ�����ӵ�ָ��
	struct FileNode *sibling_prev, *sibling_next;//ָ��ǰһ���ֵܵ�ָ���ָ���һ���ֵܵ�ָ��.
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
		cout << "        " << (this->isdir == false ? "�ļ�":"Ŀ¼");
		if (!this->isdir)
		{
			cout << "        ";
			if (this->mode == f_m::readonly)cout << "ֻ��";
			if (this->mode == f_m::readwrite)cout << "��/д";
			if(this->mode==f_m::execute) cout << "ִ��";
			if (this->mode == f_m::unprotected)cout << "������" << endl;
		}
		cout << endl;
		
	}
};
//�ļ���Ŀ¼vector���
struct MFD{
	string name;
	string pwd;
	FileNode *fn;
};
//�������ýṹ��
typedef void(*FUNC)();
struct Handler
{
	string pname;
	FUNC handler;
};

/*
*һЩȫ�ֱ��� ������������Ϊ ������Ϊ�ⲿ�ı���
*/

//�൱��MFD�� �洢���� �û������û��ļ�Ŀ¼
extern vector<MFD> Vec_Mfd;
//�ļ��򿪱� ��¼�û����ļ��ĸ��� ��ÿ���ļ���·��
extern vector<vector<string>> Vec_Afd;
//������
extern vector<string>Vec_Cmd_Line;
//��������ľ���·��·��
extern vector<string>Vec_Absolute_Path;
//��ʱ�õ��ڶ���·��
extern vector<string>Vec_Aux_Absolute_Path;
//����·�����ļ��ڵ� ȫ�ֿɱ䶯
extern FileNode *Absolute_fn;
//��ǰ·�����ļ��ڵ�
extern FileNode *Cur_fn;
//��ǰ�û�ID Ĭ��root
extern int CurID;
//ȫ������
extern vector<string> show_path;
# endif