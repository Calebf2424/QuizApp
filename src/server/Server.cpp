#include "oatpp/network/Server.hpp"

#include "../controller/DesignPatternController.hpp"
#include "../controller/VersionControlController.hpp"
#include "../controller/TestingController.hpp"
#include "../controller/SoftwareEngineeringController.hpp"
#include "../controller/OptimusPrimeController.hpp"

#include "ServerComponents.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

void run() {
  ServerComponent components;

  // Create Router for HTTP requests routing
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  // Create controllers and add all of their endpoints to the router
  auto designPatternController = std::make_shared<DesignPatternController>();
  router->addController(designPatternController);

  auto versionControlController = std::make_shared<VersionControlController>();
  router->addController(versionControlController);

  auto testingController = std::make_shared<TestingController>();
  router->addController(testingController);

  auto softwareEngineeringController =
      std::make_shared<SoftwareEngineeringController>();
  router->addController(softwareEngineeringController);

  auto optimusPrimeController = std::make_shared<OptimusPrimeController>();
  router->addController(optimusPrimeController);

  // Create HTTP connection handler with the router
  auto connectionHandler =
      oatpp::web::server::HttpConnectionHandler::createShared(router);

  // Create TCP connection provider
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                  connectionProvider);

  // Create server with TCP connections and HTTP connection handler
  oatpp::network::Server server(connectionProvider, connectionHandler);

  // Print server port info
  OATPP_LOGI("Selene Service", "Server running on port %d",
             connectionProvider->getProperty("port").getData());

  // Run server
  server.run();
}

int main() {
  // Initialize oatpp Environment
  oatpp::base::Environment::init();

  // Run application
  run();

  // Destroy oatpp Environment
  oatpp::base::Environment::destroy();

  return 0;
}
