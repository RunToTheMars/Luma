#include "Common/Signal.h"
#include <gtest/gtest.h>

TEST (Signal, OneNotify)
{
  utils::Signal<> buttonClicked;

  int x = 0;
  utils::Signal<>::Connection connection = buttonClicked.connect ([&] { x++; });

  buttonClicked.notify ();
  EXPECT_EQ (x, 1);

  buttonClicked.notify ();
  EXPECT_EQ (x, 2);
}

TEST (Signal, OneNotifyArg)
{
  utils::Signal<int> buttonClicked;

  int x = 0;
  utils::Signal<int>::Connection connection = buttonClicked.connect ([&] (int signal) { x = signal; });

  buttonClicked.notify (10);
  EXPECT_EQ (x, 10);

  buttonClicked.notify (30);
  EXPECT_EQ (x, 30);
}

TEST (Signal, TwoNotify)
{
  utils::Signal<> buttonClicked;

  int x = 0;
  utils::Signal<>::Connection connectionX = buttonClicked.connect ([&] { x++; });

  buttonClicked.notify ();
  EXPECT_EQ (x, 1);

  int y = 0;
  utils::Signal<>::Connection connectionY = buttonClicked.connect ([&] { y--; });

  buttonClicked.notify ();
  EXPECT_EQ (x, 2);
  EXPECT_EQ (y, -1);
}

TEST (Signal, DisconnectedNotify)
{
  utils::Signal<> buttonClicked;

  int x = 0;
  utils::Signal<>::Connection connection = buttonClicked.connect ([&] { x++; });

  buttonClicked.notify ();
  EXPECT_EQ (x, 1);

  auto disconnector = connection.temporaryDisconnect ();

  buttonClicked.notify ();
  EXPECT_EQ (x, 1);
}

TEST (Signal, ConnectionLost)
{
  utils::Signal<> buttonClicked = utils::Signal<> ();

  int x = 0;
  {
    utils::Signal<>::Connection connection = buttonClicked.connect ([&] { x++; });
  }

  buttonClicked.notify ();
  EXPECT_EQ (x, 0);
}
