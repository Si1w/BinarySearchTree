#ifndef BST_H
#define BST_H

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <iomanip>

class BST {
public:
    class Node {
    public:
        Node(int value, Node* left, Node* right);
        Node();
        Node(const Node& node);

        int value;
        Node* left;
        Node* right;

        friend std::ostream &operator<<(std::ostream &os, const Node &node);

        friend bool operator<(const Node &node, const int rhs);
        friend bool operator<=(const Node &node, const int rhs);
        friend bool operator==(const Node &node, const int rhs);
        friend bool operator>(const Node &node, const int rhs);
        friend bool operator>=(const Node &node, const int rhs);
        friend bool operator!=(const Node &node, const int rhs);

        friend bool operator<(const int lhs, const Node &node);
        friend bool operator<=(const int lhs, const Node &node);
        friend bool operator==(const int lhs, const Node &node);
        friend bool operator>(const int lhs, const Node &node);
        friend bool operator>=(const int lhs, const Node &node);
        friend bool operator!=(const int lhs, const Node &node);
    };

    Node*& get_root();
    void bfs(std::function<void(Node*& node)> func) const;
    size_t length() const;
    bool add_node(int value);
    Node** find_node(int value);
    Node** find_parrent(int value);
    Node** find_successor(int value);
    Node** find_predecessor(int value);
    bool delete_node(int value);

    friend std::ostream &operator<<(std::ostream &os, const BST &bst);
    BST();
    ~BST();
    BST(const BST & bst);
    BST(BST&& bst) noexcept;
    BST(std::initializer_list<int> init);
    BST& operator=(const BST & other);
    BST& operator=(BST && other) noexcept;
    BST& operator++();
    BST operator++(int);
    Node* copy_node(const Node* other_node);
    void clear();
    void clear_node(Node* node);

private:
    Node* root;
};

#endif //BST_H