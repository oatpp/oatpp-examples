# ApiClient-Demo Example
Example project of how-to use oatpp ```ApiClient``` and how it works

More about oat++:
- Website: [https://oatpp.io](https://oatpp.io)
- ```ApiClient``` docs: [https://oatpp.io/docs/component/api-client](https://oatpp.io/docs/component/api-client)
- Oat++ Repo: [https://github.com/oatpp/oatpp](https://github.com/oatpp/oatpp)

## About ApiClient

oatpp ```ApiClient``` is a mechanism which enables you to generate Web Api Clients in declarative manner.
Under the hood it uses provided ```RequestExecutor``` to perform http requests. Thus you are abstracted from the low-level http-client library implementation and can substitute any other http-client library at any time with zero code changes.  
*Roughly you may treat oatpp ```ApiClient``` as Java Retrofit for C++.*

In this example you can configure to use such RequestExecutors:
- [oatpp-curl](https://github.com/oatpp/oatpp-curl) - RequestExecutor for oatpp's ApiClient based on libcurl.
- ```oatpp::web::client::HttpRequestExecutor``` - oatpp out-of-the-box provided RequestExecutor

## Example overview

In this example you will find: 
- ```ApiClient``` built for http://httpbin.org/ web service.
- Simple (Synchronous) API calls example.
- Async API calls example processed with ```oatpp::async::Processor``` and ```oatpp::async::Coroutine```.

#### Files and Folders

```
- src/
   |- DemoApiClient.hpp     // ApiClient built for http://httpbin.org/ web service
   |- DemoApiModels.hpp     // DTOs objects for DemoApiClient
   |- SimpleExample.hpp     // Simple (Synchronous) API calls example
   |- AsyncExample.hpp      // Async API calls example
   |- main.cpp              // main is here
```

#### ApiClient declaration overview

Use ```API_CALL``` for simple (synchronous) calls.  
Use ```API_CALL_ASYNC``` for non-blocking Async calls.

```c++
class DemoApiClient : public oatpp::web::client::ApiClient {
#include OATPP_CODEGEN_BEGIN(ApiClient)
  
  API_CLIENT_INIT(DemoApiClient)
  
  ...
  
  API_CALL("GET", "get", doGet)
  API_CALL("POST", "post", doPost, BODY_STRING(String, body))
  
  ...
  
  API_CALL_ASYNC("GET", "get", doGetAsync)
  API_CALL_ASYNC("POST", "post", doPostAsync, BODY_STRING(String, body))

  ...
  
#include OATPP_CODEGEN_END(ApiClient)
};
```

#### Example calls overview

##### SimpleExample.hpp

```c++
{
  auto data = client->doGet()->readBodyToString();
  OATPP_LOGD(TAG, "[doGet] data='%s'", data->c_str());
}

{
  auto data = client->doPost("Some data passed to POST")->readBodyToString();
  OATPP_LOGD(TAG, "[doPost] data='%s'", data->c_str());
}
```

##### AsyncExample.hpp

```c++
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
```

#### Request executor configuration

In ```main.cpp``` method ```run```.
Try to substitute different ```RequestExecutors``` by switching from Curl to oatpp built-in request executor.

```c++

void run(){
  
  /* Create ObjectMapper for serialization of DTOs  */
  auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
  
  /* Create RequestExecutor which will execute ApiClient's requests */
  //auto requestExecutor = createOatppExecutor();  // <-- Uncomment this
  auto requestExecutor = createCurlExecutor();     // <-- Comment this
  
  /* DemoApiClient uses DemoRequestExecutor and json::mapping::ObjectMapper */
  /* ObjectMapper passed here is used for serialization of outgoing DTOs */
  auto client = DemoApiClient::createShared(requestExecutor, objectMapper);
  
  SimpleExample::runExample(client);
  AsyncExample::runExample(client);
  
}

```

## Requires

libcurl installed.

*you may build this example without libcurl but then you'll have to remove libcurl/otpp-curl references in the code manually*

## Build and run

1) Git-Clone examples repo:
```
$ git clone --recurse-submodules https://github.com/oatpp/oatpp-examples
```
2) CD to this example 
```
$ cd oatpp-examples/ApiClient-Demo/
```
3) Modify ```build_app.sh``` to contain proper paths to ```curl/include``` and ```curl/lib```

4) Build project
```
$ ./build_app.sh
```
This will just call g++

5) Run app
```
./run_app
```

enjoy!

## Note
If you can't build app.- probably you need to [install build-essential](https://www.google.com.ua/search?q=install+build-essentials)

### Xcode, MAC
Xcode project included

## More
- [oatpp-curl](https://github.com/oatpp/oatpp-curl) - RequestExecutor for oatpp's ApiClient based on libcurl.
- [oatpp-consul](https://github.com/oatpp/oatpp-consul) - oatpp-consul integration based on ```ApiClient```.
