#ifndef GRADIENTBOOSTING_DECISION_TREE_HPP
#define GRADIENTBOOSTING_DECISION_TREE_HPP

#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>

#include "utility.hpp"

using SampleArray = std::vector<double>;
using SampleMatrix = std::vector<SampleArray>;
using ClassArray = std::vector<size_t>;
using UnaryPredicate = std::function<bool(double)>;

using SampleMatrixIterator = std::vector<SampleArray>::iterator;
using ClassArrayIterator = std::vector<size_t>::iterator;

class UnaryPredicateWrapper {
public:
    UnaryPredicateWrapper() = delete;
    UnaryPredicateWrapper(UnaryPredicate predicate, const size_t index) :
        pred(std::move(predicate)), idx(index) {}

    bool operator()(const SampleArray& samples) const { return pred(samples[idx]); }

    size_t get_index() const { return idx; }
    void set_index(const size_t index) { idx = index; }

    UnaryPredicate get_predicate() const { return pred; }
    void set_predicate(UnaryPredicate predicate) { pred = std::move(predicate); }

private:
    UnaryPredicate pred;
    size_t idx;
};

struct LearningParameters {
    using DataMetric = std::function<bool(
            SampleMatrixIterator, SampleMatrixIterator,
            ClassArrayIterator, ClassArrayIterator)>;
    using PredicateDecider = std::function<UnaryPredicateWrapper(
            SampleMatrixIterator, SampleMatrixIterator,
            ClassArrayIterator, ClassArrayIterator)>;
    using ClassDecider = std::function<size_t(
            SampleMatrixIterator, SampleMatrixIterator,
            ClassArrayIterator, ClassArrayIterator)>;

    //    double splitting_accuracy;
    //    int max_depth;
    //    int min_samples_leaf;

    DataMetric stop_condition;
    PredicateDecider compute_predicate;
    ClassDecider compute_class;
};

struct TreeNode {
    virtual int operator()(const SampleArray& samples) const = 0;

    virtual bool is_leaf() const = 0;
};

struct LeafNode : public TreeNode {
    explicit LeafNode(const size_t idx) : cls_idx(idx) {}

    int operator()(const SampleArray&) const override { return cls_idx; }

    bool is_leaf() const override { return true; }

    size_t cls_idx;
};

struct InnerNode : public TreeNode {
    explicit InnerNode(UnaryPredicateWrapper predicate)
        : pred(std::move(predicate)) {}

    int operator()(const SampleArray& samples) const override {
        return pred(samples) ?
            left->operator()(samples) :
            right->operator()(samples);
    }

    bool is_leaf() const override { return false; }

    UnaryPredicateWrapper pred;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

class DecisionTreeClassifier {
public:
    DecisionTreeClassifier() = default;

    void fit(SampleMatrix& x_train, ClassArray& y_train) {
        if (root) return; // Tree is already trained
        root = fit_individual(x_train.begin(), x_train.end(), y_train.begin(), y_train.end());
    }

    int predict(const SampleArray& samples) {
        return root->operator()(samples);
    }

    // Make an one-liner?
    ClassArray predict(const SampleMatrix& samples) {
        ClassArray res;
        res.reserve(samples.size());
        for (const auto& s : samples) res.push_back(predict(s));
        return res;
    }

    void serialize(const std::string& filepath);
    static DecisionTreeClassifier& deserialize(const std::string& filepath);

private:
    TreeNode* root = nullptr;
    LearningParameters parameters;

    [[nodiscard]] TreeNode* fit_individual(
            SampleMatrixIterator x_begin, SampleMatrixIterator x_end,
            ClassArrayIterator y_begin, ClassArrayIterator y_end) const {
        if (!parameters.stop_condition(x_begin, x_end, y_begin, y_end)) {
            return new LeafNode(parameters.compute_class(x_begin, x_end, y_begin, y_end));
        }
        auto* node = new InnerNode(parameters.compute_predicate(x_begin, x_end, y_begin, y_end));

        std::vector<size_t> indices(x_end - x_begin);
        std::iota(indices.begin(), indices.end(), 0);

        // Basically an np.argpartition()
        auto it = std::partition(indices.begin(), indices.end(),
                [x_begin, node] (auto i) { return node->pred(*(x_begin + i)); });
        auto idx = it - indices.begin();

        reorder(x_begin, x_end, indices.begin());
        reorder(y_begin, y_end, indices.begin());

        node->left = fit_individual(x_begin, x_begin + idx, y_begin, y_begin + idx);
        node->right = fit_individual(x_begin + idx, x_end, y_begin + idx, y_end);

        return node;
    }
};

#endif //GRADIENTBOOSTING_DECISION_TREE_HPP
