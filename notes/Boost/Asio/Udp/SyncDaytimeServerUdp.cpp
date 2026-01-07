#include "SyncDaytimeServerUdp.h"

#include <array>
#include <iostream>
#include <boost/asio.hpp>

#include "Boost/Asio/Daytime.h"

void SyncDaytimeServerUdp::run()
{
  // After starting the server, run the client using the command:
  // > nc localhost 8081

  try {
    using boost::asio::ip::udp;
    
    boost::asio::io_context io;

    constexpr boost::asio::ip::port_type port(8081);
    udp::socket socket(io, udp::endpoint(udp::v4(), port));

    while (true) {
      std::array<char, 1> recvBuf;
      udp::endpoint remoteEp;
      socket.receive_from(boost::asio::buffer(recvBuf), remoteEp);

      auto message = daytime();

      boost::system::error_code ignoredErr;
      socket.send_to(boost::asio::buffer(message), remoteEp, 0, ignoredErr);
    }
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
