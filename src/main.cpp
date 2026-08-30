#include "course_planner/course_loader.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <string>

namespace {

void print_course(const course_planner::Course& course) {
  std::cout << course.id << ": " << course.title << '\n';
  if (!course.prerequisites.empty()) {
    std::cout << "  Prerequisites: ";
    for (std::size_t index = 0; index < course.prerequisites.size(); ++index) {
      std::cout << (index == 0 ? "" : ", ") << course.prerequisites[index];
    }
    std::cout << '\n';
  }
}

bool load_file(const std::string& path, course_planner::BinarySearchTree& catalog) {
  std::ifstream input(path);
  if (!input) {
    std::cerr << "Unable to open course data: " << path << '\n';
    return false;
  }
  const auto result = course_planner::load_courses(input, catalog);
  if (!result.ok()) {
    std::cerr << "Course data was not loaded:\n";
    for (const auto& error : result.errors) {
      std::cerr << "  line " << error.line << ": " << error.message << '\n';
    }
    return false;
  }
  std::cout << "Loaded " << result.courses_loaded << " courses from " << path << ".\n";
  return true;
}

}  // namespace

int main(int argc, char* argv[]) {
  course_planner::BinarySearchTree catalog;
  if (argc > 2) {
    std::cerr << "Usage: course_planner [course-data.csv]\n";
    return 2;
  }
  if (argc == 2) {
    (void)load_file(argv[1], catalog);
  }

  std::cout << "Course Planner\n";
  while (true) {
    std::cout << "\n1. Load course data\n2. Print course list\n3. Find a course\n9. Exit\nChoice: ";
    int choice = 0;
    if (!(std::cin >> choice)) {
      if (std::cin.eof()) {
        std::cout << "\n";
        return 0;
      }
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Enter a numeric menu option.\n";
      continue;
    }

    if (choice == 9) {
      std::cout << "Goodbye.\n";
      return 0;
    }
    if (choice == 1) {
      std::cout << "Course data path: ";
      std::string path;
      std::cin >> path;
      (void)load_file(path, catalog);
    } else if (choice == 2) {
      if (catalog.empty()) {
        std::cout << "Load course data first.\n";
        continue;
      }
      for (const auto& course : catalog.in_order()) {
        std::cout << course.id << ": " << course.title << '\n';
      }
    } else if (choice == 3) {
      if (catalog.empty()) {
        std::cout << "Load course data first.\n";
        continue;
      }
      std::cout << "Course ID: ";
      std::string id;
      std::cin >> id;
      const auto* course = catalog.find(course_planner::normalize_course_id(id));
      if (course) {
        print_course(*course);
      } else {
        std::cout << "Course not found.\n";
      }
    } else {
      std::cout << "Choose 1, 2, 3, or 9.\n";
    }
  }
}
