#include <httpResponse.h>

#include <buffer.h>
#include <string.h>
#include <stdio.h>
#include <string>


/// @brief 从外边提供一个buf, 将准备好的回应报文输出到buf中. 格式是正确的.
/// @param buf 
void HttpResponse::appendToBuffer(Buffer* buf) const {
    // 先是响应报文的状态行, 格式是固定的.
    char buf_[32];
    snprintf(buf_, sizeof buf, "HTTP/1.1 %d ", statusCode_);
    buf->append(buf_);
    buf->append(statusMessage_);
    buf->append("\r\n", 2);

    // 判断是否长链接
    if (closeConnection_) {
        buf->append("Connection: close\r\n");
    }
    else {
        snprintf(buf_, sizeof buf_, "Content-Length: %zd\r\n", body_.size());
        buf->append(buf_);
        buf->append("Connection: Keep-Alive\r\n");
    }

    // 这个是自定义的headers_， 自己想添加啥在main里添加。或者在onrequrest添加
    for (const auto& header : headers_) {
        buf->append(header.first);
        buf->append(": ");
        buf->append(header.second);
        buf->append("\r\n", 2);
    }

    buf->append("\r\n", 2);
    buf->append(body_); // 主题html内容
}

