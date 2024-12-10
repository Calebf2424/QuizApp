#include "PostMatchingTesting.hpp"

#include <iostream>
#include <memory>

#include "../../controller/TestingController.hpp"
#include "../app/MyApiTestClient.hpp"
#include "../app/TestComponent.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

void PostMatchingTesting::onRun() {
  // Register test components.
  TestComponent component;

  // Create client-server test runner.
  oatpp::test::web::ClientServerTestRunner runner;

  // Get object mapper component.
  OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                  objectMapper);

  // Add TestingController endpoints to the test server router.
  runner.addController(std::make_shared<TestingController>());

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
        auto response = client->getTesting();

        // Assert that server responds with 200.
        OATPP_ASSERT(response->getStatusCode() == 200);

        // Read response body as mcGetResult DTO.
        auto message = response->readBodyToDto<
            oatpp::Object<mcGetResult>>(objectMapper.get());

        // Assert that received message is as expected.
        OATPP_ASSERT(message);
        OATPP_ASSERT(message->question == message->question);

        // Parse JSON string into MatchingAnswersDTO object.
        auto matchingAnswers = MatchingAnswersDTO::createShared();
        matchingAnswers->line = "1";
        matchingAnswers->answerOne =
            "A set of conditions inputs and expected results used to "
            "verify software functionality";
        matchingAnswers->answerTwo =
            "Testing individual components or units of code in isolation";
        matchingAnswers->answerThree =
            "Testing the softwares functionality without knowledge of the "
            "internal code";
        matchingAnswers->answerFour =
            "A combination of black-box and white-box testing approaches";

        // Test POST.
        response = client->maTestingAnswers(matchingAnswers);
        OATPP_ASSERT(response->getStatusCode() == 200);

        auto answerMessage = response->readBodyToDto<
            oatpp::Object<Answer>>(objectMapper.get());
        OATPP_ASSERT(answerMessage);
        OATPP_ASSERT(answerMessage->result == "Correct");
      },
      std::chrono::minutes(10));  // Test timeout.

  // Wait for all server threads finished.
  std::this_thread::sleep_for(std::chrono::seconds(1));
}
