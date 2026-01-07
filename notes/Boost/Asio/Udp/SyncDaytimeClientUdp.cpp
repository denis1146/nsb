#include "SyncDaytimeClientUdp.h"

#include <array>
#include <iostream>
#include <boost/asio.hpp>

void SyncDaytimeClientUdp::run()
{
  constexpr char serviceName[] = "8081";
  // constexpr char hostName[] = "localhost";

  try {
    using boost::asio::ip::udp;

    boost::asio::io_context io;

    udp::resolver resolver(io);
    udp::endpoint receiverEp = *resolver.resolve(udp::v4(), serviceName, "daytime").begin();

    udp::socket socket(io);
    socket.open(udp::v4());

    std::array<char, 1> sendBuf = {{0}};
    socket.send_to(boost::asio::buffer(sendBuf), receiverEp);

    std::array<char, 128> recvBuf;
    udp::endpoint senderEp;
    size_t len = socket.receive_from(boost::asio::buffer(recvBuf), senderEp);
    std::cout.write(recvBuf.data(), len);
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
