#include <functional>
#include <iostream>
#include <memory>
#include <random>

void print_tabs(int count) {
    for (int i = 0; i < count; i++) {
        std::cout << "\t";
    }
}

template<typename T>
class BinarySearchTree {
public:
    BinarySearchTree() : root{nullptr} {}

    void insert(T value);

    T* find(std::function<bool (T const&)> pred);
    T* find(T const& value);
    T const* find(T const& value) const;

    void print() const;

private:
    struct Node {
        template <typename... Args>
        Node(Args... args) : value{args...}, left_child{nullptr}, right_child{nullptr} {}

        template <typename... Args>
        static std::unique_ptr<Node> make(Args... args) {
            return std::make_unique<Node>(args...);
        }

        static std::unique_ptr<Node> make(T value, T left, T right) {
            return { value, left, right };
        }

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
        
        T value;

        std::unique_ptr<Node> left_child;
        std::unique_ptr<Node> right_child;
    };

    std::unique_ptr<Node> root;
};

template <typename T>
void BinarySearchTree<T>::insert(T value) {
    if (!root) {
        root = Node::make(value);
        return;
    }

    Node* prev = nullptr;
    auto* current = root.get();
    while (current) {
        if (value <= current->value) {
            prev = current;
            current = current->left_child.get();
            continue;
        }

        prev = current;
        current = current->right_child.get();
    }

    if (value <= prev->value) {
        prev->left_child = Node::make(value);
        return;
    }

    prev->right_child = Node::make(value);
}

template <typename T>
T* BinarySearchTree<T>::find(T const& value) {
    auto* current = root.get();

    while (current) {
        if (current->value == value) {
            return &current->value;
        }

        if (current->value < value) {
            current = current->right_child.get();
            continue;
        }

        current = current->left_child.get();
    }

    return nullptr;
}

template <typename T>
T const* BinarySearchTree<T>::find(T const& value) const {
    auto* current = root.get();

    while (current) {
        if (current->value == value) {
            return &current->value;
        }

        if (current->value < value) {
            current = current->right_child.get();
            continue;
        }

        current = current->left_child.get();
    }

    return nullptr;
}

template <typename T>
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
}
