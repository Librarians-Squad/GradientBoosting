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
            const Range<SampleMatrix>&, const Range<ClassArray>&)>;
    using PredicateDecider = std::function<UnaryPredicateWrapper(
            const Range<SampleMatrix>&, const Range<ClassArray>&)>;
    using ClassDecider = std::function<size_t(
            const Range<SampleMatrix>&, const Range<ClassArray>&)>;

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
    //        root = fit_individual(Range(x_train), Range(y_train));
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

    TreeNode* fit_individual(Range<SampleMatrix> x_train, Range<ClassArray> y_train) {
        if (!parameters.stop_condition(x_train, y_train))
            return new LeafNode(parameters.compute_class(x_train, y_train));
        auto* node = new InnerNode(parameters.compute_predicate(x_train, y_train));

        std::vector<size_t> indices(x_train.size());
        std::iota(indices.begin(), indices.end(), 0);

//        // Basically an np.argpartition
//        auto it = std::partition(indices.begin(), indices.end(),
//                [x_train, node] (auto i) { return node->pred(*(x_train.begin() + i)); });
//        auto idx = it - indices.begin();
//
//
//        reorder(x_train, Range(indices));
//        reorder(y_train, Range(indices));
//
//        node->left = fit_individual(Range(x_train.begin(), x_train.begin() + idx),
//                                    Range(y_train.begin(), y_train.begin() + idx));
//        node->left = fit_individual(Range(x_train.begin() + idx, x_train.end()),
//                                    Range(y_train.begin() + idx, y_train.end()));
    }
};

#endif //GRADIENTBOOSTING_DECISION_TREE_HPP
