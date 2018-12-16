//
//  AsyncExample.hpp
//  ApiClient-Demo
//
//  Created by Leonid on 11/9/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef AsyncExample_hpp
#define AsyncExample_hpp

#include "./DemoApiClient.hpp"

#include "oatpp/core/async/Processor.hpp"

class AsyncExample {
private:
  typedef oatpp::web::protocol::http::incoming::Response Response;
private:
  constexpr static const char* TAG = "AsyncExample";
private:
  
  class SendDtoCoroutine : public oatpp::async::Coroutine<SendDtoCoroutine> {
  private:
    std::shared_ptr<DemoApiClient> m_client;
    oatpp::String m_message;
    v_int32 m_code;
  public:
    
    SendDtoCoroutine(const std::shared_ptr<DemoApiClient> client,
                    const oatpp::String& message,
                    v_int32 code)
      : m_client(client)
      , m_message(message)
      , m_code(code)
    {}
    
    /**
     * Create DTO and send it via doPostWithDtoAsync method
     */
    Action act() override {
      auto dto = MyRequestDto::createShared();
      dto->message = m_message;
      dto->code = m_code;
      return m_client->doPostWithDtoAsync(this, &SendDtoCoroutine::onResponse, dto);
    }
    
    /**
     * Receive response and read body asynchronously
     */
    Action onResponse(const std::shared_ptr<Response>& response) {
      return response->readBodyToStringAsync(this, &SendDtoCoroutine::onBody);
    }
    
    /**
     * Print read body from the stream and finish
     */
    Action onBody(const oatpp::String& body) {
      OATPP_LOGD(TAG, "[doPostWithDtoAsync] data='%s'", body->c_str());
      return finish();
    }
    
  };
  
  class SendCoroutine : public oatpp::async::Coroutine<SendCoroutine> {
  private:
    std::shared_ptr<DemoApiClient> m_client;
  public:
    
    SendCoroutine(const std::shared_ptr<DemoApiClient> client) : m_client(client) {}

    Action act() override {
      return m_client->doPostAsync(this, &SendDtoCoroutine::onResponse, "<POST-DATA-HERE>");
    }
    
    Action onResponse(const std::shared_ptr<Response>& response) {
      return response->readBodyToStringAsync(this, &SendDtoCoroutine::onBody);
    }
    
    Action onBody(const oatpp::String& body) {
      OATPP_LOGD(TAG, "[SendCoroutine. doPostAsync] data='%s'", body->c_str());
      return finish();
    }
    
  };
  
public:
  
  void static runExample(const std::shared_ptr<DemoApiClient>& client) {
    
    oatpp::async::Processor processor;
    
    processor.addCoroutine(SendDtoCoroutine::getBench().obtain(client, "message1", 10000));
    processor.addCoroutine(SendDtoCoroutine::getBench().obtain(client, "message2", 20000));
    processor.addCoroutine(SendDtoCoroutine::getBench().obtain(client, "message3", 30000));
    
    while (!processor.isEmpty()) {
      processor.iterate(1);
    }
    
  }
  
};

#endif /* AsyncExample_hpp */
