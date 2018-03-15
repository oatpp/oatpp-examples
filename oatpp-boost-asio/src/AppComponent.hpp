//
//  AppComponent.hpp
//  oatpp-web-starter
//
//  Created by Leonid on 3/2/18.
//  Copyright © 2018 lganzzzo. All rights reserved.
//

#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "../../lib/oatpp-lib/web/src/server/HttpConnectionHandler.hpp"
#include "../../lib/oatpp-lib/web/src/server/HttpRouter.hpp"
#include "../../lib/oatpp-lib/network/src/server/SimpleTCPConnectionProvider.hpp"

#include "../../lib/oatpp-lib/parser/src/json/mapping/ObjectMapper.hpp"

#include "../../lib/oatpp-lib/core/src/macro/component.hpp"

#include <boost/asio/io_service.hpp>

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:
  
  /**
   *  Create boost::asio::ip::tcp::endpoint
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<boost::asio::ip::tcp::endpoint>, asioEndpoint)([] {
    return std::make_shared<boost::asio::ip::tcp::endpoint>(boost::asio::ip::tcp::v4(), 8000);
  }());
  
  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());
  
  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::server::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    return oatpp::web::server::HttpConnectionHandler::createShared(router);
  }());
  
  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    return oatpp::parser::json::mapping::ObjectMapper::createShared();
  }());

};

#endif /* AppComponent_hpp */
