#include "course_planner/binary_search_tree.hpp"

#include <utility>

namespace course_planner {

bool BinarySearchTree::insert(Course course) {
  auto* link = &root_;
  while (*link) {
    if (course.id == (*link)->course.id) {
      return false;
    }
    link = course.id < (*link)->course.id ? &(*link)->left : &(*link)->right;
  }
  *link = std::make_unique<Node>(std::move(course));
  ++size_;
  return true;
}

const Course* BinarySearchTree::find(std::string_view id) const noexcept {
  const Node* current = root_.get();
  while (current) {
    if (id == current->course.id) {
      return &current->course;
    }
    current = id < current->course.id ? current->left.get() : current->right.get();
  }
  return nullptr;
}

std::vector<Course> BinarySearchTree::in_order() const {
  std::vector<Course> courses;
  courses.reserve(size_);
  append_in_order(root_.get(), courses);
  return courses;
}

void BinarySearchTree::append_in_order(const Node* node, std::vector<Course>& courses) {
  if (!node) {
    return;
  }
  append_in_order(node->left.get(), courses);
  courses.push_back(node->course);
  append_in_order(node->right.get(), courses);
}

}  // namespace course_planner
