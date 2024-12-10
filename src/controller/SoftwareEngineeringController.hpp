#ifndef SoftwareController_hpp
#define SoftwareController_hpp

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

class SoftwareEngineeringController
    : public oatpp::web::server::api::ApiController {
 public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize
   * DTOs.
   */
  SoftwareEngineeringController(
      OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper) {}

  ADD_CORS(getSoftware)
  ENDPOINT("GET", "/optimusprime/software/mc", getSoftware) {
    auto dto = mcGetResult::createShared();
    DesignPatternParser parser("data/se_mcQuestions.txt");
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

  ADD_CORS(getSoftwareAnswer)
  ENDPOINT("GET",
           "/optimusprime/software/mcAnswers",
           getSoftwareAnswer,
           QUERY(String, line),
           QUERY(Int32, guess)) {
    auto dto = Answer::createShared();
    std::string filePath = "data/se_mcAnswers.txt";
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

  ADD_CORS(tfSoftware)
  ENDPOINT("GET", "/optimusprime/software/tf", tfSoftware) {
    auto dto = tfGetQuestion::createShared();
    DesignPatternParser parser("data/se_tfQuestions.txt");
    int targetLine = 1 + (std::rand() % 1);
    auto questionData = parser.parseTFQuestion(targetLine);

    dto->question = questionData.question;
    dto->a = questionData.optionA;
    dto->b = questionData.optionB;
    dto->id = targetLine;
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(tfSoftwareAnswers)
  ENDPOINT("GET",
           "/optimusprime/software/tfAnswers",
           tfSoftwareAnswers,
           QUERY(String, line),
           QUERY(Int32, guess)) {
    auto dto = Answer::createShared();
    std::string filePath = "data/se_tfAnswers.txt";
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

  ADD_CORS(saSoftware)
  ENDPOINT("GET", "/optimusprime/software/shortAnswer", saSoftware) {
    auto dto = saGetQuestion::createShared();
    DesignPatternParser parser("data/se_saQuestions.txt");
    int targetLine = 1 + (std::rand() % 1);
    auto questionData = parser.parseSAQuestion(targetLine);

    dto->question = questionData.question;
    dto->id = targetLine;
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(saSoftwareAnswers)
  ENDPOINT("GET",
           "/optimusprime/software/saAnswers",
           saSoftwareAnswers,
           QUERY(String, line),
           QUERY(String, guess)) {
    auto dto = Answer::createShared();
    std::string filePath = "data/se_saAnswers.txt";
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

  ADD_CORS(matchingSoftware)
  ENDPOINT("GET", "/optimusprime/software/matching", matchingSoftware) {
    auto dto = MatchingQuestionDTO::createShared();
    std::string dataFilePath = "data/se_maQuestions.txt";
    DesignPatternParser parser(dataFilePath);

    int targetLine = 1 + (std::rand() % 2);
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

  ADD_CORS(maSoftwareAnswers)
  ENDPOINT("POST",
           "/optimusprime/software/maAnswers",
           maSoftwareAnswers,
           BODY_DTO(Object<MatchingAnswersDTO>, request)) {
    std::string userAnswerOne = request->answerOne;
    std::string userAnswerTwo = request->answerTwo;
    std::string userAnswerThree = request->answerThree;
    std::string userAnswerFour = request->answerFour;
    std::string lineNumber = request->line;

    auto dto = Answer::createShared();
    std::string filePath = "data/se_maAnswers.txt";
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

#endif  // SoftwareController_hpp
