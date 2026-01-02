#include <iostream>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <limits>

using namespace std;

// 栈类
template<typename t>
class stack {
private:
    t* data;// 存储数据的数组
    int top;// 栈顶指针
    int cap;// 栈容量
    void resize(int newcap);// 调大小

public:
    stack(int initcap = 100) : top(-1), cap(initcap) {
        data = new t[cap];
    }// 构造函数
    ~stack() { delete[] data; }// 析构函数
    void push(const t& value);// 入栈
    t pop();// 出栈
    t& gettop();// 得到栈顶元素
    int size() const;// 得到大小
};
// 调大小
template<typename t>
void stack<t>::resize(int newcap) {
    t* newdata = new t[newcap];
    for (int i = 0; i < top; i++)
        newdata[i] = data[i];
    delete[] data;
    data = newdata;
    cap = newcap;
}
// 入栈
template<typename t>
void stack<t>::push(const t& value) {
    if (top == cap - 1)
        resize(2 * cap);
    data[++top] = value;
}
// 出栈
template<typename t>
t stack<t>::pop() {
    if (!size())
        throw runtime_error("栈为空");
    return data[top--];
}
// 得到栈顶元素
template<typename t>
t& stack<t>::gettop() {
    if (!size())
        throw runtime_error("栈为空");
    return data[top];
}
// 得到栈大小
template<typename t>
int stack<t>::size() const {
    return top + 1;
}

// 数学计算类
class math {
private:
    bool isdigit(char c);// 判断是否为数字
    int getpriority(char op);// 运算符优先级
    bool isoperator(char c);// 判断是否为运算符
    bool isbinary(char c);// 判断是否为双目运算符
    bool isunarypos(const char* shizi, int i);// 判断是否为单目运算符位置
    double mod(double a, double b);// 取模运算
    double calculate(double a, double b, char op);// 执行运算

public:
    bool isvashizi(const char* shizi);// 检查表达式
    bool tohouzhui(const char* infix, char* houzhui);// 中缀转后缀表达式
    double strtof(const char* str);// 字符串转浮点数
    double evalhouzhui(const char* houzhui);// 计算后缀表达式
    double eval(const char* shiziession);// 计算表达式主函数
};
// 判断是否为数字字符
bool math::isdigit(char c) {
    return (c >= '0' && c <= '9') || c == '.';
}
// 获取运算符优先级
int math::getpriority(char op) {
    switch (op) {
    case '^': return 3;
    case '*': case '/': case '%': return 2;
    case '+': case '-': return 1;
    case '~': return 2;  // 单目负号
    case '(': return 0;
    default: return -1;
    }
}
// 判断是否为运算符
bool math::isoperator(char c) {
    return c == '+' || c == '-' || c == '*' ||c == '/' || c == '%' || c == '^' || c == '(' || c == ')';
}
// 判断是否为双目运算符
bool math::isbinary(char c) {
    return c == '*' || c == 'x' || c == 'X' || c == '/' || c == '%' || c == '^' || c == '+' || c == '-';
}
// 判断是否为单目运算符位置
bool math::isunarypos(const char* shizi, int i) {
    if (i == 0) return true; 
    char prev = shizi[i - 1];
    return prev == '(';
}
// 取模运算
double math::mod(double a, double b) {
    if (b == 0) {
        cerr << "错误,除数不能为零" << endl;
        return NAN;
    }
    double result = fmod(a, b);
    if (result != 0 && (a < 0) != (b < 0))
        result += b;
    return result;
}
// 执行运算
double math::calculate(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': case 'x': case 'X': return a * b;
    case '/':
        if (b == 0) {
            cerr << "错误,除数不能为零" << endl;
            return NAN;
        }
        return a / b;
    case '%': return mod(a, b);
    case '^': return pow(a, b);
    default: return 0;
    }
}
// 检查表达式
bool math::isvashizi(const char* shizi) {
    int len = strlen(shizi);
    stack<char> sta1;
    if (len == 0) {
        cerr << "错误,表达式不能为空" << endl;
        return false;
    }
    int equalCount = 0;
    int lastEqualPos = -1;
    for (int i = 0; i < len; i++) {
        if (shizi[i] == '=') {
            equalCount++;
            lastEqualPos = i;
        }
    }
    if (equalCount > 1) {
        cerr << "错误,只能出现一个=" << endl;
        return false;
    }
    if (equalCount == 1 && lastEqualPos != len - 1) {
        cerr << "错误,=必须最后面" << endl;
        return false;
    }
    for (int i = 0; i < len; i++) {
        char c = shizi[i];
        if (c == '(')
            sta1.push(c);
        else if (c == ')') {
            if (!sta1.size()) {
                cerr << "错误,括号不匹配" << endl;
                return false;
            }
            sta1.pop();
        }
        if (c != '=' && isoperator(c) && c != '(' && c != ')') {
            if (i > 0) {
                char prev = shizi[i - 1];
                if (prev != '=' && isoperator(prev) && prev != ')') {
                    if (c == '+' || c == '-') {
                        if (!isunarypos(shizi, i)) {
                            cerr << "错误,不能多个运算符连续" << endl;
                            return false;
                        }
                    }
                    else {
                        cerr << "错误,不能多个运算符连续" << endl;
                        return false;
                    }
                }
            }
        }
    }

    if (sta1.size()) {
        cerr << "错误,括号不匹配" << endl;
        return false;
    }
    return true;
}
// 中缀转后缀
bool math::tohouzhui(const char* infix, char* houzhui) {
    stack<char> sta2;
    int pos = 0;
    int len = strlen(infix);
    for (int i = 0; i < len; i++) {
        char c = infix[i];
        if (c == '=') {
            continue;
        }
        if (c == ' ') continue;
        if (isdigit(c)) {
            while (i < len && isdigit(infix[i]))
                houzhui[pos++] = infix[i++];
            houzhui[pos++] = ' ';
            i--;
        }
        else if (c == '(')
            sta2.push(c);
        else if (c == ')') {
            while (sta2.size() && sta2.gettop() != '(') {
                houzhui[pos++] = sta2.pop();
                houzhui[pos++] = ' ';
            }
            if (sta2.size() && sta2.gettop() == '(')
                sta2.pop();
            else {
                cerr << "错误,括号不匹配" << endl;
                return false;
            }
        }
        else if (isoperator(c)) {
            bool isunary = false;
            if (c == '+' || c == '-') {
                if (isunarypos(infix, i)) {
                    isunary = true;
                }
            }
            if (i > 0 && infix[i - 1] != '=' && isoperator(infix[i - 1]) && infix[i - 1] != ')') {
                if (!isunary) {
                    cerr << "错误,不能多个运算符连续" << endl;
                    return false;
                }
            }
            if (isunary) {
                if (c == '-') {
                    while (sta2.size() && getpriority(sta2.gettop()) >= getpriority('~')) {
                        houzhui[pos++] = sta2.pop();
                        houzhui[pos++] = ' ';
                    }
                    sta2.push('~');
                }
            }
            else {
                while (sta2.size() && getpriority(sta2.gettop()) >= getpriority(c)) {
                    houzhui[pos++] = sta2.pop();
                    houzhui[pos++] = ' ';
                }
                sta2.push(c);
            }
        }
    }
    while (sta2.size()) {
        if (sta2.gettop() == '(') {
            cerr << "错误,括号不匹配" << endl;
            return false;
        }
        houzhui[pos++] = sta2.pop();
        houzhui[pos++] = ' ';
    }
    houzhui[pos] = '\0';
    return true;
}
// 字符串转浮点数
double math::strtof(const char* str) {
    if (str == nullptr) return 0.0;
    double result = 0.0;
    int sign = 1;//符号
    int esign = 1;//指数符号
    int evalue = 0;//指数值
    bool xiaoshu = false;//有无小数
    double xvalue = 1.0;//小数除值
    while (*str == ' ' || *str == '\t' || *str == '\n' ||
        *str == '\r' || *str == '\f' || *str == '\v') {
        ++str;
    }
    if (*str == '+' || *str == '-') {
        sign = (*str == '-') ? -1 : 1;
        ++str;
    }
    while (*str != '\0') {
        if (*str >= '0' && *str <= '9') {
            int digit = *str - '0';
            if (xiaoshu) {
                xvalue *= 10.0;
                result += digit / xvalue;
            }
            else {
                result = result * 10.0 + digit;
            }
            ++str;
        }
        else if (*str == '.' && !xiaoshu) {
            xiaoshu = true;
            ++str;
        }
        else if (*str == 'e' || *str == 'E') {
            ++str;
            if (*str == '+' || *str == '-') {
                esign = (*str == '-') ? -1 : 1;
                ++str;
            }
            while (*str >= '0' && *str <= '9') {
                evalue = evalue * 10 + (*str - '0');
                ++str;
            }
            break;
        }
        else {
            break;
        }
    }
    double exppower = 1.0;
    int absexp = evalue * esign;
    if (absexp > 0) {
        for (int i = 0; i < absexp; ++i) {
            exppower *= 10.0;
        }
    }
    else if (absexp < 0) {
        for (int i = 0; i < -absexp; ++i) {
            exppower /= 10.0;
        }
    }
    return result * sign * exppower;
}
// 计算后缀表达式
double math::evalhouzhui(const char* houzhui) {
    stack<double> numstack;
    char buffer[50];
    int bufindex = 0;
    int len = strlen(houzhui);
    for (int i = 0; i < len; i++) {
        char c = houzhui[i];
        if (isdigit(c)) {
            bufindex = 0;
            while (i < len && isdigit(houzhui[i])) {
                buffer[bufindex++] = houzhui[i++];
            }
            buffer[bufindex] = '\0';
            double num = strtof(buffer);
            numstack.push(num);
            i--;
        }
        else if (c == '~') {
            if (!numstack.size()) {
                cerr << "错误,操作数不足" << endl;
                return NAN;
            }
            double a = numstack.pop();
            numstack.push(-a);
        }
        else if (isoperator(c) && c != '(' && c != ')') {
            if (numstack.size() < 2) {
                cerr << "错误,操作数不足" << endl;
                return NAN;
            }
            double b = numstack.pop();
            double a = numstack.pop();
            double result = calculate(a, b, c);
            if (isnan(result)) {
                cout << "输入非法，重新输入" << endl;
                return NAN;
            }
            numstack.push(result);
        }
    }
    if (numstack.size() != 1) {
        cerr << "错误,表达式错误" << endl;
        return NAN;
    }
    cout << "后缀表达式:";
    for (int i = 0; i < len; i++) {
        cout << houzhui[i];
    }
    cout << endl;
    return numstack.pop();
}
// 主要计算表达式
double math::eval(const char* shiziession) {
    char input[100];
    char houzhui[200];
    strcpy_s(input, shiziession);
    int len = strlen(input);
    if (strlen(input) == 0) {
        cerr << "错误,表达式不能为空" << endl;
        return NAN;
    }
    if (!isvashizi(input)) return NAN;
    if (len > 0 && input[len - 1] == '=')
        input[len - 1] = '\0';
    if (!tohouzhui(input, houzhui)) return NAN;
    return evalhouzhui(houzhui);
}
int main() {
    math solver;
    char input[100];
    char choice = 'y';
    do {
        cout << "输入表达式(英文符号，运算符后面单目加括号)" << endl;
        cin.getline(input, 100);
        double result = solver.eval(input);
        if (!isnan(result))
            cout << "结果:" << result << endl;
        while (true) {
            cout << "是否继续（y, n）？(含大小写)";
            cin >> choice;
            cin.ignore(INT_MAX, '\n');
            if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N')
                break;
            cout << "输入错误，重新输入" << endl;
        }
    } while (choice == 'y' || choice == 'Y');
    cout << "\n程序运行结束，按回车键退出" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return 0;
}