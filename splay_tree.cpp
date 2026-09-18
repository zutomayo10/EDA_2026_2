#include <bits/stdc++.h>

using namespace std;

struct SplayTree {
    struct Node {
        long long key;
        int size;
        Node *left = nullptr, *right = nullptr, *parent = nullptr;
        Node(long long k) : key(k), size(1) {}
    };

    Node* root = nullptr;

    static int sz(Node* x) { return x ? x->size : 0; }
    static void pull(Node* x) { if (x) x->size = 1 + sz(x->left) + sz(x->right); }

    void rotate(Node* p, Node* x) {
        Node* g = p->parent;
        if (g) (g->left == p ? g->left : g->right) = x;
        x->parent = g;
        if (p->left == x) {
            p->left = x->right;
            if (x->right) x->right->parent = p;
            x->right = p;
        } else {
            p->right = x->left;
            if (x->left) x->left->parent = p;
            x->left = p;
        }
        p->parent = x;
        pull(p);
        pull(x);
    }

    void splay(Node* x) {
        while (x->parent) {
            Node* p = x->parent;
            Node* g = p->parent;
            if (!g) {
                rotate(p, x);
            } else if ((g->left == p) == (p->left == x)) {
                rotate(g, p);
                rotate(p, x);
            } else {
                rotate(p, x);
                rotate(g, x);
            }
        }
        root = x;
    }

    bool find(long long key) {
        Node* v = root;
        Node* last = nullptr;
        bool found = false;
        while (v) {
            last = v;
            if (v->key == key) { found = true; break; }
            v = key < v->key ? v->left : v->right;
        }
        if (last) splay(last);
        return found;
    }

    pair<Node*, Node*> split(long long key) {
        find(key);
        if (!root) return {nullptr, nullptr};
        if (root->key <= key) {
            Node* r = root->right;
            root->right = nullptr;
            if (r) r->parent = nullptr;
            pull(root);
            Node* l = root;
            root = nullptr;
            return {l, r};
        } else {
            Node* l = root->left;
            root->left = nullptr;
            if (l) l->parent = nullptr;
            pull(root);
            Node* r = root;
            root = nullptr;
            return {l, r};
        }
    }

    Node* join(Node* A, Node* B) {
        if (!A) return B;
        if (!B) return A;
        Node* v = A;
        while (v->right) v = v->right;
        splay(v);
        v->right = B;
        B->parent = v;
        pull(v);
        return v;
    }

    void insert(long long key) {
        if (find(key)) return;
        auto [L, R] = split(key);
        Node* x = new Node(key);
        x->left = L; if (L) L->parent = x;
        x->right = R; if (R) R->parent = x;
        pull(x);
        root = x;
    }

    void erase(long long key) {
        if (!find(key)) return;
        Node* r = root;
        Node* L = r->left, *R = r->right;
        if (L) L->parent = nullptr;
        if (R) R->parent = nullptr;
        root = join(L, R);
        delete r;
    }

    long long kth(int k) {
        Node* v = root;
        while (v) {
            int leftSize = sz(v->left);
            if (k == leftSize + 1) { splay(v); return v->key; }
            if (k <= leftSize) v = v->left;
            else { k -= leftSize + 1; v = v->right; }
        }
        return LLONG_MIN; // k invalido
    }

    int countLessOrEqual(long long key) {
        auto [L, R] = split(key);
        int cnt = sz(L);
        root = join(L, R);
        return cnt;
    }

    int countLess(long long key) { return countLessOrEqual(key - 1); }

    int size() { return sz(root); }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    SplayTree T;
    int q;
    cin >> q;
    while (q--) {
        char op;
        long long x;
        cin >> op >> x;
        if (op == 'I') T.insert(x);
        else if (op == 'D') T.erase(x);
        else if (op == 'K') cout << T.kth((int)x) << '\n';
        else if (op == 'C') cout << T.countLess(x) << '\n';
    }
    return 0;
}
