#include <iostream>
#include <memory>
#include <iterator>
#include <utility>
#include "core.h"

std::optional<cv::Mat> ImageOperations::flip_image(const std::vector<char>& raw_bytes)
{
    if(raw_bytes.empty())
        return {};

    cv::Mat matrixJprg = cv::imdecode(cv::Mat(raw_bytes), 1);
    cv::Mat dst;
    cv::flip(matrixJprg, dst, 1);

    if(dst.empty())
        return {};

    return dst;
}

http_connection::http_connection(tcp::socket socket) : socket_(std::move(socket))
{
    request_parser_.body_limit(BODY_LIMIT);
}
void http_connection::start()
{
    read_request_async();
}
void http_connection::read_request_async()
{
    auto self = shared_from_this();
    http::async_read(socket_, buffer_, request_parser_, [self](beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);
        if (!ec)
            self->process_request();
        else
            std::cerr << ec.message() << std::endl;
    });
}
void http_connection::process_request()
{
    if (request_parser_.get().method() == http::verb::post)
    {
        std::vector<char> raw_bytes{request_parser_.get().body().cbegin(), request_parser_.get().body().cend()};
        auto opt_image = ImageOperations::flip_image(raw_bytes);
        if (opt_image)
        {
            std::vector<uchar> data;
            if (cv::imencode(".jpeg", std::move(opt_image.value()), data))
            {
                this->response_ = create_response(std::move(data), request_parser_.get().version());
                this->async_write();
            }
            else
                std::cerr << "Request " << request_parser_.get().method() << " failed: unable to encode the image into buffer";
            
            return;
        }
    }
    std::cerr << "Request " << request_parser_.get().method() << " failed: try POST request or check if the image sent exists " << std::endl;
}
http::response<http::vector_body<uchar>> http_connection::create_response(std::vector<uchar> body, unsigned hhtp_version)
{ 
    http::response<http::vector_body<uchar>> res { std::piecewise_construct,
                    std::make_tuple(std::move(body)),
                    std::make_tuple(http::status::ok, hhtp_version) };

    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "image/jpeg");
    res.content_length(res.body().size());

    return res;
}
void http_connection::async_write()
{
    auto self = shared_from_this();
    http::async_write(this->socket_, this->response_, [self](beast::error_code ec, std::size_t) { self->socket_.shutdown(tcp::socket::shutdown_send, ec); });
}


// "Loop" forever accepting new connections
void http_server(tcp::acceptor& acceptor, tcp::socket& socket)
{
    acceptor.async_accept(socket, [&](beast::error_code ec) {
        if (!ec)
            std::make_shared<http_connection>(std::move(socket))->start();
        http_server(acceptor, socket);
    });
}
