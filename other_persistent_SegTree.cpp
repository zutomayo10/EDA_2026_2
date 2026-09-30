#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

struct PersistentSegmentTreeNode {
    int value;
    int l, r;
    PersistentSegmentTreeNode* left;
    PersistentSegmentTreeNode* right;
};

class PersistentSegmentTree {
private:
    int n;

    void checkVersion(int version) const {
        if (version < 0 || version >= static_cast<int>(roots.size())) {
            throw out_of_range("Version de Segment Tree invalida");
        }
    }

    PersistentSegmentTreeNode* build(const vector<int>& values, int l, int r) {
        PersistentSegmentTreeNode* node =
            new PersistentSegmentTreeNode{0, l, r, nullptr, nullptr};

        if (l == r) {
            node->value = values[l];
            return node;
        }

        int mid = (l + r) / 2;
        node->left = build(values, l, mid);
        node->right = build(values, mid + 1, r);
        node->value = node->left->value + node->right->value;
        return node;
    }

    PersistentSegmentTreeNode* update(
        PersistentSegmentTreeNode* last,
        int position,
        int value
    ) {
        PersistentSegmentTreeNode* current =
            new PersistentSegmentTreeNode(*last);

        current->left = last->left;
        current->right = last->right;

        if (current->l == current->r) {
            current->value = value;
            return current;
        }

        int mid = (current->l + current->r) / 2;
        if (position <= mid) {
            current->left = update(last->left, position, value);
            current->right = last->right;
        } else {
            current->left = last->left;
            current->right = update(last->right, position, value);
        }

        current->value = current->left->value + current->right->value;
        return current;
    }

    int query(PersistentSegmentTreeNode* node, int ql, int qr) const {
        if (qr < node->l || node->r < ql) {
            return 0;
        }
        if (ql <= node->l && node->r <= qr) {
            return node->value;
        }

        return query(node->left, ql, qr) + query(node->right, ql, qr);
    }

public:
    vector<PersistentSegmentTreeNode*> roots;

    explicit PersistentSegmentTree(const vector<int>& values)
        : n(static_cast<int>(values.size())) {
        if (values.empty()) {
            throw invalid_argument("El arreglo no puede estar vacio");
        }
        roots.push_back(build(values, 0, static_cast<int>(values.size()) - 1));
    }

    int update(int version, int position, int value) {
        checkVersion(version);
        if (position < 0 || position >= n) {
            throw out_of_range("Posicion de update invalida");
        }
        PersistentSegmentTreeNode* newRoot =
            update(roots[version], position, value);
        roots.push_back(newRoot);
        return static_cast<int>(roots.size()) - 1;
    }

    int query(int version, int l, int r) const {
        checkVersion(version);
        if (l < 0 || r >= n || l > r) {
            throw out_of_range("Rango de query invalido");
        }
        return query(roots[version], l, r);
    }
};

int main() {
    vector<int> values = {5, 2, 7, 3};
    PersistentSegmentTree tree(values);

    int version1 = tree.update(0, 3, 10);
    int version2 = tree.update(0, 0, 1);

    cout << tree.query(0, 0, 3) << '\n';
    cout << tree.query(version1, 0, 3) << '\n';
    cout << tree.query(version2, 0, 3) << '\n';

    return 0;
}
