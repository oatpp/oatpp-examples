//
//  AsyncAcceptor.hpp
//  oatpp-asio
//
//  Created by Leonid on 3/15/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef AsyncAcceptor_hpp
#define AsyncAcceptor_hpp

#include "./Connection.hpp"
#include "../../../lib/oatpp-lib/network/src/server/ConnectionHandler.hpp"

#include <boost/asio/io_service.hpp>

namespace asio {

class AsyncAcceptor {
private:
  boost::asio::ip::tcp::acceptor m_acceptor;
  std::shared_ptr<oatpp::network::server::ConnectionHandler> m_connectionHandler;
private:
  
  void doAccept() {
    auto socket = new boost::asio::ip::tcp::socket(m_acceptor.get_io_service());
    m_acceptor.async_accept(*socket, [this, socket](boost::system::error_code ec) {
      if(ec) {
        delete socket;
      }
      auto connection = asio::Connection::createShared(std::shared_ptr<boost::asio::ip::tcp::socket>(socket));
      m_connectionHandler->handleConnection(connection);
      doAccept();
    });
  }
  
public:
  AsyncAcceptor(boost::asio::io_service& ioservice,
                    const boost::asio::ip::tcp::endpoint& endpoint,
                    const std::shared_ptr<oatpp::network::server::ConnectionHandler> connectionHandler)
    : m_acceptor(boost::asio::ip::tcp::acceptor(ioservice, endpoint))
    , m_connectionHandler(connectionHandler)
  {}
  
  void start() {
    m_acceptor.listen();
    doAccept();
  }
  
};
  
}

#endif /* AsyncAcceptor_hpp */
