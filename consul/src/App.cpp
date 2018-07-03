//
//  main.cpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

//#define OATPP_USE_TARGET
//#define OATPP_TARGET_TEST

//////////////////////////////////
// App

#include "./controller/UserController.hpp"
#include "./controller/HealthController.hpp"
#include "./AppComponent.hpp"
#include "./Logger.hpp"

//////////////////////////////////
// Test

#ifdef OATPP_TARGET_TEST
  #include "test/ControllerLevelTest.hpp"
  #include "test/RouterLevelTest.hpp"
  #include "test/ConnectionHandlerLevelTest.hpp"
#endif

//////////////////////////////////
// std

#include <iostream>

///

#include "oatpp-consul/rest/Client.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp/network/client/SimpleTCPConnectionProvider.hpp"

void tryConsul() {
  
  auto serializerConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
  serializerConfig->includeNullFields = false;
  
  auto deserializerConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();
  deserializerConfig->allowUnknownFields = false;
  
  auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared(serializerConfig, deserializerConfig);
  
  auto connectionProvider = oatpp::network::client::SimpleTCPConnectionProvider::createShared("localhost", 8500);
  auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(connectionProvider);
  
  auto client = oatpp::consul::rest::Client::createShared(requestExecutor, objectMapper);
  
  auto payload = oatpp::consul::rest::AgentCheckRegisterPayload::createShared();
  payload->id = "MyService";
  payload->name = "MyService health-check";
  payload->notes = "Check on the MyService/Health endpoint";
  payload->http = "http://host.docker.internal:8000/check/health";
  payload->method = "GET";
  payload->interval = "1s";
  payload->timeout = "1s";
  
  auto response = client->putAgentChecksRegister(payload);
  
  auto result = response->readBodyToString();
  
  OATPP_LOGD("consul", "response='%s'", result->c_str());
  
}

/**
 *  run() method.
 *  1) set Environment components.
 *  2) add ApiController's endpoints to router
 *  3) run server
 */
void run() {
  
  AppComponent components; // Create scope Environment components
  
  /* create ApiControllers and add endpoints to router */
  
  auto router = components.httpRouter.getObject();
  
  auto userController = UserController::createShared();
  userController->addEndpointsToRouter(router);
  
  auto healthController = HealthController::createShared();
  healthController->addEndpointsToRouter(router);
  
  /* create server */
  
  oatpp::network::server::Server server(components.serverConnectionProvider.getObject(),
                                        components.serverConnectionHandler.getObject());
  
  OATPP_LOGD("Server", "Running on port %u...", components.serverConnectionProvider.getObject()->getPort());
  
  server.run();
  
}

/**
 *  main
 */
int main(int argc, const char * argv[]) {
  
  oatpp::base::Environment::setLogger(new Logger());
  oatpp::base::Environment::init();
  
#if !defined(OATPP_USE_TARGET) | defined(OATPP_TARGET_APP)
  tryConsul();
  run();
#endif
  
#ifdef OATPP_TARGET_TEST
  OATPP_RUN_TEST(ControllerLevelTest);
  OATPP_RUN_TEST(RouterLevelTest);
  OATPP_RUN_TEST(ControllerLevelTest);
#endif
  
  oatpp::base::Environment::setLogger(nullptr); ///< free Logger
  
  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
  
  oatpp::base::Environment::destroy();
  
  return 0;
}
