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

#include "../../../lib/oatpp-lib/web/src/server/HttpError.hpp"
#include "../../../lib/oatpp-lib/web/src/server/api/ApiController.hpp"
#include "../../../lib/oatpp-lib/parser/src/json/mapping/ObjectMapper.hpp"
#include "../../../lib/oatpp-lib/core/src/macro/codegen.hpp"
#include "../../../lib/oatpp-lib/core/src/macro/component.hpp"

/**
 *  EXAMPLE ApiController
 *  Basic examples of howto create ENDPOINTs
 *  More details on oatpp.io
 */
class UserController : public oatpp::web::server::api::ApiController {
protected:
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
    return std::shared_ptr<UserController>(new UserController(objectMapper));
  }
  
  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)
  
  /**
   *  Insert Your endpoints here !!!
   */
  
  ENDPOINT("POST", "demo/api/users", createUser,
           BODY_DTO(UserDto::PtrWrapper, userDto)) {
    return createDtoResponse(Status::CODE_200, m_database->createUser(userDto));
  }
  
  ENDPOINT("PUT", "demo/api/users/{userId}", putUser,
           PATH(Int32, userId),
           BODY_DTO(UserDto::PtrWrapper, userDto)) {
    return createDtoResponse(Status::CODE_200, m_database->updateUser(userDto));
  }
  
  ENDPOINT("GET", "demo/api/users/{userId}", getUserById,
           PATH(Int32, userId)) {
    auto user = m_database->getUserById(userId);
    OATPP_ASSERT_HTTP(!user.isNull(), Status::CODE_404, "User not found");
    return createDtoResponse(Status::CODE_200, user);
  }
  
  ENDPOINT("GET", "demo/api/users", getUsers) {
    return createDtoResponse(Status::CODE_200, m_database->getUsers());
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
