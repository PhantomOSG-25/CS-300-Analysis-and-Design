#pragma once

#include <string>
#include <vector>

namespace course_planner {

struct Course {
  std::string id;
  std::string title;
  std::vector<std::string> prerequisites;

  bool operator==(const Course&) const = default;
};

}  // namespace course_planner
