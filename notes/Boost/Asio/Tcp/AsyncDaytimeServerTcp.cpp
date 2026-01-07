#include "AsyncDaytimeServerTcp.h"

#include <iostream>
#include <string>
#include <memory>
#include <boost/asio.hpp>

#include "Boost/Asio/Daytime.h"

namespace
{
  using boost::asio::ip::tcp;
  
  class TcpConnection: public std::enable_shared_from_this<TcpConnection>
  {
    public:
      using Pointer = std::shared_ptr<TcpConnection>;
    
      static std::shared_ptr<TcpConnection> create(boost::asio::io_context& io)
      {
        struct MakeSharedEnabler: public TcpConnection
          { MakeSharedEnabler(boost::asio::io_context& io): TcpConnection(io){} };

        return std::static_pointer_cast<TcpConnection>(std::make_shared<MakeSharedEnabler>(io));
        // return Pointer(new TcpConnection(io));
      }

      tcp::socket& socket()
      {
        return socket_;
      }

      void start()
      {
        message_ = daytime();

        boost::asio::async_write(socket_, boost::asio::buffer(message_),
          std::bind(&TcpConnection::handleWrite, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
          )
        );
      }

    private:
      tcp::socket socket_;
      std::string message_;

      TcpConnection(boost::asio::io_context& io)
      : socket_(io)
      {}

      void handleWrite(const boost::system::error_code& /* err */, size_t /* sz */)
      {}
    };

  class TcpServer
  {
    public:
      TcpServer(boost::asio::io_context& io)
      : io_(io)
      , acceptor_(io, tcp::endpoint(tcp::v4(), port_))
      {
        startAccept();
      }

    private:
      const boost::asio::ip::port_type port_{8080};
      boost::asio::io_context& io_;
      tcp::acceptor acceptor_;

      void startAccept()
      {
        TcpConnection::Pointer newConnection = TcpConnection::create(io_);
        acceptor_.async_accept(newConnection->socket(),
          std::bind(&TcpServer::handleAccept, this, newConnection,
            boost::asio::placeholders::error)
        );        
      }

      void handleAccept(TcpConnection::Pointer newConnection, const boost::system::error_code& err)
      {
        if (!err) {
          newConnection->start();
        }
        startAccept();
      }
  };
}

void AsyncDaytimeServerTcp::run()
{
  // After starting the server, run the client using the command:
  // > nc localhost 8080

  try {
    boost::asio::io_context io;
    TcpServer server(io); 
    io.run();
  }
  catch(const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
