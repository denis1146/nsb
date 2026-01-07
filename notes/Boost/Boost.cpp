#include "Boost.h"

#include <boost/scope_exit.hpp>

#include "NoteHelpers.h"
#include "ProgramOptions/ProgramOptions.h"
#include "Spirit/Spirit.h"
#include "Asio/Asio.h"

void Boost::run()
{
  constexpr char noteTitle[] = "~~~ Boost ~~~";
  nsb::showNoteTitle(noteTitle);
  // return;
  BOOST_SCOPE_EXIT(&noteTitle) { nsb::showNoteTitle(noteTitle); } BOOST_SCOPE_EXIT_END;

  ProgramOptions::run();
  Spirit::run();
  Asio::run();
}
