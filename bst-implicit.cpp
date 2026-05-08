//
// Created by Asus on 07/05/2026.
//
#include <iostream>
#include <vector>

using namespace std;

struct Node {
    int data;
    int prio;
    Node *left;
    Node *right;
    int tam;
    int lazy_add;
    bool lazy_rev;
    int mn;

    Node(int data) {
        this->data = data;
        this->prio = rand() % 100000 + 1;
        this->left = nullptr;
        this->right = nullptr;
        this->tam = 1;
        this->lazy_add = 0;
        this->lazy_rev = false;
        this->mn = data;
    }

    int posicion() {

    }


};
void push(Node* node) {
    if (node == nullptr) return;

    if (node->lazy_rev) {
        swap(node->left, node->right);
        if (node->left != nullptr) {
            node->left->lazy_rev = !node->left->lazy_rev;
        }
        if (node->right != nullptr) {
            node->right->lazy_rev = !node->right->lazy_rev;
        }
        node->lazy_rev = false;
    }

    if (node->lazy_add != 0) {
        int val = node->lazy_add;
        node->data += val;
        node->mn += val;
        if (node->left != nullptr) {
            node->left->lazy_add += val;
            node->left->mn += val;
        }
        if (node->right != nullptr) {
            node->right->lazy_add += val;
            node->right->mn += val;
        }
        node->lazy_add = 0;
    }
}
void print_all(Node* root) {
    if (root == nullptr) {
        return;

    } else {
        push(root);
        print_all(root->left);
        cout << root->data << " ";
        print_all(root->right);

    }



};
int get_min(Node* node) {
    if (node == nullptr) return INT_MAX;
    return node->mn;
}
int get_size(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->tam;
}
void upd_size(Node* node) {
    if (node == nullptr) return;
    node->tam = 1 + get_size(node->left) + get_size(node->right);
    node->mn = min(node->data, min(get_min(node->left), get_min(node->right)));
}
Node* search(Node* root, int k) {
    int pos = 0;
    if (root == nullptr) {
        return nullptr;
    }
    if (root->left  != nullptr) {
        pos = root->left->tam;
    }

    if (k == pos) {
        return root;
    }
    else if (k < pos) {
        // izquierda
        return search(root->left, k);
    }
    else {
        k = k - pos -1;
        return search(root->right, k);
    }

};



void split(Node *root, int k, Node*& left, Node*& right) {
    int pos = 0;
    if (root == nullptr) {
            right = nullptr;
         left = nullptr;
            return;
    }
    push(root);
    if (root->left  != nullptr) {
        pos = root->left->tam;
    }
    if (k <= pos) {
        split(root->left, k, left, root->left);  // resultado izquierdo se guarda en left
        // resultado derecho sera parte de la mitad mayor, asi que el root debe linkearlo

        right = root; // al final de los splits hemos guardado el valor de right final
        upd_size(root);


    }
    else { // k> pos
        // split ocurre en la derecha
        k = k - pos -1;
        split(root->right, k, root->right, right); // el resultado izquierdo de un split por hijo derecho
        // sera parte de la primera mitad inferior, por ello se linea root->right, y el resultado derecho el right
        // que buscamos
        left = root;
        upd_size(root);
    }
 // increible

}
Node* merge_1(Node* p1, Node* p2) {
    push(p1); // sumamos lazy
    push(p2); // suammos lazy
    if (p1 == nullptr) {
        return p2;
    }
    if (p2 == nullptr) {
        return p1;
    }
    // para mergear vamos a unir de acuerdo a prioridad
    if (p1->prio > p2->prio) {
        p1->right = merge_1(p1->right, p2);
        upd_size(p1);
        return p1;
    }
    else {
        p2->left = merge_1(p1, p2->left);
        upd_size(p2);
        return p2;
    }

}
Node* insert(int pos, Node* root, Node* node) {
    // vamos a hacer split de root
    Node* left= nullptr;
    Node* right = nullptr;

    split(root,pos, left, right);


    Node* resultado = merge_1(left, node);


    Node* resultado_1 = merge_1(resultado, right);
    return resultado_1;
}

struct Query {
    char type;

    int l = 0;
    int r = 0;
    int k = 0;

    long long x = 0;
};
void A(int l, int r, int x, Node*& root) {
    Node* left = nullptr;
    Node* right = nullptr;
    Node* mid = nullptr;
    split(root, l - 1, left, right);
    split(right, r - l + 1, mid, right);
    if (mid != nullptr) {
        mid->lazy_add += x;
        mid->mn += x;
    }
    root = merge_1(left, merge_1(mid, right));
}

void R(int l, int r, Node*& root) {
    Node* left = nullptr;
    Node* right = nullptr;
    Node* mid = nullptr;
    split(root, l - 1, left, right);
    split(right, r - l + 1, mid, right);
    if (mid != nullptr) {
        mid->lazy_rev = !mid->lazy_rev;
    }
    root = merge_1(left, merge_1(mid, right));
}

void O(int l, int r, int k, Node*& root) {
    int len = r - l + 1;
    k = k % len;
    if (k == 0) return;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* mid = nullptr;
    split(root, l - 1, left, right);
    split(right, r - l + 1, mid, right);
    Node* mid_left = nullptr;
    Node* mid_right = nullptr;
    split(mid, len - k, mid_left, mid_right);
    root = merge_1(left, merge_1(merge_1(mid_right, mid_left), right));
}

void I(int k, int x, Node*& root) {
    Node* node = new Node(x);
    root = insert(k, root, node);
}

void E(int k, Node*& root) {
    Node* left = nullptr;
    Node* right = nullptr;
    Node* mid = nullptr;
    split(root, k - 1, left, right);
    split(right, 1, mid, right);
    delete mid;
    root = merge_1(left, right);
}

int M(int l, int r, Node*& root) {
    Node* left = nullptr;
    Node* right = nullptr;
    Node* mid = nullptr;
    split(root, l - 1, left, right);
    split(right, r - l + 1, mid, right);
    int resultado = get_min(mid);
    root = merge_1(left, merge_1(mid, right));
    return resultado;
}

int main() {

    int n, q;
    cin >> n >> q;

    vector<int> a(n);

    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    vector<Query> queries(q);

    for (int i = 0; i < q; i++) {
        char op;
        cin >> op;
        queries[i].type = op;
        if (op == 'A') {
            cin >> queries[i].l >> queries[i].r >> queries[i].x;
        } else if (op == 'R') {
            cin >> queries[i].l >> queries[i].r;
        } else if (op == 'O') {
            cin >> queries[i].l >> queries[i].r >> queries[i].k;
        } else if (op == 'I') {
            cin >> queries[i].k >> queries[i].x;
        } else if (op == 'E') {
            cin >> queries[i].k;
        } else if (op == 'M') {
            cin >> queries[i].l >> queries[i].r;
        }
    }


    // vamos a insertar los valores de entrada dentro de la estructura Nodo
    Node* root = nullptr;
    for (int i = 0; i < n; i++) {
        Node* node = new Node(a[i]);
        root = insert(i, root, node);
        print_all(root);
    }

    for (int i = 0; i < q; i++) {
        Query& qr = queries[i];
        if (qr.type == 'A') {
            A(qr.l, qr.r, (int)qr.x, root);
        } else if (qr.type == 'R') {
            R(qr.l, qr.r, root);
        } else if (qr.type == 'O') {
            O(qr.l, qr.r, qr.k, root);
        } else if (qr.type == 'I') {
            I(qr.k, (int)qr.x, root);
        } else if (qr.type == 'E') {
            E(qr.k, root);
        } else if (qr.type == 'M') {
            cout << M(qr.l, qr.r, root) << "\n";
        }
    }

    return 0;

    return 0;
}

