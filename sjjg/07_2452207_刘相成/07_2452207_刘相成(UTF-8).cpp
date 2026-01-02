#include<iostream>
#include<limits>
#include<stdexcept>

using namespace std;

// 优先队列类
class priority__queue {
private:
    int* heap; // 存储元素数组
    int cap; // 容量
    int size; // 大小
    void min_heap(int i);//最小堆化
    void swap_node(int i, int j); //交换两个结点
public:
    priority__queue(int cap);// 构造函数
    ~priority__queue();// 析构函数
    void insert(int value);// 插入元素
    int get_min(); // 提取最小元素
    int show_min();    // 获取最小元素(非删除)
    int get_size();  // 获取当前堆的大小
    bool is_empty(); // 是否为空
    void print(); // 打印元素
};
// 构造函数
priority__queue::priority__queue(int N) {
    cap = N;
    size = 0;
    heap = new int[cap];
}
// 析构函数
priority__queue::~priority__queue() {
    delete[] heap;
    heap = nullptr;
}
// 交换两个结点
void priority__queue::swap_node(int i, int j) {
    int temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}
// 最小堆化
void priority__queue::min_heap(int i) {
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int smallest = i;
    if (l < size && heap[l] < heap[i]) {
        smallest = l;
    }
    if (r < size && heap[r] < heap[smallest]) {
        smallest = r;
    }
    if (smallest != i) {
        swap_node(i, smallest);
        min_heap(smallest);
    }
}
// 插入元素
void priority__queue::insert(int value) {
    size++;
    int index = size - 1;
    heap[index] = value;
    while (index != 0 && heap[(index - 1) / 2] > heap[index]) {
        swap_node(index, (index - 1) / 2);
        index = (index - 1) / 2;
    }
}
// 提取最小元素
int priority__queue::get_min() {
    if (size <= 0) {
        throw runtime_error("优先队列为空");
    }
    if (size == 1) {
        size--;
        return heap[0];
    }
    int root = heap[0];
    heap[0] = heap[size - 1];
    size--;
    min_heap(0);
    return root;
}
// 获取最小元素
int priority__queue::show_min() {
    if (size <= 0) {
        throw runtime_error("优先队列为空");
    }
    return heap[0];
}
// 获取当前堆的大小
int priority__queue::get_size() {
    return size;
}

// 是否为空
bool priority__queue::is_empty() {
    return size == 0;
}

// 打印元素
void priority__queue::print() {
    if (size == 0) {
        cout << "空";
        return;
    }
    for (int i = 0; i < size; i++) {
        cout << heap[i];
        if (i < size - 1) {
            cout << ' ';
        }
    }
}
int main() {
    cout << "输入木头锯成块数(1-10000)" << endl;
    int n = 0;
    while (1) {
        while (!(cin >> n) || n <= 0 || n > 10000) {
            cout << "输入错误，重新输入" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
        char next = cin.peek();
        while (next == ' ') {
            cin.get();
            next = cin.peek();
        }
        if (next != '\n' && next != EOF) {
            cout << "多于输入，重新输入" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        break;
    }
    cout << "输入" << n << "块木头的长度" << endl;
    priority__queue wood(n);
    bool inputFlag = true;
    while (inputFlag) {
        inputFlag = false;
        int cur_len;
        for (int i = 0; i < n; i++) {
            if (!(cin >> cur_len) || cur_len <= 0) {
                cout << "输入错误，重新输入" << endl;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                inputFlag = true;
                break;
            }
            wood.insert(cur_len);
        }
        if (inputFlag) {
            while (!wood.is_empty()) {
                wood.get_min();
            }
            continue;
        }
        char next = cin.peek();
        while (next == ' ') {
            cin.get();
            next = cin.peek();
        }
        if (next != '\n' && next != EOF) {
            cout << "多于输入，重新输入" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            inputFlag = true;
            while (!wood.is_empty()) {
                wood.get_min();
            }
            continue;
        }
    }
    long long totalCost = 0;
    cout << "锯数木的方法(逆序)" << endl;
    while (wood.get_size() > 1) {
        int min1 = wood.get_min();
        int min2 = wood.get_min();
        int newLength = min1 + min2;
        totalCost += newLength;
        wood.insert(newLength);
        cout << "原来木头序列:";
        wood.print();
        cout <<"||选择"<<newLength<<"拆成" << min1 <<"与" << min2 ;
        cout << " 此次花费:" << newLength << endl;
    }
    if (wood.get_size() == 1) {
        cout << "木头原长:" << wood.show_min()  << endl;
    }
    cout << "总花费:" << totalCost << endl;
    cout << "\n程序运行结束，按回车键退出" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return 0;
}