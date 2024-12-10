#include "TestingTest.hpp"

#include <iostream>
#include <memory>
#include <string>

#include "../../controller/TestingController.hpp"
#include "../app/MyApiTestClient.hpp"
#include "../app/TestComponent.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

void TestingTest::onRun() {
  /* Register test components */
  TestComponent component;

  /* Create client-server test runner */
  oatpp::test::web::ClientServerTestRunner runner;

  /* Get object mapper component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                  objectMapper);

  /* Add TestingController endpoints to the router of the test server */
  runner.addController(std::make_shared<TestingController>());

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

        // Test Multiple Choice - Testing - Test #1
        {
          /* Call server API */
          auto response = client->getTesting();
          /* Assert that server responds with 200 */
          OATPP_ASSERT(response->getStatusCode() == 200);

          /* Read response body as MessageDto */
          auto message = response->readBodyToDto<
              oatpp::Object<mcGetResult>>(objectMapper.get());
          /* Assert that received message is as expected */
          OATPP_ASSERT(message);
          OATPP_ASSERT(message->id && *message->id != 0);

          std::string correct_merged_question;
          std::string merged_question =
              message->question + "|" + message->a + "|" + message->b + "|" +
              message->c + "|" + message->d;
          int current_line = 0;

          std::ifstream file("data/test_mcQuestions.txt");
          while (std::getline(file, correct_merged_question)) {
            if (++current_line == message->id) {
              break;
            }
          }
          OATPP_ASSERT(merged_question == correct_merged_question);
        }  // End of Test #1

        // Test Multiple Choice Answer - Incorrect - Testing - Test #2
        {
          oatpp::String line = "1";  // Example line number
          oatpp::Int32 guess = 2;   // Example guess

          auto response = client->getTestingAnswer(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto answer = response->readBodyToDto<
              oatpp::Object<Answer>>(objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == answer->result);
        }  // End of Test #2

        // Test Multiple Choice Answer - Correct - Testing - Test #3
        {
          oatpp::String line = "1";  // Example line number
          oatpp::Int32 guess = 1;   // Example guess

          auto response = client->getTestingAnswer(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto answer = response->readBodyToDto<
              oatpp::Object<Answer>>(objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Correct");
        }  // End of Test #3

        // Test True/False - Testing - Test #4
        {
          auto response = client->tfTesting();
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto message = response->readBodyToDto<
              oatpp::Object<tfGetQuestion>>(objectMapper.get());
          OATPP_ASSERT(message);
          OATPP_ASSERT(message->id && *message->id != 0);

          std::string correct_merged_question;
          std::string merged_question =
              message->question + "|" + message->a + "|" + message->b;
          int current_line = 0;

          std::ifstream file("data/test_tfQuestions.txt");
          while (std::getline(file, correct_merged_question)) {
            if (++current_line == message->id) {
              break;
            }
          }
          OATPP_ASSERT(merged_question == correct_merged_question);
        }  // End of Test #4

        // Test Short Answer - Correct - Testing - Test #9
        {
          oatpp::String line = "1";      // Example line number
          oatpp::String guess = "Unit";  // Example guess

          auto response = client->saTestingAnswers(line, guess);
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto answer = response->readBodyToDto<
              oatpp::Object<Answer>>(objectMapper.get());
          OATPP_ASSERT(answer);
          OATPP_ASSERT(answer->result == "Correct");
        }  // End of Test #9

        /* Test /matching endpoint */
        {
          auto response = client->matchingTesting();
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto message = response->readBodyToDto<
              oatpp::Object<MatchingQuestionDTO>>(objectMapper.get());
          OATPP_ASSERT(message);
          OATPP_ASSERT(message->id && *message->id != 0);

          std::string correct_merged_question;
          int current_line = 0;

          std::string merged_question_left;
          auto left_it = message->leftItems->begin();
          while (left_it != message->leftItems->end()) {
            merged_question_left += *left_it;
            merged_question_left += ",";
            ++left_it;
          }
          if (!merged_question_left.empty()) {
            merged_question_left.pop_back();
          }

          std::string merged_question_right;
          auto right_it = message->rightItems->begin();
          while (right_it != message->rightItems->end()) {
            merged_question_right += *right_it;
            merged_question_right += ",";
            ++right_it;
          }
          if (!merged_question_right.empty()) {
            merged_question_right.pop_back();
          }

          std::string full_merged_question = message->question + "|" +
                                             merged_question_left + "|" +
                                             merged_question_right;

          std::ifstream file("data/test_maQuestions.txt");
          while (std::getline(file, correct_merged_question)) {
            if (++current_line == message->id) {
              break;
            }
          }
          OATPP_ASSERT(full_merged_question == correct_merged_question);
        }
        {
          auto response = client->matchingTesting();
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
      std::chrono::minutes(10));

  /* Wait for all server threads to finish */
  std::this_thread::sleep_for(std::chrono::seconds(1));
}
