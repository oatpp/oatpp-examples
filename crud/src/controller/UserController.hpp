//
//  UserController.hpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef UserController_hpp
#define UserController_hpp

#include "../db/Database.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

/**
 *  EXAMPLE ApiController
 *  Basic examples of howto create ENDPOINTs
 *  More details on oatpp.io
 */
class UserController : public oatpp::web::server::api::ApiController {
public:
  UserController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
private:
  
  /**
   *  Inject Database component
   */
  OATPP_COMPONENT(std::shared_ptr<Database>, m_database);
public:
  
  /**
   *  Inject @objectMapper component here as default parameter
   *  Do not return bare Controllable* object! use shared_ptr!
   */
  static std::shared_ptr<UserController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                 objectMapper)){
    return std::make_shared<UserController>(objectMapper);
  }
  
  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)
  
  /**
   *  Insert Your endpoints here !!!
   */
  
  ENDPOINT_INFO(root) {
    info->summary = "Index.html page";
    info->addResponse<UserDto::ObjectWrapper>(Status::CODE_200, "text/html");
  }
  ENDPOINT("GET", "/", root) {
    const char* html =
    "<html lang='en'>"
    "<head>"
    "<meta charset=utf-8/>"
    "</head>"
    "<body>"
    "<p>Hello CRUD example project!</p>"
    "<a href='swagger/ui'>Checkout Swagger-UI page</a>"
    "</body>"
    "</html>";
    auto response = createResponse(Status::CODE_200, html);
    response->putHeader(Header::CONTENT_TYPE, "text/html");
    return response;
  }
  
  ENDPOINT_INFO(createUser) {
    info->summary = "Create new User";
    info->addConsumes<UserDto::ObjectWrapper>("application/json");
    info->addResponse<UserDto::ObjectWrapper>(Status::CODE_200, "application/json");
  }
  ENDPOINT("POST", "demo/api/users", createUser,
           BODY_DTO(UserDto::ObjectWrapper, userDto)) {
    return createDtoResponse(Status::CODE_200, m_database->createUser(userDto));
  }
  
  
  ENDPOINT_INFO(putUser) {
    info->summary = "Update User by userId";
    info->addConsumes<UserDto::ObjectWrapper>("application/json");
    info->addResponse<UserDto::ObjectWrapper>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ENDPOINT("PUT", "demo/api/users/{userId}", putUser,
           PATH(Int32, userId),
           BODY_DTO(UserDto::ObjectWrapper, userDto)) {
    userDto->id = userId;
    return createDtoResponse(Status::CODE_200, m_database->updateUser(userDto));
  }
  
  
  ENDPOINT_INFO(getUserById) {
    info->summary = "Get one User by userId";
    info->addResponse<UserDto::ObjectWrapper>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ENDPOINT("GET", "demo/api/users/{userId}", getUserById,
           PATH(Int32, userId)) {
    auto user = m_database->getUserById(userId);
    OATPP_ASSERT_HTTP(user, Status::CODE_404, "User not found");
    return createDtoResponse(Status::CODE_200, user);
  }
  
  
  ENDPOINT_INFO(getUsers) {
    info->summary = "get all stored users";
    info->addResponse<List<UserDto::ObjectWrapper>::ObjectWrapper>(Status::CODE_200, "application/json");
  }
  ENDPOINT("GET", "demo/api/users", getUsers) {
    return createDtoResponse(Status::CODE_200, m_database->getUsers());
  }
  
  
  ENDPOINT_INFO(deleteUser) {
    info->summary = "Delete User by userId";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ENDPOINT("DELETE", "demo/api/users/{userId}", deleteUser,
           PATH(Int32, userId)) {
    bool success = m_database->deleteUser(userId);
    OATPP_ASSERT_HTTP(success, Status::CODE_417, "User not deleted. Perhaps no such User in the Database");
    return createResponse(Status::CODE_200, "User successfully deleted");
  }
  
  /**
   *  Finish ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_END(ApiController)
  
};

#endif /* UserController_hpp */
