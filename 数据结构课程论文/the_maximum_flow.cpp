//网络最大流算法性能分析
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>

using namespace std;

// 最小值
template <typename T>
T my_min(T a, T b) {
    return (a < b) ? a : b;
}

//动态数组 _vector
template <typename T>
class _vector {
private:
    T* data;// 动态数组的数据指针
    size_t _size;// 当前元素个数
    size_t _capacity; // 当前分配的内存容量
    // 分配内存
    void reallocate(size_t new_cap) {
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < _size; ++i) {
            new_data[i] = data[i];
        }
        if (data) 
            delete[] data;
        data = new_data;
        _capacity = new_cap;
    }

public:
    _vector() : data(nullptr), _size(0), _capacity(0) {}
    _vector(const _vector& other) : data(nullptr), _size(0), _capacity(0) {
        if (other._size > 0) {
            reallocate(other._capacity);
            _size = other._size;
            for (size_t i = 0; i < _size; ++i) {
                data[i] = other.data[i];
            }
        }
    }
    // 赋值运算符
    _vector& operator=(const _vector& other) {
        if (this != &other) {
            if (data) 
                delete[] data;
            data = nullptr;
            _size = 0;
            _capacity = 0;
            if (other._size > 0) {
                reallocate(other._capacity);
                _size = other._size;
                for (size_t i = 0; i < _size; ++i) {
                    data[i] = other.data[i];
                }
            }
        }
        return *this;
    }
    ~_vector() {
        if (data) 
            delete[] data;
    }
    // 添加元素
    void push_back(const T& value) {
        if (_size >= _capacity) {
            size_t new_cap = (_capacity == 0) ? 4 : _capacity * 2;
            reallocate(new_cap);
        }
        data[_size++] = value;
    }
    // 下标访问
    T& operator[](size_t index) {
        return data[index];
    }
    // 下标访问
    const T& operator[](size_t index) const {
        return data[index];
    }
    // 获取大小
    size_t size() const {
        return _size;
    }
    // 清空
    void clear() {
        _size = 0;
    }
    // 调整大小
    void resize(size_t new_size, const T& val = T()) {
        if (new_size > _capacity) {
            reallocate(new_size);
        }
        for (size_t i = _size; i < new_size; ++i) {
            data[i] = val;
        }
        _size = new_size;
    }
    // 判空
    bool empty() const {
        return _size == 0;
    }
    // 迭代器 begin
    T* begin() {
        return data;
    }
    // 迭代器 end
    T* end() {
        return data + _size;
    }
    const T* begin() const {
        return data;
    }
    const T* end() const {
        return data + _size;
    }
};

// 队列 _queue
template <typename T>
class _queue {
private:
    struct Node {
        T val;// 节点存储的值
        Node* next; // 下一个节点指针
        Node(T v) : val(v), next(nullptr) {}
    };
    Node* head;// 队头指针
    Node* tail; // 队尾指针
    size_t _size;// 队列大小

public:
    _queue() : head(nullptr), tail(nullptr), _size(0) {}
    ~_queue() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // 入队
    void push(T val) {
        Node* node = new Node(val);
        if (!tail) {
            head = tail = node;
        }
        else {
            tail->next = node;
            tail = node;
        }
        _size++;
    }

    // 出队
    void pop() {
        if (!head) return;
        Node* temp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete temp;
        _size--;
    }
    // 获取队头元素
    T front() {
        if (!head) {
            cerr << "Queue is empty!" << endl;
            exit(1);
        }
        return head->val;
    }
    // 判空
    bool empty() {
        return _size == 0;
    }
    // 获取大小
    size_t size() {
        return _size;
    }
};


const long long INF = LLONG_MAX;

struct _edge {
    int to; // 目标节点索引
    long long cap; // 当前剩余容量
    long long orig_cap;// 初始容量
    int rev;// 反向边在adj[to]中的下标
    bool is_rev; // 标记是否为反向生成的虚边
    // 构造函数
    _edge() : to(0), cap(0), orig_cap(0), rev(0), is_rev(false) {}
    _edge(int t, long long c, long long oc, int r, bool ir): to(t), cap(c), orig_cap(oc), rev(r), is_rev(ir) {}
};

// 字符串，命名
struct _string {
    const char* str; // 字符串指针
    _string(const char* s = "") : str(s) {}
    // 获取字符串
    const char* c_str() const {
        return str;
    }
};


class max_flow {
protected:
    int n; // 节点总数
    int s, t; // 源点和汇点
    _vector<_vector<_edge>> adj;// 邻接表
    long long max_flow_val;// 最大流结果
    _string algo_name;// 算法名称

public:
    long long augmentation_count;// 增广次数统计
    max_flow(int nodes, const char* name): n(nodes), max_flow_val(0), augmentation_count(0), algo_name(name) {}
    virtual ~max_flow() {}
    // 设置起始终止点
    void setSourceSink(int source, int sink) {
        s = source;
        t = sink;
    }
    // 初始化图结构
    void initGraph(int N, const _vector<long long>& _edge_data) {
        this->n = N;
        adj.resize(n);
        for (int i = 0; i < n; ++i) adj[i].clear();
        max_flow_val = 0;
        augmentation_count = 0;
        for (size_t i = 0; i < _edge_data.size(); i += 3) {
            int u = (int)_edge_data[i];
            int v = (int)_edge_data[i + 1];
            long long cap = _edge_data[i + 2];
            add_edge(u, v, cap);
        }
    }

    // 添加边
    void add_edge(int u, int v, long long cap) {
        _edge a(v, cap, cap, (int)adj[v].size(), false);
        _edge b(u, 0, 0, (int)adj[u].size(), true);
        adj[u].push_back(a);
        adj[v].push_back(b);
    }
    virtual long long solve() = 0;
    // 获取名称
    const char* getName() const {
        return algo_name.c_str();
    }

    // 打印
    void printFlowDetails() {
        cout << "\n[ " << algo_name.c_str() << " ]流量分配(前20条非零边):" << endl;
        int count = 0;
        for (int u = 0; u < n; ++u) {
            for (size_t i = 0; i < adj[u].size(); ++i) {
                const _edge& e = adj[u][i];
                if (!e.is_rev) {
                    long long flow = e.orig_cap - e.cap;
                    if (flow > 0) {
                        if (count < 20) {
                            cout << u << " -> " << e.to << " : ["
                                << flow << " / " << e.orig_cap << "]" << endl;
                        }
                        count++;
                    }
                }
            }
        }
        cout << "总计 " << count << " 条边有流量" << endl;
    }
};

// Ford-Fulkerson (DFS)
class FordFulkerson : public max_flow {
    _vector<bool> visited;

public:
    FordFulkerson(int nodes) : max_flow(nodes, "Ford-Fulkerson(DFS)") {
        visited.resize(nodes, false);
    }
    // DFS寻找增广路
    long long dfs(int u, long long flow) {
        if (u == t) return flow;
        visited[u] = true;
        for (size_t i = 0; i < adj[u].size(); ++i) {
            _edge& e = adj[u][i];
            if (!visited[e.to] && e.cap > 0) {
                long long pushed = dfs(e.to, my_min(flow, e.cap));
                if (pushed > 0) {
                    e.cap -= pushed;
                    adj[e.to][e.rev].cap += pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }
    // 求解最大流
    long long solve() override {
        max_flow_val = 0;
        while (true) {
            for (size_t i = 0; i < visited.size(); ++i) visited[i] = false;
            long long pushed = dfs(s, INF);
            if (pushed == 0) break;
            max_flow_val += pushed;
            augmentation_count++; 
        }
        return max_flow_val;
    }
};

//Edmonds-Karp (BFS)
class EdmondsKarp : public max_flow {
    struct PathInfo {
        int p_node;      // 父节点ID
        int p__edge_idx; // 边在父节点邻接表中的索引
        PathInfo(int pn = -1, int pe = -1) : p_node(pn), p__edge_idx(pe) {}
    };

public:
    EdmondsKarp(int nodes) : max_flow(nodes, "Edmonds-Karp(BFS)") {}
    // 求解最大流
    long long solve() override {
        max_flow_val = 0;
        while (true) {
            _vector<PathInfo> parent;
            parent.resize(n, PathInfo(-1, -1));
            _queue<int> q;
            q.push(s);
            parent[s] = PathInfo(-2, -2);
            bool found = false;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                if (u == t) {
                    found = true;
                    break;
                }
                for (size_t i = 0; i < adj[u].size(); ++i) {
                    _edge& e = adj[u][i];
                    if (parent[e.to].p_node == -1 && e.cap > 0) {
                        parent[e.to] = PathInfo(u, (int)i);
                        q.push(e.to);
                    }
                }
            }
            if (!found)
                break;
            long long path_flow = INF;
            int curr = t;
            while (curr != s) {
                int p = parent[curr].p_node;
                int idx = parent[curr].p__edge_idx;
                path_flow = my_min(path_flow, adj[p][idx].cap);
                curr = p;
            }
            curr = t;
            while (curr != s) {
                int p = parent[curr].p_node;
                int idx = parent[curr].p__edge_idx;
                adj[p][idx].cap -= path_flow;
                int rev_idx = adj[p][idx].rev;
                adj[curr][rev_idx].cap += path_flow;
                curr = p;
            }
            max_flow_val += path_flow;
            augmentation_count++;
        }
        return max_flow_val;
    }
};

//Dinic
class Dinic : public max_flow {
    _vector<int> level; // 分层图等级
    _vector<int> ptr;   // 当前弧指针

public:
    Dinic(int nodes) : max_flow(nodes, "Dinic") {
        level.resize(nodes);
        ptr.resize(nodes);
    }
    // 构建分层图BFS
    bool bfs() {
        for (size_t i = 0; i < level.size(); ++i) level[i] = -1;
        level[s] = 0;
        _queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (size_t i = 0; i < adj[u].size(); ++i) {
                _edge& e = adj[u][i];
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    // 多路增广DFS
    long long dfs(int u, long long pushed) {
        if (pushed == 0 || u == t) return pushed;
        for (int& cid = ptr[u]; cid < (int)adj[u].size(); ++cid) {
            _edge& e = adj[u][cid];
            if (level[u] + 1 != level[e.to] || e.cap == 0) 
                continue;
            long long tr = dfs(e.to, my_min(pushed, e.cap));
            if (tr == 0) 
                continue;
            e.cap -= tr;
            adj[e.to][e.rev].cap += tr;
            return tr;
        }
        return 0;
    }
    // 求解最大流
    long long solve() override {
        max_flow_val = 0;
        while (bfs()) {
            for (size_t i = 0; i < ptr.size(); ++i) ptr[i] = 0; // reset ptr
            while (long long pushed = dfs(s, INF)) {
                max_flow_val += pushed;
                augmentation_count++; 
            }
        }
        return max_flow_val;
    }
};

// 随机数生成
class SimpleRand {
    unsigned long long seed; // 随机数种子

public:
    SimpleRand(unsigned long long s = 12345) : seed(s) {}

    // 生成下一个整数
    int next_int(int min, int max) {
        seed = seed * 6364136223846793005ULL + 1;
        return min + (seed % (max - min + 1));
    }
};

// 生成测试数据文件
void generate_data(const char* filename, int N, int M, int max_cap) {
    cout << "正在生成数据 " << filename << " (N=" << N << ", M=" << M << ")" << endl;
    ofstream out(filename);
    SimpleRand rng;
    int s = 0;
    int t = N - 1;
    out << N << " " << M << " " << s << " " << t << endl;
    for (int i = 0; i < N - 1; ++i) {
        out << i << " " << i + 1 << " " << rng.next_int(1, max_cap) << endl;
    }
    for (int i = 0; i < M - (N - 1); ++i) {
        int u = rng.next_int(0, N - 1);
        int v = rng.next_int(0, N - 1);
        if (u == v) { i--; continue; }
        out << u << " " << v << " " << rng.next_int(1, max_cap) << endl;
    }
    out.close();
}


int main() {
    // 程序入口，执行测试逻辑
    // 生成测试数据
    const char* FILENAME = "test_data.txt";//稀疏，稠密，大规模图
	char choice;
    cout << "选择测试数据（1稀疏图，2稠密图，3大规模图）:";
	bool flag = true;
    while (flag) {
        flag = false;
        cin >> choice;
        char next = cin.peek();
        while (next == ' ') {
            cin.get();
            next = cin.peek();
        }
        if (next != '\n' && next != EOF) {
            cout << "多于输入，重新输入" << endl;
            cin.ignore(INT_MAX, '\n');
            flag = true;
            continue;
        }
        switch (choice) {
        case '1': {
            int N = 100;
            int M = 500;
            generate_data(FILENAME, N, M, 100);//稀疏
            break;
        }
        case '2': {
            int N = 500;
            int M = 5000;
            generate_data(FILENAME, N, M, 1000);//稠密
            break;
        }
        case '3': {
            int N = 2000;
            int M = 20000;
            generate_data(FILENAME, N, M, 5000);//大规模 
            break;
        }
        default: {
            cout << "无效选择，重新输入" << endl;
            flag = true;
            break;
        }
        }
    }
    
    // 读取数据
    ifstream in(FILENAME);
    if (!in) {
        cerr << "无法打开文件" << endl;
        return 1;
    }
    int fN, fM, fS, fT;
    in >> fN >> fM >> fS >> fT;
    // 存储边数据
    _vector<long long> _edge_data;
    int u, v;
    long long c;
    while (in >> u >> v >> c) {
        _edge_data.push_back(u);
        _edge_data.push_back(v);
        _edge_data.push_back(c);
    }
    in.close();
    cout << "\n算法比较" << endl;
    cout << "节点数:" << fN << ",边数:" << fM << endl;
    max_flow* solvers[3];
    solvers[0] = new FordFulkerson(fN);
    solvers[1] = new EdmondsKarp(fN);
    solvers[2] = new Dinic(fN);
    //运行评测
    for (int i = 0; i < 3; ++i) {
        max_flow* solver = solvers[i];
        // 初始化图
        solver->setSourceSink(fS, fT);
        solver->initGraph(fN, _edge_data);
        // 计时
        clock_t start = clock();
        long long flow = solver->solve();
        clock_t end = clock();
        double ms = double(end - start) / CLOCKS_PER_SEC * 1000.0;
        cout << "\n算法:" << solver->getName() << endl;
        cout << "最大流:" << flow << endl;
        cout << "耗时:" << ms << "ms" << endl;
        cout << "增广次数:" << solver->augmentation_count << endl;
        // 打印部分流量
        solver->printFlowDetails();
    }
    for (int i = 0; i < 3; ++i) 
        delete solvers[i];
    cout << "\n程序运行结束，按回车键退出" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return 0;
}