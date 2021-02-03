#ifndef HTTP_SERVER_CALLBACKS_H
#define HTTP_SERVER_CALLBACKS_H

#include <functional>

#include <memory>

namespace drogon {

class HttpRequest;
using HttpRequestPtr = std::shared_ptr<HttpRequest>;
class HttpResponse;
using HttpResponsePtr = std::shared_ptr<HttpResponse>;
class WebSocketConnectionImpl;
using WebSocketConnectionPtr = std::shared_ptr<WebSocketConnectionImpl>;

using HttpAsyncCallback = std::function<void(const HttpRequestPtr &,std::function<void(const HttpResponsePtr &)> &&)>;
using WebSocketNewAsyncCallback = std::function<void(const HttpRequestPtr &, std::function<void(const HttpResponsePtr &)> &&,
				const WebSocketConnectionPtr &)>;

}

#endif