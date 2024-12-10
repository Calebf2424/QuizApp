#include "VersionControlTest.hpp"

#include <iostream>
#include <memory>
#include <string>

#include "../../controller/VersionControlController.hpp"
#include "../app/MyApiTestClient.hpp"
#include "../app/TestComponent.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

void VersionControlTest::onRun() {
  // Register test components
  TestComponent component;

  // Create client-server test runner
  oatpp::test::web::ClientServerTestRunner runner;
  // Get object mapper component
  OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                  objectMapper);

  // Add VersionControlController endpoints to the router of the test server
  runner.addController(std::make_shared<VersionControlController>());

  // Run test
  runner.run(
      [this, &runner] {
        // Get client connection provider for Api Client
        OATPP_COMPONENT(
            std::shared_ptr<oatpp::network::ClientConnectionProvider>,
            clientConnectionProvider);

        // Get object mapper component
        OATPP_COMPONENT(
            std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

        // Create HTTP request executor for Api Client
        auto requestExecutor =
            oatpp::web::client::HttpRequestExecutor::createShared(
                clientConnectionProvider);

        // Create Test API client
        auto client =
            MyApiTestClient::createShared(requestExecutor, objectMapper);

        // Test Multiple Choice - Version Control - Test #1
        {
          auto response = client->getVersionControl();
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto message = response->readBodyToDto<oatpp::Object<mcGetResult>>(
              objectMapper.get());
          OATPP_ASSERT(message);
          OATPP_ASSERT(message->id && *message->id != 0);

          std::string correctMergedQuestion;
          std::string mergedQuestion =
              message->question + "|" + message->a + "|" + message->b + "|" +
              message->c + "|" + message->d;
          int currentLine = 0;

          std::ifstream file("data/vc_mcQuestions.txt");
          while (std::getline(file, correctMergedQuestion)) {
            if (++currentLine == message->id) {
              break;
            }
          }
          OATPP_ASSERT(mergedQuestion == correctMergedQuestion);
        }

        // Test Multiple Choice Answer - Incorrect Test - Version Control
        {
          oatpp::String line = "1";
          oatpp::Int32 guess = 2;

          auto response = client->getVersionControlAnswer(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto answer = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == answer->result);
        }

        // Test Multiple Choice Answer - Correct Test - Version Control
        {
          oatpp::String line = "1";
          oatpp::Int32 guess = 1;

          auto response = client->getVersionControlAnswer(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto answer = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Correct");
        }

        // Test True/False - Version Control - Test #4
        {
          auto response = client->tfVersionControl();
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto message = response->readBodyToDto<oatpp::Object<tfGetQuestion>>(
              objectMapper.get());
          OATPP_ASSERT(message);
          OATPP_ASSERT(message->id && *message->id != 0);

          std::string correctMergedQuestion;
          std::string mergedQuestion =
              message->question + "|" + message->a + "|" + message->b;
          int currentLine = 0;

          std::ifstream file("data/vc_tfQuestions.txt");
          while (std::getline(file, correctMergedQuestion)) {
            if (++currentLine == message->id) {
              break;
            }
          }
          OATPP_ASSERT(mergedQuestion == correctMergedQuestion);
        }

        // Test Short Answer Answers Correct - Version Control - Test #9
        {
          oatpp::String line = "1";
          oatpp::String guess = "Singleton";

          auto response = client->saVersionControlAnswers(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto answer = response->readBodyToDto<oatpp::Object<Answer>>(
              objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Correct");
        }

        // Test /matching endpoint
        {
          auto response = client->matchingVersionControl();
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto message = response->readBodyToDto<
              oatpp::Object<MatchingQuestionDTO>>(objectMapper.get());
          OATPP_ASSERT(message);
          OATPP_ASSERT(message->id && *message->id != 0);

          std::string correctMergedQuestion;
          int currentLine = 0;

          std::string mergedQuestionLeft;
          for (const auto& leftItem : *message->leftItems) {
            mergedQuestionLeft += leftItem + ",";
          }
          if (!mergedQuestionLeft.empty()) {
            mergedQuestionLeft.pop_back();
          }

          std::string mergedQuestionRight;
          for (const auto& rightItem : *message->rightItems) {
            mergedQuestionRight += rightItem + ",";
          }
          if (!mergedQuestionRight.empty()) {
            mergedQuestionRight.pop_back();
          }

          std::string fullMergedQuestion =
              message->question + "|" + mergedQuestionLeft + "|" +
              mergedQuestionRight;

          std::ifstream file("data/vc_maQuestions.txt");
          while (std::getline(file, correctMergedQuestion)) {
            if (++currentLine == message->id) {
              break;
            }
          }
          OATPP_ASSERT(fullMergedQuestion == correctMergedQuestion);
        }
      },
      std::chrono::minutes(10) /* test timeout */);

  // Wait for all server threads to finish
  std::this_thread::sleep_for(std::chrono::seconds(1));
}
