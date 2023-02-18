#pragma once

#include <functional>
#include <vector>

template <class T, class Container = std::vector<T>, class Compare = std::less<typename Container::value_type>>
class PriorityQueue {
public:
    PriorityQueue() {
    }

    void Push(const T& value) {
        data_.push_back(value);
        size_t index = data_.size() - 1;
        while (index > 0 && !Compare()(data_[Ancestor(index)], data_[index])) {
            std::swap(data_[index], data_[Ancestor(index)]);
            index = Ancestor(index);
        }
    }

    T Top() const {
        return data_[0];
    }

    void Pop() {
        size_t index = 0;
        std::swap(data_[index], data_.back());
        data_.pop_back();
        while (LeftChild(index) < data_.size()) {
            size_t right_child = RightChild(index);
            size_t left_child = LeftChild(index);
            if (right_child < data_.size() && Compare()(data_[right_child], data_[left_child]) &&
                Compare()(data_[right_child], data_[index])) {
                std::swap(data_[index], data_[right_child]);
                index = RightChild(index);
            } else if (Compare()(data_[left_child], data_[index])) {
                std::swap(data_[index], data_[left_child]);
                index = LeftChild(index);
            } else {
                break;
            }
        }
    }

    size_t Size() const {
        return data_.size();
    }

private:
    Container data_;
    size_t Ancestor(size_t index) {
        return (index + 1) / 2 - 1;
    }
    size_t LeftChild(size_t index) {
        return index * 2 + 1;
    }
    size_t RightChild(size_t index) {
        return index * 2 + 2;
    }
};
