#include <bits/stdc++.h>
using namespace std;

struct PersistentTrie {
    static const int BITS = 30;
    struct Node {
        int cnt;
        int child[2];
    };
    vector<Node> trie;
    vector<int> version_roots;

    PersistentTrie() {
        trie.push_back({0, {0, 0}});
        version_roots.push_back(0);
    }

    int insert(int version, long long value) {
        int newRoot = insert(version_roots[version], BITS - 1, value);
        version_roots.push_back(newRoot);
        return (int)version_roots.size() - 1;
    }

    int insert(int prev, int bit, long long value) {
        int node = (int)trie.size();
        trie.push_back(trie[prev]);
        trie[node].cnt++;
        if (bit < 0) return node;
        int b = (value >> bit) & 1;
        trie[node].child[b] = insert(trie[prev].child[b], bit - 1, value);
        return node;
    }

    long long maxXorInRange(int versionL, int versionR, long long x) {
        return maxXorRec(version_roots[versionL], version_roots[versionR], BITS - 1, x);
    }

    long long maxXorRec(int nodeL, int nodeR, int bit, long long x) {
        if (bit < 0) return 0;
        int b = (x >> bit) & 1;
        int want = b ^ 1;
        int cntWant = trie[trie[nodeR].child[want]].cnt - trie[trie[nodeL].child[want]].cnt;
        if (cntWant > 0)
            return (1LL << bit) + maxXorRec(trie[nodeL].child[want], trie[nodeR].child[want], bit - 1, x);
        return maxXorRec(trie[nodeL].child[b], trie[nodeR].child[b], bit - 1, x);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    PersistentTrie T;
    for (int i = 0; i < n; ++i) {
        long long x;
        cin >> x;
        T.insert(i, x);
    }

    int q;
    cin >> q;
    while (q--) {
        int l, r;
        long long x;
        cin >> l >> r >> x;
        cout << T.maxXorInRange(l - 1, r, x) << '\n';
    }
    return 0;
}
