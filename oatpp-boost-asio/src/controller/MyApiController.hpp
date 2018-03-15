//
//  MyApiController.hpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef MyApiController_hpp
#define MyApiController_hpp

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
class MyApiController : public oatpp::web::server::api::ApiController {
protected:
  MyApiController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:
  
  /**
   *  Inject @objectMapper component here as default parameter
   *  Do not return bare Controllable* object! use shared_ptr!
   */
  static std::shared_ptr<MyApiController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                       objectMapper)){
    return std::shared_ptr<MyApiController>(new MyApiController(objectMapper));
  }
  
  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)
  
  /**
   *  Insert Your endpoints here !!!
   */
  
  ENDPOINT("GET", "/", root) {
    return createResponse(Status::CODE_200, "Hello Boost-Asio");
  }
  
  /**
   *  Finish ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_END(ApiController)
};

#endif /* MyApiController_hpp */
