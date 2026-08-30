#pragma once

#include "course_planner/binary_search_tree.hpp"

#include <cstddef>
#include <istream>
#include <string>
#include <vector>

namespace course_planner {

struct LoadError {
  std::size_t line;
  std::string message;
};

struct LoadResult {
  std::size_t courses_loaded{0};
  std::vector<LoadError> errors;
  [[nodiscard]] bool ok() const noexcept { return errors.empty(); }
};

[[nodiscard]] LoadResult load_courses(std::istream& input, BinarySearchTree& destination);
[[nodiscard]] std::string normalize_course_id(std::string value);

}  // namespace course_planner
