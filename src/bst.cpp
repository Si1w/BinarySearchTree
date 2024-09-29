#include "bst.h"

BST::Node* BST::copy_node(const Node* other_node) {
    if (other_node == nullptr) return nullptr;
    Node* new_node = new Node(other_node->value, nullptr, nullptr);
    new_node->left = copy_node(other_node->left);
    new_node->right = copy_node(other_node->right);
    return new_node;
}

BST::BST(const BST& other) : root(nullptr) {
    if (other.root != nullptr) {
        root = copy_node(other.root);
    }
}

BST& BST::operator=(const BST& other) {
    if (this == &other) return *this;
    
    clear();
    
    if (other.root != nullptr) {
        root = copy_node(other.root);
    } else {
        root = nullptr;
    }
    return *this;
}


BST::BST(BST&& other) noexcept : root(other.root) {
    other.root = nullptr;
}

BST& BST::operator=(BST&& other) noexcept {
    if (this == &other) return *this;
    
    clear();
    
    root = other.root;
    other.root = nullptr;
    return *this;
}

void BST::clear() {
    clear_node(root);
    root = nullptr;
}

void BST::clear_node(Node* node) {
    if (node == nullptr) return;
    clear_node(node->left);
    clear_node(node->right);
    delete node;
}

BST::Node::Node(int value, Node *left, Node *right) : value(value), left(left), right(right) {};

BST::Node::Node() : value(0), left(nullptr), right(nullptr) {};

BST::Node::Node(const Node &node) : value(node.value), left(node.left), right(node.right) {};

std::ostream &operator<<(std::ostream &os, const BST::Node &node)
{
    os << std::left;
    os << std::setw(18) << &node << "==> value:" << std::setw(10) << node.value;
    os << "left:" << std::setw(17) << node.left;
    os << "right:" << node.right << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const BST &bst)
{
    os << "********************************************************************************" << std::endl;
    auto lambda = [&os](BST::Node *&node) -> void
    {
        os << *node;
    };
    bst.bfs(lambda);
    os << "binary search tree size: " << bst.length() << std::endl;
    os << "********************************************************************************" << std::endl;

    return os;
}

BST::Node*& BST::get_root() {
    return root;
}

void BST::bfs(std::function<void(Node*& node)> func) const {
    if (root == nullptr) return;
    
    std::queue<Node*> queue;
    queue.push(root);
    
    while (!queue.empty()) {
        Node* current = queue.front();
        queue.pop();
        func(current);
        if (current->left) queue.push(current->left);
        if (current->right) queue.push(current->right);
    }
}

size_t BST::length() const {
    size_t count = 0;
    bfs([&count](Node*& node) {count++;});
    return count;
}

bool BST::add_node(int value) {
    if (root == nullptr) {
        root = new Node(value, nullptr, nullptr);
        return true;
    }

    Node* current = root;
    Node* parent = nullptr;

    while (current != nullptr) {
        if (value == current->value) {
            return false; 
        }
        
        parent = current;
        if (value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    Node* new_node = new Node(value, nullptr, nullptr);
    if (value < parent->value) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }

    return true;
}

BST::Node** BST::find_node(int value) {
    Node** current_ptr = &(root);

    while (*current_ptr != nullptr) {
        if ((*current_ptr)->value == value) return current_ptr;
        else if ((*current_ptr)->value < value) current_ptr = &(*current_ptr)->right;
        else current_ptr = &(*current_ptr)->left;
    }

    return nullptr;
}

BST::Node** BST::find_parrent(int value) {
    Node** current_ptr = &(root);
    Node** parent_ptr = nullptr;

    while (*current_ptr != nullptr) {
        if ((*current_ptr)->value == value) return parent_ptr;
        else if ((*current_ptr)->value < value) {
            parent_ptr = current_ptr;
            current_ptr = &(*current_ptr)->right;
        }
        else {
            parent_ptr = current_ptr;
            current_ptr = &(*current_ptr)->left;
        }
    }

    return nullptr;
}

BST::Node** BST::find_successor(int value) {
    Node** current_ptr = find_node(value);
    if (*current_ptr == nullptr || current_ptr == nullptr) return nullptr;

    // if has right child, successor is right child's leftmost child
    if ((*current_ptr)->right != nullptr) {
        Node** successor_ptr = &((*current_ptr)->right);
        while ((*successor_ptr)->left != nullptr) {
            successor_ptr = &((*successor_ptr)->left);
        }
        return successor_ptr;
    }

    // if no right child, successor is first parent that is greater than current node
    else {
        Node** successor_ptr = find_parrent(value);
        while (*successor_ptr != root && (*successor_ptr)->right == *current_ptr) {
            current_ptr = successor_ptr;
            successor_ptr = find_parrent((*current_ptr)->value);
        }
        if (*successor_ptr == root) return nullptr;
        return successor_ptr;
    }
}

BST::Node** BST::find_predecessor(int value)
{
    Node** node_ptr = find_node(value);
    if (node_ptr == nullptr || *node_ptr == nullptr) return nullptr;

    Node* node = *node_ptr;
    if (node->left == nullptr) return nullptr;

    Node** predecessor_ptr = &node->left;
    while ((*predecessor_ptr)->right != nullptr) {
        predecessor_ptr = &(*predecessor_ptr)->right;
    }
    return predecessor_ptr;
}

bool BST::delete_node(int value)
{
    Node** node_ptr = find_node(value);
    if (node_ptr == nullptr || *node_ptr == nullptr) return false;

    Node* node = *node_ptr;

    // node has two children
    if (node->left != nullptr && node->right != nullptr) {
        Node** predecessor_ptr = find_predecessor(value);
        if (predecessor_ptr == nullptr) return false;
        Node* predecessor = *predecessor_ptr;
        
        // remove predecessor from tree
        *predecessor_ptr = predecessor->left;
        
        // replace current node with predecessor
        predecessor->left = node->left;
        predecessor->right = node->right;
        *node_ptr = predecessor;
    }
    // node has one or no children
    else {
        *node_ptr = (node->left != nullptr) ? node->left : node->right;
    }

    delete node;
    return true;
}

BST::BST() : root(nullptr) {}

BST::~BST() {
    std::vector<Node*> nodes;
    this->bfs([&nodes](BST::Node*& node) { nodes.push_back(node); });
    for (auto& node : nodes)
        delete node;
}

BST::BST(std::initializer_list<int> init) {
    root = nullptr;
    for (int value : init) {
        add_node(value);
    }
}

BST& BST::operator++() {
    std::vector<Node *> nodes;
    bfs([&nodes](BST::Node *&node) { nodes.push_back(node); });
    for (Node* node : nodes) {
        node->value++;
    }
    return *this;
}

BST BST::operator++(int) {
    auto copy = (*this);
    ++(*this);
    return copy;
}

bool operator<(const BST::Node &node, const int rhs)
{
    return node.value < rhs;
}

bool operator<=(const BST::Node &node, const int rhs)
{
    return node.value <= rhs;
}

bool operator==(const BST::Node &node, const int rhs)
{
    return node.value == rhs;
}

bool operator>(const BST::Node &node, const int rhs)
{
    return node.value > rhs;
}

bool operator>=(const BST::Node &node, const int rhs)
{
    return node.value >= rhs;
}

bool operator!=(const BST::Node &node, const int rhs)
{
    return node.value != rhs;
}

bool operator<(const int lhs, const BST::Node &node)
{
    return node > lhs;
}

bool operator<=(const int lhs, const BST::Node &node)
{
    return node >= lhs;
}

bool operator==(const int lhs, const BST::Node &node)
{
    return node == lhs;
}

bool operator>(const int lhs, const BST::Node &node)
{
    return node < lhs;
}

bool operator>=(const int lhs, const BST::Node &node)
{
    return node <= lhs;
}

bool operator!=(const int lhs, const BST::Node &node)
{
    return node != lhs;
}