//
//  Database.hpp
//  crud
//
//  Created by Leonid on 3/13/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef Database_hpp
#define Database_hpp

#include "../dto/UserDto.hpp"
#include "model/User.hpp"

#include "../../../lib/oatpp/core/src/concurrency/SpinLock.hpp"
#include <unordered_map>

/**
 *  Trivial in-memory Database based on unordered_map container.
 *  For demo purposes only :)
 */
class Database {
private:
  oatpp::concurrency::SpinLock::Atom m_atom; ///< Atomic boolean for SpinLock
  v_int32 m_idCounter; ///< counter to generate userIds
  std::unordered_map<v_int32, User> m_usersById; ///< Map userId to User
private:
  User serializeFromDto(const UserDto::PtrWrapper& userDto);
  UserDto::PtrWrapper deserializeToDto(const User& user);
public:
  
  Database()
    : m_atom(false)
    , m_idCounter(0)
  {}
  
  UserDto::PtrWrapper createUser(const UserDto::PtrWrapper& userDto);
  UserDto::PtrWrapper updateUser(const UserDto::PtrWrapper& userDto);
  UserDto::PtrWrapper getUserById(v_int32 id);
  oatpp::data::mapping::type::List<UserDto::PtrWrapper>::PtrWrapper getUsers();
  bool deleteUser(v_int32 id);
  
};

#endif /* Database_hpp */
