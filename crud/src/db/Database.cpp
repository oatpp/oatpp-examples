//
//  Database.cpp
//  crud
//
//  Created by Leonid on 3/13/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#include "Database.hpp"

User Database::serializeFromDto(const UserDto::PtrWrapper& userDto){
  User user;
  if(!userDto->id.isNull()){
    user.id = userDto->id->getValue();
  }
  user.firstName = userDto->firstName;
  user.lastName = userDto->lastName;
  userDto->friends->forEach([&user](const oatpp::data::mapping::type::StringPtrWrapper& friendId){
    user.friends.push_back(friendId);
  });
  return user;
}

UserDto::PtrWrapper Database::deserializeToDto(const User& user){
  auto dto = UserDto::createShared();
  dto->id = user.id;
  dto->firstName = user.firstName;
  dto->lastName = user.lastName;
  auto it = user.friends.begin();
  while (it != user.friends.end()) {
    dto->friends->pushBack(*it++);
  }
  return dto;
}

UserDto::PtrWrapper Database::createUser(const UserDto::PtrWrapper& userDto){
  oatpp::concurrency::SpinLock lock(m_atom);
  auto user = serializeFromDto(userDto);
  user.id = m_idCounter++;
  m_usersById[user.id] = user;
  return deserializeToDto(user);
}

UserDto::PtrWrapper Database::updateUser(const UserDto::PtrWrapper& userDto){
  oatpp::concurrency::SpinLock lock(m_atom);
  auto user = serializeFromDto(userDto);
  if(user.id < 0){
    throw std::runtime_error("User Id cannot be less than 0");
  }
  m_usersById[user.id] = user;
  return deserializeToDto(user);
}

UserDto::PtrWrapper Database::getUserById(v_int32 id){
  oatpp::concurrency::SpinLock lock(m_atom);
  auto it = m_usersById.find(id);
  if(it == m_usersById.end()){
    return UserDto::PtrWrapper::empty();
  }
  return deserializeToDto(it->second);
}

oatpp::data::mapping::type::List<UserDto::PtrWrapper>::PtrWrapper Database::getUsers(){
  oatpp::concurrency::SpinLock lock(m_atom);
  auto result = oatpp::data::mapping::type::List<UserDto::PtrWrapper>::createShared();
  auto it = m_usersById.begin();
  while (it != m_usersById.end()) {
    result->pushBack(deserializeToDto(it->second));
    it++;
  }
  return result;
}

bool Database::deleteUser(v_int32 id){
  oatpp::concurrency::SpinLock lock(m_atom);
  auto it = m_usersById.find(id);
  if(it == m_usersById.end()){
    return false;
  }
  m_usersById.erase(it);
  return true;
}
