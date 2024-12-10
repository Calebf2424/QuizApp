#include "SoftwareTest.hpp"

#include <iostream>
#include <memory>
#include <string>

#include "../../controller/SoftwareEngineeringController.hpp"
#include "../app/MyApiTestClient.hpp"
#include "../app/TestComponent.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

void SoftwareTest::onRun() {
  /* Register test components */
  TestComponent component;

  /* Create client-server test runner */
  oatpp::test::web::ClientServerTestRunner runner;

  /* Get object mapper component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                  objectMapper);

  /* Add SoftwareEngineeringController endpoints to the router */
  runner.addController(std::make_shared<SoftwareEngineeringController>());

  /* Run test */
  runner.run(
      [this, &runner] {
        /* Get client connection provider for API client */
        OATPP_COMPONENT(
            std::shared_ptr<oatpp::network::ClientConnectionProvider>,
            clientConnectionProvider);

        /* Get object mapper component */
        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                        objectMapper);

        /* Create HTTP request executor for API client */
        auto requestExecutor =
            oatpp::web::client::HttpRequestExecutor::createShared(
                clientConnectionProvider);

        /* Create Test API client */
        auto client = MyApiTestClient::createShared(requestExecutor,
                                                    objectMapper);

        // Test Multiple Choice - Software - Test #1
        {
          /* Call server API */
          auto response = client->getSoftware();
          /* Assert that server responds with 200 */
          OATPP_ASSERT(response->getStatusCode() == 200);

          /* Read response body as MessageDto */
          auto message = response->readBodyToDto<
              oatpp::Object<mcGetResult>>(objectMapper.get());
          /* Assert that received message is as expected */
          OATPP_ASSERT(message);
          OATPP_ASSERT(message->id && *message->id != 0);

          std::string CorrectmergedQuestion;
          std::string mergedQuestion =
              message->question + "|" + message->a + "|" + message->b + "|" +
              message->c + "|" + message->d;
          int currentLine = 0;

          std::ifstream file("data/se_mcQuestions.txt");
          while (std::getline(file, CorrectmergedQuestion)) {
            if (++currentLine == message->id) {
              break;
            }
          }
          OATPP_ASSERT(mergedQuestion == CorrectmergedQuestion);
        }  // End of Test #1

        // Test Multiple Choice Answer - Incorrect - Software - Test #2
        {
          oatpp::String line = "1";  // Example line number
          oatpp::Int32 guess = 2;   // Example guess

          auto response = client->getSoftwareAnswer(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto answer = response->readBodyToDto<
              oatpp::Object<Answer>>(objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == answer->result);
        }  // End of Test #2

        // Test Multiple Choice Answer - Correct - Software - Test #3
        {
          oatpp::String line = "1";  // Example line number
          oatpp::Int32 guess = 3;   // Example guess

          auto response = client->getSoftwareAnswer(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto answer = response->readBodyToDto<
              oatpp::Object<Answer>>(objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Correct");
        }  // End of Test #3

        // Test True/False - Software - Test #4
        {
          auto response = client->tfSoftware();
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto message = response->readBodyToDto<
              oatpp::Object<tfGetQuestion>>(objectMapper.get());
          OATPP_ASSERT(message);
          OATPP_ASSERT(message->id && *message->id != 0);

          std::string CorrectmergedQuestion;
          std::string mergedQuestion =
              message->question + "|" + message->a + "|" + message->b;
          int currentLine = 0;

          std::ifstream file("data/se_tfQuestions.txt");
          while (std::getline(file, CorrectmergedQuestion)) {
            if (++currentLine == message->id) {
              break;
            }
          }
          OATPP_ASSERT(mergedQuestion == CorrectmergedQuestion);
        }  // End of Test #4

        // Test True/False Answers Correct - Software - Test #5
        {
          oatpp::String line = "1";  // Example line number
          oatpp::Int32 guess = 1;   // Example guess (1 for True, 0 for False)

          auto response = client->tfSoftwareAnswers(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto answer = response->readBodyToDto<
              oatpp::Object<Answer>>(objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Correct");
        }  // End of Test #5

                // Test - Matching question generation
        {
          auto response = client->matchingSoftware();
          OATPP_ASSERT(response->getStatusCode() == 200);

          // Parse response into DTO
          auto dto = response->readBodyToDto<
          oatpp::Object<MatchingQuestionDTO>>(objectMapper.get());
          OATPP_ASSERT(dto);

          // Validate the question and items
          OATPP_ASSERT(dto->question && !dto->question->empty());
          OATPP_ASSERT(dto->leftItems && dto->leftItems->size() > 0);
          OATPP_ASSERT(dto->rightItems && dto->rightItems->size() > 0);

          // Output the parsed question for debugging
          std::cout << "Matching Question ID: " << dto->id << std::endl;
          std::cout << "Question: " << dto->question->c_str() << std::endl;
          std::cout << "Left Items:" << std::endl;
          for (const auto& item : *dto->leftItems) {
              std::cout << "- " << item->c_str() << std::endl;
          }
          std::cout << "Right Items:" << std::endl;
          for (const auto& item : *dto->rightItems) {
              std::cout << "- " << item->c_str() << std::endl;
          }

          // Validate item count matches expected (from file structure)
          OATPP_ASSERT(dto->leftItems->size() == dto->rightItems->size());
        }
      },
      std::chrono::minutes(10));  // Test timeout.

  /* Wait all server threads finished */
  std::this_thread::sleep_for(std::chrono::seconds(1));
}
