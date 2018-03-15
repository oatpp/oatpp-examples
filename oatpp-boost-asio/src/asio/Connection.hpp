//
//  Connection.hpp
//  oatpp-boost-asio
//
//  Created by Leonid on 3/15/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef asio_Connection_hpp
#define asio_Connection_hpp

#include "../../../lib/oatpp-lib/core/src/data/stream/Stream.hpp"
#include <boost/asio/ip/tcp.hpp>

namespace asio {

/**
 *  Connection manages boost::asio::ip::tcp::socket and provides IOStream interface
 */
class Connection : public oatpp::base::Controllable, public oatpp::data::stream::IOStream {
public:
  typedef oatpp::os::io::Library Library;
public:
  /* Pool for fast allocations of Connection object and shared_ptr's control block */
  SHARED_OBJECT_POOL(Shared_Connection_Pool, Connection, 32);
private:
  std::shared_ptr<boost::asio::ip::tcp::socket> m_socket;
public:
  Connection(const std::shared_ptr<boost::asio::ip::tcp::socket>& socket)
  : m_socket(socket)
  {}
public:
  
  static std::shared_ptr<Connection> createShared(const std::shared_ptr<boost::asio::ip::tcp::socket>& socket){
    return Shared_Connection_Pool::allocateShared(socket);
  }
  
  ~Connection() {
    m_socket->close();
  }
  
  Library::v_size write(const void *buff, Library::v_size count) override {
    return m_socket->write_some(boost::asio::buffer(buff, count));
  }
  
  Library::v_size read(void *buff, Library::v_size count) override {
    try{
      return m_socket->read_some(boost::asio::buffer(buff, count));
    } catch(boost::system::system_error& error) {
      return 0;
    }
  }
  
};
  
}

#endif /* asio_Connection_hpp */
