//
//  MyApiClient.hpp
//  tls-libressl
//
//  Created by Leonid on 6/23/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef MyApiClient_hpp
#define MyApiClient_hpp

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/macro/codegen.hpp"

class MyApiClient : public oatpp::web::client::ApiClient {
#include OATPP_CODEGEN_BEGIN(ApiClient)
  
  API_CLIENT_INIT(MyApiClient)
  
  API_CALL("GET", "/get", apiGet)
  
  API_CALL_ASYNC("GET", "/get", apiGetAsync)
  
#include OATPP_CODEGEN_END(ApiClient)
};

#endif /* MyApiClient_hpp */
