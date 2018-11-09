//
//  SimpleExample.hpp
//  ApiClient-Demo
//
//  Created by Leonid on 11/9/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef SimpleExample_hpp
#define SimpleExample_hpp

#include "./DemoApiClient.hpp"

class SimpleExample {
private:
  constexpr static const char* TAG = "SimpleExample";
public:
  
  void static runExample(const std::shared_ptr<DemoApiClient>& client) {
    
    {
      auto data = client->doGet()->readBodyToString();
      OATPP_LOGD(TAG, "[doGet] data='%s'", data->c_str());
    }
    
    {
      auto data = client->doPost("Some data passed to POST")->readBodyToString();
      OATPP_LOGD(TAG, "[doPost] data='%s'", data->c_str());
    }
    
    {
      auto data = client->doGetAnything("path-parameter")->readBodyToString();
      OATPP_LOGD(TAG, "[doGetAnything] data='%s'", data->c_str());
    }
    
    {
      auto data = client->doPostAnything("path-parameter", "Some body here")->readBodyToString();
      OATPP_LOGD(TAG, "[doPostAnything] data='%s'", data->c_str());
    }
    
    {
      auto dto = MyRequestDto::createShared();
      dto->message = "Some message";
      dto->code = 200;
      auto data = client->doPostWithDto(dto)->readBodyToString();
      OATPP_LOGD(TAG, "[doPostWithDto] data='%s'", data->c_str());
    }
    
  }
  
};

#endif /* SimpleExample_hpp */
