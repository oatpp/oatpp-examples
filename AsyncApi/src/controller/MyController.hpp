//
//  MyController.hpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef MyController_hpp
#define MyController_hpp

#include "../dto/MyDTOs.hpp"
#include "../Utils.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/web/server/HttpError.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include <unordered_map>

/**
 *  EXAMPLE ApiController
 *  Basic examples of howto create ENDPOINTs
 *  More details on oatpp.io
 */
class MyController : public oatpp::web::server::api::ApiController {
protected:
  MyController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:
  
  /**
   *  Inject @objectMapper component here as default parameter
   *  Do not return bare Controllable* object! use shared_ptr!
   */
  static std::shared_ptr<MyController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                      objectMapper)){
    return std::shared_ptr<MyController>(new MyController(objectMapper));
  }
  
  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)
  
  ENDPOINT_ASYNC("GET", "/favicon.ico", Favicon) {
    
    ENDPOINT_ASYNC_INIT(Favicon)
    
    Action act() override {
      return _return(controller->createResponse(Status::CODE_404, ""));
    }
    
  };
  
  ENDPOINT_ASYNC("GET", "/video", Video) {
    
    ENDPOINT_ASYNC_INIT(Video)
    
    const char* page =
    "<html lang='en'>"
      "<head>"
        "<meta charset=utf-8/>"
      "</head>"
      "<body>"
        "<div id='player'>"
          "<video width='352' height='198' controls>"
            "<source src='stream_1000k_48k_640x360' type='video/mp4'>"
          "</video>"
        "</div>"
      "</body>"
    "</html>";
    
    Action act() override {
      return _return(controller->createResponse(Status::CODE_200, page));
    }
    
  };
  
  ENDPOINT_ASYNC("GET", "/play_list", PlayList) {
    
    ENDPOINT_ASYNC_INIT(PlayList)
    
    static String::PtrWrapper& getFile() {
      static String::PtrWrapper file = loadFromFile("/Users/leonid/Documents/test/video/out2.m3u8");
      return file;
    }
    
    Action act() override {
      OATPP_LOGD("Server", "play_list");
      auto response = controller->createResponse(Status::CODE_200, getFile());
      response->headers->put(Header::CONTENT_TYPE, "application/x-mpegURL");
      response->headers->put("Access-Control-Allow-Origin", "*");
      response->headers->put("Access-Control-Allow-Headers", "Range");
      response->headers->put(Header::CONNECTION, Header::Value::CONNECTION_KEEP_ALIVE);
      return _return(response);
    }
    
  };
  
  ENDPOINT_ASYNC("GET", "/*", Stream) {
    
    ENDPOINT_ASYNC_INIT(Stream)
    
    oatpp::base::String::PtrWrapper currFile = nullptr;
    
    static oatpp::base::String::PtrWrapper getFileFromDisk(const oatpp::base::String::PtrWrapper& name) {
      oatpp::base::String::PtrWrapper dir = "/Users/leonid/Documents/test/video/";
      oatpp::base::String::PtrWrapper filename = dir + name;
      oatpp::base::String::PtrWrapper file = loadFromFile(filename->c_str());
      return file;
    }
    
    static std::unordered_map<std::string, oatpp::base::String::PtrWrapper>& getCache() {
      static std::unordered_map<std::string, oatpp::base::String::PtrWrapper> cache;
      return cache;
    }
    
    static oatpp::base::String::PtrWrapper getFile(const oatpp::base::String::PtrWrapper& name) {
      auto& file = getCache()[name->c_str()];
      if(!file.isNull()) {
        return file;
      }
      file = getFileFromDisk(name);
      return file;
    }
    
    Action act() override {
      if(!request->getPathTail()) {
        OATPP_LOGD("Server", "Null Tail:");
        auto currHeader = request->headers->getFirstEntry();
        while (currHeader != nullptr) {
          auto key = currHeader->getKey();
          auto val = currHeader->getValue();
          OATPP_LOGD("Header", "'%s': '%s'", key->c_str(), val->c_str());
          currHeader = currHeader->getNext();
        }
        OATPP_ASSERT_HTTP(request->getPathTail(), Status::CODE_400, "Range is invalid");
      }
      OATPP_LOGD("Server", "RequestFile='%s'", request->getPathTail()->c_str());
      currFile = getFile(request->getPathTail());
      
      auto rangeStr = request->getHeader(Header::RANGE);
      if(rangeStr.isNull()) {
        return startStream();
      }
      
      auto range = oatpp::web::protocol::http::Range::parse(rangeStr.getPtr());
      OATPP_ASSERT_HTTP(range.isValid(), Status::CODE_400, "Range is invalid");
      
      return continueStream(range);
    }
    
    Action startStream() {
      auto range = oatpp::web::protocol::http::Range(oatpp::web::protocol::http::Range::UNIT_BYTES, 0, 0);
      return continueStream(range);
    }
    
    Action continueStream(oatpp::web::protocol::http::Range& range) {
      //OATPP_LOGD("stream", "%d-%d", range.start, range.end);
      OATPP_ASSERT_HTTP(range.isValid(), Status::CODE_400, "Range is invalid");
      if(range.end == 0) {
        //range.end = range.start + 8192;
        range.end = currFile->getSize() - 1;
        if(range.end >= currFile->getSize()) {
          range.end = currFile->getSize() - 1;
        }
      }
      
      auto currHeader = request->headers->getFirstEntry();
      while (currHeader != nullptr) {
        auto key = currHeader->getKey();
        auto val = currHeader->getValue();
        OATPP_LOGD("Header", "'%s': '%s'", key->c_str(), val->c_str());
        currHeader = currHeader->getNext();
      }
      
      auto chunk = oatpp::base::String::createShared(&currFile->getData()[range.start], (v_int32)(range.end - range.start), false);
      
      auto response = controller->createResponse(Status::CODE_206, chunk);
      
      oatpp::web::protocol::http::ContentRange contentRange(oatpp::web::protocol::http::ContentRange::UNIT_BYTES,
                                                            range.start, range.end, currFile->getSize(), true);
      
      OATPP_LOGD("stream", "range=%s", contentRange.toString()->c_str());
      
      response->headers->put("Accept-Ranges", "bytes");
      response->headers->put(Header::CONTENT_TYPE, "video/MP2T");
      response->headers->put(Header::CONTENT_RANGE, contentRange.toString());
      response->headers->put(Header::CONNECTION, Header::Value::CONNECTION_KEEP_ALIVE);
      
      return _return(response);
      
    }
    
  };
  
  
  ENDPOINT_ASYNC("OPTIONS", "/*", All) {
    
    ENDPOINT_ASYNC_INIT(All)
    
    Action act() override {
      
      OATPP_LOGD("url", "\n'%s'\n", request->getPathTail()->c_str());
      
      auto currHeader = request->headers->getFirstEntry();
      while (currHeader != nullptr) {
        auto key = currHeader->getKey();
        auto val = currHeader->getValue();
        OATPP_LOGD("Header", "'%s': '%s'", key->c_str(), val->c_str());
        currHeader = currHeader->getNext();
      }
      return _return(controller->createResponse(Status::CODE_200, ""));
    }
    
  };
  
  
  /**
   *  Finish ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_END(ApiController)
  
};

#endif /* MyController_hpp */
