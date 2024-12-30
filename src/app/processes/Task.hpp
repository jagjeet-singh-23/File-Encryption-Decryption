// Task.hpp
#ifndef TASK_HPP
#define TASK_HPP

#include "../fileHandling/IO.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

enum class Action { ENCRYPT, DECRYPT };

struct Task {
  std::string filePath;
  std::fstream f_stream;
  Action action;

  Task(std::fstream &&stream, Action act, std::string filePath)
      : f_stream(std::move(stream)), action(act),
        filePath(std::move(filePath)) {}

  std::string toString() const {
    std::ostringstream oss;
    oss << filePath << ","
        << (action == Action::ENCRYPT ? "ENCRYPT" : "DECRYPT");
    return oss.str();
  }

  static Task fromString(const std::string &taskData) {
    std::istringstream iss(taskData);
    std::string filePath;
    std::string actionStr;

    if (std::getline(iss, filePath, ',') && std::getline(iss, actionStr)) {
      Action action;
      if (actionStr == "ENCRYPT") {
        action = Action::ENCRYPT;
      } else if (actionStr == "DECRYPT") {
        action = Action::DECRYPT;
      } else {
        throw std::runtime_error("Invalid action: " + actionStr);
      }

      IO io(filePath);
      std::fstream f_stream = std::move(io.getFileStream());
      if (f_stream.is_open()) {
        return Task(std::move(f_stream), action, std::move(filePath));
      } else {
        throw std::runtime_error("Failed to open file: " + filePath);
      }
    } else {
      throw std::runtime_error("Invalid task data format");
    }
  }
};

#endif
