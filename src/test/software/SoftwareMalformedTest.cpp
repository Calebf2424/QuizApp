#include "SoftwareMalformedTest.hpp"

#include <iostream>
#include <memory>

#include "../../controller/SoftwareEngineeringController.hpp"
#include "../app/MyApiTestClient.hpp"
#include "../app/TestComponent.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

void SoftwareMalformedTest::onRun() {
  // Register test components.
  TestComponent component;

  // Create client-server test runner.
  oatpp::test::web::ClientServerTestRunner runner;

  // Get object mapper component.
  OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                  objectMapper);

  // Add SoftwareController endpoints to the router of the server.
  runner.addController(std::make_shared<SoftwareEngineeringController>());

  // Run test.
  runner.run(
      [this, &runner] {
        // Get client connection provider for API client.
        OATPP_COMPONENT(
            std::shared_ptr<oatpp::network::ClientConnectionProvider>,
            clientConnectionProvider);

        // Get object mapper component.
        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                        objectMapper);

        // Create HTTP request executor for API client.
        auto requestExecutor =
            oatpp::web::client::HttpRequestExecutor::createShared(
                clientConnectionProvider);

        // Create Test API client.
        auto client = MyApiTestClient::createShared(requestExecutor,
                                                    objectMapper);

        // Test Multiple Choice Answer - Malformed Test - Software - Test #1.
        {
          oatpp::String line = "a";  // Example line number.
          oatpp::Int32 guess = 2;    // Example guess.

          auto response = client->getSoftwareAnswer(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 400);

          auto answer = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Invalid Entry");
        }

        // Test True/False Answers Correct - Software - Test #5.
        {
          oatpp::String line = "a";  // Example line number.
          oatpp::Int32 guess = 1;    // Example guess (1 for True, 0 for False).

          auto response = client->tfSoftwareAnswers(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 400);

          auto answer = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Invalid Entry");
        }

        // Test Short Answer Answers Incorrect - Software - Test #8.
        {
          oatpp::String line = "a";      // Example line number.
          oatpp::String guess = "Answer";  // Example guess.

          auto response = client->saSoftwareAnswers(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 400);

          auto answer = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Invalid Entry");
        }

        // Test Matching Answers - Malformed Test - Software.
        {
          auto matchingAnswers = MatchingAnswersDTO::createShared();
          matchingAnswers->line = "a";
          matchingAnswers->answerOne = "Answer1";
          matchingAnswers->answerTwo = "Answer2";
          matchingAnswers->answerThree = "Answer3";
          matchingAnswers->answerFour = "Answer4";

          // Test POST.
          auto response = client->maSoftwareAnswers(matchingAnswers);
          OATPP_ASSERT(response->getStatusCode() == 400);

          auto answerMessage = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answerMessage);
          OATPP_ASSERT(answerMessage->result == "Invalid Entry");
        }
      },
      std::chrono::minutes(10));  // Test timeout.

  // Wait for all server threads to finish.
  std::this_thread::sleep_for(std::chrono::seconds(1));
}
