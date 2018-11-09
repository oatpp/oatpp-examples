//
//  main.cpp
//  ApiClient-Demo
//
//  Created by Leonid on 3/14/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#include "AsyncExample.hpp"
#include "SimpleExample.hpp"

#include "DemoApiClient.hpp"
#include "Logger.hpp"

#include "oatpp-curl/RequestExecutor.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp/network/client/SimpleTCPConnectionProvider.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include <iostream>

std::shared_ptr<oatpp::web::client::RequestExecutor> createOatppExecutor() {
  auto connectionProvider = oatpp::network::client::SimpleTCPConnectionProvider::createShared("httpbin.org", 80);
  return oatpp::web::client::HttpRequestExecutor::createShared(connectionProvider);
}

std::shared_ptr<oatpp::web::client::RequestExecutor> createCurlExecutor() {
  return oatpp::curl::RequestExecutor::createShared("http://httpbin.org/", false /* set verbose=true for dubug info */);
}

void run(){
  
  /* Create ObjectMapper for serialization of DTOs  */
  auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
  
  /* Create RequestExecutor which will execute ApiClient's requests */
  //auto requestExecutor = createOatppExecutor();
  auto requestExecutor = createCurlExecutor();
  
  /* DemoApiClient uses DemoRequestExecutor and json::mapping::ObjectMapper */
  /* ObjectMapper passed here is used for serialization of outgoing DTOs */
  auto client = DemoApiClient::createShared(requestExecutor, objectMapper);
  
  SimpleExample::runExample(client);
  AsyncExample::runExample(client);
  
}

int main(int argc, const char * argv[]) {
  oatpp::base::Environment::setLogger(new Logger()); // set Logger to print logs
  oatpp::base::Environment::init();
  
  run();
  
  oatpp::base::Environment::setLogger(nullptr); // free Logger
  
  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
  
  oatpp::base::Environment::destroy();
}
