//
//  ControllerLevelTest.hpp
//  crud
//
//  Created by Leonid on 3/13/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef ControllerLevelTest_hpp
#define ControllerLevelTest_hpp

#include "oatpp/test/UnitTest.hpp"

class ControllerLevelTest : public oatpp::test::UnitTest {
public:
  
  ControllerLevelTest() : UnitTest("TEST[ControllerLevelTest]"){}
  bool onRun() override;
  
};

#endif /* ControllerLevelTest_hpp */
