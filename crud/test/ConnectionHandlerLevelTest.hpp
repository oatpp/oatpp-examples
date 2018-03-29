//
//  ConnectionHandlerLevelTest.hpp
//  crud
//
//  Created by Leonid on 3/13/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef ConnectionHandlerLevelTest_hpp
#define ConnectionHandlerLevelTest_hpp

#include "oatpp/test/UnitTest.hpp"

class ConnectionHandlerLevelTest : public oatpp::test::UnitTest {
public:
  
  ConnectionHandlerLevelTest() : UnitTest("TEST[ConnectionHandlerLevelTest]"){}
  bool onRun() override;
  
};

#endif /* ConnectionHandlerLevelTest_hpp */
