//
//  DemoRequestExecutor.hpp
//  ApiClient-Demo
//
//  Created by Leonid on 3/14/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef DemoRequestExecutor_hpp
#define DemoRequestExecutor_hpp

#include "../../lib/oatpp-lib/web/src/client/RequestExecutor.hpp"
#include "../../lib/oatpp-lib/core/src/data/stream/ChunkedBuffer.hpp"

/**
 *  RequestExecutor receives params from ApiClient, executes request and returns Response
 *  DemoRequestExecutor will just print params to logs
 */
class DemoRequestExecutor : public oatpp::web::client::RequestExecutor {
public:
  std::shared_ptr<Response> execute(const String::SharedWrapper& method,
                                    const String::SharedWrapper& path,
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
};

#endif /* DemoRequestExecutor_hpp */
