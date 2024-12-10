#ifndef OPController_hpp
#define OPController_hpp

#include <memory>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>

#include "../service/parseMC.hpp"
#include "../dto/OptimusPrimeDTOs.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)  // Begin Codegen

class OptimusPrimeController : public oatpp::web::server::api::ApiController {
 public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  OptimusPrimeController(
      OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper) {}

  ADD_CORS(getServices)  // Needed to allow for cross-domain access
  ENDPOINT("GET", "/optimusprime", getServices) {
    auto dto = GetServices::createShared();
    std::ifstream file("data/services.txt");
    std::string line;
    auto servicesList = oatpp::List<oatpp::String>::createShared();

    while (std::getline(file, line)) {
      servicesList->push_back(line.c_str());
    }
    file.close();

    dto->services = servicesList;
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(getRandomQuestion)  // Needed to allow for cross-domain access
  ENDPOINT("GET", "/optimusprime/randomQuestion", getRandomQuestion) {
    auto dto = GetRandomQuestion::createShared();
    std::string topic;
    std::string type;
    int Topictarget = 1 + (std::rand() % 4);
    int Typetarget = 1 + (std::rand() % 4);

    switch (Topictarget) {
      case 1:
        topic = "versioncontrol";
        break;
      case 2:
        topic = "testing";
        break;
      case 3:
        topic = "designpattern";
        break;
      case 4:
        topic = "software";
        break;
    }

    switch (Typetarget) {
      case 1:
        type = "mc";
        break;
      case 2:
        type = "tf";
        break;
      case 3:
        type = "shortAnswer";
        break;
      case 4:
        type = "matching";
        break;
    }

    dto->link = "http://localhost:8400/optimusprime/" + topic + "/" + type;
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(getSpecificQuestion)  // Needed to allow for cross-domain access
  ENDPOINT("GET",
           "/optimusprime/question",
           getSpecificQuestion,
           QUERY(String, topic),
           QUERY(String, type)) {
    auto dto = GetRandomQuestion::createShared();
    dto->link = "http://localhost:8400/optimusprime/" + topic + "/" + type;
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(getSpecificTopic)  // Needed to allow for cross-domain access
  ENDPOINT("GET",
           "/optimusprime/questionTopic",
           getSpecificTopic,
           QUERY(String, topic)) {
    auto dto = GetRandomQuestion::createShared();
    std::string type;
    int Typetarget = 1 + (std::rand() % 4);

    switch (Typetarget) {
      case 1:
        type = "mc";
        break;
      case 2:
        type = "tf";
        break;
      case 3:
        type = "shortAnswer";
        break;
      case 4:
        type = "matching";
        break;
    }

    dto->link = "http://localhost:8400/optimusprime/" + topic + "/" + type;
    return createDtoResponse(Status::CODE_200, dto);
  }

  ADD_CORS(getSpecificType)  // Needed to allow for cross-domain access
  ENDPOINT("GET",
           "/optimusprime/questionType",
           getSpecificType,
           QUERY(String, type)) {
    auto dto = GetRandomQuestion::createShared();
    std::string topic;
    int Topictarget = 1 + (std::rand() % 4);

    switch (Topictarget) {
      case 1:
        topic = "versioncontrol";
        break;
      case 2:
        topic = "designpattern";
        break;
      case 3:
        topic = "software";
        break;
      case 4:
        topic = "testing";
        break;
    }

    dto->link = "http://localhost:8400/optimusprime/" + topic + "/" + type;
    return createDtoResponse(Status::CODE_200, dto);
  }
};  // End of OptimusPrimeController

#include OATPP_CODEGEN_END(ApiController)  // End Codegen

#endif  // OPController_hpp
