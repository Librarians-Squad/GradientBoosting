#ifndef GRADIENTBOOSTING_HEADER_HPP
#define GRADIENTBOOSTING_HEADER_HPP


#include <utility>
#include <functional>


class Predicate {
public:
    explicit Predicate(std::function<bool(double)> func = nullptr,
              int feature_idx = -1)
    : function(std::move(func)),
      feature_idx(feature_idx)
    {}

    bool operator()(const std::vector<double>& features) const {
        return function(features[feature_idx]);
    }

private:
    std::function<bool(double)> function;
    int feature_idx;
};


class TreeNode {
public:
    explicit TreeNode(bool is_leaf)
    : _is_leaf(is_leaf)
    {}

    [[nodiscard]] bool is_leaf() const {
        return this->_is_leaf;
    }
protected:
    bool _is_leaf;
};


class InnerNode : public TreeNode {
public:
    InnerNode()
    : TreeNode(false),
      predicate(Predicate()),
      left_child(nullptr),
      right_child(nullptr)
    {}

private:
    Predicate predicate;
    TreeNode* left_child;
    TreeNode* right_child;
};


class LeafNode : public TreeNode {
public:
    LeafNode()
    : TreeNode(true),
      value()
    {}

private:
    std::string value;
};


class DecisionTreeClassifier {
public:
    explicit DecisionTreeClassifier() {
        this->root = new InnerNode();
    }

    void fit(const std::vector<std::vector<double>>& x_train, const std::vector<double>& y_train) {
        // ...
    }

    int predict(int* question) {
        // ...
    }
    int* predict(int** questions) {
        // ...
    }

    void save_configuration(const std::string& filepath) {
        // ...
    }

    static DecisionTreeClassifier& browse_tree(const std::string& filepath) {
        // ...
    }

private:
    InnerNode* root;
};


#endif //GRADIENTBOOSTING_HEADER_HPP
