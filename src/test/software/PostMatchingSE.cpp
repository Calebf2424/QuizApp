#include "PostMatchingSE.hpp"

#include <iostream>
#include <memory>

#include "../../controller/SoftwareEngineeringController.hpp"
#include "../app/MyApiTestClient.hpp"
#include "../app/TestComponent.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

void PostMatchingSE::onRun() {
  // Register test components.
  TestComponent component;

  // Create client-server test runner.
  oatpp::test::web::ClientServerTestRunner runner;

  // Get object mapper component.
  OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                  objectMapper);

  // Add SoftwareEngineeringController endpoints to the router.
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

        // Call server API.
        auto response = client->getSoftware();

        // Assert that server responds with 200.
        OATPP_ASSERT(response->getStatusCode() == 200);

        // Read response body as MessageDto.
        auto message = response->readBodyToDto<
            oatpp::Object<mcGetResult>>(objectMapper.get());

        // Assert that received message is as expected.
        OATPP_ASSERT(message);
        OATPP_ASSERT(message->question == message->question);

        // Parse JSON string into MatchingAnswersDTO object.
        auto matchingAnswers = MatchingAnswersDTO::createShared();
        matchingAnswers->line = "1";
        matchingAnswers->answerOne =
            "A foundational structure with pre-written code to support the "
            "development of software applications";
        matchingAnswers->answerTwo =
            "The process of restructuring existing code without changing its "
            "external behavior to improve its readability and maintainability";
        matchingAnswers->answerThree =
            "A reusable solution to a common software design problem";
        matchingAnswers->answerFour =
            "Simplifying complex systems by modeling essential features and "
            "hiding unnecessary details";

        // Test POST.
        response = client->maSoftwareAnswers(matchingAnswers);
        OATPP_ASSERT(response->getStatusCode() == 200);

        auto answerMessage = response->readBodyToDto<
            oatpp::Object<Answer>>(objectMapper.get());
        OATPP_ASSERT(answerMessage);
        OATPP_ASSERT(answerMessage->result == "Correct");
      },
      std::chrono::minutes(10));  // Test timeout.

  // Wait for all server threads to finish.
  std::this_thread::sleep_for(std::chrono::seconds(1));
}
