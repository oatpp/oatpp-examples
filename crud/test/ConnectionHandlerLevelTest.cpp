//
//  ConnectionHandlerLevelTest.cpp
//  crud
//
//  Created by Leonid on 3/13/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#include "ConnectionHandlerLevelTest.hpp"

#include "../../lib/oatpp-lib/core/src/async/Routine.hpp"
#include "../../lib/oatpp-lib/core/src/async/Processor.hpp"
#include "../../lib/oatpp-lib/core/src/base/String.hpp"
#include "../../lib/oatpp-lib/core/test/Checker.hpp"


namespace {
  
  typedef oatpp::async::Routine Routine;
  typedef oatpp::async::Error Error;
  typedef oatpp::async::Action Action;
  typedef oatpp::async::Block Block;
  typedef oatpp::base::String String;
  
  Routine::Builder createRoutine(){
    return Routine::
    _do({
      [] {
        //OATPP_LOGD("R", "1.1");
        return nullptr;
      },
      nullptr
    })._then({
      [] {
        //OATPP_LOGD("R", "1.2");
        return Routine::_do({
          [] {
            //OATPP_LOGD("R", "1.2.1");
            //throw std::runtime_error("opachki!");
            return nullptr;
          },
          nullptr
        })._then({
          [] {
            //OATPP_LOGD("R", "1.2.2");
            return nullptr;
          },
          nullptr
        });
      },
      [] (const Error& error) {
        //OATPP_LOGD("R", "1.2_error");
        return nullptr;
      }
    })._then({
      [] {
        //OATPP_LOGD("R", "1.3");
        return nullptr;
      },
      nullptr
    });
  }
  
}



bool ConnectionHandlerLevelTest::onRun() {
  
  for(v_int32 i = 0; i < 1; i ++) {
  
    oatpp::async::Processor processor;
    {
      oatpp::test::PerformanceChecker checker("create");
      for(v_int32 i = 0; i < 100000; i ++) {
        processor.addRoutine(createRoutine());
      }
    }
    
    {
      oatpp::test::PerformanceChecker checker("calc");
      while(processor.iterate()){
      }
    }
    
  }
  
  /*
  for(v_int32 i = 0; i < 10000; i++) {
    {
      //OATPP_LOGD("AAA", "begin");
      //OATPP_LOGD("AAA", "1");
      //OATPP_LOGD("AAA", "2 %s", "huy");
      //OATPP_LOGD("AAA", "2.1");
      //OATPP_LOGD("AAA", "2.1.1");
      //OATPP_LOGD("AAA", "2.2 %s", "huy");
      //OATPP_LOGD("AAA", "2.3");
      //OATPP_LOGD("AAA", "3");
      //OATPP_LOGD("AAA", "3.1");
      auto str1 = String::createShared("begin");
      auto str2 = String::createShared("1");
      auto str3 = String::createShared("huy");
      auto str4 = String::createShared("2.1");
      auto str5 = String::createShared("2.1.1");
      auto str6 = String::createShared("huy");
      auto str7 = String::createShared("2.3");
      auto str8 = String::createShared("3");
      auto str9 = String::createShared("3.1");
    }
  }
   */
  
  //OATPP_LOGD(TAG, "counter=%d", Task::counter);
  
  return true;
}
