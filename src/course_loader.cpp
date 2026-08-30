#include "course_planner/course_loader.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <unordered_set>
#include <utility>

namespace course_planner {
namespace {

std::string trim(std::string value) {
  const auto not_space = [](unsigned char character) { return !std::isspace(character); };
  const auto first = std::find_if(value.begin(), value.end(), not_space);
  const auto last = std::find_if(value.rbegin(), value.rend(), not_space).base();
  return first < last ? std::string(first, last) : std::string{};
}

std::vector<std::string> split_csv_line(const std::string& line) {
  std::vector<std::string> fields;
  std::stringstream stream(line);
  std::string field;
  while (std::getline(stream, field, ',')) {
    fields.push_back(trim(std::move(field)));
  }
  if (!line.empty() && line.back() == ',') {
    fields.emplace_back();
  }
  return fields;
}

}  // namespace

std::string normalize_course_id(std::string value) {
  value = trim(std::move(value));
  std::transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
    return static_cast<char>(std::toupper(character));
  });
  return value;
}

LoadResult load_courses(std::istream& input, BinarySearchTree& destination) {
  LoadResult result;
  std::vector<std::pair<std::size_t, Course>> parsed;
  std::unordered_set<std::string> identifiers;
  std::string line;
  std::size_t line_number = 0;

  while (std::getline(input, line)) {
    ++line_number;
    if (trim(line).empty()) {
      continue;
    }
    auto fields = split_csv_line(line);
    if (fields.size() < 2 || fields[0].empty() || fields[1].empty()) {
      result.errors.push_back({line_number, "expected a non-empty course ID and title"});
      continue;
    }

    Course course{normalize_course_id(fields[0]), fields[1], {}};
    if (!identifiers.insert(course.id).second) {
      result.errors.push_back({line_number, "duplicate course ID: " + course.id});
      continue;
    }
    for (std::size_t index = 2; index < fields.size(); ++index) {
      const auto prerequisite = normalize_course_id(fields[index]);
      if (prerequisite.empty()) {
        result.errors.push_back({line_number, "prerequisite IDs cannot be empty"});
      } else if (prerequisite == course.id) {
        result.errors.push_back({line_number, "a course cannot require itself"});
      } else {
        course.prerequisites.push_back(prerequisite);
      }
    }
    parsed.emplace_back(line_number, std::move(course));
  }

  for (const auto& [source_line, course] : parsed) {
    for (const auto& prerequisite : course.prerequisites) {
      if (!identifiers.contains(prerequisite)) {
        result.errors.push_back({source_line, "unknown prerequisite " + prerequisite + " for " + course.id});
      }
    }
  }
  if (!result.errors.empty()) {
    return result;
  }

  BinarySearchTree replacement;
  for (auto& [source_line, course] : parsed) {
    (void)source_line;
    if (!replacement.insert(std::move(course))) {
      result.errors.push_back({0, "unexpected duplicate while building the catalog"});
      return result;
    }
  }
  result.courses_loaded = replacement.size();
  destination = std::move(replacement);
  return result;
}

}  // namespace course_planner
