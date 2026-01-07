#include "SyncDaytimeServerTcp.h"

#include <string>
#include <iostream>
#include <boost/asio.hpp>

#include "Boost/Asio/Daytime.h"

void SyncDaytimeServerTcp::run()
{
  // After starting the server, run the client using the command:
  // > nc localhost 8080

  using boost::asio::ip::tcp;
  constexpr boost::asio::ip::port_type port(8080);

  try {
    boost::asio::io_context io;
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), port));

    while (true) {
      tcp::socket socket(io);
      acceptor.accept(socket);

      const auto dt = daytime();

      boost::system::error_code ignoredErr;
      boost::asio::write(socket, boost::asio::buffer(dt), ignoredErr);
    }
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
}
