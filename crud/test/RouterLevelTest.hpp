//
//  RouterLevelTest.hpp
//  crud
//
//  Created by Leonid on 3/13/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef RouterLevelTest_hpp
#define RouterLevelTest_hpp

#include "oatpp/test/UnitTest.hpp"
  
class RouterLevelTest : public oatpp::test::UnitTest {
public:
  
  RouterLevelTest() : UnitTest("TEST[RouterLevelTest]"){}
  bool onRun() override;
  
};

#endif /* RouterLevelTest_hpp */
