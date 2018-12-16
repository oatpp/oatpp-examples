//
//  HealthController.hpp
//  consul-integration
//
//  Created by Leonid on 7/3/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef HealthController_hpp
#define HealthController_hpp

#include "oatpp-consul/rest/DTOs.hpp"
#include "../dto/HealthDto.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

/**
 *  EXAMPLE ApiController
 *  Basic examples of howto create ENDPOINTs
 *  More details on oatpp.io
 */
class HealthController : public oatpp::web::server::api::ApiController {
protected:
  HealthController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:
  
  /**
   *  Inject @objectMapper component here as default parameter
   *  Do not return bare Controllable* object! use shared_ptr!
   */
  static std::shared_ptr<HealthController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                        objectMapper)){
    return std::shared_ptr<HealthController>(new HealthController(objectMapper));
  }
  
  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)
  
  /**
   *  Insert Your endpoints here !!!
   */
  
  ENDPOINT("GET", "check/health", healthCheck) {
    
    auto status = HealthDto::createShared();
    status->status = "healthy";
    
    OATPP_LOGD("health", "check");
    
    return createDtoResponse(Status::CODE_200, status);
  }
  
  /**
   *  Finish ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_END(ApiController)
  
};

#endif /* HealthController_hpp */
