#include "VersionControlMalformedTest.hpp"

#include <iostream>
#include <memory>

#include "../../controller/VersionControlController.hpp"
#include "../app/MyApiTestClient.hpp"
#include "../app/TestComponent.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

void VersionControlMalformedTest::onRun() {
  // Register test components.
  TestComponent component;

  // Create client-server test runner.
  oatpp::test::web::ClientServerTestRunner runner;

  // Get object mapper component.
  OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                  objectMapper);

  // Add VersionControlController endpoints to the router of the test server.
  runner.addController(std::make_shared<VersionControlController>());

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

        // Create test API client.
        auto client = MyApiTestClient::createShared(requestExecutor,
                                                    objectMapper);

        // Test Multiple Choice Answer
        {
          oatpp::String line = "a";  // Example line number.
          oatpp::Int32 guess = 2;    // Example guess.

          auto response = client->getVersionControlAnswer(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 400);

          auto answer = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Invalid Entry");
        }  // End of Test #1.

        // Test True/False Answers Correct
        {
          oatpp::String line = "a";
          oatpp::Int32 guess = 1;
                                     // False).

          auto response = client->tfVersionControlAnswers(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 400);

          auto answer = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Invalid Entry");
        }  // End of Test #5.

        // Test Short Answer Answers Incorrect
        {
          oatpp::String line = "a";      // Example line number.
          oatpp::String guess = "Answer";  // Example guess.

          auto response = client->saVersionControlAnswers(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 400);

          auto answer = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Invalid Entry");
        }  // End of Test #8.

        // Test POST - Malformed Matching Answers.
        {
          auto matchingAnswers = MatchingAnswersDTO::createShared();
          matchingAnswers->line = "a";
          matchingAnswers->answerOne = "Answer1";
          matchingAnswers->answerTwo = "Answer2";
          matchingAnswers->answerThree = "Answer3";
          matchingAnswers->answerFour = "Answer4";

          auto response = client->maVersionControlAnswers(matchingAnswers);
          OATPP_ASSERT(response->getStatusCode() == 400);

          auto answerMessage = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answerMessage);
          OATPP_ASSERT(answerMessage->result == "Invalid Entry");
        }
      },
      std::chrono::minutes(10) /* test timeout */);

  // Wait for all server threads to finish.
  std::this_thread::sleep_for(std::chrono::seconds(1));
}
