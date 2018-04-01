//
//  Utils.hpp
//  AsyncApi
//
//  Created by Leonid on 3/31/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef Utils_hpp
#define Utils_hpp

#include "oatpp/core/concurrency/SpinLock.hpp"
#include "oatpp/core/base/String.hpp"

#include <unordered_map>
#include <string>

class StaticFilesManager {
private:
  oatpp::base::String::PtrWrapper m_basePath;
  oatpp::concurrency::SpinLock::Atom m_atom;
  std::unordered_map<std::string, std::shared_ptr<oatpp::base::String>> m_cache;
private:
  oatpp::base::String::PtrWrapper getExtension(const oatpp::base::String::PtrWrapper& filename);
public:
  
  StaticFilesManager(const oatpp::base::String::PtrWrapper& basePath)
    : m_basePath(basePath)
    , m_atom(false)
  {}
  
  std::shared_ptr<oatpp::base::String> getFile(const oatpp::base::String::PtrWrapper& path);
  
  oatpp::base::String::PtrWrapper guessMimeType(const oatpp::base::String::PtrWrapper& filename);
  
};

std::shared_ptr<oatpp::base::String> loadFromFile(const char* fileName);
oatpp::base::String::PtrWrapper formatText(const char* text, ...);
v_int64 getMillisTickCount();

#endif /* Utils_hpp */
