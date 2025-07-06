#include "Common/Signal.h"
#include <gtest/gtest.h>

TEST (Signal, VoidNotify)
{
  Common::Signal<>::Connection connection;
  Common::Signal<> buttonClicked;

  int x = 0;
  connection = buttonClicked.connect ([&] { x++; });

  buttonClicked.notify ();
  EXPECT_EQ (x, 1);

  buttonClicked.notify ();
  EXPECT_EQ (x, 2);

  int y = 0;
  connection = buttonClicked.connect ([&] { y++; });

  buttonClicked.notify ();
  EXPECT_EQ (x, 2);
  EXPECT_EQ (y, 1);
}

TEST (Signal, IntNotify)
{
  Common::Signal<int>::Connection connection;
  Common::Signal<int> buttonClicked;

  int x = 0;
  connection = buttonClicked.connect ([&] (int val) { x = val; });

  buttonClicked.notify (1);
  EXPECT_EQ (x, 1);

  buttonClicked.notify (2);
  EXPECT_EQ (x, 2);

  int y = 0;
  connection = buttonClicked.connect ([&] (int val) { y = val; });

  buttonClicked.notify (1);
  EXPECT_EQ (x, 2);
  EXPECT_EQ (y, 1);
}

TEST (Signal, TwoNotify)
{
  Common::Signal<> buttonClicked;

  int x = 0;
  Common::Signal<>::Connection connectionX = buttonClicked.connect ([&] { x++; });

  buttonClicked.notify ();
  EXPECT_EQ (x, 1);

  int y = 0;
  Common::Signal<>::Connection connectionY = buttonClicked.connect ([&] { y--; });

  buttonClicked.notify ();
  EXPECT_EQ (x, 2);
  EXPECT_EQ (y, -1);
}

TEST (Signal, DisconnectedNotify)
{
  Common::Signal<> buttonClicked;

  int x = 0;
  Common::Signal<>::Connection connection = buttonClicked.connect ([&] { x++; });

  buttonClicked.notify ();
  EXPECT_EQ (x, 1);

  auto disconnector = connection.temporaryDisconnect ();

  buttonClicked.notify ();
  EXPECT_EQ (x, 1);
}

TEST (Signal, ConnectionLost)
{
  Common::Signal<> buttonClicked = Common::Signal<> ();

  int x = 0;
  {
    Common::Signal<>::Connection connection = buttonClicked.connect ([&] { x++; });
  }

  buttonClicked.notify ();
  EXPECT_EQ (x, 0);
}
