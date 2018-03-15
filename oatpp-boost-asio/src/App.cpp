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

#include "./asio/AsyncAcceptor.hpp"
#include "./controller/MyApiController.hpp"
#include "./AppComponent.hpp"
#include "./Logger.hpp"

//////////////////////////////////
// std

#include <iostream>

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
  
  auto MyApiController = MyApiController::createShared();
  MyApiController->addEndpointsToRouter(router);
  
  /* create asio service */
  
  boost::asio::io_service ioservice;
  auto endpoint = components.asioEndpoint.getObject();
  
  asio::AsyncAcceptor acceptor(ioservice, *endpoint, components.serverConnectionHandler.getObject());
  acceptor.start();
  
  OATPP_LOGD("App", "Server is running on port: %d", endpoint->port());
  
  ioservice.run();
  
}

/**
 *  main
 */
int main(int argc, const char * argv[]) {
  
  oatpp::base::Environment::setLogger(new Logger());
  oatpp::base::Environment::init();
  
#if !defined(OATPP_USE_TARGET) | defined(OATPP_TARGET_APP)
  run();
#endif
  
#ifdef OATPP_TARGET_TEST
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
