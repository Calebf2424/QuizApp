#ifndef Controller_hpp
#define Controller_hpp

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "../dto/QuestionDTOs.hpp"
#include "../service/findMalformed.hpp"
#include "../service/parseMC.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)  // Begin Codegen

class DesignPatternController : public oatpp::web::server::api::ApiController {
 public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize
   * DTOs.
   */
  DesignPatternController(
      OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper) {}

  ADD_CORS(getDesignPattern)  // Allow cross-domain access
  ENDPOINT("GET", "/optimusprime/designpattern/mc", getDesignPattern) {
    auto dto = mcGetResult::createShared();
    DesignPatternParser parser("data/dp_mcQuestions.txt");
    int targetLine = 1 + (std::rand() % 2);  // Generate random line
    auto questionData = parser.parseMCQuestion(targetLine);

    dto->question = questionData.question;
    dto->a = questionData.optionA;
    dto->b = questionData.optionB;
    dto->c = questionData.optionC;
    dto->d = questionData.optionD;
    dto->id = targetLine;
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(getDesignPatternAnswer)  // Allow cross-domain access
  ENDPOINT("GET",
           "/optimusprime/designpattern/mcAnswers",
           getDesignPatternAnswer,
           QUERY(String, line),
           QUERY(Int32, guess)) {
    auto dto = Answer::createShared();
    std::string filePath = "data/dp_mcAnswers.txt";
    findMalformed errorChecker;

    if (errorChecker.isNotNumber(line) || errorChecker.fileError(filePath) ||
        errorChecker.validGuess(guess, 1, 4)) {
      dto->result = "Invalid Entry";
      return createDtoResponse(Status::CODE_400, dto);
    } else {
      int targetLine = std::stoi(line);
      std::ifstream file(filePath);
      std::string lineContent;
      int currentLine = 0;

      while (std::getline(file, lineContent)) {
        if (++currentLine == targetLine) {
          if (std::stoi(lineContent) == guess) {
            dto->result = "Correct";
          } else {
            switch (std::stoi(lineContent)) {
              case 1: lineContent = "A"; break;
              case 2: lineContent = "B"; break;
              case 3: lineContent = "C"; break;
              case 4: lineContent = "D"; break;
            }
            dto->result = "Incorrect - the correct answer is: " + lineContent;
          }
          return createDtoResponse(Status::CODE_200, dto);
        }
      }
      dto->result = "Incorrect - the correct answer is: " + lineContent;
      return createDtoResponse(Status::CODE_200, dto);
    }
  }

  ADD_CORS(tfDesignPattern)  // Allow cross-domain access
  ENDPOINT("GET", "/optimusprime/designpattern/tf", tfDesignPattern) {
    auto dto = tfGetQuestion::createShared();
    DesignPatternParser parser("data/dp_tfQuestions.txt");
    int targetLine = 1 + (std::rand() % 1);  // Generate random line
    auto questionData = parser.parseTFQuestion(targetLine);

    dto->question = questionData.question;
    dto->a = questionData.optionA;
    dto->b = questionData.optionB;
    dto->id = targetLine;
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(tfDesignPatternAnswers)  // Allow cross-domain access
  ENDPOINT("GET",
           "/optimusprime/designpattern/tfAnswers",
           tfDesignPatternAnswers,
           QUERY(String, line),
           QUERY(Int32, guess)) {
    auto dto = Answer::createShared();
    std::string filePath = "data/dp_tfAnswers.txt";
    findMalformed errorChecker;

    if (errorChecker.isNotNumber(line) || errorChecker.fileError(filePath) ||
        errorChecker.validGuess(guess, 1, 2)) {
      dto->result = "Invalid Entry";
      return createDtoResponse(Status::CODE_400, dto);
    } else {
      std::ifstream file(filePath);
      std::string lineContent;
      int currentLine = 0;

      while (std::getline(file, lineContent)) {
        if (++currentLine == std::stoi(line)) {
          if (std::stoi(lineContent) == guess) {
            dto->result = "Correct";
          } else {
            switch (std::stoi(lineContent)) {
              case 1: lineContent = "True"; break;
              case 2: lineContent = "False"; break;
            }
            dto->result = "Incorrect - the correct answer is: " + lineContent;
          }
          return createDtoResponse(Status::CODE_200, dto);
        }
      }
      dto->result = "Incorrect";
      return createDtoResponse(Status::CODE_200, dto);
    }
  }

  ADD_CORS(saDesignPattern)  // Allow cross-domain access
  ENDPOINT("GET", "/optimusprime/designpattern/shortAnswer", saDesignPattern) {
    auto dto = saGetQuestion::createShared();
    DesignPatternParser parser("data/dp_saQuestions.txt");
    int targetLine = 1 + (std::rand() % 1);  // Generate random line
    auto questionData = parser.parseSAQuestion(targetLine);

    dto->question = questionData.question;
    dto->id = targetLine;
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(saDesignPatternAnswers)  // Allow cross-domain access
  ENDPOINT("GET",
           "/optimusprime/designpattern/saAnswers",
           saDesignPatternAnswers,
           QUERY(String, line),
           QUERY(String, guess)) {
    auto dto = Answer::createShared();
    std::string filePath = "data/dp_saAnswers.txt";
    findMalformed errorChecker;

    if (errorChecker.isNotNumber(line) || errorChecker.fileError(filePath)) {
      dto->result = "Invalid Entry";
      return createDtoResponse(Status::CODE_400, dto);
    } else {
      std::ifstream file(filePath);
      std::string convertedString = guess->c_str();
      std::string lineContent;
      int currentLine = 0;

      while (std::getline(file, lineContent)) {
        if (++currentLine == std::stoi(line)) {
          if (lineContent == convertedString) {
            dto->result = "Correct";
          } else {
            dto->result =
                "Incorrect - the correct answer is: " + lineContent;
          }
          return createDtoResponse(Status::CODE_200, dto);
        }
      }
      dto->result = "Incorrect";
      return createDtoResponse(Status::CODE_200, dto);
    }
  }

  ADD_CORS(matchingDesignPattern)  // Allow cross-domain access
  ENDPOINT("GET",
           "/optimusprime/designpattern/matching",
           matchingDesignPattern) {
    auto dto = MatchingQuestionDTO::createShared();
    std::string dataFilePath = "data/dp_maQuestions.txt";
    DesignPatternParser parser(dataFilePath);

    int targetLine = 1 + (std::rand() % 2);  // Generate random line
    auto questionData = parser.parseMatchingQuestion(targetLine);

    dto->question = questionData.question;

    auto leftItems = oatpp::List<oatpp::String>::createShared();
    for (const auto& item : questionData.leftItems) {
      leftItems->push_back(item.c_str());
    }
    dto->leftItems = leftItems;

    auto rightItems = oatpp::List<oatpp::String>::createShared();
    for (const auto& item : questionData.rightItems) {
      rightItems->push_back(item.c_str());
    }
    dto->rightItems = rightItems;
    dto->id = targetLine;

    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(maDesignPatternAnswers)  // Allow cross-domain access
  ENDPOINT("POST",
           "/optimusprime/designpattern/maAnswers",
           maDesignPatternAnswers,
           BODY_DTO(Object<MatchingAnswersDTO>, request)) {
    std::string userAnswerOne = request->answerOne;
    std::string userAnswerTwo = request->answerTwo;
    std::string userAnswerThree = request->answerThree;
    std::string userAnswerFour = request->answerFour;
    std::string lineNumber = request->line;

    auto dto = Answer::createShared();
    std::string filePath = "data/dp_maAnswers.txt";
    findMalformed errorChecker;

    if (errorChecker.isNotNumber(lineNumber) ||
        errorChecker.fileError(filePath)) {
      dto->result = "Invalid Entry";
      return createDtoResponse(Status::CODE_400, dto);
    } else {
      std::ifstream file(filePath);
      std::string userAnswersCombined =
          userAnswerOne + " |" + userAnswerTwo + " |" + userAnswerThree +
          " |" + userAnswerFour;

      std::string lineContent;
      int currentLine = 0;

      while (std::getline(file, lineContent)) {
        if (++currentLine == std::stoi(lineNumber)) {
          if (lineContent == userAnswersCombined) {
            dto->result = "Correct";
          } else {
            dto->result =
                "Incorrect - the correct answer is: " + lineContent;
          }
          return createDtoResponse(Status::CODE_200, dto);
        }
      }
      dto->result = "Incorrect - the correct answer is: " + lineContent;
      return createDtoResponse(Status::CODE_200, dto);
    }
  }
};  // End of DesignPatternController

#include OATPP_CODEGEN_END(ApiController)  // End Codegen

#endif  // Controller_hpp
