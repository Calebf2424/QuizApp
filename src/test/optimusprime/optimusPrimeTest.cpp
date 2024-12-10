#include "optimusPrimeTest.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../../controller/OptimusPrimeController.hpp"
#include "../app/MyApiTestClient.hpp"
#include "../app/TestComponent.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

void optimusPrimeTest::onRun() {
  // Register test components.
  TestComponent component;

  // Create client-server test runner.
  oatpp::test::web::ClientServerTestRunner runner;

  // Get object mapper component.
  OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                  objectMapper);

  // Add OptimusPrimeController endpoints to the router.
  runner.addController(std::make_shared<OptimusPrimeController>());

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

        // Test - Services List works.
        {
          auto response = client->getServices();
          OATPP_ASSERT(response->getStatusCode() == 200);
          auto message =
              response->readBodyToDto<oatpp::Object<GetServices>>(
                  objectMapper.get());
          OATPP_ASSERT(message);
          OATPP_ASSERT(message->services == message->services);
        }

        // Test - Random Question for all cases
        const std::vector<std::string> topics = {
            "versioncontrol", "testing", "designpattern", "software"};
        const std::vector<std::string> types = {
            "mc", "tf", "shortAnswer", "matching"};

        for (const auto& topic : topics) {
          for (const auto& type : types) {
            auto response = client->getSpecificQuestion(topic, type);
            OATPP_ASSERT(response->getStatusCode() == 200);

            auto message =
                response->readBodyToDto<oatpp::Object<GetRandomQuestion>>(
                    objectMapper.get());
            OATPP_ASSERT(message);
            std::string expectedLink =
                "http://localhost:8400/optimusprime/" + topic + "/" + type;
            OATPP_ASSERT(message->link == expectedLink);
          }
        }

        // Test - Specific Topic with all random types
        for (const auto& topic : topics) {
          auto response = client->getSpecificTopic(topic);
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto message =
              response->readBodyToDto<oatpp::Object<GetRandomQuestion>>(
                  objectMapper.get());
          OATPP_ASSERT(message);
          OATPP_ASSERT(message->link->find(
                           "http://localhost:8400/optimusprime/" + topic + "/")
                       != std::string::npos);
        }

        // Test - Specific Type with all random topics
        for (const auto& type : types) {
          auto response = client->getSpecificType(type);
          OATPP_ASSERT(response->getStatusCode() == 200);

          auto message =
              response->readBodyToDto<oatpp::Object<GetRandomQuestion>>(
                  objectMapper.get());
          OATPP_ASSERT(message);
          OATPP_ASSERT(message->link->find(
                           "http://localhost:8400/optimusprime/")
                       != std::string::npos);
        }
      },
      std::chrono::minutes(10));  // Test timeout.

  // Wait for all server threads to finish.
  std::this_thread::sleep_for(std::chrono::seconds(1));
}
