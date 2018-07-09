//
//  DemoController.hpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef DemoController_hpp
#define DemoController_hpp

#include "oatpp-consul/Client.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

/**
 *  EXAMPLE ApiController
 *  Basic examples of howto create ENDPOINTs
 *  More details on oatpp.io
 */
class DemoController : public oatpp::web::server::api::ApiController {
protected:
  DemoController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
private:
  
  /**
   *  Inject Database component
   */
  OATPP_COMPONENT(std::shared_ptr<oatpp::consul::Client>, m_consulClient);
public:
  
  /**
   *  Inject @objectMapper component here as default parameter
   *  Do not return bare Controllable* object! use shared_ptr!
   */
  static std::shared_ptr<DemoController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                 objectMapper)){
    return std::shared_ptr<DemoController>(new DemoController(objectMapper));
  }
  
  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)
  
  /**
   *  Insert Your endpoints here !!!
   */
  
  ENDPOINT("PUT", "demo/consul/kv/{key}", storeValue,
           PATH(String, key),
           BODY_STRING(String, value)) {
    m_consulClient->kvPut(key, value);
    return createResponse(Status::CODE_200, "value stored");
  }
  
  ENDPOINT("GET", "demo/consul/kv/{key}", getValue,
           PATH(String, key)) {
    return createResponse(Status::CODE_200, m_consulClient->kvGet(key));
  }

  
  /**
   *  Finish ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_END(ApiController)
  
};

#endif /* DemoController_hpp */
