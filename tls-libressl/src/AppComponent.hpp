//
//  AppComponent.hpp
//  oatpp-web-starter
//
//  Created by Leonid on 3/2/18.
//  Copyright © 2018 lganzzzo. All rights reserved.
//

#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "client/MyApiClient.hpp"

#include "oatpp-libressl/server/ConnectionProvider.hpp"
#include "oatpp-libressl/client/ConnectionProvider.hpp"
#include "oatpp-libressl/Config.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"

#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"

#include "oatpp/network/client/SimpleTCPConnectionProvider.hpp"
#include "oatpp/network/server/SimpleTCPConnectionProvider.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/macro/component.hpp"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:
  
  /**
   *  Create ConnectionProvider component which listens on the port
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
    /* non_blocking connections should be used with AsyncHttpConnectionHandler for AsyncIO */
    auto config = oatpp::libressl::Config::createDefaultServerConfig ("key.pem", "cert.crt");
    return oatpp::libressl::server::ConnectionProvider::createShared(config, 8443, true /* true for non_blocking */);
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
    /* Async ConnectionHandler for Async IO and Coroutine based endpoints */
    return oatpp::web::server::AsyncHttpConnectionHandler::createShared(router);
  }());
  
  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    auto serializerConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
    auto deserializerConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();
    deserializerConfig->allowUnknownFields = false;
    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared(serializerConfig, deserializerConfig);
    return objectMapper;
  }());
  
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, sslClientConnectionProvider) ("clientConnectionProvider", [] {
    auto config = oatpp::libressl::Config::createShared();
    tls_config_insecure_noverifycert(config->getTLSConfig());
    tls_config_insecure_noverifyname(config->getTLSConfig());
    return oatpp::libressl::client::ConnectionProvider::createShared(config, "httpbin.org", 443);
    //return oatpp::network::client::SimpleTCPConnectionProvider::createShared("httpbin.org", 80);
  }());
  
  OATPP_CREATE_COMPONENT(std::shared_ptr<MyApiClient>, myApiClient)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, connectionProvider, "clientConnectionProvider");
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);
    auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(connectionProvider);
    return MyApiClient::createShared(requestExecutor, objectMapper);
  }());

};

#endif /* AppComponent_hpp */
