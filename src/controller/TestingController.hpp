#ifndef TestingController_hpp
#define TestingController_hpp

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

#include OATPP_CODEGEN_BEGIN(ApiController)

class TestingController : public oatpp::web::server::api::ApiController {
 public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize
   * DTOs.
   */
  TestingController(
      OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper) {}

  ADD_CORS(getTesting)
  ENDPOINT("GET", "/optimusprime/testing/mc", getTesting) {
    auto dto = mcGetResult::createShared();
    DesignPatternParser parser("data/test_mcQuestions.txt");
    int target = 1 + (std::rand() % 2);
    auto questionData = parser.parseMCQuestion(target);

    dto->question = questionData.question;
    dto->a = questionData.optionA;
    dto->b = questionData.optionB;
    dto->c = questionData.optionC;
    dto->d = questionData.optionD;
    dto->id = target;
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(getTestingAnswer)
  ENDPOINT("GET",
           "/optimusprime/testing/mcAnswers",
           getTestingAnswer,
           QUERY(String, line),
           QUERY(Int32, guess)) {
    auto dto = Answer::createShared();
    std::string filePath = "data/test_mcAnswers.txt";
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
            dto->result = "Incorrect the correct answer is " + lineContent;
          }
          return createDtoResponse(Status::CODE_200, dto);
        }
      }
    }
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(tfTesting)
  ENDPOINT("GET", "/optimusprime/testing/tf", tfTesting) {
    auto dto = tfGetQuestion::createShared();
    DesignPatternParser parser("data/test_tfQuestions.txt");
    int targetLine = 1 + (std::rand() % 1);
    auto questionData = parser.parseTFQuestion(targetLine);

    dto->question = questionData.question;
    dto->a = questionData.optionA;
    dto->b = questionData.optionB;
    dto->id = targetLine;
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(tfTestingAnswers)
  ENDPOINT("GET",
           "/optimusprime/testing/tfAnswers",
           tfTestingAnswers,
           QUERY(String, line),
           QUERY(Int32, guess)) {
    auto dto = Answer::createShared();
    std::string filePath = "data/test_tfAnswers.txt";
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
            dto->result = "Incorrect the correct answer is " + lineContent;
          }
          return createDtoResponse(Status::CODE_200, dto);
        }
      }
    }
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(saTesting)
  ENDPOINT("GET", "/optimusprime/testing/shortAnswer", saTesting) {
    auto dto = saGetQuestion::createShared();
    DesignPatternParser parser("data/test_saQuestions.txt");
    int targetLine = 1 + (std::rand() % 1);
    auto questionData = parser.parseSAQuestion(targetLine);

    dto->question = questionData.question;
    dto->id = targetLine;
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(saTestingAnswers)
  ENDPOINT("GET",
           "/optimusprime/testing/saAnswers",
           saTestingAnswers,
           QUERY(String, line),
           QUERY(String, guess)) {
    auto dto = Answer::createShared();
    std::string filePath = "data/test_saAnswers.txt";
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
            dto->result = "Incorrect the correct answer is " + lineContent;
          }
          return createDtoResponse(Status::CODE_200, dto);
        }
      }
    }
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(matchingTesting)
  ENDPOINT("GET", "/optimusprime/testing/matching", matchingTesting) {
    auto dto = MatchingQuestionDTO::createShared();
    std::string dataFilePath = "data/test_maQuestions.txt";
    DesignPatternParser parser(dataFilePath);

    int targetLine = 1 + (std::rand() % 1);
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

  ADD_CORS(maTestingAnswers)
  ENDPOINT("POST",
           "/optimusprime/testing/maAnswers",
           maTestingAnswers,
           BODY_DTO(Object<MatchingAnswersDTO>, request)) {
    std::string userAnswerOne = request->answerOne;
    std::string userAnswerTwo = request->answerTwo;
    std::string userAnswerThree = request->answerThree;
    std::string userAnswerFour = request->answerFour;
    std::string lineNumber = request->line;

    auto dto = Answer::createShared();
    std::string filePath = "data/test_maAnswers.txt";
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
            dto->result = "Incorrect the correct answer is " + lineContent;
          }
          return createDtoResponse(Status::CODE_200, dto);
        }
      }
    }
    return createDtoResponse(Status::CODE_200, dto);
  }
};

#include OATPP_CODEGEN_END(ApiController)

#endif  // TestingController_hpp
