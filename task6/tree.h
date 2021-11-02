#pragma once

#include <memory>

namespace bintree {
    template<typename T>
    struct TNode : std::enable_shared_from_this<TNode<T>> { // enables shared_from_this method
        using TNodePtr = std::shared_ptr<TNode<T>>;
        using TNodeConstPtr = std::shared_ptr<const TNode<T>>;
        using WeakParentPtr = std::weak_ptr<TNode<T>>; // for parent to be weak_ptr

        bool hasLeft() const {
            return bool(left);
        }

        bool hasRight() const {
            return bool(right);
        }

        bool hasParent() const {
            return parent.expired(); // weak_ptr check
        }

        T &getValue() {
            return value;
        }

        const T &getValue() const {
            return value;
        }

        TNodePtr getLeft() {
            return left;
        }

        TNodeConstPtr getLeft() const {
            return left;
        }

        TNodePtr getRight() {
            return right;
        }

        TNodeConstPtr getRight() const {
            return right;
        }

        TNodePtr getParent() {
            return parent.lock(); // return shared_ptr of the object or empty shared_ptr
        }

        TNodeConstPtr getParent() const {
            return parent.lock(); // return shared_ptr of the object or empty shared_ptr
        }

        static TNodePtr createLeaf(T v) {
            // make_shared needs a public constructor => call constructor inside
            return TNodePtr(new TNode(v));
        }

        static TNodePtr fork(T v, TNode *left, TNode *right) {
            // make_shared needs a public constructor => call constructor inside
            // ->shared_from_this() to match new TNodePtr init class fields
            TNodePtr ptr(new TNode(v, left->shared_from_this(), right->shared_from_this()));
            setParent(ptr->getLeft(), ptr);
            setParent(ptr->getRight(), ptr);
            return ptr;
        }

        TNodePtr replaceLeft(TNodePtr l) {
            setParent(l, this->shared_from_this()); // get shared_ptr, no double free
            std::swap(l, left);
            return l;
        }

        TNodePtr replaceRight(TNodePtr r) {
            setParent(r, this->shared_from_this()); // get shared_ptr, no double free
            std::swap(r, right);
            return r;
        }

        TNodePtr replaceRightWithLeaf(T v) {
            return replaceRight(createLeaf(v));
        }

        TNodePtr replaceLeftWithLeaf(T v) {
            return replaceLeft(createLeaf(v));
        }

        TNodePtr removeLeft() {
            return replaceLeft(nullptr);
        }

        TNodePtr removeRight() {
            return replaceRight(nullptr);
        }

    private:
        T value;
        TNodePtr left = nullptr;
        TNodePtr right = nullptr;
        WeakParentPtr parent = WeakParentPtr(); // weak_ptr => no cyclic references

        explicit TNode(T v)
                : value(v) {
        }

        TNode(T v, TNodePtr left, TNodePtr right) // TNode* -> TNodePtr in init class fields
                : value(v), left(left), right(right) {
        }

        static void setParent(TNodePtr node, WeakParentPtr parent) { // parent is weak_ptr
            if (node)
                node->parent = parent;
        }
    };
}
