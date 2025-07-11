#include "Common/Signal.h"
#include <gtest/gtest.h>

TEST (Signal, VoidNotify)
{
  Common::Signal<> signal;

  int x = 0;

  Common::Slots slots;
  slots.connect (signal, [&] { x++; });

  signal.notify ();
  EXPECT_EQ (x, 1);

  signal.notify ();
  EXPECT_EQ (x, 2);

  int y = 0;
  slots.connect (signal, [&] { y++; });

  signal.notify ();
  EXPECT_EQ (x, 3);
  EXPECT_EQ (y, 1);
}

TEST (Signal, IntNotify)
{
  Common::Slots slots;
  Common::Signal<int> signal;

  int x = 0;
  slots.connect (signal, [&] (int i) { x = i; });

  signal.notify (1);
  EXPECT_EQ (x, 1);

  signal.notify (2);
  EXPECT_EQ (x, 2);

  int y = 0;
  slots.connect (signal, [&] (int i) { y = i; });

  signal.notify (3);
  EXPECT_EQ (x, 3);
  EXPECT_EQ (y, 3);
}

TEST (Signal, DisconnectAll)
{
  Common::Slots slots;
  Common::Signal<> signal;

  int x = 0;
  int y = 0;
  slots.connect (signal, [&] { x++; });
  slots.connect (signal, [&] { y++; });

  signal.notify ();
  EXPECT_EQ (x, 1);
  EXPECT_EQ (y, 1);

  {
    slots.setConnetionsEnabled (false);
    signal.notify ();
    EXPECT_EQ (x, 1);
    EXPECT_EQ (y, 1);
    slots.setConnetionsEnabled (true);
  }

  signal.notify ();
  EXPECT_EQ (x, 2);
  EXPECT_EQ (y, 2);
}

TEST (Signal, DisconnectSingle)
{
  Common::Slots slots;
  Common::Signal<> signal;

  int x = 0;
  int y = 0;
  std::weak_ptr<Common::Connection> connectionWptr = slots.connect (signal, [&] { x++; });
  slots.connect (signal, [&] { y++; });

  signal.notify ();
  EXPECT_EQ (x, 1);
  EXPECT_EQ (y, 1);

  {
    std::shared_ptr<Common::Connection> connectionSptr = connectionWptr.lock ();
    connectionSptr->setEnabled (false);
    signal.notify ();
    EXPECT_EQ (x, 1);
    EXPECT_EQ (y, 2);
    connectionSptr->setEnabled (true);
  }

  signal.notify ();
  EXPECT_EQ (x, 2);
  EXPECT_EQ (y, 3);
}

TEST (Signal, SlotsLost)
{
  Common::Signal<> signal = Common::Signal<> ();

  int x = 0;
  {
    Common::Slots slots;
    slots.connect (signal, [&] { x++; });

    signal.notify ();
    EXPECT_EQ (x, 1);
  }

  signal.notify ();
  EXPECT_EQ (x, 1);
}

TEST (Signal, SignalLost)
{
  Common::Slots slots;

  int x = 0;
  {
    Common::Signal<> signal = Common::Signal<> ();
    slots.connect (signal, [&] { x++; });

    signal.notify ();
    EXPECT_EQ (x, 1);
  }

  EXPECT_EQ (x, 1);
}
