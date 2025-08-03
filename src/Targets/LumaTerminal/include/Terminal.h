#pragma once

#include "Luma/Core/Window.h"
#include <memory>

class TerminalImpl;
class Terminal : public Luma::Core::Window
{
public:
  Terminal ();
  ~Terminal ();

  void renderEvent (const Luma::Core::RenderEvent &event) override;
  void keyEvent (const Luma::Core::KeyEvent &event) override;
  void charEvent (const Luma::Core::CharEvent &event) override;
  void mouseEvent (const Luma::Core::MouseEvent &event) override;
  void focusInEvent () override;
  void focusOutEvent () override;
  void scaleEvent (const Luma::Core::ScaleEvent &event) override;

private:
  std::unique_ptr<TerminalImpl> mPimpl;
};
