//
//  main.cpp
//  ApiClient-Demo
//
//  Created by Leonid on 3/14/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#include "./DemoApiClient.hpp"
#include "./DemoRequestExecutor.hpp"
#include "./Logger.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include <iostream>

void run(){
  
  /* Create RequestExecutor which will execute ApiClient's requests */
  auto requestExecutor = std::make_shared<DemoRequestExecutor>();
  
  /* Create ObjectMapper for serialization of DTOs  */
  auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
  
  /* DemoApiClient uses DemoRequestExecutor and json::mapping::ObjectMapper */
  DemoApiClient client(requestExecutor, objectMapper);
  
  /* Call methods of DemoApiClient. DemoRequestExecutor will print request data to console */
  
  client.getRoot();
  
  client.postData("some_auth_token", "<post-data>"); // simple POST request with string data
  
  auto user = UserDto::createShared();
  user->id = 123456789;
  user->name = "Ivan";
  user->surname = "Ovsyanochka";
  
  client.postUser("some_auth_token", user); // POST request with DTO serialized via ObjectMapper
  
  client.getUsers("some_auth_token", 0, 100, "userName", "userLastName", 85); // GET request with path-variables, and query-variables
  
  client.getRootAsync(nullptr, nullptr);
  client.postDataAsync(nullptr, nullptr, "some_auth_token", "<post-data>");
  
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
