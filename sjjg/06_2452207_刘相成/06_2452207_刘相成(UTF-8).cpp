#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <limits>

using namespace std;

// 成员节点
struct Member {
    char name[50];// 成员姓名
    Member** kid;// 节点数组指针
    int curcnt;// 当前节点数量
    int cap;// 节点数组容量
    Member(const char* n);// 构造函数
};
// 构造函数
Member::Member(const char* n) {
    strcpy(name, n);
    kid = nullptr;
    curcnt = 0;
    cap = 0;
}
// 家谱树类
class Tree {
private:
    Member* root;// 根节点
    Member* find(Member* current, const char* name);// 查找成员
    void expand(Member* parent); // 扩展子节点数组容量
    void remove(Member* node);// 释放内存
    bool findanc(Member* current, const char* targetname,Member*& anc, int& curgen, int targen); // 查找距离己辈第n代祖先
    void finddes(Member* member, int curgen, int targen);// 查找第n代子孙
    void showshu(Member* node, int level, string prefix);// 显示子树

public:
    Tree();// 构造函数
    ~Tree();// 析构函数
    bool exists(const char* name);// 检查姓名是否存在
    void init(const char* ancName);// 初始化家谱
    bool createfam(const char* parentName, int count,char kidNames[][50]);// 完善家谱
    bool addone(const char* parentName, const char* childName); // 添加单个子女
    bool dissolve(const char* memberName);// 解散局部家庭
    bool change(const char* oldName, const char* newName);// 更改家庭成员姓名
    bool findrel(const char* name, int ancGen, int desgen); // 查找亲属
    Member* findpar(Member* current, const char* name);// 查找父节点
    void printchi(Member* member);// 打印成员及其子孙
    void show();// 显示家谱图
};
// 构造函数
Tree::Tree() : root(nullptr) {}
// 析构函数
Tree::~Tree() {
    remove(root);
}
// 检查指定成员是否存在
bool Tree::exists(const char* name) {
    return find(root, name) != nullptr;
}

// 初始化家谱
void Tree::init(const char* ancName) {
    root = new Member(ancName);
    cout << "此家谱的祖先是：" << ancName << endl;
}

// 完善家谱
bool Tree::createfam(const char* parentName, int count,
    char kidNames[][50]) {
    Member* parent = find(root, parentName);
    if (parent == nullptr) {
        cout << "未找到" << parentName << endl;
        return false;
    }
    if (parent->curcnt > 0) {
        cout << parentName << " 已有家庭！" << endl;
        return false;
    }
    parent->kid = new Member * [count];
    parent->cap = count;
    for (int i = 0; i < count; i++) {
        parent->kid[i] = new Member(kidNames[i]);
        parent->curcnt++;
    }
    printchi(parent);
    return true;
}

// 给指定成员添加一个子女
bool Tree::addone(const char* parentName, const char* childName) {
    Member* parent = find(root, parentName);
    if (parent == nullptr) {
        cout << "未找到" << parentName<< endl;
        return false;
    }
    expand(parent);
    parent->kid[parent->curcnt] = new Member(childName);
    parent->curcnt++;
    printchi(parent);
    return true;
}

// 解散局部家庭
bool Tree::dissolve(const char* memberName) {
    if (root == nullptr) {
        cout << "家谱为空" << endl;
        return false;
    }
    if (strcmp(root->name, memberName) == 0) {
        cout << "不能解散祖先" << endl;
        return false;
    }
    Member* parent = findpar(root, memberName);
    if (parent == nullptr) {
        cout << "未找到" << memberName << endl;
        return false;
    }
    Member* toDelete = nullptr;
    int index = -1;
    for (int i = 0; i < parent->curcnt; i++) {
        if (strcmp(parent->kid[i]->name, memberName) == 0) {
            toDelete = parent->kid[i];
            index = i;
            break;
        }
    }
    if (toDelete != nullptr) {
        cout << "解散家庭：" << toDelete->name << endl;
        printchi(toDelete);

        for (int i = index; i < parent->curcnt - 1; i++) {
            parent->kid[i] = parent->kid[i + 1];
        }
        parent->curcnt--;
        remove(toDelete);
        return true;
    }
    return false;
}

// 更改成员姓名
bool Tree::change(const char* oldName, const char* newName) {
    Member* member = find(root, oldName);
    if (member == nullptr) {
        cout << "未找到" << oldName << endl;
        return false;
    }
    if (exists(newName)) {
        cout << newName << " 已存在！" << endl;
        return false;
    }
    strcpy(member->name, newName);
    cout << oldName << "更名为" << newName << endl;
    return true;
}

// 查找亲戚
bool Tree::findrel(const char* name, int ancGen, int desgen) {
    Member* member = find(root, name);
    if (member == nullptr) {
        cout << "未找到" << name  << endl;
        return false;
    }
    if(ancGen > 0|| desgen > 0)
        cout << "\n=== 查找结果 ===" << endl;
    if (ancGen > 0) {
        Member* anc = nullptr;
        int curgen = 0;
        if (findanc(root, name, anc, curgen, ancGen)) {
            if (anc != nullptr) {
                cout << "距离己辈第" << ancGen << "代祖先：" << anc->name << endl;
            }
            else cout << "距离己辈第" << ancGen << "代祖先：无" << endl;

        }
        else cout << "距离己辈第" << ancGen << "代祖先：无" << endl;
    }
    if (desgen > 0) {
        cout << "距离己辈第" << desgen << "代子孙：";
        bool hasDescendants = false;
        for (int i = 0; i < member->curcnt; i++) {
            if (desgen == 1) {
                hasDescendants = true;
                break;
            }
            else {
                Member* temp = member->kid[i];
                for (int j = 1; j < desgen; j++) {
                    if (temp->curcnt > 0) {
                        hasDescendants = true;
                        break;
                    }
                    if (temp->curcnt == 0) 
                        break;
                    temp = temp->kid[0];
                }
                if (hasDescendants) 
                    break;
            }
        }
        if (hasDescendants) {
            finddes(member, 0, desgen);
            cout << endl;
        }
        else cout << "无" << endl;
    }
    if (ancGen > 0 || desgen > 0)
        cout << "================" << endl;
    else cout << "无查找操作" << endl;
    return true;
}

// 查找指定成员的父节点
Member* Tree::findpar(Member* current, const char* name) {
    if (current == nullptr) return nullptr;
    for (int i = 0; i < current->curcnt; i++) {
        if (strcmp(current->kid[i]->name, name) == 0) {
            return current;
        }
        Member* result = findpar(current->kid[i], name);
        if (result != nullptr) return result;
    }
    return nullptr;
}

// 打印指定成员和其子女
void Tree::printchi(Member* member) {
    if (member == nullptr) return;
    cout << member->name << "的子女：";
    if (member->curcnt == 0) {
        cout << "无" << endl;
    }
    else {
        for (int i = 0; i < member->curcnt; i++) {
            cout << member->kid[i]->name;
            if (i != member->curcnt - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }
}

// 显示家谱
void Tree::show() {
    if (root == nullptr) {
        cout << "家谱为空" << endl;
        return;
    }
    cout << "\n=== 家谱图 ===" << endl;
    showshu(root, 0, "");
    cout << "==================\n" << endl;
}

// 查找指定成员节点
Member* Tree::find(Member* current, const char* name) {
    if (current == nullptr) return nullptr;
    if (strcmp(current->name, name) == 0) return current;
    for (int i = 0; i < current->curcnt; i++) {
        Member* result = find(current->kid[i], name);
        if (result != nullptr) return result;
    }
    return nullptr;
}

// 增加父节点的子节点容量
void Tree::expand(Member* parent) {
    if (parent->cap == 0) {
        parent->cap = 2;
        parent->kid = new Member * [parent->cap];
    }
    else if (parent->curcnt >= parent->cap) {
        int newcap = parent->cap * 2;
        Member** newkid = new Member * [newcap];
        for (int i = 0; i < parent->curcnt; i++) {
            newkid[i] = parent->kid[i];
        }
        delete[] parent->kid;
        parent->kid = newkid;
        parent->cap = newcap;
    }
}

// 释放节点及其子孙
void Tree::remove(Member* node) {
    if (node == nullptr) return;
    for (int i = 0; i < node->curcnt; i++) {
        remove(node->kid[i]);
    }
    if (node->kid != nullptr) {
        delete[] node->kid;
    }
    delete node;
}

// 查找距离己辈第n代祖先
bool Tree::findanc(Member* current, const char* targetname,Member*& anc, int& curgen, int targen) {
    if (current == nullptr) return false;
    if (strcmp(current->name, targetname) == 0) {
        return true;
    }
    for (int i = 0; i < current->curcnt; i++) {
        if (findanc(current->kid[i], targetname, anc, curgen, targen)) {
            curgen++;
            if (curgen == targen) {
                anc = current;
            }
            return true;
        }
    }
    return false;
}

// 查找并打印第n代子孙
void Tree::finddes(Member* member, int curgen, int targen) {
    if (member == nullptr) {
        cout << "无";
        return;
    }
    if (curgen == targen) {
        cout << member->name << " ";
        return;
    }
    for (int i = 0; i < member->curcnt; i++) {
        finddes(member->kid[i], curgen + 1, targen);
    }
}

// 显示家谱
void Tree::showshu(Member* node, int level, string prefix) {
    if (node == nullptr) return;
    if (level == 0) {
        cout << node->name << endl;
    }
    else {
        cout << prefix << "├── " << node->name << endl;
    }
    for (int i = 0; i < node->curcnt; i++) {
        string newPrefix = prefix;
        if (level > 0) {
            if (i == node->curcnt - 1) {
                newPrefix += "    ";
            }
            else {
                newPrefix += "│   ";
            }
        }
        showshu(node->kid[i], level + 1, newPrefix);
    }
}

// 显示菜单
void menu() {
    cout << "****    家谱管理系统    ***" << endl;
    cout << "****    A —— 完善家谱    ***" << endl;
    cout << "****    B —— 添加成员    ***" << endl;
    cout << "****    C —— 解散家庭    ***" << endl;
    cout << "****    D —— 更改姓名    ***" << endl;
    cout << "****    E —— 查找亲属    ***" << endl;
    cout << "****    F —— 退出程序    ***" << endl;
    cout << "=============================" << endl;
}

int main() {
    Tree tree;
    char choice;
    menu();
    cout << "建立家谱" << endl;
    cout << "请输入祖先姓名：";
    char ancName[50];
    bool validanc = false;
    while (!validanc) {
        if (!(cin.getline(ancName, 50))) {
            cout << "输入无效，请重新输入：";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (strlen(ancName) == 0) {
            cout << "不能为空，请重新输入：";
            continue;
        }
        bool hasSpace = false;
        for (int i = 0; i < (int)strlen(ancName); i++) {
            if (ancName[i] == ' ' || ancName[i] == '\t') {
                hasSpace = true;
                break;
            }
        }
        if (hasSpace) {
            cout << "只能输入一个（不含空格），请重新输入：";
            continue;
        }
        validanc = true;
    }
    tree.init(ancName);
    tree.show();
    while (true) {
        bool validChoice = false;
        while (!validChoice) {
            cout << "请选择操作：";
            if (!(cin >> choice)) {
                cout << "输入无效" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }
            char next = cin.peek();
            while (next == ' ') {
                cin.get();
                next = cin.peek();
            }
            if (next != '\n' && next != '\t') {
                cout << "只能输入一个字符" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }
            if (choice == 'A' || choice == 'a' || choice == 'B' || choice == 'b' ||
                choice == 'C' || choice == 'c' || choice == 'D' || choice == 'd' ||
                choice == 'E' || choice == 'e' || choice == 'F' || choice == 'f') {
                validChoice = true;
            }
            else cout << "无效选择" << endl;
        }
        bool success = false;
        if (choice == 'A' || choice == 'a') {
            char name[50];
            cout << "请输入建立家庭者姓名：";
            bool validParentName = false;
            while (!validParentName) {
                if (!(cin >> name)) {
                    cout << "输入无效，请重新输入：";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                char next = cin.peek();
                while (next == ' ') {
                    cin.get();
                    next = cin.peek();
                }
                if (next != '\n' && next != '\t') {
                    cout << "只能输入一个姓名，请重新输入：";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                validParentName = true;
            }
            if (!tree.exists(name)) {
                cout << "未找到" << name  << endl;
                cout << endl;
                continue;
            }
            int count;
            cout << "请输入" << name << "的儿女人数：";
            bool validCount = false;
            while (!validCount) {
                if (!(cin >> count)) {
                    cout << "输入无效，请重新输入：";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                char next = cin.peek();
                while (next == ' ') {
                    cin.get();
                    next = cin.peek();
                }
                if (next != '\n' && next != '\t') {
                    cout << "只能输入一个数字，请重新输入：";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                if (count <= 0) {
                    cout << "儿女人数必须大于0，请重新输入：";
                    continue;
                }
                validCount = true;
            }
            cout << "请依次输入" << name << "的儿女姓名：";
            while (true) {
                bool once = 0;
                bool validInput = true;
                int inputCount = 0;
                char kidNames[100][50];
                for (int i = 0; i < count; i++) {
                    if (!(cin >> kidNames[i])) {
                        validInput = false;
                        break;
                    }
                    inputCount++;
                    if (tree.exists(kidNames[i])) {
                        cout  << kidNames[i] << " 已存在，请重新输入所有姓名：";
                        once = 1;
                        validInput = false;
                        break;
                    }
                    for (int j = 0; j < i; j++) {
                        if (strcmp(kidNames[i], kidNames[j]) == 0) {
                            cout  << kidNames[i] << " 重复输入，请重新输入所有姓名：";
                            once = 1;
                            validInput = false;
                            break;
                        }
                    }
                    if (!validInput) break;
                }
                if (!validInput || inputCount != count) {
                    if(!once)
                    cout << "输入姓名数量不正确，请重新输入" << count << "个姓名：";
                    validInput = false;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                char next = cin.peek();
                while (next == ' ') {
                    cin.get();
                    next = cin.peek();
                }
                if (next != '\n' && next != '\t') {
                    if (!once)
                    cout << "输入过多的姓名，请重新输入" << count << "个姓名：";
                    validInput = false;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                if (validInput) {
                    success = tree.createfam(name, count, kidNames);
                    break;
                }
            }
        }
        else if (choice == 'B' || choice == 'b') {
            char name[50];
            cout << "请输入添加子女者姓名：";
            bool validParentName = false;
            while (!validParentName) {
                if (!(cin >> name)) {
                    cout << "输入无效，请重新输入：";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                char next = cin.peek();
                while (next == ' ') {
                    cin.get();
                    next = cin.peek();
                }
                if (next != '\n' && next != '\t') {
                    cout << "只能输入一个姓名，请重新输入：";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                validParentName = true;
            }
            if (!tree.exists(name)) {
                cout << "未找到" << name << endl;
                cout << endl;
                continue;
            }
            char childName[50];
            bool validInput = false;
            cout << "请输入" << name << "新添子女姓名：";
            while (!validInput) {
                if (!(cin >> childName)) {
                    cout << "输入无效，请重新输入：";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                char next = cin.peek();
                while (next == ' ') {
                    cin.get();
                    next = cin.peek();
                }
                if (next != '\n' && next!= '\t') {
                    cout << "只能输入一个姓名，请重新输入：";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                if (tree.exists(childName)) {
                    cout << "姓名 " << childName << " 已存在，请重新输入：";
                    continue;
                }
                validInput = true;
            }
            success = tree.addone(name, childName);
        }
        else if (choice == 'C' || choice == 'c') {
            char name[50];
            cout << "请输入解散家庭者姓名：";
            bool validParentName = false;
            while (!validParentName) {
                if (!(cin >> name)) {
                    cout << "输入无效，请重新输入：";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                char next = cin.peek();
                while (next == ' ') {
                    cin.get();
                    next = cin.peek();
                }
                if (next != '\n' && next != '\t') {
                    cout << "只能输入一个姓名，请重新输入：";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                validParentName = true;
            }
            if (!tree.exists(name)) {
                cout << "未找到" << name << endl;
                cout << endl;
                continue;
            }
            success = tree.dissolve(name);
        }
        else if (choice == 'D' || choice == 'd') {
            char oldName[50], newName[50];
            cout << "请输入原姓名：";
            bool validParentName = false;
            while (!validParentName) {
                if (!(cin >> oldName)) {
                    cout << "输入无效，请重新输入：";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                char next = cin.peek();
                while (next == ' ') {
                    cin.get();
                    next = cin.peek();
                }
                if (next != '\n' && next != '\t') {
                    cout << "只能输入一个姓名，请重新输入：";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                validParentName = true;
            }
            if (!tree.exists(oldName)) {
                cout << "未找到" << oldName << endl;
                cout << endl;
                continue;
            }
            bool nameExists;
            do {
                cout << "请输入新姓名：";
                cin >> newName;
                nameExists = (strcmp(oldName, newName) != 0) && tree.exists(newName);
                if (nameExists) {
                    cout << newName << " 已存在！" << endl;
                }
            } while (nameExists);
            success = tree.change(oldName, newName);
        }
        else if (choice == 'E' || choice == 'e') {
            int ancGen, desgen;
            char name[50];
            cout << "请输入查找的成员姓名：";
            bool validParentName = false;
            while (!validParentName) {
                if (!(cin >> name)) {
                    cout << "输入无效，请重新输入：";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                char next = cin.peek();
                while (next == ' ') {
                    cin.get();
                    next = cin.peek();
                }
                if (next != '\n' && next != '\t') {
                    cout << "只能输入一个姓名，请重新输入：";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                validParentName = true;
            }
            if (!tree.exists(name)) {
                cout << "未找到" << name << endl;
                cout << endl;
                continue;
            }
            cout << "请输入要查找的距离己辈几代祖先（0不查找）：";
            while (!(cin >> ancGen) || ancGen < 0) {
                cout << "错误，请重新输入：";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            cout << "请输入要查找的距离己辈几代的子孙（0不查找）：";
            while (!(cin >> desgen) || desgen < 0) {
                cout << "错误，请重新输入：";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            success = tree.findrel(name, ancGen, desgen);
        }
        else if (choice == 'F' || choice == 'f') {
            cout << "退出程序！" << endl;
            break;
        }
        if (success && choice != 'E' && choice != 'e' && choice != 'F' && choice != 'f') {
            tree.show();
        }
        cout << endl;
    }
    cout << "\n程序运行结束，按回车键退出" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return 0;
}