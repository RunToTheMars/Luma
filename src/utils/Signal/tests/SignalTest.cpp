#include <gtest/gtest.h>
#include "Signal/Signal.h"

TEST (Signal, test)
{
  utils::signal<> signal;
  signal.notify ();
}
