#include<iostream>
#include<iomanip>
#include <limits>
using namespace std;

class stack;
// 节点类，表示迷宫中的位置坐标
class node {
    friend class stack;
    int row;// 行坐标
    int col;// 列坐标
    node* next;// 指向下一个节点的指针
public:
    // 构造函数
    node(int r, int c) :row(r), col(c), next(nullptr) {}
    ~node() {}// 析构函数
public:
    // 打印节点坐标信息
    void print() {
        cout << '(' << row << ',' << col << ')';
    }
};

// 栈类，用于存储迷宫路径
class stack {
    node* top;// 栈顶指针
    int size;// 栈的大小
public:
    stack() :top(nullptr), size(0) {}// 构造函数
    ~stack() {// 析构函数，释放所有节点内存
        while (top != nullptr) {
            node* cur = top;
            top = top->next;
            delete cur;
        }
    }
    void insert(int x, int y);// 向栈中插入新节点
    void remove(); // 删除栈顶节点
    node* get_top();// 获取栈顶节点
    node* top_next() { return top->next; }// 获取栈顶的下一个节点
    int getsize() { return size; }// 获取栈的大小
    void getlist(bool& is_road);// 获取并打印路径列表
};

// 向栈中插入新节点
void stack::insert(int x, int y) {
    node* cur = new node(x, y);
    if (size != 0) {
        cur->next = top;
    }
    top = cur;
    size++;
}

// 删除栈顶节点
void stack::remove() {
    if (top == nullptr) {
        return;
    }
    node* cur = top; 
    top = top->next;
    delete cur;
    size--;
}

// 获取栈顶节点
node* stack::get_top() {
    return top;
}

// 创建二维动态数组
template<typename T>
T** arr_create(const int n) {
    T** maze = new T * [n + 1];
    for (int i = 0; i <= n; ++i) {
        maze[i] = new T[n + 1];   
    }
    return maze;
}

//释放二维动态数组内存
template<typename T>
void arr_break(T** a, const int n) {
    for (int i = 0; i <= n; ++i) {
        delete[] a[i];
    }
    delete[] a; 
}

// 打印路径
void stack::getlist(bool& is_road) {
    stack p;
    node* cur = top;
    if (cur == nullptr || size == 1)
        return;
    for (int i = 0; i < size; i++) {
        p.insert(cur->row, cur->col);
        cur = cur->next;
    }
    cout << "迷宫路径：" << endl;
    node* curp = p.get_top();
    // 打印路径
    for (int i = 0; i < size && curp != nullptr; i++) {
        is_road = 1;// 标记存在路径
        curp->print();
        if (i < size - 1 && curp->next != nullptr)
            cout << "--->";
        curp = curp->next;
    }
    cout << endl;
}

int dir[4][2] = { -1,0,0,-1,1,0,0,1 };
// 深度优先搜索迷宫路径
void dfs(stack& s, int** maze, bool** visit, int x, int y, int n, bool& is_road) {
    s.insert(x, y); 
    visit[x][y] = 1;
    // 到出口，打印路径
    if (x == n || y == n || x == 1 || y == 1) {
        s.getlist(is_road);
    }
    // 四个方向
    for (int i = 0; i < 4; i++) {
        int curx = x + dir[i][0]; 
        int cury = y + dir[i][1];
        // 检查下一个位置是否合法
        if (curx<1 || cury<1 || curx>n || cury>n || visit[curx][cury] == 1 || maze[curx][cury] == 1)
            continue;
        // 通路，继续
        if (maze[curx][cury] == 0) {
            dfs(s, maze, visit, curx, cury, n, is_road);  // 递归搜索
            s.remove();
            visit[curx][cury] = 0; 
        }
    }
}

// 打印迷宫地图
void print_maze(int** maze, int n) {
    cout << "迷宫地图：" << endl;
    cout << setw(10) << ' '; 
    for (int i = 1; i <= n; i++) {
        cout << "第" << i << "列" << "   ";
    }
    cout << endl;
    // 打印迷宫内容
    for (int i = 1; i <= n; i++) {
        cout << "  " << "第" << i << "行    ";
        for (int j = 1; j <= n; j++) {
            cout << " " << maze[i][j] << "      "; 
        }
        cout << endl << endl;
    }
}

// 检查起始位置
bool is_valid_position(int** maze, int n, int startx, int starty) {
    if (startx < 1 || startx > n || starty < 1 || starty > n) {
        cout << "错误：起始位置必须在迷宫范围内(1-" << n << ")" << endl;
        return false;
    }
    if (maze[startx][starty] == 1) {
        cout << "错误：起始位置不能是墙（1）" << endl;
        return false;
    }
    return true;
}

// 输入起始位置并且检查
void get_start_position(int** maze, int n, int& startx, int& starty) {
    bool valid_start = false;
    while (!valid_start) {
        cout << "请输入起始位置x,y:" << endl;
        if (!(cin >> startx >> starty)) {
            cout << "错误：输入必须为整数" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        char next;
        next = cin.peek();
        while (next == ' ') {
            cin.get();
            next = cin.peek();
        }
        if (next != '\n' && next != EOF) {
            cout << "错误：只能输入两个整数" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (is_valid_position(maze, n, startx, starty)) {
            valid_start = true;
        }
        else {
            cout << "请重新输入" << endl;
        }
    }
}

// 检查迷宫输入
bool is_valid_maze_value(int value) {
    return value == 0 || value == 1;
}

// 输入迷宫并且检查
void get_valid_maze(int** maze, int n) {
    bool valid_input = false;
    while (!valid_input) {
        cout << "请输入地图（1为墙，0为路）：" << endl;
        valid_input = true;
        for (int i = 1; i <= n; i++) {
            cout << "第" << i << "行: ";
            for (int j = 1; j <= n; j++) {
                int value;
                if (!(cin >> value)) {
                    cout << "错误：输入必须为整数（0或1）" << endl;
                    valid_input = false;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    break;
                }
                if (!is_valid_maze_value(value)) {
                    cout << "错误：输入值必须为0或1" << endl;
                    valid_input = false;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    break;
                }
                maze[i][j] = value;
            }
            if (!valid_input) {
                cout << "请重新输入整个迷宫" << endl;
                break;
            }
            char next;
            next = cin.peek();
            while (next == ' ') {
                cin.get();
                next = cin.peek();
            }
            if (next != '\n' && next != EOF) {
                cout << "错误：第" << i << "行输入个数过多或者含有小数" << endl;
                valid_input = false;
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }
        }
    }
}

//检查是否存在起点
bool is_start(int** maze, int n) {
    for (int i = 1; i <= n; i++) {
        if (maze[1][i] == 0)return true;
        if (maze[n][i] == 0)return true;
        if (maze[i][1] == 0)return true;
        if (maze[i][n] == 0)return true;
    }
    return false;
}

// 检查n的范围
bool is_valid_n(int n) {
    return n >= 1 && n <= 100;
}

// 输入n并且检查
int get_valid_n() {
    int n;
    bool valid_n = false;
    while (!valid_n) {
        cout << "输入地图（n*n）大小,n(1-100)为：";
        if (!(cin >> n)) {
            cout << "错误：输入必须为整数" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        char next;
        next = cin.peek();
        while (next == ' ') {
            cin.get();
            next = cin.peek();
        }
        if (next != '\n' && next != EOF) {
            cout << "错误：只能输入一个整数" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (!is_valid_n(n)) {
            cout << "错误：n必须在1到100之间" << endl;
            continue;
        }
        valid_n = true;
    }
    return n;
}


int main() {
    int n = get_valid_n(); 
    int** maze = arr_create<int>(n); 
    bool** visit = arr_create<bool>(n);
    get_valid_maze(maze, n);// 迷宫输入
    if (!is_start(maze, n)) {
        print_maze(maze, n);
        cout << "不存在起点,无路径" << endl;
        return 0;
    }
    int startx, starty;
    get_start_position(maze, n, startx, starty);  // 得到起始位置x,y
    print_maze(maze, n);
    bool is_road = 0;
    stack s;
    dfs(s, maze, visit, startx, starty, n, is_road); // 深度优先搜索
    if (!is_road) cout << "不存在路径" << endl;
    arr_break<int>(maze, n);
    arr_break<bool>(visit, n);
    cout << "\n程序运行结束，按回车键退出" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return 0;
}