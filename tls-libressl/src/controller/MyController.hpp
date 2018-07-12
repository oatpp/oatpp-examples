//
//  MyController.hpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef MyController_hpp
#define MyController_hpp

#include "../client/MyApiClient.hpp"
#include "../dto/MyDTOs.hpp"

#include "oatpp/web/client/RequestExecutor.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

/**
 *  EXAMPLE ApiController
 *  Basic examples of howto create ENDPOINTs
 *  More details on oatpp.io
 */
class MyController : public oatpp::web::server::api::ApiController {
protected:
  typedef MyController __ControllerType;
public:
  OATPP_COMPONENT(std::shared_ptr<MyApiClient>, myApiClient);
protected:
  MyController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:
  
  /**
   *  Inject @objectMapper component here as default parameter
   *  Do not return bare Controllable* object! use shared_ptr!
   */
  static std::shared_ptr<MyController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                    objectMapper)){
    return std::shared_ptr<MyController>(new MyController(objectMapper));
  }
  
  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)
  
  /**
   *  Hello World endpoint Coroutine mapped to the "/" (root)
   */
  ENDPOINT_ASYNC("GET", "/", Root) {
    
    ENDPOINT_ASYNC_INIT(Root)
    
    /**
     *  Coroutine entrypoint act()
     *  returns Action (what to do next)
     */
    Action act() override {
      auto dto = HelloDto::createShared();
      dto->message = "Hello Async!";
      dto->server = Header::Value::SERVER;
      dto->userAgent = request->getHeader(Header::USER_AGENT);
      return _return(controller->createDtoResponse(Status::CODE_200, dto));
    }
    
  };
  
  /**
   *  Echo body endpoint Coroutine. Mapped to "/body/string".
   *  Returns body received in the request
   */
  ENDPOINT_ASYNC("GET", "/body/string", EchoStringBody) {
    
    ENDPOINT_ASYNC_INIT(EchoStringBody)
    
    Action act() override {
      /* return Action to start child coroutine to read body */
      return request->readBodyToStringAsync(this, &EchoStringBody::returnResponse);
    }
    
    Action returnResponse(const oatpp::String& body){
      /* return Action to return created OutgoingResponse */
      return _return(controller->createResponse(Status::CODE_200, body));
    }
    
  };
  
  /**
   *  Echo body endpoint Coroutine. Mapped to "/body/dto".
   *  Deserialize DTO reveived, and return same DTO
   *  Returns body as MessageDto received in the request
   */
  ENDPOINT_ASYNC("GET", "/body/dto", EchoDtoBody) {
    
    ENDPOINT_ASYNC_INIT(EchoDtoBody)
    
    Action act() override {
      return request->readBodyToDtoAsync<MessageDto>(this, &EchoDtoBody::returnResponse, controller->getDefaultObjectMapper());
    }
    
    Action returnResponse(const MessageDto::ObjectWrapper& body){
      return _return(controller->createDtoResponse(Status::CODE_200, body));
    }
    
  };
  
  ENDPOINT_ASYNC("GET", "/api/get", TestApiGet) {
    
    ENDPOINT_ASYNC_INIT(TestApiGet)
    
    Action act() override {
      return controller->myApiClient->apiGetAsync(this, &TestApiGet::onResponse);
    }
    
    Action onResponse(const std::shared_ptr<IncomingResponse>& response){
      return response->readBodyToStringAsync(this, &TestApiGet::returnResult);
    }
    
    Action returnResult(const oatpp::String& body) {
      return _return(controller->createResponse(Status::CODE_200, body));
    }
    
  };
  
  /**
   *  Finish ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_END(ApiController)
  
};

#endif /* MyController_hpp */
