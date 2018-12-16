//
//  DemoApiClient.hpp
//  ApiClient-Demo
//
//  Created by Leonid on 3/14/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef DemoApiClient_hpp
#define DemoApiClient_hpp

#include "DemoApiModels.hpp"

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

class DemoApiClient : public oatpp::web::client::ApiClient {
#include OATPP_CODEGEN_BEGIN(ApiClient)
  
  API_CLIENT_INIT(DemoApiClient)
  
  //-----------------------------------------------------------------------------------------------
  // Synchronous calls
  
  API_CALL_ASYNC("GET", "/", getRootAsync)
  
  API_CALL("GET", "get", doGet)
  API_CALL("POST", "post", doPost, BODY_STRING(String, body))
  API_CALL("PUT", "put", doPut, BODY_STRING(String, body))
  API_CALL("PATCH", "patch", doPatch, BODY_STRING(String, body))
  API_CALL("DELETE", "delete", doDelete)
  
  API_CALL("POST", "post", doPostWithDto, BODY_DTO(MyRequestDto::ObjectWrapper, body))
  
  API_CALL("GET", "anything/{parameter}", doGetAnything, PATH(String, parameter))
  API_CALL("POST", "anything/{parameter}", doPostAnything, PATH(String, parameter), BODY_STRING(String, body))
  API_CALL("PUT", "anything/{parameter}", doPutAnything, PATH(String, parameter), BODY_STRING(String, body))
  API_CALL("PATCH", "anything/{parameter}", doPatchAnything, PATH(String, parameter), BODY_STRING(String, body))
  API_CALL("DELETE", "anything/{parameter}", doDeleteAnything, PATH(String, parameter))
  
  //-----------------------------------------------------------------------------------------------
  // Same calls but async
  
  API_CALL_ASYNC("GET", "get", doGetAsync)
  API_CALL_ASYNC("POST", "post", doPostAsync, BODY_STRING(String, body))
  API_CALL_ASYNC("PUT", "put", doPutAsync, BODY_STRING(String, body))
  API_CALL_ASYNC("PATCH", "patch", doPatchAsync, BODY_STRING(String, body))
  API_CALL_ASYNC("DELETE", "delete", doDeleteAsync)
  
  API_CALL_ASYNC("POST", "post", doPostWithDtoAsync, BODY_DTO(MyRequestDto::ObjectWrapper, body))
  
  API_CALL_ASYNC("GET", "anything/{parameter}", doGetAnythingAsync, PATH(String, parameter))
  API_CALL_ASYNC("POST", "anything/{parameter}", doPostAnythingAsync, PATH(String, parameter), BODY_STRING(String, body))
  API_CALL_ASYNC("PUT", "anything/{parameter}", doPutAnythingAsync, PATH(String, parameter), BODY_STRING(String, body))
  API_CALL_ASYNC("PATCH", "anything/{parameter}", doPatchAnythingAsync, PATH(String, parameter), BODY_STRING(String, body))
  API_CALL_ASYNC("DELETE", "anything/{parameter}", doDeleteAnythingAsync, PATH(String, parameter))
  
#include OATPP_CODEGEN_END(ApiClient)
};

#endif /* DemoApiClient_hpp */
