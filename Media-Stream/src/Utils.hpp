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
#include "oatpp/core/Types.hpp"

#include <unordered_map>
#include <string>

class StaticFilesManager {
private:
  oatpp::String m_basePath;
  oatpp::concurrency::SpinLock::Atom m_atom;
  std::unordered_map<oatpp::String, oatpp::String> m_cache;
private:
  oatpp::String getExtension(const oatpp::String& filename);
public:
  
  StaticFilesManager(const oatpp::String& basePath)
    : m_basePath(basePath)
    , m_atom(false)
  {}
  
  oatpp::String getFile(const oatpp::String& path);
  
  oatpp::String guessMimeType(const oatpp::String& filename);
  
};

oatpp::String loadFromFile(const char* fileName);
oatpp::String formatText(const char* text, ...);
v_int64 getMillisTickCount();

#endif /* Utils_hpp */
