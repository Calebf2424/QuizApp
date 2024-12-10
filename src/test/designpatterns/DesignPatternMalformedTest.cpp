#include "DesignPatternMalformedTest.hpp"

#include <iostream>
#include <memory>

#include "../../controller/DesignPatternController.hpp"
#include "../app/MyApiTestClient.hpp"
#include "../app/TestComponent.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

void DesignPatternMalformedTest::onRun() {
  // Register test components
  TestComponent component;

  // Create client-server test runner
  oatpp::test::web::ClientServerTestRunner runner;

  // Get object mapper component
  OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                  objectMapper);

  // Add DesignPatternController endpoints to the router of the test server
  runner.addController(std::make_shared<DesignPatternController>());

  // Run test
  runner.run(
      [this, &runner] {
        // Get client connection provider for API Client
        OATPP_COMPONENT(
            std::shared_ptr<oatpp::network::ClientConnectionProvider>,
            clientConnectionProvider);

        // Get object mapper component
        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                        objectMapper);

        // Create HTTP request executor for API Client
        auto requestExecutor =
            oatpp::web::client::HttpRequestExecutor::createShared(
                clientConnectionProvider);

        // Create Test API client
        auto client =
            MyApiTestClient::createShared(requestExecutor, objectMapper);

        // Test Multiple Choice Answer - Malformed Test - Design Patterns
        {
          oatpp::String line = "a";  // Example line number
          oatpp::Int32 guess = 2;    // Example guess

          auto response = client->getDesignPatternAnswer(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 400);

          auto answer = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Invalid Entry");
        }  // End of Test #1

        // Test True/False Answers Correct - Design Patterns - Test #5
        {
          oatpp::String line = "a";  // Example line number
          oatpp::Int32 guess = 1;    // Example guess (1 for True, 0 for False)

          auto response = client->tfDesignPatternAnswers(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 400);

          auto answer = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Invalid Entry");
        }  // End of Test #5

        // Test Short Answer Answers Incorrect - Design Patterns - Test #8
        {
          oatpp::String line = "a";       // Example line number
          oatpp::String guess = "Answer";  // Example guess

          auto response = client->saDesignPatternAnswers(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 400);

          auto answer = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Invalid Entry");
        }  // End of Test #8

        // Test POST Matching Answers Malformed - Design Patterns
        {
          auto matchingAnswers = MatchingAnswersDTO::createShared();
          matchingAnswers->line = "a";
          matchingAnswers->answerOne = "Answer1";
          matchingAnswers->answerTwo = "Answer2";
          matchingAnswers->answerThree = "Answer3";
          matchingAnswers->answerFour = "Answer4";

          auto response = client->maDesignPatternAnswers(matchingAnswers);
          OATPP_ASSERT(response->getStatusCode() == 400);

          auto answerMessage = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answerMessage);
          OATPP_ASSERT(answerMessage->result == "Invalid Entry");
        }
      },
      std::chrono::minutes(10) /* test timeout */);

  // Wait for all server threads to finish
  std::this_thread::sleep_for(std::chrono::seconds(1));
}
