//
//  Utils.cpp
//  AsyncApi
//
//  Created by Leonid on 3/31/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#include "Utils.hpp"

#include <fstream>

oatpp::base::String::PtrWrapper loadFromFile(const char* fileName) {
  
  std::ifstream file (fileName, std::ios::in|std::ios::binary|std::ios::ate);
  
  if (file.is_open()) {
    
    auto result = oatpp::base::String::createShared((v_int32) file.tellg());
    file.seekg(0, std::ios::beg);
    file.read((char*)result->getData(), result->getSize());
    file.close();
    return result;
    
  }
  
  return oatpp::base::String::PtrWrapper::empty();
  
}
