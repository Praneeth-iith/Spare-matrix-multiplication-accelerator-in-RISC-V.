#include <bits/stdc++.h>
using namespace std;

static const int MAX_DIM = 200; 
static const string TEST_S_PATH = "test.s";
static const string VM_CMD = "./vm --run test.s > vm_out.txt";
static const string VM_OUT_FILE = "tester_out.txt";

uint64_t packDims(uint32_t rows, uint32_t cols) {
    return (uint64_t(cols) << 32) | uint64_t(rows);
}

int randInt(int lo, int hi) { 
    return lo + rand() % (hi - lo + 1);
}

int sparsityA() {
    double r = double(rand()) / RAND_MAX;
    if (r < 0.7) return randInt(0, 30);
    return randInt(31, 100); 
}

int sparsityB() {
    double r = double(rand()) / RAND_MAX;
    if (r < 0.7) return randInt(70, 100);
    return randInt(0, 69);
}

vector<vector<long long>> genMatrix(int r, int c, int s) {
    vector<vector<long long>> M(r, vector<long long>(c, 0));
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            if (rand() % 100 < s)
                M[i][j] = randInt(1, 9);
    return M;
}

vector<vector<long long>> matmul(const vector<vector<long long>>& A,
                                 const vector<vector<long long>>& B,
                                 int a, int b, int c) {
    vector<vector<long long>> C(a, vector<long long>(c, 0));
    for (int i = 0; i < a; i++)
        for (int k = 0; k < b; k++)
            if (A[i][k] != 0)
                for (int j = 0; j < c; j++)
                    C[i][j] += A[i][k] * B[k][j];
    return C;
}

vector<long long> flatten(const vector<vector<long long>>& M, int r, int c) {
    vector<long long> flat;
    flat.reserve(r * c);
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            flat.push_back(M[i][j]);
    return flat;
}

string buildLine(const string& label, int r, int c, const vector<long long>& flat) {
    uint64_t header = packDims(r, c);
    stringstream ss;
    ss << label << ": .dword 0x"
       << hex << setw(16) << setfill('0') << header << dec;
    for (auto x : flat) ss << "," << x;
    return ss.str();
}

bool patch(const string &l1, const string &l2) {
    ifstream fin(TEST_S_PATH);
    if (!fin.is_open()) return false;
    vector<string> lines; string s;
    while (getline(fin, s)) lines.push_back(s);
    fin.close();

    bool f1 = false, f2 = false;
    for (auto &L : lines) {
        string t = L; 
        t.erase(0, t.find_first_not_of(" \t"));
        if (!f1 && t.rfind("array1:", 0) == 0) { L = l1; f1 = true; }
        else if (!f2 && t.rfind("array2:", 0) == 0) { L = l2; f2 = true; }
    }

    ofstream fout(TEST_S_PATH);
    for (auto &L : lines) fout << L << "\n";

    return f1 && f2;
}

bool runVM() {
    return system(VM_CMD.c_str()) == 0;
}

bool readHW(vector<long long>& out) {
    ifstream fin("tester_out.txt"); 
    if (!fin.is_open()) return false;

    long long x;
    while (fin >> x)
        out.push_back(x);

    return true;
}



vector<tuple<int,int,int>> generateDims(int t, long long MAX_OPS) {
    vector<tuple<int,int,int>> dims;
    dims.reserve(t);

    if (MAX_OPS < t) {
        cerr << "ERROR: MAX_OPS < t, impossible to have t tests with a*b*c >= 1 each\n";
        return dims;
    }

    long long remaining = MAX_OPS;

    for (int i = 0; i < t; ++i) {
        int a = 1, b = 1, c = 1;

        long long tests_left = t - i;
        long long min_ops_for_rest = tests_left - 1;
        long long max_for_this = remaining - min_ops_for_rest;
        if (max_for_this < 1) max_for_this = 1;

        bool found = false;
        for (int tries = 0; tries < 10000; ++tries) {
            a = randInt(1, min(MAX_DIM, (int)max_for_this));
            b = randInt(1, min(MAX_DIM, (int)max_for_this));
            long long ab = 1LL * a * b;
            if (ab > max_for_this) continue;

            long long max_c = max_for_this / ab;
            if (max_c < 1) continue;

            int c_lim = (int)min<long long>(MAX_DIM, max_c);
            c = randInt(1, c_lim);

            long long ops = 1LL * a * b * c;
            if (ops >= 1 && ops <= max_for_this) {
                remaining -= ops;
                found = true;
                break;
            }
        }

        if (!found) {
            a = b = c = 1;
            remaining -= 1;
        }

        dims.emplace_back(a,b,c);
    }

    return dims;
}

int main() {
    srand(time(NULL));
    int t;
    long long MAX_OPS;
    cout << "Enter number of testcases: ";
    cin >> t;
    cout << "Enter max sum(a*b*c): ";
    cin >> MAX_OPS;

    auto dims = generateDims(t, MAX_OPS);
    if ((int)dims.size() != t) {
        return 1;
    }

    long long total_ops = 0;
    for (auto &tp : dims) {
        int a,b,c;
        tie(a,b,c) = tp;
        total_ops += 1LL * a * b * c;
    }
    cout << "Total a*b*c over all tests = " << total_ops
         << " (<= " << MAX_OPS << ")\n";

    for (int tc = 1; tc <= t; tc++) {
        ofstream clear("tester_out.txt", ios::trunc);
        clear.close();
        int a,b,c;
        tie(a,b,c) = dims[tc-1];

        int sA = sparsityA(), sB = sparsityB();
        auto A = genMatrix(a, b, sA);
        auto B = genMatrix(b, c, sB);
        auto C = matmul(A, B, a, b, c);

        auto fA = flatten(A, a, b);
        auto fB = flatten(B, b, c);
        auto fC = flatten(C, a, c);

        string line1 = buildLine("array1", a, b, fA);
        string line2 = buildLine("array2", b, c, fB);

        patch(line1, line2);
        runVM();

        vector<long long> hw;
        readHW(hw);

        if (hw == fC) {
            cout << "Test " << tc << " PASSED\n";
        } else {
            cout << "Test " << tc << " FAILED\n";
            cout << "Dims: A=" << a << "x" << b << ", B=" << b
                 << "x" << c << ", expected outputs = " << (a*c)
                 << ", received = " << hw.size() << "\n";
            cout << "Expected: ";
            for (auto v : fC) cout << v << " ";
            cout << "\nReceived: ";
            for (auto v : hw) cout << v << " ";
            cout << "\n";
            break;
        }
    }

    return 0;
}
