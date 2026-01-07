#include "SyncDaytimeClientTcp.h"

#include <iostream>
#include <array>
#include <boost/asio.hpp>

namespace 
{

  namespace asio = boost::asio;

}

void SyncDaytimeClientTcp::run()
{
  // Start the server with the command:
  // > nc -l 8080

  constexpr char hostName[] = "localhost";
  // constexpr char serviceName[] = "daytime";
  constexpr char serviceName[] = "8080";
  try {
    asio::io_context io;
  
    using tcp = asio::ip::tcp;
    // using asio::ip::tcp; // <<<<<
    tcp::resolver resolver(io);
    tcp::resolver::results_type endpoints = resolver.resolve(hostName, serviceName);
    tcp::socket socket(io);
    asio::connect(socket, endpoints);
    // tcp::endpoint ep( asio::ip::make_address("127.0.0.1"), 8080);
    // socket.connect(ep);

    while (true)
    {
      std::array<char, 128> buf;
      boost::system::error_code err;

      size_t len = socket.read_some(asio::buffer(buf), err);
      if (err == asio::error::eof) {
        std::cout << "Connection closed cleanly by peer" << std::endl;
        return;
      }
      else if (err){
        throw boost::system::system_error(err);
      }

      std::cout.write(buf.data(), len);
    }    
  }
  catch(const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
