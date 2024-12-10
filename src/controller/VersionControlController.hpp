#ifndef VERSIONCONTROLLER_HPP
#define VERSIONCONTROLLER_HPP

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

class VersionControlController
    : public oatpp::web::server::api::ApiController {
 public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/
   * deserialize DTOs.
   */
  VersionControlController(
      OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper) {}

  ADD_CORS(getVersionControl)
  ENDPOINT("GET", "/optimusprime/versioncontrol/mc",
           getVersionControl) {
    auto dto = mcGetResult::createShared();
    DesignPatternParser parser("data/vc_mcQuestions.txt");
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

  ADD_CORS(getVersionControlAnswer)
  ENDPOINT("GET", "/optimusprime/versioncontrol/mcAnswers",
           getVersionControlAnswer, QUERY(String, line),
           QUERY(Int32, guess)) {
    auto dto = Answer::createShared();
    std::string filePath = "data/vc_mcAnswers.txt";
    findMalformed errorChecker;

    if (errorChecker.isNotNumber(line) ||
        errorChecker.fileError(filePath) ||
        errorChecker.validGuess(guess, 1, 4)) {
      dto->result = "Invalid Entry";
      return createDtoResponse(Status::CODE_400, dto);
    }

    int targetLine = std::stoi(line);
    std::ifstream file(filePath);

    std::string lineContent;
    int currentLine = 0;

    while (std::getline(file, lineContent)) {
      if (++currentLine == targetLine) {
        if (std::stoi(lineContent) == guess) {
          dto->result = "Correct";
          return createDtoResponse(Status::CODE_200, dto);
        } else {
          switch (std::stoi(lineContent)) {
            case 1:
              lineContent = "A";
              break;
            case 2:
              lineContent = "B";
              break;
            case 3:
              lineContent = "C";
              break;
            case 4:
              lineContent = "D";
              break;
          }
          dto->result =
              "Incorrect the correct answer is " + lineContent;
          return createDtoResponse(Status::CODE_200, dto);
        }
      }
    }

    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(tfVersionControl)
  ENDPOINT("GET", "/optimusprime/versioncontrol/tf",
           tfVersionControl) {
    auto dto = tfGetQuestion::createShared();
    DesignPatternParser parser("data/vc_tfQuestions.txt");
    int targetLine = 1 + (std::rand() % 1);
    auto questionData = parser.parseTFQuestion(targetLine);

    dto->question = questionData.question;
    dto->a = questionData.optionA;
    dto->b = questionData.optionB;
    dto->id = targetLine;

    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(tfVersionControlAnswers)
  ENDPOINT("GET", "/optimusprime/versioncontrol/tfAnswers",
           tfVersionControlAnswers, QUERY(String, line),
           QUERY(Int32, guess)) {
    auto dto = Answer::createShared();
    std::string filePath = "data/vc_tfAnswers.txt";
    findMalformed errorChecker;

    if (errorChecker.isNotNumber(line) ||
        errorChecker.fileError(filePath) ||
        errorChecker.validGuess(guess, 1, 2)) {
      dto->result = "Invalid Entry";
      return createDtoResponse(Status::CODE_400, dto);
    }

    std::ifstream file(filePath);
    std::string lineContent;
    int currentLine = 0;

    while (std::getline(file, lineContent)) {
      if (++currentLine == std::stoi(line)) {
        if (std::stoi(lineContent) == guess) {
          dto->result = "Correct";
          return createDtoResponse(Status::CODE_200, dto);
        } else {
          switch (std::stoi(lineContent)) {
            case 1:
              lineContent = "A";
              break;
            case 2:
              lineContent = "B";
              break;
          }
          dto->result =
              "Incorrect the correct answer is " + lineContent;
          return createDtoResponse(Status::CODE_200, dto);
        }
      }
    }

    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(saVersionControl)
  ENDPOINT("GET", "/optimusprime/versioncontrol/shortAnswer",
           saVersionControl) {
    auto dto = saGetQuestion::createShared();
    DesignPatternParser parser("data/vc_saQuestions.txt");
    int targetLine = 1 + (std::rand() % 1);
    auto questionData = parser.parseSAQuestion(targetLine);

    dto->question = questionData.question;
    dto->id = targetLine;

    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(saVersionControlAnswers)
  ENDPOINT("GET", "/optimusprime/versioncontrol/saAnswers",
           saVersionControlAnswers, QUERY(String, line),
           QUERY(String, guess)) {
    auto dto = Answer::createShared();
    std::string filePath = "data/vc_saAnswers.txt";
    std::ifstream file(filePath);
    findMalformed errorChecker;

    if (errorChecker.isNotNumber(line) ||
        errorChecker.fileError(filePath)) {
      dto->result = "Invalid Entry";
      return createDtoResponse(Status::CODE_400, dto);
    }

    std::string convertedString = guess->c_str();
    std::string lineContent;
    int currentLine = 0;

    while (std::getline(file, lineContent)) {
      if (++currentLine == std::stoi(line)) {
        if (lineContent == convertedString) {
          dto->result = "Correct";
          return createDtoResponse(Status::CODE_200, dto);
        } else {
          dto->result =
              "Incorrect the correct answer is " + lineContent;
          return createDtoResponse(Status::CODE_200, dto);
        }
      }
    }

    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(matchingVersionControl)
  ENDPOINT("GET", "/optimusprime/versioncontrol/matching",
          matchingVersionControl) {
    auto dto = MatchingQuestionDTO::createShared();
    std::string dataFilePath = "data/vc_maQuestions.txt";
    DesignPatternParser parser(dataFilePath);

    // Generate a random line number
    int targetLine = 1 + (std::rand() % 2);

    // Parse the matching question
    auto questionData = parser.parseMatchingQuestion(targetLine);

    // Populate the DTO fields
    dto->question = questionData.question;

    // Convert std::vector<std::string> to oatpp::List<oatpp::String>
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

  ADD_CORS(maVersionControlAnswers)
  ENDPOINT("POST", "/optimusprime/versioncontrol/maAnswers",
          maVersionControlAnswers,
          BODY_DTO(Object<MatchingAnswersDTO>, request)) {
    std::string userAnswerOne = request->answerOne;
    std::string userAnswerTwo = request->answerTwo;
    std::string userAnswerThree = request->answerThree;
    std::string userAnswerFour = request->answerFour;
    std::string lineNumber = request->line;

    auto dto = Answer::createShared();
    std::string filePath = "data/vc_maAnswers.txt";
    findMalformed errorChecker;

    if (errorChecker.isNotNumber(lineNumber) ||
        errorChecker.fileError(filePath)) {
      dto->result = "Invalid Entry";
      return createDtoResponse(Status::CODE_400, dto);
    }

    std::ifstream file(filePath);
    std::string userAnswersCombined =
        userAnswerOne + " |" + userAnswerTwo + " |" +
        userAnswerThree + " |" + userAnswerFour;

    std::string lineContent;
    int currentLine = 0;

    // Read the file line by line
    while (std::getline(file, lineContent)) {
      if (++currentLine == std::stoi(lineNumber)) {
        if (lineContent == userAnswersCombined) {
          dto->result = "Correct";
          return createDtoResponse(Status::CODE_200, dto);
        } else {
          dto->result =
              "Incorrect the correct answer is " + lineContent;
          return createDtoResponse(Status::CODE_200, dto);
        }
      }
    }
    return createDtoResponse(Status::CODE_200, dto);
  }
};

#include OATPP_CODEGEN_END(ApiController)

#endif  // VERSIONCONTROLLER_HPP
