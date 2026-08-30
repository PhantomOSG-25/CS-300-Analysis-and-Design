#pragma once

#include "course_planner/course.hpp"

#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>

namespace course_planner {

class BinarySearchTree {
 public:
  BinarySearchTree() = default;
  BinarySearchTree(BinarySearchTree&&) noexcept = default;
  BinarySearchTree& operator=(BinarySearchTree&&) noexcept = default;
  BinarySearchTree(const BinarySearchTree&) = delete;
  BinarySearchTree& operator=(const BinarySearchTree&) = delete;

  [[nodiscard]] bool insert(Course course);
  [[nodiscard]] const Course* find(std::string_view id) const noexcept;
  [[nodiscard]] std::vector<Course> in_order() const;
  [[nodiscard]] std::size_t size() const noexcept { return size_; }
  [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

 private:
  struct Node {
    explicit Node(Course value) : course(std::move(value)) {}
    Course course;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
  };

  static void append_in_order(const Node* node, std::vector<Course>& courses);

  std::unique_ptr<Node> root_;
  std::size_t size_{0};
};

}  // namespace course_planner
