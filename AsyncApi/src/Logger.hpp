//
//  Logger.hpp
//  oatpp-web-starter
//
//  Created by Leonid on 3/2/18.
//  Copyright © 2018 lganzzzo. All rights reserved.
//

#ifndef Logger_hpp
#define Logger_hpp

#include "oatpp/core/concurrency/SpinLock.hpp"
#include "oatpp/core/base/Environment.hpp"

#include <iostream>

/**
 *  Environment logger.
 *  All logs from OATPP_LOGV(...), OATPP_LOGD(...), OATPP_LOGE(...) go here
 *  You may ignore or redirect them here
 */
class Logger : public oatpp::base::Logger {
private:
  oatpp::concurrency::SpinLock::Atom m_atom;
public:
  
  Logger()
    : m_atom(false)
  {}
  
  void log(v_int32 priority, const std::string& tag, const std::string& message) override {
    oatpp::concurrency::SpinLock lock(m_atom);
    std::cout << tag << ":" << message << "\n";
  }
  
};

#endif /* Logger_hpp */
