#include "AsyncDaytimeServerUdp.h"

#include <iostream>
#include <array>
#include <memory>
#include <boost/asio.hpp>

#include "Boost/Asio/Daytime.h"

namespace
{
  using boost::asio::ip::udp;

  class UdpServer
  {
    public:
      UdpServer(boost::asio::io_context& io)
      : socket_(io, udp::endpoint(udp::v4(), port_))
      {
        startReceive();
      }

    private:
      const boost::asio::ip::port_type port_{8081};
      udp::socket socket_;
      udp::endpoint remoteEp_;
      std::array<char, 1> recvBuf_;

      void startReceive()
      {
        socket_.async_receive_from(boost::asio::buffer(recvBuf_),
          remoteEp_,
          std::bind(&UdpServer::handleReceive, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred)
        );
      }

      void handleReceive(const boost::system::error_code err,
        std::size_t /* bytesTransferred */)
      {
        if (!err) {
          auto message = std::make_shared<std::string>(daytime());

          socket_.async_send_to(boost::asio::buffer(*message), remoteEp_,
            std::bind(&UdpServer::handleSend, this, message,
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred)
          );

          startReceive();
        }
      }

      void handleSend(std::shared_ptr<std::string> /* message */,
        const boost::system::error_code /* err */,
        std::size_t /* bytesTransferred */)
      {}
  };
}

void AsyncDaytimeServerUdp::run()
{
  // After starting the server, run the client using the command:
  // > nc localhost 8081

  try {
    boost::asio::io_context io;
    UdpServer server(io);
    io.run();
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
