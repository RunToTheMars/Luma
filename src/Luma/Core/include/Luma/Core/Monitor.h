#pragma once

#include "Luma/Core/Object.h"
#include "Luma/Geom/Rect.h"
#include "Luma/Geom/Vector.h"

namespace Luma::Core
{
using namespace Luma::Geom;

class Application;
class Monitor;
class MonitorList;
class Window;
class VideoMode;
class VideoModeList;

class Monitor: public Object
{
public:
  ~Monitor ();

  Vec2I pos () const;
  RectI workRect () const;
  Vec2I physicalSize () const;
  Vec2F contentScale () const;
  const char *name () const;

  VideoMode videoMode () const;
  VideoModeList videoModeList () const;

  bool operator== (const Monitor &rhs) const;

private:
  friend class Luma::Core::Application;
  friend class Luma::Core::MonitorList;
  friend class Luma::Core::Window;

  Monitor (void *impl);

  Monitor (const Monitor &) = delete;
  Monitor &operator= (const Monitor &) = delete;

private:
  void *mPimpl = nullptr;
};

class MonitorList
{
public:
  ~MonitorList ();

  int count () const;
  Monitor operator[] (int i) const;

private:
  MonitorList ();

  void *mPimpl = nullptr;
  int mCount;
};

class VideoMode
{
public:
  ~VideoMode ();

  int width () const;
  int height () const;
  Vec2I size () const;

  int redBits () const;
  int greenBits () const;
  int blueBits () const;

  int refreshRate () const;

private:
  friend class Monitor;
  friend class VideoModeList;
  VideoMode (const void *videoModeImpl);

  const void *mPimpl = nullptr;
};

class VideoModeList
{
public:
  ~VideoModeList ();

  int count () const;
  VideoMode operator[] (int i) const;

private:
  friend class Monitor;
  VideoModeList (void *monitorPimpl);

  const void *mPimpl = nullptr;
  int mCount;
};
}  // namespace GL
