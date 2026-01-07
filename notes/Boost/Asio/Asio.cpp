#include "Asio.h"

#include <boost/scope_exit.hpp>
#include <boost/asio.hpp>

#include "NoteHelpers.h"
#include "Timers/Timers.h"
#include "Tcp/SyncDaytimeClientTcp.h"
#include "Tcp/SyncDaytimeServerTcp.h"
#include "Tcp/AsyncDaytimeServerTcp.h"
#include "Udp/SyncDaytimeClientUdp.h"
#include "Udp/AsyncDaytimeServerUdp.h"

void Asio::run()
{
  constexpr char noteTitle[] = "Asio";
  nsb::showNoteTitle(noteTitle);
  return;
  BOOST_SCOPE_EXIT(&noteTitle) { nsb::showNoteTitle(noteTitle); } BOOST_SCOPE_EXIT_END;

  // # Timers
  // asioTimers();

  // # TCP
  // SyncDaytimeClientTcp::run();  // > nc -l 8080
  // SyncDaytimeServerTcp::run();  // > nc localhost 8080
  // AsyncDaytimeServerTcp::run(); // > nc 127.0.0.1 8080

  // # UDP
  // SyncDaytimeClientUdp::run();  //
  // SyncDaytimeServerUdp::run();  // > nc -u localhost 8081
  // AsyncDaytimeServerUdp::run();  // > nc -u localhost 8081
}
