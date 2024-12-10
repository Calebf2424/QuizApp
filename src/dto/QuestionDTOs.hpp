#ifndef SHAPE_DTO_hpp
#define SHAPE_DTO_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "OptimusPrimeDTOs.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class mcGetResult : public oatpp::DTO {
  DTO_INIT(mcGetResult, DTO)

  DTO_FIELD(String, question);
  DTO_FIELD(String, a);
  DTO_FIELD(String, b);
  DTO_FIELD(String, c);
  DTO_FIELD(String, d);
  DTO_FIELD(Int32, id);
};

class Answer : public oatpp::DTO {
  DTO_INIT(Answer, DTO)

  DTO_FIELD(String, result);
};

class tfGetQuestion : public oatpp::DTO {
  DTO_INIT(tfGetQuestion, DTO)

  DTO_FIELD(String, question);
  DTO_FIELD(String, a);
  DTO_FIELD(String, b);
  DTO_FIELD(Int32, id);
};

class saGetQuestion : public oatpp::DTO {
  DTO_INIT(saGetQuestion, DTO)

  DTO_FIELD(String, question);
  DTO_FIELD(Int32, id);
};

class MatchingQuestionDTO : public oatpp::DTO {
  DTO_INIT(MatchingQuestionDTO, DTO)

  DTO_FIELD(String, question);
  DTO_FIELD(List<String>, leftItems);
  DTO_FIELD(List<String>, rightItems);
  DTO_FIELD(Int32, id);
};

class MatchingAnswersDTO : public oatpp::DTO {
  DTO_INIT(MatchingAnswersDTO, DTO)

  DTO_FIELD(String, line);
  DTO_FIELD(String, answerOne);
  DTO_FIELD(String, answerTwo);
  DTO_FIELD(String, answerThree);
  DTO_FIELD(String, answerFour);
};

#include OATPP_CODEGEN_END(DTO)

#endif  // SHAPE_DTO_hpp
