//
//  UserDto.hpp
//  crud
//
//  Created by Leonid on 3/13/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef UserDto_hpp
#define UserDto_hpp

#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class UserDto : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(UserDto, Object)
  
  DTO_FIELD(Int32, id);
  DTO_FIELD(String, firstName, "first-name");
  DTO_FIELD(String, lastName, "last-name");
  DTO_FIELD(List<String>::ObjectWrapper, friends) = List<String>::createShared();
  
};

#include OATPP_CODEGEN_END(DTO)

#endif /* UserDto_hpp */
