#include <bits/stdc++.h>
using namespace std;

template<typename T>
struct PersistentSegmentTree {
    struct Node {
        T val;
        int l, r;
    };
    int n;
    vector<Node> tree;
    vector<int> version_roots;

    PersistentSegmentTree(vector<T>& a) {
        n = (int)a.size();
        version_roots.push_back(build(0, n - 1, a));
    }

    int build(int lo, int hi, vector<T>& a) {
        int node = (int)tree.size();
        tree.push_back({});
        if (lo == hi) {
            tree[node].val = a[lo];
            return node;
        }
        int mid = (lo + hi) / 2;
        int l = build(lo, mid, a);
        int r = build(mid + 1, hi, a);
        tree[node].l = l;
        tree[node].r = r;
        tree[node].val = tree[l].val + tree[r].val;
        return node;
    }

    int update(int version, int pos, T value) {
        int newRoot = update(version_roots[version], 0, n - 1, pos, value);
        version_roots.push_back(newRoot);
        return (int)version_roots.size() - 1;
    }

    int update(int prev, int lo, int hi, int pos, T value) {
        int node = (int)tree.size();
        tree.push_back(tree[prev]);
        if (lo == hi) {
            tree[node].val = value;
            return node;
        }
        int mid = (lo + hi) / 2;
        if (pos <= mid) tree[node].l = update(tree[prev].l, lo, mid, pos, value);
        else tree[node].r = update(tree[prev].r, mid + 1, hi, pos, value);
        tree[node].val = tree[tree[node].l].val + tree[tree[node].r].val;
        return node;
    }

    T query(int version, int l, int r) {
        return query(version_roots[version], 0, n - 1, l, r);
    }

    T query(int node, int lo, int hi, int l, int r) {
        if (r < lo || hi < l) return T(0);
        if (l <= lo && hi <= r) return tree[node].val;
        int mid = (lo + hi) / 2;
        return query(tree[node].l, lo, mid, l, r) + query(tree[node].r, mid + 1, hi, l, r);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> a(n);
    for (auto& x : a) cin >> x;

    PersistentSegmentTree<long long> S(a);

    int q;
    cin >> q;
    while (q--) {
        char op;
        cin >> op;
        if (op == 'c') {
            int version, pos;
            long long x;
            cin >> version >> pos >> x;
            S.update(version, pos - 1, x);
        } else {
            int version, l, r;
            cin >> version >> l >> r;
            cout << S.query(version, l - 1, r - 1) << '\n';
        }
    }
    return 0;
}
