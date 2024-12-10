#ifndef DESIGN_PATTERN_PARSER_H
#define DESIGN_PATTERN_PARSER_H

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>

// Structure to hold parsed multiple-choice question data
struct QuestionData {
  std::string question;
  std::string optionA;
  std::string optionB;
  std::string optionC;
  std::string optionD;
};

// Structure to hold parsed true/false question data
struct TFQuestionData {
  std::string question;
  std::string optionA;
  std::string optionB;
};

// Structure to hold parsed short answer question data
struct SAQuestionData {
  std::string question;
};

// Structure to hold parsed matching question data
struct MatchingQuestionData {
  std::string question;
  std::vector<std::string> leftItems;
  std::vector<std::string> rightItems;
};

// Class to handle question parsing
class DesignPatternParser {
 private:
  std::string filePath;

  // Helper function to split a line into fields
  std::vector<std::string> splitLine(const std::string& line, char delimiter,
                                     int expectedFields) const {
    std::vector<std::string> fields;
    size_t start = 0;
    size_t end = line.find(delimiter);

    while (end != std::string::npos) {
      fields.push_back(line.substr(start, end - start));
      start = end + 1;
      end = line.find(delimiter, start);
    }

    fields.push_back(line.substr(start));  // Add the last field

    if (fields.size() != expectedFields) {
      throw std::runtime_error("Malformed line: incorrect number of fields");
    }

    return fields;
  }

 public:
  // Constructor to set file path
  explicit DesignPatternParser(const std::string& filePath) : filePath(filePath) {}

  // Function to parse multiple-choice question
  QuestionData parseMCQuestion(int targetLine) const {
    std::ifstream file(filePath);
    std::string line;
    int currentLine = 0;

    while (std::getline(file, line)) {
      if (++currentLine == targetLine) {
        auto fields = splitLine(line, '|', 5);  // Expect 5 fields for MC
        return QuestionData{fields[0], fields[1], fields[2], fields[3], fields[4]};
      }
    }

    throw std::runtime_error("Line number out of range in file: " + filePath);
  }

  // Function to parse true/false question
  TFQuestionData parseTFQuestion(int targetLine) const {
    std::ifstream file(filePath);
    std::string line;
    int currentLine = 0;

    while (std::getline(file, line)) {
      if (++currentLine == targetLine) {
        auto fields = splitLine(line, '|', 3);  // Expect 3 fields for TF
        return TFQuestionData{fields[0], fields[1], fields[2]};
      }
    }

    throw std::runtime_error("Line number out of range in file: " + filePath);
  }

  // Function to parse short answer question
  SAQuestionData parseSAQuestion(int targetLine) const {
    std::ifstream file(filePath);
    std::string line;
    int currentLine = 0;

    while (std::getline(file, line)) {
      if (++currentLine == targetLine) {
        auto fields = splitLine(line, '|', 1);  // Expect 1 field for SA
        return SAQuestionData{fields[0]};
      }
    }

    throw std::runtime_error("Line number out of range in file: " + filePath);
  }

  // Helper function to split a string by a delimiter
  std::vector<std::string> splitString(const std::string& s, char delimiter) const {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != std::string::npos) {
      tokens.push_back(s.substr(start, end - start));
      start = end + 1;
      end = s.find(delimiter, start);
    }

    tokens.push_back(s.substr(start));  // Add the last token
    return tokens;
  }

  // Function to parse matching question
  MatchingQuestionData parseMatchingQuestion(int targetLine) const {
    std::ifstream file(filePath);
    std::string line;
    int currentLine = 0;

    while (std::getline(file, line)) {
      if (++currentLine == targetLine) {
        auto fields = splitLine(line, '|', 3);  // Expect 3 fields
        MatchingQuestionData data;
        data.question = fields[0];
        data.leftItems = splitString(fields[1], ',');   // Split left items
        data.rightItems = splitString(fields[2], ',');  // Split right items
        return data;
      }
    }

    throw std::runtime_error("Line number out of range in file: " + filePath);
  }
};

#endif  // DESIGN_PATTERN_PARSER_H
