//
//  DemoApiModels.hpp
//  ApiClient-Demo
//
//  Created by Leonid on 11/9/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef DemoApiModels_hpp
#define DemoApiModels_hpp

#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class MyRequestDto : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(MyRequestDto, Object)
  
  DTO_FIELD(String, message);
  DTO_FIELD(Int32, code);
  
};

class HttpBinResponseDto : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(HttpBinResponseDto, Object)
  
  DTO_FIELD(String, data);
  DTO_FIELD(Fields<String>::ObjectWrapper, headers);
  DTO_FIELD(String, method);
  DTO_FIELD(String, origin);
  DTO_FIELD(String, url);
  
};

#include OATPP_CODEGEN_END(DTO)

#endif /* DemoApiModels_hpp */
