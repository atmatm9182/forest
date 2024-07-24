#include <functional>
#include <iostream>
#include <concepts>

void print_tabs(int count) {
    for (int i = 0; i < count; i++) {
        std::cout << "\t";
    }
}

template<std::totally_ordered T>
class BinarySearchTree {
private:
    struct Node {
        template <typename... Args>
        Node(Args... args) : value{args...}, left_child{nullptr}, right_child{nullptr} {}

        void print_with_indent(int indent) const {
            print_tabs(indent);

            std::cout << value << std::endl;

            if (left_child) {
                left_child->print_with_indent(indent + 1);
            } else {
                print_tabs(indent + 1);
                std::cout << "<no left node>" << std::endl;
            }

            if (right_child) {
                right_child->print_with_indent(indent + 1);
            } else {
                print_tabs(indent + 1);
                std::cout << "<no right node>" << std::endl;
            }
        }

        void free() {
            if (left_child) {
                left_child->free();
            }

            if (right_child) {
                right_child->free();
            }

            delete this;
        }

        Node* delete_elem(T const& value);
        
        T value;

        Node* left_child;
        Node* right_child;
    };

public:
    BinarySearchTree() : root{nullptr} {}

    void insert(T value);

    T* find(std::function<bool (T const&)> pred);
    T* find(T const& value);
    T const* find(T const& value) const;

    Node* delete_element(T const& value);

    void print() const;

    ~BinarySearchTree() {
        if (root->left_child) {
            root->left_child->free();
        }

        if (root->right_child) {
            root->right_child->free();
        }

        delete root;
    }

private:
    Node* root;
};

template <std::totally_ordered T>
void BinarySearchTree<T>::insert(T value) {
    auto* new_node = new Node(value);
    
    if (!root) {
        root = new_node;
        return;
    }

    Node* prev = nullptr;
    auto* current = root;
    while (current) {
        if (value <= current->value) {
            prev = current;
            current = current->left_child;
            continue;
        }

        prev = current;
        current = current->right_child;
    }

    if (value <= prev->value) {
        prev->left_child = new_node;
        return;
    }

    prev->right_child = new_node;
}

template <std::totally_ordered T>
T* BinarySearchTree<T>::find(T const& value) {
    auto* current = root;

    while (current) {
        if (current->value == value) {
            return &current->value;
        }

        if (current->value < value) {
            current = current->right_child;
            continue;
        }

        current = current->left_child;
    }

    return nullptr;
}

template <std::totally_ordered T>
T const* BinarySearchTree<T>::find(T const& value) const {
    auto* current = root;

    while (current) {
        if (current->value == value) {
            return &current->value;
        }

        if (current->value < value) {
            current = current->right_child;
            continue;
        }

        current = current->left_child;
    }

    return nullptr;
}

template <std::totally_ordered T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::delete_element(T const& value) {
    return root = root->delete_elem(value);
}

template <std::totally_ordered T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::Node::delete_elem(T const& value) {
    if (!this) {
        return nullptr;
    }
    
    if (value < this->value) {
        left_child = left_child->delete_elem(value);
        return this;
    }

    if (value > this->value) {
        right_child = right_child->delete_elem(value);
        return this;
    }

    if (!(left_child || right_child)) {
        delete this;
    }

    if (!left_child) {
        auto* right = right_child;
        delete this;
        return right;
    }

    if (!right_child) {
        auto* left = left_child;
        delete this;
        return left;
    }

    auto* parent = this;
    auto* succ = parent->right_child;
    while (succ->left_child) {
        parent = succ;
        succ = parent->left_child;
    }

    this->value = succ->value;

    if (parent->left_child == succ) {
        parent->left_child = succ->right_child;
    } else {
        parent->right_child = succ->right_child;
    }

    delete succ;

    return this;
}

template <std::totally_ordered T>
void BinarySearchTree<T>::print() const {
    if (root) {
        std::cout << "root: ";
        root->print_with_indent(0);
    }
}

int main() {
    BinarySearchTree<int> bst{};

    bst.insert(0);
    bst.insert(0);
    bst.insert(1);
    bst.insert(7);
    bst.insert(2);
    bst.insert(4);
    bst.insert(3);
    bst.insert(5);
    bst.insert(6);
    bst.insert(69);

    bst.print();

    bst.delete_element(7);

    bst.print();
}
