//
//  main.cpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

//#define OATPP_USE_TARGET
//#define OATPP_TARGET_TEST

//////////////////////////////////
// App

#include "./controller/DemoController.hpp"
#include "./AppComponent.hpp"
#include "./Logger.hpp"

//////////////////////////////////
// Test

#ifdef OATPP_TARGET_TEST
  #include "test/ControllerLevelTest.hpp"
  #include "test/RouterLevelTest.hpp"
  #include "test/ConnectionHandlerLevelTest.hpp"
#endif

//////////////////////////////////
// std

#include <iostream>

#include "oatpp-kafka/protocol/V0.hpp"
#include "oatpp-kafka/protocol/Produce.hpp"
#include "oatpp-kafka/protocol/mapping/ObjectMapper.hpp"

#include "oatpp-kafka/protocol/MessageSet.hpp"

#include "oatpp/network/client/SimpleTCPConnectionProvider.hpp"

#include "oatpp/algorithm/CRC.hpp"
#include "oatpp/encoding/Hex.hpp"

void sendMessage(oatpp::data::stream::OutputStream* stream, const oatpp::String& message) {
  v_int32 size = htonl(message->getSize());
  stream->write(&size, 4);
  stream->write(message->getData(), message->getSize());
}

void test() {
  
  oatpp::String str = " ";
  oatpp::String strHex(8);
  auto value = oatpp::algorithm::CRC32::calc(str->getData(), str->getSize());
  oatpp::encoding::Hex::writeWord32(value, strHex->getData());
  
  OATPP_LOGD("crc32", "value='%s'", strHex->c_str());
  
  auto serializerConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
  auto deserializerConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();
  
  auto jsonMapper = oatpp::parser::json::mapping::ObjectMapper::createShared(serializerConfig, deserializerConfig);
  
  auto cp = oatpp::network::client::SimpleTCPConnectionProvider::createShared("localhost", 9092);
  auto c = cp->getConnection();
  if(!c) {
    OATPP_LOGD("kafka", "can't connect");
    return;
  }
  
  auto mapper = oatpp::kafka::protocol::mapping::ObjectMapper::createShared();
  
  auto stream = oatpp::data::stream::ChunkedBuffer::createShared();
  /*auto request = oatpp::kafka::protocol::v0::MetadataRequest::createRequest();
  request->topics->pushBack("test");
  //request->topics->pushBack("demo");
  
  mapper->write(stream, request);
  
  sendMessage(c.get(), stream->toString());
   */
  
  auto request = oatpp::kafka::protocol::ProduceRequest::createRequest();
  request->requiredAcks = 1;
  request->timeout = 10000;
  request->topics = request->topics->createShared();
  
  auto topic = oatpp::kafka::protocol::TopicData::createShared();
  request->topics->pushBack(topic);
  topic->topicName = "test";
  topic->partitions = topic->partitions->createShared();
  
  auto partitionData = oatpp::kafka::protocol::ProduceRequestTopicPartitionData::createShared();
  topic->partitions->pushBack(partitionData);
  partitionData->partition = 0;
  
  auto messageSet = oatpp::kafka::protocol::MessageSet::createShared();
  partitionData->messageSet = messageSet;
  
  auto message = oatpp::kafka::protocol::MessageV0::createShared();
  messageSet->addMessage(message);
  
  message->crc = 0;
  message->attributes = 0;
  message->key = oatpp::kafka::protocol::Bytes(oatpp::base::StrBuffer::createShared("some key"));
  message->value = oatpp::kafka::protocol::Bytes(oatpp::base::StrBuffer::createShared("hello from oatpp!!!"));
  
  mapper->write(stream, request);
  
  auto json = jsonMapper->writeToString(request);
  OATPP_LOGD("kafka", "request='%s'", json->c_str());
  
  sendMessage(c.get(), stream->toString());
  
  auto buffer = oatpp::data::buffer::IOBuffer::createShared();
  v_int64 read = c->read(buffer->getData(), buffer->getSize());
  if(read == 0) {
    OATPP_LOGD("kafka", "zero response");
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  } else {
   
    oatpp::parser::ParsingCaret caret(&((p_char8)buffer->getData())[4], buffer->getSize() - 4);
    //auto response = mapper->readFromCaret<oatpp::kafka::protocol::v0::MetadataResponse>(caret);
    auto response = mapper->readFromCaret<oatpp::kafka::protocol::ProduceResponse>(caret);
    
    if(response) {
      auto json = jsonMapper->writeToString(response);
      OATPP_LOGD("kafka", "response='%s'", json->c_str());
    }
    
    
  }
  
  OATPP_LOGD("kafka", "response. size=%d", read);
  
}

/**
 *  run() method.
 *  1) set Environment components.
 *  2) add ApiController's endpoints to router
 *  3) run server
 */
void run() {
  
  AppComponent components; // Create scope Environment components
  
  /* create ApiControllers and add endpoints to router */
  
  auto router = components.httpRouter.getObject();
  
  auto DemoController = DemoController::createShared();
  DemoController->addEndpointsToRouter(router);
  
  /* create server */
  
  oatpp::network::server::Server server(components.serverConnectionProvider.getObject(),
                                        components.serverConnectionHandler.getObject());
  
  OATPP_LOGD("Server", "Running on port %u...", components.serverConnectionProvider.getObject()->getPort());
  
  server.run();
  
}

/**
 *  main
 */
int main(int argc, const char * argv[]) {
  
  oatpp::base::Environment::setLogger(new Logger());
  oatpp::base::Environment::init();
  
#if !defined(OATPP_USE_TARGET) | defined(OATPP_TARGET_APP)
  //test();
  run();
#endif
  
#ifdef OATPP_TARGET_TEST
  OATPP_RUN_TEST(ControllerLevelTest);
  OATPP_RUN_TEST(RouterLevelTest);
  OATPP_RUN_TEST(ControllerLevelTest);
#endif
  
  oatpp::base::Environment::setLogger(nullptr); ///< free Logger
  
  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
  
  oatpp::base::Environment::destroy();
  
  return 0;
}
