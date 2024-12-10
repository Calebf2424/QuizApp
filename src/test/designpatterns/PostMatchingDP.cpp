#include "PostMatchingDP.hpp"

#include <iostream>
#include <memory>

#include "../../controller/DesignPatternController.hpp"
#include "../app/MyApiTestClient.hpp"
#include "../app/TestComponent.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

void PostMatchingDP::onRun() {
  // Register test components.
  TestComponent component;

  // Create client-server test runner.
  oatpp::test::web::ClientServerTestRunner runner;

  // Get object mapper component.
  OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                  objectMapper);

  // Add DesignPatternController endpoints to the router.
  runner.addController(std::make_shared<DesignPatternController>());

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
        auto response = client->getDesignPattern();

        // Assert that server responds with 200.
        OATPP_ASSERT(response->getStatusCode() == 200);

        // Read response body as MessageDto.
        auto message = response->readBodyToDto<
            oatpp::Object<mcGetResult>>(objectMapper.get());

        // Assert that received message is as expected.
        OATPP_ASSERT(message);
        OATPP_ASSERT(message->question == message->question);

        // Parse JSON string into MatchingAnswersDTO object - Test Case 1.
        {
          auto matchingAnswers = MatchingAnswersDTO::createShared();
          matchingAnswers->line = "1";
          matchingAnswers->answerOne =
              "A creational design pattern that lets you ensure that a class "
              "has only one instance while providing a global access point to "
              "this instance";
          matchingAnswers->answerTwo =
              "A creational design pattern that lets you construct complex "
              "objects step by step";
          matchingAnswers->answerThree =
              "A creational design pattern that provides an interface for "
              "creating objects in a superclass but allows subclasses to alter "
              "the type of objects that will be created.";
          matchingAnswers->answerFour =
              "A structural design pattern that lets you attach new behaviors "
              "to objects by placing these objects inside special wrapper "
              "objects that contain the behaviors.";

          // Test POST.
          response = client->maDesignPatternAnswers(matchingAnswers);
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto answerMessage = response->readBodyToDto<
              oatpp::Object<Answer>>(objectMapper.get());
          OATPP_ASSERT(answerMessage);
          OATPP_ASSERT(answerMessage->result == "Correct");
        }

        // Parse JSON string into MatchingAnswersDTO object - Test Case 2.
        {
          auto matchingAnswers = MatchingAnswersDTO::createShared();
          matchingAnswers->line = "2";
          matchingAnswers->answerOne =
              "Creates new objects by copying an existing object";
          matchingAnswers->answerTwo =
              "Provides an interface for creating families of related or "
              "dependent objects without specifying their concrete classes";
          matchingAnswers->answerThree =
              "Decouples abstraction from implementation allowing both to "
              "vary independently";
          matchingAnswers->answerFour =
              "Composes objects into tree-like structures to represent "
              "part-whole hierarchies";

          // Test POST.
          response = client->maDesignPatternAnswers(matchingAnswers);
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto answerMessage = response->readBodyToDto<
              oatpp::Object<Answer>>(objectMapper.get());
          OATPP_ASSERT(answerMessage);
          OATPP_ASSERT(answerMessage->result == "Correct");
        }
      },
      std::chrono::minutes(10));  // Test timeout.

  // Wait for all server threads to finish.
  std::this_thread::sleep_for(std::chrono::seconds(1));
}
