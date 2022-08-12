#include <optional>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast;         // <boost/beast.hpp>
namespace http = beast::http;           // <boost/beast/http.hpp>
namespace net = boost::asio;            // <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // <boost/asio/ip/tcp.hpp>


namespace ImageOperations
{
    std::optional<cv::Mat> flip_image(const std::vector<char>&);
}

class http_connection : public std::enable_shared_from_this<http_connection>
{
    static const unsigned BODY_LIMIT{ 8388608 }; // response body limit is 8 mb by default, so both request and response limits set 8 mb
    tcp::socket socket_;
    beast::flat_buffer buffer_{ 8388608 };
    http::request_parser<http::vector_body<uchar>> request_parser_{};
    http::response<http::vector_body<uchar>> response_{};

public:
    http_connection() = delete;
    http_connection(const http_connection&) = delete;
    http_connection(http_connection&&) = default;
    http_connection& operator=(const http_connection&) = delete;
    http_connection& operator=(http_connection&&) = default;
    ~http_connection() = default;

    http_connection(tcp::socket);
    void start();

private:
    static http::response<http::vector_body<uchar>> create_response(std::vector<uchar>, unsigned hhtp_version);

    void read_request_async();
    void process_request();
    void async_write();
};

void http_server(tcp::acceptor&, tcp::socket&);
