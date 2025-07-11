#pragma once

#include "Geometry/Rect.h"
#include "Geometry/Vector.h"

namespace std
{
template <typename T>
class optional;
}

namespace GL
{
class Application;
class Monitor;
class MonitorList;
class Window;
class VideoMode;
class VideoModeList;

class Monitor
{
public:
  ~Monitor ();

  Geom::Vec2I pos () const;
  Geom::RectI workRect () const;
  Geom::Vec2I physicalSize () const;
  void contentScale (float *xScale, float *yScale) const;
  const char *name () const;

  GL::VideoMode videoMode () const;
  GL::VideoModeList videoModeList () const;

  bool operator== (const Monitor &rhs) const;

private:
  friend class GL::Application;
  friend class GL::MonitorList;
  friend class GL::Window;
  friend class std::optional<Monitor>;

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
