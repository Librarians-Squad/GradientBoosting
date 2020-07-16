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
        this->root = (InnerNode*)teach_node(x_train, y_train);
    }

    int predict(const std::vector<double>& question) {
        TreeNode* current_node = this->root;
        while (true) {
            if (current_node->is_leaf()) {
                return ((LeafNode*)current_node)->get_class();
            } else {
                auto* node = (InnerNode*)current_node;

                if (node->use_predicate(question)) {
                    current_node = node->get_left();
                } else {
                    current_node = node->get_right();
                }
            }
        }
    }

    std::vector<int> predict(const std::vector<std::vector<double>>& questions) {
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
    static TreeNode* teach_node(const std::vector<std::vector<double>>& x_train,
                    const std::vector<double>& y_train) {

        /*** split test ***/

        TreeNode* resulting_node = nullptr;

        if (/*** we can split ***/) {
            /*** find out best predicate and feature for next splitting ***/
            /*** split matrix of samples to right matrix and left matrix ***/
            InnerNode* node = new InnerNode(predicate, feature_idx);

            node->set_left(teach_node(left_x_train, left_y_train));
            node->set_right(teach_node(right_x_train, right_y_train));

            resulting_node = node;
        } else {
            /*** find out leaf class and returns LeafNode ***/
            resulting_node = new LeafNode(class_idx);
        }

        return resulting_node;
    }
};


#endif //GRADIENTBOOSTING_HEADER_HPP
