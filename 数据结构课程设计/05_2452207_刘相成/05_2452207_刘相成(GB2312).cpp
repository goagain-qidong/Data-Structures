#include <iostream>
#include <stdexcept>
#include <cmath>
#include <limits>
using namespace std;

 //队列类
template<typename T>
class queue {
private:
    struct node {
        T data;// 节点数据
        node* next;// 指向下一节点的指针
        node(T element) : data(element), next(nullptr) {} // 构造函数
        ~node() {}// 析构函数
    };
    node* front; // 头指针
    node* rear;  // 尾指针
    int size;// 大小

public:
    queue() : front(nullptr), rear(nullptr), size(0) {} // 构造函数
    ~queue() { clear(); } // 析构函数

    void push(T element); // 入队
    T pop();// 出队
    T getfront();// 获得队头元素
    int getsize();// 获得队列大小
    void clear();// 清空队列
};

//入队
template<typename T>
void queue<T>::push(T element) {
    if (rear == nullptr) {
        rear = new node(element);
        front = rear;
    }
    else {
        rear->next = new node(element);
        rear = rear->next;
    }
    size++;
}

//出队
template<typename T>
T queue<T>::pop() {
    if (front == nullptr) {
        throw std::runtime_error("队列为空");
    }
    T element = front->data;
    node* temp = front;
    front = front->next;
    size--;
    if (size == 0) rear = nullptr;
    delete temp;
    return element;
}

//获得队头元素
template<typename T>
T queue<T>::getfront() {
    if (front == nullptr) {
        throw std::runtime_error("队列为空");
    }
    return front->data;
}

//获得大小
template<typename T>
int queue<T>::getsize() {
    return size;
}

//清空队列
template<typename T>
void queue<T>::clear() {
    while (size) {
        pop();
    }
}


//检查并分配编号
template<typename T>
void check(queue<T>& a, queue<T>& b, int n) {
    bool flag = 1;
    while (flag) {
        flag = 0;
        double data = 0;
        char next;
        int count = 0;
        cout << "依次输入顾客编号（大于0且不超过2^31-1），空格分开" << endl;
        for (int i = 0; i < n; i++) {
            cin >> data;
            // 为正整数且在int范围内
            if (cin.fail() || data <= 0 || data > INT_MAX || floor(data) != data) {
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cout << "不是规定范围内的正整数，全部重新输入" << endl;
                a.clear();
                b.clear();
                flag = 1;
                break;
            }
            next = cin.peek();
            // 中间用空格分隔
            if (i < n - 1) {
                if (next != ' ') {
                    cin.clear();
                    cin.ignore(INT_MAX, '\n');
                    cout << "多于输入，全部重新输入" << endl;
                    a.clear();
                    b.clear();
                    flag = 1;
                    break;
                }
            }
            // 末尾不能有多余字符
            if (i == n - 1) {
                while (next == ' ') {
                    cin.get();
                    next = cin.peek();
                }
                if (next != '\n' && next != EOF) {
                    cin.clear();
                    cin.ignore(INT_MAX, '\n');
                    cout << "多余输入，全部重新输入" << endl;
                    a.clear();
                    b.clear();
                    flag = 1;
                    break;
                }
            }
            // 根据奇偶分配不同队列
            T intData = (T)(data);
            if (intData & 1)
                a.push(intData);
            else b.push(intData);
            count++;
        }
        if (!flag) cin.ignore(INT_MAX, '\n');
    }
}

//检查顾客总数
void check_n(int& n) {
    while (1) {
        double input;
        cout << "顾客总数（不超过1000且不小于1）" << endl;
        cin >> input;
        if (cin.fail() || input <= 0 || input > 1000 || floor(input) != input) {
            cout << "不是1-1000的正整数，重新输入" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
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
            continue;
        }
        cin.ignore(INT_MAX, '\n');
        n = (int)(input);
        break;
    }
}

//输出编号
void print(int& n, queue<int>& a, queue<int>& b) {
    while (n) {
        if (a.getsize()) { // 优先奇数
            cout << a.pop();
            if (n != 1) cout << ' ';
            n--;
            if (a.getsize()) {  // 第二个奇数
                cout << a.pop();
                if (n != 1) cout << ' ';
                n--;
            }
        }
        if (b.getsize()) {// 偶数
            cout << b.pop();
            if (n != 1) cout << ' ';
            n--;
        }
    }
    //cout<<'*'<<endl;
    //测试末尾空格
 }

int main() {
    int n;
    check_n(n);
    queue<int> a, b;
    check(a, b, n);
    print(n, a, b);
    cout << "\n程序运行结束，按回车键退出" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return 0;
}