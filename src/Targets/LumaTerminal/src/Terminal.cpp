#include "Terminal.h"
#include "TerminalImpl.inl"

Terminal::Terminal (): Luma::Core::Window ({800, 600}, "Terminal"), mPimpl (std::make_unique<TerminalImpl> (*this))
{
}

Terminal::~Terminal () = default;

void Terminal::renderEvent (const Luma::Core::RenderEvent &event)
{
  mPimpl->renderEvent (event);
}

void Terminal::keyEvent (const Luma::Core::KeyEvent &event)
{
  mPimpl->keyEvent (event);
}

void Terminal::charEvent (const Luma::Core::CharEvent &event)
{
  mPimpl->charEvent (event);
}

void Terminal::mouseEvent (const Luma::Core::MouseEvent &event)
{
  mPimpl->mouseEvent (event);
}

void Terminal::focusInEvent ()
{
  mPimpl->focusInEvent ();
}

void Terminal::focusOutEvent ()
{
  mPimpl->focusOutEvent ();
}

void Terminal::scaleEvent (const Luma::Core::ScaleEvent &event)
{
  mPimpl->scaleEvent (event);
}
