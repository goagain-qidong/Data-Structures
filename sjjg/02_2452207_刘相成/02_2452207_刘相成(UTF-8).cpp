#include<iostream>
#include<limits>
using namespace std;

#define ll long long 
class linklist;
// 链表结点结构体
struct listnode {
	friend class linklist;
	ll data;// 结点存储的数据
	listnode* next; // 指向下一个结点的指针
	listnode* last;// 指向上一个结点的指针
	// 构造函数
	listnode(ll dt = 0) :data(dt), next(nullptr), last(nullptr) {}
	// 析构函数
	~listnode() {}
};

// 链表类
class linklist {
public:
	listnode* head;// 链表头指针
	listnode* back;// 链表尾指针
	int size;  // 链表长度
	// 构造函数
	linklist() :head(nullptr), back(nullptr), size(0) {}
	// 析构函数
	~linklist() { clear(); }
	void insert(ll dt);  // 在链表尾部插入新结点
	void clear(); // 清空链表，释放所有结点内存
	void print(); // 打印链表内容
};

// 打印链表内容
void linklist::print() {
	if (size == 0) {
		cout << "链表为空，无法打印" << endl;
		return;
	}
	listnode* cur = head;
	while (cur) {
		cout << cur->data << ' ';  
		cur = cur->next;
	}
	cout << endl;
}

// 清空链表，释放内存
void linklist::clear() {
	if (size == 0) return;
	listnode* temp = head;
	for (int i = 0; i < size; i++) {
		head = head->next;  
		delete temp;
		temp = head;
	}
	size = 0;
	head = nullptr;
	back = nullptr;
}

// 在链表尾部插入新结点
void linklist::insert(ll dt) {
	listnode* p = new listnode(dt);
	if (size == 0) {
		head = p;
	}
	else {
		back->next = p;  
		p->last = back;
	}
	back = p;
	size++;
}

// 检查输入
void cin_true(linklist& x) {
	ll n;
	while (1) {
		cin >> n;
		if (cin.fail()) {
			cerr << "输入不是有效正整数，请重新输入整个序列" << endl;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			x.clear();
			continue;
		}
		else {
			if (n == -1) {
				break;
			}
			else if (n <= 0) {
				cerr << "输入不是有效正整数，请重新输入整个序列" << endl;
				cin.ignore(INT_MAX, '\n');
				x.clear(); 
				continue;
			}
			else {
				if (x.size > 0 && n < x.back->data) {
					cerr << "输入序列不是非降序的，请重新输入整个序列" << endl;
					cin.ignore(INT_MAX, '\n'); 
					x.clear();
					continue;
				}
				x.insert(n);
			}
		}
	}
}

// 找交集
void compare(linklist& a, linklist& b) {
	listnode* cura = a.head;
	listnode* curb = b.head;
	int num = 0; 
	// 双指针法遍历两个链表
	while ((cura) && (curb)) {
		if (cura->data == curb->data) {
			if (num > 0) {
				cout << ' ';
			}
			cout << cura->data;
			cura = cura->next;
			curb = curb->next;
			num++;
		}
		else if (cura->data < curb->data) {
			cura = cura->next;
		}
		else curb = curb->next;
	}
	if (num == 0) cout << "NULL";
	cout << endl;
}

int main() {
	cout << "请输入两个非降序正整数链表序列(数字空格分开，以-1结束输入)" << endl;
	linklist a;
	linklist b;
	cout << "输入序列1：" << endl;
	cin_true(a);
	cout << "输入序列2：" << endl;
	cin_true(b);
	cout << endl << "交集结果：";
	compare(a, b);
	cout << "\n程序运行结束，按回车键退出" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
	return 0;
}