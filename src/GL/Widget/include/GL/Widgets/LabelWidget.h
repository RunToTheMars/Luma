#pragma once

#include "GL/Widget.h"
#include <memory>

namespace GL
{
class LabelWidgetImpl;
class LabelWidget : public GL::Widget
{
public:
  LabelWidget () noexcept;
  ~LabelWidget () noexcept override;

  void setText ();
  void setPosition ();

  void renderEvent () noexcept override;

private:
  std::unique_ptr<LabelWidgetImpl> mPimpl;
};
}  // namespace GL
