//
//  User.hpp
//  crud
//
//  Created by Leonid on 3/13/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef db_User_hpp
#define db_User_hpp

#include "../../../../lib/oatpp/core/src/base/String.hpp"
#include "../../../../lib/oatpp/core/src/base/Environment.hpp"

#include <string>
#include <list>

/**
 *  Object of User stored in the Demo-Database
 */
class User {
public:
  v_int32 id;
  std::string firstName;
  std::string lastName;
  std::list<std::string> friends;
};

#endif /* db_User_hpp */
