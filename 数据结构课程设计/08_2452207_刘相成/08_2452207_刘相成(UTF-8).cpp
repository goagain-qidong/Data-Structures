#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <limits>
#include <ctime> 
#include <fstream>

using namespace std;

const int max_v = 100; // 最大顶点数
const int max_e = max_v * (max_v - 1) / 2; // 最大边数
const int inf = INT_MAX;

// 边结构体
struct edge {
    char start[50];
    char end[50];
    int weight;
};

// 并查集类Kruskal
class find_set {
private:
    int parent[max_v]; //存储父节点的数组
    int count; //元素数量
public:
    find_set(int n);// 构造函数
    int find(int i);// 查找元素的根节点
    void unite(int i, int j);// 合并两个集合
};

// grid_sys 类
class grid_sys {
private:
    char v_name[max_v][50]; // 存储顶点名称
    int matrix[max_v][max_v]; // 邻接矩阵
    int num_v; // 当前顶点数
    edge store_e[max_e]; // 存储所有边的数组
    int num_e; // 当前边数
    edge mst_final[max_v]; // 存储MST结果的数组
    int mst_e_cnt; // MST边的数量
    bool is_mst; // 标记MST是否生成
    int find_index(const char* name);  // 查找顶点下标
    void sort_e();// 边排序，冒泡排序
    void reset_graph_data(); // 重置图数据（内部辅助函数）

public:
    grid_sys();// 构造函数
    void create_v(); // 创建顶点
    void add_e();// 添加边
    void mst_prim(); // 构造最小生成树Prim
    void mst_kruskal(); // 构造最小生成树Kruskal
    void show_mst(); // 显示最小生成树
    void read_from_file(); // 从文件读取数据
    void menu();// 显示菜单
};
// 构造函数
find_set::find_set(int n) {
    count = n;
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }
}
// 查找元素的根节点
int find_set::find(int i) {
    if (parent[i] == i) {
        return i;
    }
    return parent[i] = find(parent[i]);
}
// 合并两个集合
void find_set::unite(int i, int j) {
    int rootI = find(i);
    int rootJ = find(j);
    if (rootI != rootJ) {
        parent[rootI] = rootJ;
        count--;
    }
}
// 构造函数
grid_sys::grid_sys() {
    num_v = 0;
    num_e = 0;
    mst_e_cnt = 0;
    is_mst = false;
    for (int i = 0; i < max_v; i++) {
        for (int j = 0; j < max_v; j++) {
            matrix[i][j] = inf;
        }
    }
}
// 查找顶点下标
int grid_sys::find_index(const char* name) {
    for (int i = 0; i < num_v; i++) {
        if (strcmp(v_name[i], name) == 0) {
            return i;
        }
    }
    return -1;
}
// 边排序，冒泡排序
void grid_sys::sort_e() {
    for (int i = 0; i < num_e - 1; i++) {
        for (int j = 0; j < num_e - 1 - i; j++) {
            if (store_e[j].weight > store_e[j + 1].weight) {
                edge temp = store_e[j];
                store_e[j] = store_e[j + 1];
                store_e[j + 1] = temp;
            }
        }
    }
}
// 重置图数据
void grid_sys::reset_graph_data() {
    num_v = 0;
    num_e = 0;
    mst_e_cnt = 0;
    is_mst = false;
    for (int i = 0; i < max_v; i++) {
        for (int j = 0; j < max_v; j++) {
            matrix[i][j] = inf;
        }
    }
}
//创建顶点
void grid_sys::create_v() {
    cout << "请输入顶点的个数: ";
    int n;
    while (1) {
        if (!(cin >> n)) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "输入无效，请重新输入" << endl;
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
        if (n < 2) {
            cout << "顶点数量过少，至少为2" << endl;
            continue;
        }
        if (n > max_v) {
            cout << "顶点数量过多，最大支持 " << max_v << " 个" << endl;
            continue;
        }
        break;
    }
    reset_graph_data();
    num_v = n;
    cout << "请依次输入各顶点的名称：" << endl;
    int inputCount = 0;
    while (inputCount < n) {
        cout << "顶点 " << (inputCount + 1) << ": ";
        if (!(cin >> v_name[inputCount])) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "输入无效，请重新输入" << endl;
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
        bool duplicate = false;
        for (int i = 0; i < inputCount; i++) {
            if (strcmp(v_name[i], v_name[inputCount]) == 0) {
                cout << "顶点名称重复，请重新输入" << endl;
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            inputCount++;
        }
    }
    cout << "顶点创建完成" << endl;
}
// 添加边
void grid_sys::add_e() {
    if (num_v == 0) {
        cout << "请先创建顶点" << endl;
        return;
    }
    cout << "请输入两个顶点及边(权重)，输入 ? ? 0 结束：" << endl;
    char v1[50], v2[50];
    int weight;
    num_e = 0;
    is_mst = false;
    for (int i = 0; i < num_v; i++) {
        for (int j = 0; j < num_v; j++) {
            matrix[i][j] = inf;
        }
    }

    while (true) {
        cout << "输入格式: 顶点1 顶点2 权重: ";
        if (!(cin >> v1 >> v2 >> weight)) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "输入无效，请重新输入" << endl;
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
        if (strcmp(v1, "?") == 0 && strcmp(v2, "?") == 0 && weight == 0) {
            break;
        }
        int idx1 = find_index(v1);
        int idx2 = find_index(v2);
        if (idx1 == -1 || idx2 == -1) {
            cout << "顶点不存在或结束语错误，请重新输入" << endl;
            continue;
        }
        if (idx1 == idx2) {
            cout << "边的起点和终点不能相同，请重新输入" << endl;
            continue;
        }
        if (weight <= 0) {
            cout << "权重必须是正数，请重新输入" << endl;
            continue;
        }
        bool newedge = true;
        for (int i = 0; i < num_e; i++) {
            if ((strcmp(store_e[i].start, v1) == 0 && strcmp(store_e[i].end, v2) == 0) ||
                (strcmp(store_e[i].start, v2) == 0 && strcmp(store_e[i].end, v1) == 0)) {
                if (matrix[idx1][idx2] != inf) {
                    cout << "该边已存在，将更新权重" << endl;
                }
                store_e[i].weight = weight;
                newedge = false;
                break;
            }
        }
        if (newedge) {
            if (num_e >= max_e) {
                cout << "错误: 边数达到上限" << endl;
                break;
            }
            strcpy(store_e[num_e].start, v1);
            strcpy(store_e[num_e].end, v2);
            store_e[num_e].weight = weight;
            num_e++;
        }
        matrix[idx1][idx2] = weight;
        matrix[idx2][idx1] = weight;
    }
    cout << "边添加完成" << endl;
}
//构造最小生成树Prim算法
void grid_sys::mst_prim() {
    if (num_v == 0) {
        cout << "请先创建系统并添加边" << endl;
        return;
    }
    if (num_e < num_v - 1) {
        cout << "错误：边数量不足,顶点数(" << num_v << ")，至少需要 " << (num_v - 1) << " 条边。" << endl;
        return;
    }
    cout << "请输入起始顶点: ";
    char startName[50];
    if (!(cin >> startName)) {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cout << "输入无效" << endl;
        return;
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
        return;
    }
    int startIdx = find_index(startName);
    if (startIdx == -1) {
        cout << "顶点不存在" << endl;
        return;
    }
    clock_t start_time = clock();
    int lowcost[max_v];
    int closest[max_v];
    bool in_mst[max_v];
    for (int i = 0; i < num_v; i++) {
        lowcost[i] = matrix[startIdx][i];
        closest[i] = startIdx;
        in_mst[i] = false;
    }
    lowcost[startIdx] = 0;
    in_mst[startIdx] = true;
    mst_e_cnt = 0;
    cout << "正在生成Prim最小生成树" << endl;
    for (int i = 1; i < num_v; i++) {
        int minVal = inf;
        int k = -1;
        for (int j = 0; j < num_v; j++) {
            if (!in_mst[j] && lowcost[j] < minVal) {
                minVal = lowcost[j];
                k = j;
            }
        }
        if (k == -1) {
            cout << "生成失败：图不连通" << endl;
            is_mst = false;
            return;
        }
        strcpy(mst_final[mst_e_cnt].start, v_name[closest[k]]);
        strcpy(mst_final[mst_e_cnt].end, v_name[k]);
        mst_final[mst_e_cnt].weight = minVal;
        mst_e_cnt++;
        in_mst[k] = true;
        for (int j = 0; j < num_v; j++) {
            if (!in_mst[j] && matrix[k][j] < lowcost[j]) {
                lowcost[j] = matrix[k][j];
                closest[j] = k;
            }
        }
    }
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    is_mst = true;
    //show_mst();
    cout << "生成完毕" << endl;
    cout << "Prim 算法耗费时间: " << fixed << setprecision(6) << elapsed_time << " 秒" << endl;
}
// 构造最小生成树Kruskal算法
void grid_sys::mst_kruskal() {
    if (num_v == 0) {
        cout << "请先创建系统并添加边" << endl;
        return;
    }
    if (num_e < num_v - 1) {
        cout << "错误：边数量不足。顶点数(" << num_v << ")，至少需要 " << (num_v - 1) << " 条边。" << endl;
        return;
    }
    clock_t start_time = clock();
    sort_e();
    find_set uf(num_v);
    mst_e_cnt = 0;
    cout << "正在生成Kruskal最小生成树" << endl;
    for (int i = 0; i < num_e && mst_e_cnt < num_v - 1; i++) {
        int idx1 = find_index(store_e[i].start);
        int idx2 = find_index(store_e[i].end);
        if (uf.find(idx1) != uf.find(idx2)) {
            uf.unite(idx1, idx2);
            mst_final[mst_e_cnt] = store_e[i];
            mst_e_cnt++;
        }
    }
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    if (mst_e_cnt < num_v - 1) {
        cout << "生成失败：图不连通" << endl;
        is_mst = false;
        return;
    }
    is_mst = true;
    //show_mst();
    cout << "生成完毕" << endl;
    cout << "Kruskal 算法耗费时间: " << fixed << setprecision(6) << elapsed_time << " 秒" << endl;
}

// 显示最小生成树
void grid_sys::show_mst() {
    if (!is_mst) {
        cout << "请先执行操作 C 或 D 构造最小生成树" << endl;
        return;
    }
    cout << "最小生成树的顶点及边为：" << endl;
    int totalCost = 0;
    int count = 0;
    for (int i = 0; i < mst_e_cnt; i++) {
        cout << mst_final[i].start << "<-(" << mst_final[i].weight << ")->" << mst_final[i].end << "\t";
        totalCost += mst_final[i].weight;
        count++;
        if (count % 3 == 0) cout << endl;
    }
    cout << "\n最小总造价为: " << totalCost << endl;
}
//从文件读取数据
void grid_sys::read_from_file() {
    ifstream fin("input.txt");
    if (!fin.is_open()) {
        cout << "错误: 无法打开 input.txt 文件！" << endl;
        return;
    }
    int n;
    fin >> n;
    reset_graph_data();
    num_v = n;
    for (int i = 0; i < num_v; i++) {
        fin >> v_name[i];
    }
    char v1[50], v2[50];
    int weight;
    num_e = 0;
    while (fin >> v1 >> v2 >> weight) {
        int idx1 = find_index(v1);
        int idx2 = find_index(v2);
        bool newedge = true;
        for (int i = 0; i < num_e; i++) {
            if ((strcmp(store_e[i].start, v1) == 0 && strcmp(store_e[i].end, v2) == 0) ||
                (strcmp(store_e[i].start, v2) == 0 && strcmp(store_e[i].end, v1) == 0)) {
                store_e[i].weight = weight;
                newedge = false;
                break;
            }
        }
        if (newedge) {
            strcpy(store_e[num_e].start, v1);
            strcpy(store_e[num_e].end, v2);
            store_e[num_e].weight = weight;
            num_e++;
        }
        matrix[idx1][idx2] = weight;
        matrix[idx2][idx1] = weight;
    }
    fin.close();
    cout << "文件读取成功,有效边数: " << num_e << " 条" << endl;
    is_mst = false;
}
//显示菜单
void grid_sys::menu() {
    cout << endl;
    cout << "**       电网造价模拟系统          **" << endl;
    cout << "========================================" << endl;
    cout << "** A --- 创建顶点(2-100)           **" << endl;
    cout << "** B --- 重新添加边(记录最新输入)  **" << endl;
    cout << "** C --- Prim构造最小生成树        **" << endl;
    cout << "** D --- Kruskal构造最小生成树     **" << endl;
    cout << "** E --- 显示上次生成的最小生成树  **" << endl;
    cout << "** G --- 从文件读取(仅为加快输入)  **" << endl;
    cout << "** F --- 退出程序                  **" << endl;
    cout << "========================================" << endl;
    cout << endl;
}
int main() {
    grid_sys sys;
    sys.menu();
    while (true) {
        cout << "\n请选择操作: ";
        char choice;
        if (!(cin >> choice)) {
            cout << "输入无效，请重新输入" << endl;
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
        if (choice >= 'a' && choice <= 'z') {
            choice -= 32;
        }
        switch (choice) {
        case 'A':
            sys.create_v();
            break;
        case 'B':
            sys.add_e();
            break;
        case 'C':
            sys.mst_prim();
            break;
        case 'D':
            sys.mst_kruskal();
            break;
        case 'E':
            sys.show_mst();
            break;
        case 'G':
            sys.read_from_file();
            break;
        case 'F':
            cout << "退出程序" << endl;
            return 0;
        default:
            cout << "无效选择，重新输入" << endl;
        }
    }
    cout << "\n程序运行结束，按回车键退出" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return 0;
}