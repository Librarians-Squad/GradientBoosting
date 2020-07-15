#ifndef GRADIENTBOOSTING_HEADER_HPP
#define GRADIENTBOOSTING_HEADER_HPP


#include <functional>


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
    InnerNode(std::function<bool(double)>& predicate, int feature_idx)
    : TreeNode(false),
      predicate(predicate),
      feature_idx(feature_idx),
      left_child(nullptr),
      right_child(nullptr)
    {}

    [[nodiscard]] bool use_predicate(const std::vector<double>& sample) const {
        return this->predicate(sample[this->feature_idx]);
    }

    // not sure, that we need this method
    [[nodiscard]] int get_feature_idx() const {
        return this->feature_idx;
    }

    [[nodiscard]] TreeNode* get_left() const {
        return this->left_child;
    }

    [[nodiscard]] TreeNode* get_right() const {
        return this->right_child;
    }

    void set_left(TreeNode* node) {
        this->left_child = node;
    }

    void set_right(TreeNode* node) {
        this->right_child = node;
    }

private:
    std::function<bool(double)> predicate;
    int feature_idx;
    TreeNode* left_child;
    TreeNode* right_child;
};


class LeafNode : public TreeNode {
public:
    explicit LeafNode(int class_idx)
    : TreeNode(true),
      class_idx(class_idx)
    {}

    [[nodiscard]] int get_class() const {
        return this->class_idx;
    }

private:
    int class_idx;
};


class DecisionTreeClassifier {
public:
    explicit DecisionTreeClassifier() {
        this->root = nullptr;
    }

    void fit(const std::vector<std::vector<double>>& x_train, const std::vector<double>& y_train) {
        this->root = DecisionTreeClassifier::teach_node(x_train, y_train);
    }

    int predict(const std::vector<double>& question) {
        InnerNode* current_node = this->root;
        while (true) {
            if (current_node->use_predicate(question)) {
                if (current_node->get_left()->is_leaf()) {
                    return static_cast<LeafNode*>(current_node->get_left())->get_class();
                } else {
                    current_node = static_cast<InnerNode*>(current_node->get_left());
                }
            } else {
                if (current_node->get_right()->is_leaf()) {
                    return static_cast<LeafNode*>(current_node->get_right())->get_class();
                } else {
                    current_node = static_cast<InnerNode*>(current_node->get_right());
                }
            }
        }
    }

    std::vector<int>& predict(const std::vector<std::vector<double>>& questions) {
        std::vector<int> answers{};
        for (const auto& question : questions) {
            int answer = this->predict(question);
            answers.push_back(answer);
        }
        return answers;
    }

    void save_configuration(const std::string& filepath) {
        // ...
    }

    static DecisionTreeClassifier& browse_tree(const std::string& filepath) {
        // ...
    }

private:
    InnerNode* root;

    // recursive function to teach decision tree
    static InnerNode* teach_node(const std::vector<std::vector<double>>& x_train,
                    const std::vector<double>& y_train) {
        /*** some computings ***/

        InnerNode* node = new InnerNode(best_predicate, feature_idx);
        if (/*** teaching break condition ***/) {
            node->set_left(teach_node(left_x_train, left_y_train));
            node->set_right(teach_node(right_x_train, right_y_train));
        } else {
            int left_class_idx = /*** some computings ***/;
            int right_class_idx = /*** some computings ***/;

            node->set_left(new LeafNode(left_class_idx))
            node->set_right(new LeafNode(right_class_idx))
        }
        return node;
    }
};


#endif //GRADIENTBOOSTING_HEADER_HPP
