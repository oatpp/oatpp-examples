//
//  MediaController.hpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef MediaController_hpp
#define MediaController_hpp

#include "../hls/Playlist.hpp"
#include "../Utils.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/web/server/HttpError.hpp"
#include "oatpp/web/protocol/http/outgoing/ChunkedBufferBody.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include <unordered_map>

/**
 *  EXAMPLE ApiController
 *  Basic examples of howto create ENDPOINTs
 *  More details on oatpp.io
 */
class MediaController : public oatpp::web::server::api::ApiController {
public:
  typedef MediaController __ControllerType;
protected:
  MediaController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:
  OATPP_COMPONENT(std::shared_ptr<StaticFilesManager>, staticFileManager);
  OATPP_COMPONENT(std::shared_ptr<Playlist>, livePlaylist);
public:
  
  /**
   *  Inject @objectMapper component here as default parameter
   *  Do not return bare Controllable* object! use shared_ptr!
   */
  static std::shared_ptr<MediaController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                      objectMapper)){
    return std::shared_ptr<MediaController>(new MediaController(objectMapper));
  }
  
  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)
  
  
  /**
   *  index.html endpoint
   */
  ENDPOINT_ASYNC("GET", "/", Root) {
    
    ENDPOINT_ASYNC_INIT(Root)
    
    const char* pageTemplate =
    "<html lang='en'>"
      "<head>"
        "<meta charset=utf-8/>"
      "</head>"
      "<body>"
        "<p>Hello Asynchronous Media-Stream!</p>"
        "<a href='media/live'>Checkout Live Stream</a></br>"
        "<a href='video/video1.mp4'>Play video1.mp4</a></br>"
        "<a href='video/video2.mp4'>Play video2.mp4</a>"
        "<p>"
          "Note:""</br>"
          "Http Live Streaming supported by Safari browser only.""</br>"
          "You may also use VLC player to play this stream"
        "</p>"
      "</body>"
    "</html>";
    
    Action act() override {
      return _return(controller->createResponse(Status::CODE_200, pageTemplate));
    }
    
  };
  
  /**
   *  VideoPage.html endpoint
   */
  ENDPOINT_ASYNC("GET", "video/*", Video) {
    
    ENDPOINT_ASYNC_INIT(Video)
    
    const char* pageTemplate =
    "<html lang='en'>"
      "<head>"
        "<meta charset=utf-8/>"
      "</head>"
      "<body>"
        "<div id='player'>"
          "<p>%s</p>"
          "<video width='640' height='400' controls>"
            "<source src='/media/%s' type='%s'>"
          "</video>"
        "</div>"
      "</body>"
    "</html>";
    
    Action act() override {
      auto filename = request->getPathTail();
      OATPP_ASSERT_HTTP(filename, Status::CODE_400, "Filename is empty");
      auto mime = controller->staticFileManager->guessMimeType(filename);
      OATPP_ASSERT_HTTP(mime, Status::CODE_500, "Can't guess file mime-type");
      auto file = controller->staticFileManager->getFile(request->getPathTail());
      OATPP_ASSERT_HTTP(file.get() != nullptr, Status::CODE_404, "File not found");
      auto page = formatText(pageTemplate, filename->c_str(), filename->c_str(), mime->c_str());
      return _return(controller->createResponse(Status::CODE_200, page));
    }
    
  };
  
  /**
   *  Live streaming playlist endpoint
   */
  ENDPOINT_ASYNC("GET", "media/live", Live) {
    
    ENDPOINT_ASYNC_INIT(Live)
    
    static v_int64 getTime0(){
      static v_int64 time0 = getMillisTickCount();
      return time0;
    }
    
    Action act() override {
      v_int64 time = getMillisTickCount() - getTime0();
      auto response = controller->createResponse(Status::CODE_200, controller->livePlaylist->generateForTime(time, 5));
      response->headers->put("Accept-Ranges", "bytes");
      response->headers->put(Header::CONNECTION, Header::Value::CONNECTION_KEEP_ALIVE);
      response->headers->put(Header::CONTENT_TYPE, "application/x-mpegURL");
      return _return(response);
    }
    
  };
  
  /**
   *  Media files download endpoint with support for Range requests
   */
  ENDPOINT_ASYNC("GET", "media/*", Static) {
    
    ENDPOINT_ASYNC_INIT(Static)
    
    Action act() override {
      
      auto filename = request->getPathTail();
      OATPP_ASSERT_HTTP(filename, Status::CODE_400, "Filename is empty");
      
      //OATPP_LOGD("Server", "Request filename='%s'", filename->c_str());
      
      auto file = controller->staticFileManager->getFile(filename);
      
      OATPP_ASSERT_HTTP(file.get() != nullptr, Status::CODE_404, "File not found");
      
      std::shared_ptr<OutgoingResponse> response;
      
      auto rangeStr = request->getHeader(Header::RANGE);
      if(!rangeStr) {
        response = getFullFileResponse(file);
      } else {
        response = getRangeResponse(rangeStr, file);
      }
      
      response->headers->put("Accept-Ranges", "bytes");
      response->headers->put(Header::CONNECTION, Header::Value::CONNECTION_KEEP_ALIVE);
      auto mimeType = controller->staticFileManager->guessMimeType(request->getPathTail());
      if(mimeType) {
        response->headers->put(Header::CONTENT_TYPE, mimeType);
      } else {
        OATPP_LOGD("Server", "Unknown Mime-Type. Header not set");
      }
      
      return _return(response);
    }
    
    std::shared_ptr<OutgoingResponse> getFullFileResponse(const oatpp::String& file) {
      //OATPP_LOGD("Server", "fullfile");
      return controller->createResponse(Status::CODE_200, file);
    }

    std::shared_ptr<OutgoingResponse> getRangeResponse(const oatpp::String& rangeStr,
                                                       const oatpp::String& file) {
      
      auto range = oatpp::web::protocol::http::Range::parse(rangeStr.getPtr());
      
      if(range.end == 0) {
        range.end = file->getSize() - 1;
      }
      
      OATPP_ASSERT_HTTP(range.isValid() &&
                        range.start < file->getSize() &&
                        range.end > range.start &&
                        range.end < file->getSize(), Status::CODE_416, "Range is invalid");
      
      auto chunk = oatpp::String((const char*)&file->getData()[range.start], (v_int32)(range.end - range.start + 1), false);
      
      auto response = controller->createResponse(Status::CODE_206, chunk);
      
      oatpp::web::protocol::http::ContentRange contentRange(oatpp::web::protocol::http::ContentRange::UNIT_BYTES,
                                                            range.start, range.end, file->getSize(), true);
      
      OATPP_LOGD("Server", "range=%s", contentRange.toString()->c_str());
      
      response->headers->put(Header::CONTENT_RANGE, contentRange.toString());
      return response;
      
    }
    
  };
  
  
  /**
   *  Finish ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_END(ApiController)
  
};

#endif /* MediaController_hpp */
