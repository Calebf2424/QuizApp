#ifndef MALFORMED_H
#define MALFORMED_H

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>

class findMalformed {
 public:
  bool isNotNumber(const std::string& str) {
    try {
      size_t pos;
      std::stod(str, &pos);
      return pos != str.length();
    } catch (const std::invalid_argument&) {
      return true;
    } catch (const std::out_of_range&) {
      return true;
    }
    return false;
  }

  bool validGuess(int guess, int min, int max) {
    return !(guess >= min && guess <= max);
  }

  bool fileError(const std::string& filePath) {
    std::ifstream file(filePath);
    return !file.is_open();
  }
};

#endif  // MALFORMED_H
