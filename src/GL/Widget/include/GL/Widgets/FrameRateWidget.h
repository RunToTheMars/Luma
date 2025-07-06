#pragma once

#include "GL/Widget.h"
#include <memory>

namespace GL
{
class FrameRateWidgetImpl;
class FrameRateWidget : public GL::Widget
{
public:
  FrameRateWidget () noexcept;
  ~FrameRateWidget () noexcept override;

  void renderEvent () noexcept override;

private:
  std::unique_ptr<FrameRateWidgetImpl> mPimpl;
};
}
