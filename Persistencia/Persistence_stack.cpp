#include <bits/stdc++.h>
using namespace std;

template<typename T>
struct PersistentStack {
    struct Node {
        T data;
        int next;
    };
    vector<Node> nodes;
    vector<int> version_roots;

    PersistentStack() {
        version_roots.push_back(-1);
    }

    int push(int version, T data) {
        nodes.push_back({data, version_roots[version]});
        version_roots.push_back((int)nodes.size() - 1);
        return (int)version_roots.size() - 1;
    }

    int pop(int version) {
        int root = version_roots[version];
        version_roots.push_back(root == -1 ? -1 : nodes[root].next);
        return (int)version_roots.size() - 1;
    }

    bool empty(int version) {
        return version_roots[version] == -1;
    }

    T top(int version) {
        int root = version_roots[version];
        return root == -1 ? T(0) : nodes[root].data;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    PersistentStack<long long> S;
    int q;
    cin >> q;
    for (int i = 1; i <= q; ++i) {
        int t, m;
        cin >> t >> m;
        if (m == 0) S.pop(t);
        else S.push(t, S.top(t) + m);
    }

    long long res = 0;
    for (int i = 1; i <= q; ++i) res += S.top(i);
    cout << res << '\n';
    return 0;
}
