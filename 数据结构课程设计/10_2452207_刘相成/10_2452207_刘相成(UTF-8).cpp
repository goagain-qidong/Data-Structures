#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

// 排序算法比较类
class sort_comp {
private:
    int* ori_data; // 原始数据数组指针
    int* data; // 排序用数组指针
    long long swap_cnt; // 交换计数
    int n; // 数据数量
    void reset(); //还原数据到原始状态
    void _swap(int& a, int& b); //交换整数
    int partition(int low, int high); //快速排序的分区
    void quick_sortRec(int low, int high); //快速排序的递归
    void heapify(int size, int i); //堆排序的堆化
    void merge(int l, int m, int r, int* tempArr); //归并排序的合并
    void merge_sortRec(int l, int r, int* tempArr); // 归并排序的递归
    int getMax(); // 基数排序获取最大值
    void countSort(int exp, int* output); //基数排序的计数排序

public:
    // 构造函数
    sort_comp() {
        n = 0;
        ori_data = nullptr;
        data = nullptr;
        swap_cnt = 0;
        srand((unsigned)time(NULL));
    }
    ~sort_comp() {
        if (ori_data) delete[] ori_data;
        if (data) delete[] data;
    }// 析构函数
    void check_n(); //生成随机数，输入验证
    void bubble_sort(); // 冒泡排序
    void selection_sort(); // 选择排序
    void insertion_sort(); // 直接插入排序
    void shell_sort(); // 希尔排序
    void quick_sort(); // 快速排序
    void heap_sort(); //堆排序
    void merge_sort(); //归并排序
    void radix_sort(); // 基数排序
    void print(const char* name, clock_t start); // 打印结果
    void menu(); // 显示菜单
};

// 还原数据到原始状态
void sort_comp::reset() {
    for (int i = 0; i < n; i++) {
        data[i] = ori_data[i];
    }
    swap_cnt = 0;
}
// 基数排序的计数排序
void sort_comp::countSort(int exp, int* output) {
    int count[10] = { 0 };
    for (int i = 0; i < n; i++)
        count[(data[i] / exp) % 10]++;
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];
    for (int i = n - 1; i >= 0; i--) {
        output[count[(data[i] / exp) % 10] - 1] = data[i];
        count[(data[i] / exp) % 10]--;
        swap_cnt++;
    }
    for (int i = 0; i < n; i++) {
        data[i] = output[i];
        swap_cnt++;
    }
}
// 交换元素
void sort_comp::_swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
    swap_cnt++;
}
// 快速排序的分区
int sort_comp::partition(int low, int high) {
    int pivot = data[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (data[j] < pivot) {
            i++;
            _swap(data[i], data[j]);
        }
    }
    _swap(data[i + 1], data[high]);
    return (i + 1);
}
// 堆排序的堆化
void sort_comp::heapify(int size, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < size && data[l] > data[largest]) largest = l;
    if (r < size && data[r] > data[largest]) largest = r;
    if (largest != i) {
        _swap(data[i], data[largest]);
        heapify(size, largest);
    }
}
// 归并排序的合并
void sort_comp::merge(int l, int m, int r, int* tempArr) {
    int i = l;
    int j = m + 1;
    int k = 0;
    while (i <= m && j <= r) {
        if (data[i] <= data[j]) {
            tempArr[k++] = data[i++];
        }
        else {
            tempArr[k++] = data[j++];
        }
        swap_cnt++;
    }
    while (i <= m) {
        tempArr[k++] = data[i++];
        swap_cnt++;
    }
    while (j <= r) {
        tempArr[k++] = data[j++];
        swap_cnt++;
    }
    for (i = 0; i < k; i++) {
        data[l + i] = tempArr[i];
        swap_cnt++;
    }
}
// 快速排序的递归
void sort_comp::quick_sortRec(int low, int high) {
    if (low < high) {
        int pi = partition(low, high);
        quick_sortRec(low, pi - 1);
        quick_sortRec(pi + 1, high);
    }
}
// 归并排序的递归
void sort_comp::merge_sortRec(int l, int r, int* tempArr) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sortRec(l, m, tempArr);
        merge_sortRec(m + 1, r, tempArr);
        merge(l, m, r, tempArr);
    }
}
// 基数排序获取最大值
int sort_comp::getMax() {
    int mx = data[0];
    for (int i = 1; i < n; i++)
        if (data[i] > mx) mx = data[i];
    return mx;
}
// 选择排序
void sort_comp::selection_sort() {
    reset();
    clock_t start = clock();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++)
            if (data[j] < data[min_idx]) min_idx = j;
        if (min_idx != i) _swap(data[min_idx], data[i]);
    }
    print("选择排序", start);
}
// 冒泡排序
void sort_comp::bubble_sort() {
    reset();
    clock_t start = clock();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                _swap(data[j], data[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
    print("冒泡排序", start);
}
// 归并排序
void sort_comp::merge_sort() {
    reset();
    clock_t start = clock();
    int* tempArr = new int[n];
    merge_sortRec(0, n - 1, tempArr);
    delete[] tempArr;
    print("归并排序", start);
}
// 希尔排序
void sort_comp::shell_sort() {
    reset();
    clock_t start = clock();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = data[i];
            int j;
            for (j = i; j >= gap && data[j - gap] > temp; j -= gap) {
                data[j] = data[j - gap];
                swap_cnt++;
            }
            data[j] = temp;
            swap_cnt++;
        }
    }
    print("希尔排序", start);
}
// 快速排序
void sort_comp::quick_sort() {
    reset();
    clock_t start = clock();
    quick_sortRec(0, n - 1);
    print("快速排序", start);
}
// 直接插入排序
void sort_comp::insertion_sort() {
    reset();
    clock_t start = clock();
    for (int i = 1; i < n; i++) {
        int key = data[i];
        int j = i - 1;
        while (j >= 0 && data[j] > key) {
            data[j + 1] = data[j];
            swap_cnt++;
            j--;
        }
        data[j + 1] = key;
        swap_cnt++;
    }
    print("直接插入排序", start);
}
// 堆排序
void sort_comp::heap_sort() {
    reset();
    clock_t start = clock();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(n, i);
    for (int i = n - 1; i > 0; i--) {
        _swap(data[0], data[i]);
        heapify(i, 0);
    }
    print("堆排序", start);
}
// 基数排序
void sort_comp::radix_sort() {
    reset();
    clock_t start = clock();
    int m = getMax();
    int* output = new int[n];
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(exp, output);
    delete[] output;
    print("基数排序", start);
}
// 打印结果
void sort_comp::print(const char* name, clock_t start) {
    clock_t end = clock();
    double duration = double(end - start) / CLOCKS_PER_SEC * 1000;
    cout << name << endl;
    cout << "用时:" << duration << " ms" << endl;
    cout << "交换/移动:" << swap_cnt << endl;
    cout << endl;
}
// 显示菜单
void sort_comp::menu() {
    cout << "**    排序算法比较             **" << endl;
    cout << "========================================" << endl;
    cout << "** 1 --- 冒泡排序              **" << endl;
    cout << "** 2 --- 选择排序              **" << endl;
    cout << "** 3 --- 直接插入排序          **" << endl;
    cout << "** 4 --- 希尔排序              **" << endl;
    cout << "** 5 --- 快速排序              **" << endl;
    cout << "** 6 --- 堆排序                **" << endl;
    cout << "** 7 --- 归并排序              **" << endl;
    cout << "** 8 --- 基数排序              **" << endl;
    cout << "** 9 --- 退出程序              **" << endl;
    cout << "========================================" << endl;
}
// 生成随机数，输入验证
void sort_comp::check_n() {
    while (true) {
        cout << "请输入要产生的随机数的个数(1000到100000): ";
        cin >> n;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "输入错误,请输入有效的数字" << endl;
            continue;
        }
        if (n < 100 || n > 100000) {
            cout << "输入超出范围，请输入100到100000之间的数字" << endl;
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
        break;
    }
    if (ori_data) delete[] ori_data;
    if (data) delete[] data;
    ori_data = new int[n];
    data = new int[n];
    for (int i = 0; i < n; i++) {
        ori_data[i] = rand();
    }
    cout << "已生成 " << n << " 个随机数" << endl;
}
int main() {
    sort_comp sc;
    sc.check_n();
    int choice;
    sc.menu();
    while (true) {
        cout << "请选择排序算法: ";
        cin >> choice;
        // 输入验证
        if (cin.fail()) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "无效输入，请输入1-9之间的数字" << endl;
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
        switch (choice) {
        case 1:
            sc.bubble_sort();
            break;
        case 2:
            sc.selection_sort();
            break;
        case 3:
            sc.insertion_sort();
            break;
        case 4:
            sc.shell_sort();
            break;
        case 5:
            sc.quick_sort();
            break;
        case 6:
            sc.heap_sort();
            break;
        case 7:
            sc.merge_sort();
            break;
        case 8:
            sc.radix_sort();
            break;
        case 9:
            return 0;
        default: cout << "无效输入,请输入1-9之间的数字" << endl;
        }
    }
    cout << "\n程序运行结束，按回车键退出" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return 0;
}