//
//  DemoApiClient.hpp
//  ApiClient-Demo
//
//  Created by Leonid on 3/14/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef DemoApiClient_hpp
#define DemoApiClient_hpp

#include "oatpp/web/client/ApiClient.hpp"

#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO) // Begin DTO codegen section

class Entity : public oatpp::data::mapping::type::Object {
  DTO_INIT(Entity /*Class*/, Object /*Extends*/) // Object is the base class for all DTOs
  DTO_FIELD(Int64, id, "entityId");
};

class UserDto : public Entity {
  DTO_INIT(UserDto /*Class*/, Entity /*Extends*/)
  DTO_FIELD(String, name);
  DTO_FIELD(String, surname);
};

#include OATPP_CODEGEN_END(DTO) // end DTO codegen section


class DemoApiClient : public oatpp::web::client::ApiClient {
#include OATPP_CODEGEN_BEGIN(ApiClient)
  
  API_CLIENT_INIT(DemoApiClient)
  
  API_CALL("GET", "/", getRoot)
  
  API_CALL("POST", "/post", postData,
           HEADER(String, token, "x-auth-token"),
           BODY_STRING(String, data))
  
  API_CALL("POST", "/users", postUser,
           HEADER(String, token, "x-auth-token"),
           BODY_DTO(UserDto::ObjectWrapper, user))
  
  API_CALL("GET", "/users/offset/{offset}/limit/{limit}", getUsers,
           HEADER(String, token, "x-auth-token"),
           PATH(Int32, offset),
           PATH(Int32, limit),
           QUERY(String, name),
           QUERY(String, lastName, "last_name"),
           QUERY(Int32, age))
  
  API_CALL_ASYNC("GET", "/", getRootAsync)
  
  API_CALL_ASYNC("POST", "/post", postDataAsync,
                 HEADER(String, token, "x-auth-token"),
                 BODY_STRING(String, data))
  
#include OATPP_CODEGEN_END(ApiClient)
};

#endif /* DemoApiClient_hpp */
