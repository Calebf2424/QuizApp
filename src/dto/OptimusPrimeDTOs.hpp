#ifndef OPTPRIME_DTO_hpp
#define OPTPRIME_DTO_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "QuestionDTOs.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetServices : public oatpp::DTO {
  DTO_INIT(GetServices, DTO /* Extends */)

  DTO_FIELD(oatpp::List<oatpp::String>, services);
};

class GetRandomQuestion : public oatpp::DTO {
  DTO_INIT(GetRandomQuestion, DTO /* Extends */)

  DTO_FIELD(String, link);
};

#include OATPP_CODEGEN_END(DTO)

#endif  // OPTPRIME_DTO_hpp
