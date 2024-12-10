#ifndef MyApiTestClient_hpp
#define MyApiTestClient_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/web/client/ApiClient.hpp"

/* Begin API client code generation */
#include OATPP_CODEGEN_BEGIN(ApiClient)

/**
 * Test API client.
 * Use this client to call application APIs.
 */
class MyApiTestClient : public oatpp::web::client::ApiClient {
  API_CLIENT_INIT(MyApiTestClient)

  // DesignPattern APIs
  API_CALL("GET", "/optimusprime/designpattern/mc", getDesignPattern)
  API_CALL("GET", "/optimusprime/designpattern/mcAnswers",
           getDesignPatternAnswer, QUERY(oatpp::String, line),
           QUERY(oatpp::Int32, guess))
  API_CALL("GET", "/optimusprime/designpattern/tf", tfDesignPattern)
  API_CALL("GET", "/optimusprime/designpattern/tfAnswers",
           tfDesignPatternAnswers, QUERY(oatpp::String, line),
           QUERY(oatpp::Int32, guess))
  API_CALL("GET", "/optimusprime/designpattern/shortAnswer", saDesignPattern)
  API_CALL("GET", "/optimusprime/designpattern/saAnswers",
           saDesignPatternAnswers, QUERY(oatpp::String, line),
           QUERY(oatpp::String, guess))
  API_CALL("GET", "/optimusprime/designpattern/matching", matchingDesignPattern)
  API_CALL("POST", "/optimusprime/designpattern/maAnswers",
           maDesignPatternAnswers, BODY_DTO(oatpp::Object<MatchingAnswersDTO>,
           body))

  // VersionControl APIs
  API_CALL("GET", "/optimusprime/versioncontrol/mc", getVersionControl)
  API_CALL("GET", "/optimusprime/versioncontrol/mcAnswers",
           getVersionControlAnswer, QUERY(oatpp::String, line),
           QUERY(oatpp::Int32, guess))
  API_CALL("GET", "/optimusprime/versioncontrol/tf", tfVersionControl)
  API_CALL("GET", "/optimusprime/versioncontrol/tfAnswers",
           tfVersionControlAnswers, QUERY(oatpp::String, line),
           QUERY(oatpp::Int32, guess))
  API_CALL("GET", "/optimusprime/versioncontrol/shortAnswer", saVersionControl)
  API_CALL("GET", "/optimusprime/versioncontrol/saAnswers",
           saVersionControlAnswers, QUERY(oatpp::String, line),
           QUERY(oatpp::String, guess))
  API_CALL("GET", "/optimusprime/versioncontrol/matching",
           matchingVersionControl)
  API_CALL("POST", "/optimusprime/versioncontrol/maAnswers",
           maVersionControlAnswers, BODY_DTO(oatpp::Object<MatchingAnswersDTO>,
           body))

  // Testing APIs
  API_CALL("GET", "/optimusprime/testing/mc", getTesting)
  API_CALL("GET", "/optimusprime/testing/mcAnswers", getTestingAnswer,
           QUERY(oatpp::String, line), QUERY(oatpp::Int32, guess))
  API_CALL("GET", "/optimusprime/testing/tf", tfTesting)
  API_CALL("GET", "/optimusprime/testing/tfAnswers", tfTestingAnswers,
           QUERY(oatpp::String, line), QUERY(oatpp::Int32, guess))
  API_CALL("GET", "/optimusprime/testing/shortAnswer", saTesting)
  API_CALL("GET", "/optimusprime/testing/saAnswers", saTestingAnswers,
           QUERY(oatpp::String, line), QUERY(oatpp::String, guess))
  API_CALL("GET", "/optimusprime/testing/matching", matchingTesting)
  API_CALL("POST", "/optimusprime/testing/maAnswers", maTestingAnswers,
           BODY_DTO(oatpp::Object<MatchingAnswersDTO>, body))

  // SoftwareEngineering APIs
  API_CALL("GET", "/optimusprime/software/mc", getSoftware)
  API_CALL("GET", "/optimusprime/software/mcAnswers", getSoftwareAnswer,
           QUERY(oatpp::String, line), QUERY(oatpp::Int32, guess))
  API_CALL("GET", "/optimusprime/software/tf", tfSoftware)
  API_CALL("GET", "/optimusprime/software/tfAnswers", tfSoftwareAnswers,
           QUERY(oatpp::String, line), QUERY(oatpp::Int32, guess))
  API_CALL("GET", "/optimusprime/software/shortAnswer", saSoftware)
  API_CALL("GET", "/optimusprime/software/saAnswers", saSoftwareAnswers,
           QUERY(oatpp::String, line), QUERY(oatpp::String, guess))
  API_CALL("GET", "/optimusprime/software/matching", matchingSoftware)
  API_CALL("POST", "/optimusprime/software/maAnswers", maSoftwareAnswers,
           BODY_DTO(oatpp::Object<MatchingAnswersDTO>, body))

  // OptimusPrime APIs
  API_CALL("GET", "/optimusprime", getServices)
  API_CALL("GET", "/optimusprime/randomQuestion", getRandomQuestion)
  API_CALL("GET",
           "/optimusprime/question",
           getSpecificQuestion,
           QUERY(String, topic),
           QUERY(String, type))
  API_CALL("GET",
           "/optimusprime/questionTopic",
           getSpecificTopic,
           QUERY(String, topic))
  API_CALL("GET",
           "/optimusprime/questionType",
           getSpecificType,
           QUERY(String, type))
};

/* End API client code generation */
#include OATPP_CODEGEN_END(ApiClient)

#endif  // MyApiTestClient_hpp
