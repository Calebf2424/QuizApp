#include "PostMatchingVC.hpp"

#include <iostream>
#include <memory>

#include "../../controller/VersionControlController.hpp"
#include "../app/MyApiTestClient.hpp"
#include "../app/TestComponent.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

void PostMatchingVC::onRun() {
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
        // Get client connection provider for Api Client.
        OATPP_COMPONENT(
            std::shared_ptr<oatpp::network::ClientConnectionProvider>,
            clientConnectionProvider);

        // Get object mapper component.
        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                        objectMapper);

        // Create http request executor for Api Client.
        auto requestExecutor =
            oatpp::web::client::HttpRequestExecutor::createShared(
                clientConnectionProvider);

        // Create Test API client.
        auto client = MyApiTestClient::createShared(requestExecutor,
                                                    objectMapper);

        // Call server API.
        auto response = client->getVersionControl();

        // Assert that server responds with 200.
        OATPP_ASSERT(response->getStatusCode() == 200);

        // Read response body as MessageDto.
        auto message = response->readBodyToDto<oatpp::Object<mcGetResult>>(
            objectMapper.get());

        // Assert that received message is as expected.
        OATPP_ASSERT(message);
        OATPP_ASSERT(message->question == message->question);

        // Parse JSON string into MatchingAnswersDTO object.
        auto matchingAnswers = MatchingAnswersDTO::createShared();
        matchingAnswers->line = "1";
        matchingAnswers->answerOne =
            "A tool that helps software developers manage changes "
            "to source code over time by maintaining a history "
            "of modifications";
        matchingAnswers->answerTwo =
            "Relies on a single central server that stores all versions of the "
            "codebase";
        matchingAnswers->answerThree =
            "Allows developers to clone the entire repository including its "
            "history onto their local machine";
        matchingAnswers->answerFour =
            "The process of creating an independent line of development in a "
            "repository allowing developers to work on features or "
            "fixes without affecting the main codebase";

        // Test POST.
        response = client->maVersionControlAnswers(matchingAnswers);
        OATPP_ASSERT(response->getStatusCode() == 200);

        auto answerMessage = response->readBodyToDto<oatpp::Object<Answer>>(
            objectMapper.get());
        OATPP_ASSERT(answerMessage);
        OATPP_ASSERT(answerMessage->result == "Correct");
      },
      std::chrono::minutes(10) /* test timeout */);

  // Wait for all server threads to finish.
  std::this_thread::sleep_for(std::chrono::seconds(1));
}
