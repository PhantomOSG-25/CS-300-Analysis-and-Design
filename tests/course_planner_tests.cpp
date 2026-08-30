#include "course_planner/course_loader.hpp"

#include <iostream>
#include <sstream>
#include <string>

namespace {
int failures = 0;

void check(bool condition, const std::string& message) {
  if (!condition) {
    std::cerr << "FAIL: " << message << '\n';
    ++failures;
  }
}

void tree_operations_are_ordered_and_unique() {
  course_planner::BinarySearchTree tree;
  check(tree.insert({"CS300", "Algorithms", {"CS200"}}), "first insert succeeds");
  check(tree.insert({"CS100", "Foundations", {}}), "lower insert succeeds");
  check(tree.insert({"CS400", "Advanced", {"CS300"}}), "higher insert succeeds");
  check(!tree.insert({"CS300", "Duplicate", {}}), "duplicate insert is rejected");
  check(tree.size() == 3, "tree size excludes duplicates");
  const auto ordered = tree.in_order();
  check(ordered[0].id == "CS100" && ordered[1].id == "CS300" && ordered[2].id == "CS400",
        "in-order traversal sorts IDs");
  check(tree.find("CS300") != nullptr, "existing course is found");
  check(tree.find("CS999") == nullptr, "missing course is absent");
}

void valid_input_replaces_the_catalog_atomically() {
  std::istringstream input(
      "cs100,Foundations\nCS200,Data Structures,CS100\nCS300,Algorithms,CS100,CS200\n");
  course_planner::BinarySearchTree tree;
  check(tree.insert({"OLD100", "Old data", {}}), "initial catalog setup succeeds");
  const auto result = course_planner::load_courses(input, tree);
  check(result.ok() && result.courses_loaded == 3, "valid input loads all courses");
  check(tree.find("CS300") != nullptr, "loaded course is searchable");
  check(tree.find("OLD100") == nullptr, "successful load replaces old data");
  check(tree.find("CS300")->prerequisites.size() == 2, "all prerequisites are retained");
}

void invalid_input_reports_lines_and_preserves_the_catalog() {
  std::istringstream input(
      "CS100,Foundations\nCS100,Duplicate\nCS200,Data Structures,CS999\nCS300,,CS100\n");
  course_planner::BinarySearchTree tree;
  check(tree.insert({"SAFE100", "Preserved", {}}), "preserved catalog setup succeeds");
  const auto result = course_planner::load_courses(input, tree);
  check(!result.ok(), "invalid input fails");
  check(result.errors.size() == 3, "duplicate, unknown prerequisite, and missing title are reported");
  check(tree.size() == 1 && tree.find("SAFE100") != nullptr, "failed load is atomic");
}

void self_and_empty_prerequisites_are_rejected() {
  std::istringstream input("CS100,Foundations,CS100,\n");
  course_planner::BinarySearchTree tree;
  const auto result = course_planner::load_courses(input, tree);
  check(result.errors.size() == 2, "self and empty prerequisites are both reported");
  check(tree.empty(), "invalid data does not change the catalog");
}
}  // namespace

int main() {
  tree_operations_are_ordered_and_unique();
  valid_input_replaces_the_catalog_atomically();
  invalid_input_reports_lines_and_preserves_the_catalog();
  self_and_empty_prerequisites_are_rejected();
  if (failures == 0) {
    std::cout << "All course planner tests passed.\n";
  }
  return failures == 0 ? 0 : 1;
}
