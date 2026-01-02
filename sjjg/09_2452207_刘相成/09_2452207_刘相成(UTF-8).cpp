#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <cstdio>

using namespace std;

// 简化的字符串类
class Str {
private:
    char* s;
    int n;

public:
    Str() {
        n = 0; 
        s = new char[1]; 
        s[0] = '\0';
    }
    Str(const char* in) {
        if (!in) { 
            n = 0; 
            s = new char[1]; 
            s[0] = '\0'; 
        }
        else { 
            n = strlen(in); 
            s = new char[n + 1]; 
            strcpy(s, in); 
        }
    }
    Str(const Str& o) {
        n = o.n; 
        s = new char[n + 1]; 
        strcpy(s, o.s);
    }
    Str& operator=(const Str& o) {
        if (this == &o) 
            return *this;
        delete[] s; 
        n = o.n; 
        s = new char[n + 1]; 
        strcpy(s, o.s);
        return *this;
    }
    ~Str() { delete[] s; }
    bool operator==(const Str& o) const { return strcmp(s, o.s) == 0; }
    const char* c() const { return s; }
};

ostream& operator<<(ostream& os, const Str& st) { 
    os << st.c(); 
    return os; 
}
istream& operator>>(istream& is, Str& st) {
    char buf[1024]; 
    is >> buf; 
    st = Str(buf); 
    return is;
}

// 简化的向量类
template <typename T>
class Vec {
private:
    T* arr;
    int cap;
    int cnt;
    void grow(int nc) {
        T* na = new T[nc];
        for (int i = 0; i < cnt; i++) 
            na[i] = arr[i];
        delete[] arr; 
        arr = na; 
        cap = nc;
    }

public:
    Vec() { cap = 10; cnt = 0; arr = new T[cap]; }
    Vec(int sz, T v) {
        cap = sz > 0 ? sz : 10; 
        cnt = sz; 
        arr = new T[cap];
        for (int i = 0; i < sz; i++) 
            arr[i] = v;
    }
    Vec(const Vec& o) {
        cap = o.cap; 
        cnt = o.cnt; 
        arr = new T[cap];
        for (int i = 0; i < cnt; i++) 
            arr[i] = o.arr[i];
    }
    Vec& operator=(const Vec& o) {
        if (this == &o) 
            return *this;
        delete[] arr; 
        cap = o.cap; 
        cnt = o.cnt; 
        arr = new T[cap];
        for (int i = 0; i < cnt; i++) 
            arr[i] = o.arr[i];
        return *this;
    }
    ~Vec() { delete[] arr; }
    void add(const T& e) { 
        if (cnt == cap) 
            grow(cap * 2); 
        arr[cnt++] = e; 
    }
    T& operator[](int i) { 
        return arr[i]; 
    }
    const T& operator[](int i) const { 
        return arr[i]; 
    }
    int len() const { 
        return cnt; 
    }
    void clr() { 
        cnt = 0; 
    }
};

// 课程结构体
struct Crs {
    Str id, nm;
    int hr, fix; // 学时, 指定学期
    Vec<Str> pre; // 先修课
    int sem, idx, lvl; // 分配学期, 索引, 拓扑层级
    Crs() : hr(0), fix(0), sem(0), idx(-1), lvl(1) {}
};

// 排课系统
class Sys {
private:
    Vec<Crs> db; // 课程库
    Vec<int> lim; // 学期名额
    int mat[8][5][10]; // 课表矩阵
    int find(const Str& id); // 根据课程查找索引
    void line(ostream& os, int w1, int w2); // 打印表格分割线

public:
    Sys(); // 构造函数
    bool load(const char* fn); // 从文件读取课程数据和学期限制
    void plan(); // 根据先修关系和学期名额分配课程
    bool fill(); // 分配到学期的课程填入课表矩阵
    bool put(int s, int d, int p, int len, int id); // 课程放入课表
    void write(const char* fn); // 将课表结果导出到文件
    void show(); // 显示课表
    void prt(ostream& os); // 打印课表
};

// 构造函数
Sys::Sys() {
    for (int s = 0; s < 8; s++)
        for (int d = 0; d < 5; d++)
            for (int p = 0; p < 10; p++) mat[s][d][p] = -1;
}

// 根据课程查找索引
int Sys::find(const Str& id) {
    for (int i = 0; i < db.len(); i++) if (db[i].id == id) return i;
    return -1;
}

// 打印表格分割线
void Sys::line(ostream& os, int w1, int w2) {
    os << "+" << setfill('-') << setw(w1) << "-" << "+";
    for (int i = 0; i < 5; i++) os << setfill('-') << setw(w2) << "-" << "+";
    os << setfill(' ') << endl;
}

// 从文件读取课程数据和学期限制
bool Sys::load(const char* fn) {
    ifstream f(fn);
    if (!f.is_open()) {
        cout << "无法打开，请改名为input.txt或者将文件移到改程序目录下 " << fn << endl;
        return false;
    }
    char buf[1024];
    if (f.getline(buf, 1024)) {
        char* t = strtok(buf, " \t\r\n");
        if (t) {
            t = strtok(nullptr, " \t\r\n");
            while (t) { 
                lim.add(atoi(t)); 
                t = strtok(nullptr, " \t\r\n"); 
            }
        }
    }
    while (f.getline(buf, 1024)) {
        if (strlen(buf) < 2) 
            continue;
        Vec<Str> tk;
        char* t = strtok(buf, " \t\r\n");
        while (t) { 
            tk.add(Str(t)); 
            t = strtok(nullptr, " \t\r\n"); 
        }
        if (tk.len() < 4) 
            continue;
        Crs c;
        c.id = tk[0]; 
        c.nm = tk[1];
        c.hr = atoi(tk[2].c()); 
        c.fix = atoi(tk[3].c());
        for (int i = 4; i < tk.len(); i++) 
            c.pre.add(tk[i]);
        c.idx = db.len();
        db.add(c);
    }
    f.close();
    int tot = 0;
    for (int i = 0; i < lim.len(); i++) tot += lim[i];
    if (tot != db.len()) {
        cout << "数量不匹配: 计划 " << tot << " vs 实际 " << db.len() << endl;
        return false;
    }
    cout << "\n读取成功: " << db.len() << " 门课\n" << endl;
    return true;
}

// 根据先修关系和学期名额分配课程
void Sys::plan() {
    bool chg = true;
    while (chg) {
        chg = false;
        for (int i = 0; i < db.len(); i++) {
            int mp = 0;
            for (int j = 0; j < db[i].pre.len(); j++) {
                int k = find(db[i].pre[j]);
                if (k != -1 && db[k].lvl > mp) 
                    mp = db[k].lvl;
            }
            int nl = mp + 1;
            if (db[i].fix != 0) 
                nl = db[i].fix;
            if (nl > db[i].lvl) { 
                db[i].lvl = nl; 
                chg = true; 
            }
        }
    }
    // 确定学期课程
    Vec<int> cur(lim.len(), 0);
    Vec<bool> done(db.len(), false);
    for (int i = 0; i < db.len(); i++) {
        if (db[i].fix != 0) {
            int si = db[i].fix - 1;
            if (si >= 0 && si < lim.len()) {
                db[i].sem = db[i].fix; 
                cur[si]++; 
                done[i] = true;
            }
        }
    }
    // 拓扑层级分配剩余课程
    for (int s = 0; s < lim.len(); s++) {
        while (cur[s] < lim[s]) {
            int cand = -1;
            for (int i = 0; i < db.len(); i++) {
                if (!done[i] && db[i].lvl <= (s + 1)) { 
                    cand = i; 
                    break; 
                }
            }
            if (cand != -1) {
                db[cand].sem = s + 1; 
                done[cand] = true; 
                cur[s]++;
            }
            else break;
        }
    }
}

// 分配到学期的课程填入课表
bool Sys::fill() {
    bool isok = true;
    for (int s = 0; s < lim.len(); s++) {
        Vec<int> lst;
        for (int i = 0; i < db.len(); i++) 
            if (db[i].sem == (s + 1)) lst.add(i);
        for (int i = 0; i < lst.len(); i++)
            for (int j = i + 1; j < lst.len(); j++)
                if (db[lst[i]].hr < db[lst[j]].hr) {
                    int t = lst[i]; 
                    lst[i] = lst[j]; 
                    lst[j] = t;
                }
        int ld[5] = { 0 }, am[5] = { 0 }, pm[5] = { 0 };
        for (int k = 0; k < lst.len(); k++) {
            int idx = lst[k];
            int h = db[idx].hr;
            Vec<int> blks;
            // 拆分课时块
            if (h == 6) { 
                blks.add(3); 
                blks.add(3); 
            }
            else if (h == 5) { 
                blks.add(3); 
                blks.add(2); 
            }
            else if (h == 4) { 
                blks.add(2); 
                blks.add(2); 
            }
            else if (h == 3) 
                blks.add(3);
            else if (h == 2) 
                blks.add(2);
            else for (int x = 0; x < h; x++) 
                blks.add(1);
            // 课时最小的日期作为起始日
            int sd = 0, mn = 999;
            for (int d = 0; d < 5; d++) if (ld[d] < mn) {
                mn = ld[d]; 
                sd = d; 
            }
            int cd = sd; 
            for (int c = 0; c < blks.len(); c++) {
                int dur = blks[c];
                bool ok = false;
                int td = cd;

                for (int t = 0; t < 5; t++) { 
                    int d = td % 5;
                    bool isAm = (am[d] <= pm[d]);
                    Vec<int> slots; 
                    // 根据时长和上下午确定起始
                    if (dur == 3) {
                        if (isAm) { 
                            slots.add(2); 
                            slots.add(7); 
                        }
                        else { 
                            slots.add(7); 
                            slots.add(2); 
                        }
                    }
                    else if (dur == 2) {
                        if (isAm) { 
                            slots.add(0); 
                            slots.add(2); 
                            slots.add(5); 
                            slots.add(7); 
                        }
                        else { 
                            slots.add(5); 
                            slots.add(7); 
                            slots.add(0); 
                            slots.add(2); 
                        }
                    }
                    else { 
                        for (int p = 0; p < 10; p++) 
                            slots.add(isAm ? p : 9 - p);
                    }
                    for (int z = 0; z < slots.len(); z++) {
                        int p = slots[z];
                        if (put(s, d, p, dur, idx)) {
                            ok = true; ld[d] += dur;
                            for (int x = 0; x < dur; x++) { 
                                if (p + x < 5) am[d]++; 
                                else pm[d]++; 
                            }
                            break;
                        }
                    }
                    if (ok) break;
                    td++;
                }
                if (!ok) {
                    cout << db[idx].nm << " 安排失败" << endl;
                    isok = false;
                }
                cd = (cd + 2) % 5; 
            }
        }
    }
    return isok;
}

// 课程放入课表
bool Sys::put(int s, int d, int p, int len, int id) {
    if (p + len > 10) return false;
    for (int k = 0; k < len; k++) if (mat[s][d][p + k] != -1) return false;
    for (int k = 0; k < len; k++) mat[s][d][p + k] = id;
    return true;
}

// 导出文件
void Sys::write(const char* fn) {
    ofstream f(fn);
    if (!f.is_open()) return;
    prt(f);
    f.close();
    cout << "已导出至 " << fn << endl;
}

// 显示课表
void Sys::show() { prt(cout); }

// 打印课表
void Sys::prt(ostream& os) {
    const int w1 = 10, w2 = 22;
    const char* wd[] = { "周一", "周二", "周三", "周四", "周五" };

    for (int s = 0; s < lim.len(); s++) {
        os << "=================================================================================\n";
        os << "                          第 " << (s + 1) << " 学期课表\n";
        os << "=================================================================================\n";
        line(os, w1, w2);
        os << "|" << left << "  " << setw(w1 - 2) << "节次" << "|";
        for (int d = 0; d < 5; d++) os << "  " << setw(w2 - 2) << wd[d] << "|";
        os << endl;
        line(os, w1, w2);

        for (int p = 0; p < 10; p++) {
            char sn[20]; sprintf(sn, "第%d节", p + 1);
            os << "|" << left << "  " << setw(w1 - 2) << sn << "|";
            for (int d = 0; d < 5; d++) {
                int id = mat[s][d][p];
                if (id != -1) os << "  " << setw(w2 - 2) << db[id].nm << "|";
                else os << setw(w2) << " " << "|";
            }
            os << endl;
            if (p == 1 || p == 4 || p == 6 || p == 9) line(os, w1, w2);
        }
        os << endl << endl;
    }
}

int main() {
    Sys sys;
    if (!sys.load("input.txt")) return 0;
    sys.plan();
    if (!sys.fill())
        return 0;
	sys.show();//用于直接在控制台显示课表
    sys.write("timetable.txt");
    cout << "\n程序运行结束，按回车键退出" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return 0;
}
