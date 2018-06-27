//
//  DemoRequestExecutor.hpp
//  ApiClient-Demo
//
//  Created by Leonid on 3/14/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef DemoRequestExecutor_hpp
#define DemoRequestExecutor_hpp

#include "oatpp/web/client/RequestExecutor.hpp"
#include "oatpp/core/data/stream/ChunkedBuffer.hpp"

/**
 *  RequestExecutor receives params from ApiClient, executes request and returns Response
 *  DemoRequestExecutor will just print params to logs
 */
class DemoRequestExecutor : public oatpp::web::client::RequestExecutor {
public:
  std::shared_ptr<Response> execute(const String& method,
                                    const String& path,
                                    const std::shared_ptr<Headers>& userDefinedHeaders,
                                    const std::shared_ptr<Body>& body) override
  {
   
    OATPP_LOGD("Executor", "begin");
    OATPP_LOGD(method->c_str(), "path='%s'", path->c_str());
    
    auto allHeaders = Headers::createShared();
    
    /* add userDefinedHeaders to allHeaders if some */
    if(userDefinedHeaders) {
      auto curr = userDefinedHeaders->getFirstEntry();
      while (curr != nullptr) {
        allHeaders->put(curr->getKey(), curr->getValue());
        curr = curr->getNext();
      }
    }
    
    /* if body not null add body-defined headers to allHeaders */
    if(body) {
      body->declareHeaders(allHeaders);
    }
    
    /* print allHeaders which participate in request */
    if(allHeaders->count() > 0) {
      OATPP_LOGD("Headers", "");
      auto curr = allHeaders->getFirstEntry();
      while (curr != nullptr) {
        OATPP_LOGD(curr->getKey()->c_str(), curr->getValue()->c_str());
        curr = curr->getNext();
      }
    }
    
    /* print body data */
    if(body) {
      auto stream = oatpp::data::stream::ChunkedBuffer::createShared();
      body->writeToStream(stream);
      OATPP_LOGD("Body", "%s", stream->toString()->c_str());
    }
    
    OATPP_LOGD("Executor", "end\n");
    
    return nullptr;
    
  }
  
  virtual Action executeAsync(oatpp::async::AbstractCoroutine* parentCoroutine,
                              AsyncCallback callback,
                              const String& method,
                              const String& path,
                              const std::shared_ptr<Headers>& headers,
                              const std::shared_ptr<Body>& body) override {
    OATPP_LOGD("Executor", "AsyncCall::");
    execute(method, path, headers, body);
    return Action::_FINISH;
  }
  
};

#endif /* DemoRequestExecutor_hpp */
