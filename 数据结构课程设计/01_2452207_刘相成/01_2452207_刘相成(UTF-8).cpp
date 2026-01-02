#define _CRT_SECURE_NO_WARNINGS  
#include<iostream>
#include<cstring>    
#include<iomanip>    
#include<limits>    
using namespace std;

#define MAX_LENGTH 40  
class linklist;

// 存储单个考生的所有信息的类
class student {
    friend class linklist;
    int number; // 考号
    char name[MAX_LENGTH];// 姓名
    char sex[8]; // 性别
    int age; // 年龄
    char application_category[MAX_LENGTH];  // 报考类别
public:
    student* next;// 指向下一个考生的指针
    // 构造函数，初始化
    student(int number, const char name[], const char sex[], int age, const char app[]) : number(number), age(age) {
        strncpy_s(this->name, name, MAX_LENGTH - 1);
        strncpy_s(this->sex, sex, sizeof(this->sex) - 1);
        strncpy_s(this->application_category, app, MAX_LENGTH - 1);
        this->next = nullptr;
    }
    ~student() {}  // 析构函数
    // 复制构造函数
    student(const student& other)
        : number(other.number), age(other.age) {
        strncpy_s(this->name, other.name, MAX_LENGTH - 1);
        strncpy_s(this->sex, other.sex, sizeof(this->sex) - 1);
        strncpy_s(this->application_category, other.application_category, MAX_LENGTH - 1);
        this->next = nullptr;
    }
};

// 储存所有考生信息的链表类
class linklist {
    student* head;// 链表头指针
    int size;  // 链表当前大小
    int width[5];  // 各列显示宽度，格式化输出
public:
    linklist() : head(nullptr), size(0) {
        for (int i = 0; i < 5; i++) {
            width[i] = 0;
        }
    };// 构造函数，初始化
    ~linklist() { clear(); }// 析构函数：释放所有节点内存
    void insert(int position, bool should_print = true);  // 在指定位置插入考生
    void remove(int number);  // 根据考号删除考生
    void find(int number);// 根据考号查找考生
    void update(int number); // 根据考号更新考生信息
    void student_count(); // 统计考生信息
    void print_all();// 打印所有考生信息
    int get_size() const { return size; } // 获取考生人数

private:
    void clear(); // 清空链表
    void print_student(const student* s) const;// 打印单个考生信息
    student* create_student(); // 创建新考生对象
    student* create_student2(int exclude_number = -1); // 带考号验证的创建
    void update_width(int number, const char name[], const char sex[], int age, const char app[]); // 更新宽度
    bool find_student(int number, student*& prev, student*& curr);    // 查找考生并返回前驱和当前节点
    bool input_data(int& number, char name[], char sex[], int& age, char app[], int exclude_number = -1); // 输入考生数据
    bool is_exists(int number, int exclude_number = -1) const; // 检查考号是否已存在
};

// 检查输入
bool safe_input(int& value) {
    while (true) {
        cin >> value;
        if (cin.fail()) {
            cerr << "输入格式错误，请重新输入" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        cin.ignore(INT_MAX, '\n');
        return true;
    }
}

// 带范围检查的输入
bool safe_input_range(int& value, int min_val, int max_val) {
    while (true) {
        if (!safe_input(value)) {
            continue;
        }
        if (value < min_val || value > max_val) {
            cerr << "输入值超出范围 [" << min_val << "-" << max_val << "]，请重新输入" << endl;
            continue;
        }
        return true;
    }
}

// 字符串检查
bool safe_string_input(char* buffer, int buffer_size) {
    while (true) {
        cin.getline(buffer, buffer_size);
        if (strlen(buffer) == 0) {
            cerr << "输入不能为空，请重新输入" << endl;
            continue;
        }
        return true;
    }
}

// 检查考号是否已存在
bool linklist::is_exists(int number, int exclude_number) const {
    student* current = head;
    while (current != nullptr) {
        if (exclude_number != -1 && current->number == exclude_number) {
            current = current->next;
            continue;
        }
        if (current->number == number) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// 输入考生数据并验证
bool linklist::input_data(int& number, char name[], char sex[], int& age, char app[], int exclude_number) {
    while (true) {
        cout << "请输入考号: ";
        if (!safe_input_range(number, 1, INT_MAX)) {
            continue;
        }
        if (is_exists(number, exclude_number)) {
            cerr << "考号 " << number << " 已存在，请重新输入" << endl;
            continue;
        }
        break;
    }
    cout << "请输入姓名: ";
    safe_string_input(name, MAX_LENGTH);
    while (true) {
        cout << "请输入性别(男/女): ";
        safe_string_input(sex, 8);
        if (strcmp(sex, "男") == 0 || strcmp(sex, "女") == 0) {
            break;
        }
        cerr << "性别只能是'男'或'女'，请重新输入" << endl;
    }
    cout << "请输入年龄: ";
    while (!safe_input_range(age, 1, 100)) {}
    cout << "请输入报考类别: ";
    safe_string_input(app, MAX_LENGTH);
    return true;
}

// 创建对象
student* linklist::create_student() {
    int number, age;
    char name[MAX_LENGTH], sex[8], app[MAX_LENGTH];
    cout << "请依次输入考生的信息：" << endl;
    input_data(number, name, sex, age, app);
    update_width(number, name, sex, age, app);
    return new student(number, name, sex, age, app);
}

// 修改对象
student* linklist::create_student2(int exclude_number) {
    int number, age;
    char name[MAX_LENGTH], sex[8], app[MAX_LENGTH];
    cout << "请依次输入考生的新信息：" << endl;
    input_data(number, name, sex, age, app, exclude_number);
    update_width(number, name, sex, age, app);
    return new student(number, name, sex, age, app);
}

// 更新显示宽度
void linklist::update_width(int number, const char name[], const char sex[], int age, const char app[]) {
    char temp[20];
    snprintf(temp, sizeof(temp), "%d", number);
    width[0] = max(width[0], (int)strlen(temp));
    width[1] = max(width[1], (int)strlen(name));
    width[2] = max(6,(int)strlen(sex));
    width[3] = max(width[3], (int)strlen(temp));
    snprintf(temp, sizeof(temp), "%d", age);
    width[3] = max(width[3], (int)strlen(temp));
    width[4] = max(width[4], (int)strlen(app));
}

// 在指定位置插入考生
void linklist::insert(int position, bool should_print) {
    if (position < 1 || position > size + 1) {
        cerr << "插入位置错误,有效范围: 1-" << size + 1 << endl;
        return;
    }
    student* new_student = create_student();
    if (position == 1) {
        new_student->next = head;
        head = new_student;
    }
    else {
        student* current = head;
        for (int i = 1; i < position - 1; i++) {
            current = current->next;
        }
        new_student->next = current->next;
        current->next = new_student;
    }
    size++;
    if (should_print) {
        cout << "插入成功" << endl;
        cout << "当前考生列表：" << endl;
        print_all();
    }
}

// 查找考生
bool linklist::find_student(int number, student*& prev, student*& curr) {
    prev = nullptr;
    curr = head;
    while (curr != nullptr) {
        if (curr->number == number) {
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

// 根据考号删除考生
void linklist::remove(int number) {
    student* prev = nullptr;
    student* curr = nullptr;
    if (!find_student(number, prev, curr)) {
        cerr << "考号为 " << number << " 的考生不存在" << endl;
        return;
    }
    cout << "将要删除的考生信息：" << endl;
    print_student(curr);
    if (prev == nullptr) {
        head = curr->next;
    }
    else {
        prev->next = curr->next;
    }
    delete curr;
    size--;
    cout << "删除成功" << endl;
    cout << "当前考生列表：" << endl;
    print_all();
}

// 根据考号查找并显示考生信息
void linklist::find(int number) {
    student* prev = nullptr;
    student* curr = nullptr;
    if (!find_student(number, prev, curr)) {
        cerr << "考号为 " << number << " 的考生不存在" << endl;
        return;
    }
    cout << "查找到的考生信息：" << endl;
    cout << left << setw(max(width[0], 4) + 4) << "考号";
    cout << left << setw(width[1] + 4) << "姓名";
    cout << left << setw(width[2] + 4) << "性别";
    cout << left << setw(max(width[3], 4) + 4) << "年龄";
    cout << left << setw(width[4] + 4) << "报考类别" << endl;

    print_student(curr);
}

// 根据考号修改考生信息
void linklist::update(int number) {
    student* prev = nullptr;
    student* curr = nullptr;
    if (!find_student(number, prev, curr)) {
        cerr << "考号为 " << number << " 的考生不存在" << endl;
        return;
    }
    cout << "当前考生信息：" << endl;
    print_student(curr);
    cout << "请输入新的信息：" << endl;
    student* new_student = create_student2(number);
    new_student->next = curr->next;
    if (prev == nullptr) {
        head = new_student;
    }
    else {
        prev->next = new_student;
    }
    delete curr;
    cout << "修改成功\n当前考生列表：" << endl;
    print_all();
}

// 统计考生信息
void linklist::student_count() {
    if (size == 0) {
        cout << "考生信息系统为空" << endl;
        return;
    }
    char choice;
    while (true) {
        cout << "请选择统计方式(1总人数,2按性别统计,3按报考类别统计)" << endl;
        cout << "请选择: ";
        cin >> choice;
        cin.ignore(INT_MAX, '\n');
        if (choice >= '1' && choice <= '3') break;
        cerr << "输入错误，请重新选择" << endl;
    }
    if (choice == '1') {
        cout << "考生总人数: " << size << endl;
        return;
    }
    if (choice == '2') {
        char gender[8];
        while (true) {
            cout << "请输入要统计的性别(男/女): ";
            cin.getline(gender, sizeof(gender));
            if (strcmp(gender, "男") == 0 || strcmp(gender, "女") == 0) break;
            cerr << "性别只能是'男'或'女'！" << endl;
        }
        int count = 0;
        student* current = head;
        while (current != nullptr) {
            if (strcmp(current->sex, gender) == 0) {
                count++;
            }
            current = current->next;
        }
        cout << gender << "性考生人数: " << count << endl;
        return;
    }
    if (choice == '3') {
        char category[MAX_LENGTH];
        cout << "请输入要统计的报考类别: ";
        safe_string_input(category, MAX_LENGTH);
        int count = 0;
        student* current = head;
        while (current != nullptr) {
            if (strcmp(current->application_category, category) == 0) {
                count++;
            }
            current = current->next;
        }
        cout << "报考" << category << "的考生人数: " << count << endl;
    }
}

// 打印单个考生信息
void linklist::print_student(const student* s) const {
    if (s == nullptr) return;
    cout << left << setw(max(width[0], 4) + 4) << s->number;
    cout << left << setw(width[1] + 4) << s->name;
    cout << left << setw(width[2] + 4) << s->sex;
    cout << left << setw(max(width[3], 4) + 4) << s->age;
    cout << left << setw(width[4] + 4) << s->application_category << endl;
}

// 打印所有考生信息
void linklist::print_all() {
    if (size == 0) {
        cout << "考生列表为空" << endl;
        return;
    }
    cout << left << setw(max(width[0], 4) + 4) << "考号";
    cout << left << setw(width[1] + 4) << "姓名";
    cout << left << setw(width[2] + 4) << "性别";
    cout << left << setw(max(width[3], 4) + 4) << "年龄";
    cout << left << setw(width[4] + 4) << "报考类别" << endl;
    cout << setfill('-') << setw(80) << "" << setfill(' ') << endl;
    student* current = head;
    while (current != nullptr) {
        print_student(current);
        current = current->next;
    }
}

// 清空链表，释放内存
void linklist::clear() {
    student* current = head;
    while (current != nullptr) {
        student* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
    size = 0;
}

int main() {
    linklist student_system;
    cout << "=== 考生信息管理系统 ===" << endl;
    int initial_count;
    do {
        cout << "请输入初始考生人数: ";
    } while (!safe_input_range(initial_count, 1, 10000));
    cout << "接下来请依次输入考生的考号，姓名，性别，年龄及报考类别" << endl;
    if (initial_count > 0) {
        cout << "请依次输入" << initial_count << "名考生的信息：" << endl;
        for (int i = 1; i <= initial_count; i++) {
            cout << "\n第" << i << "名考生：" << endl;
            student_system.insert(i, false);
        }
    }
    cout << "\n初始考生列表：" << endl;
    student_system.print_all();
    while (true) {
        cout << "\n选择您要进行的操作(1插入考生,2删除考生,3查找考生,4修改考生信息,5统计,0退出系统)" << endl;
        char choice;
        while (true) {
            cout << "请选择操作(0-5): ";
            cin >> choice;
            cin.ignore(INT_MAX, '\n');

            if (choice >= '0' && choice <= '5') break;
            cerr << "输入错误，请重新选择" << endl;
        }
        cout << endl;
        if (choice == '0') {
            break;
        }
        switch (choice) {
        case '1': {  // 插入
            int position;
            cout << "请输入插入位置: ";
            if (safe_input_range(position, 1, student_system.get_size() + 1)) {
                student_system.insert(position);
            }
            break;
        }
        case '2': {  // 删除
            int number;
            cout << "请输入要删除的考生考号: ";
            if (safe_input(number)) {
                student_system.remove(number);
            }
            break;
        }
        case '3': {  // 查找
            int number;
            cout << "请输入要查找的考生考号: ";
            if (safe_input(number)) {
                student_system.find(number);
            }
            break;
        }
        case '4': {  // 修改
            int number;
            cout << "请输入要修改的考生考号: ";
            if (safe_input(number)) {
                student_system.update(number);
            }
            break;
        }
        case '5': {  // 统计
            student_system.student_count();
            break;
        }
        }
    }
    cout << "考生信息管理系统关闭" << endl;
    cout << "\n程序运行结束，按回车键退出" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return 0;
}