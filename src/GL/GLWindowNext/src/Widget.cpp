#include "GL/Widget.h"

namespace GL
{
class WidgetImpl
{
public:
  Widget *parent = nullptr;
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
};
}  // namespace GL

namespace GL
{
Widget::Widget (Widget *parent) : m_pimpl (std::make_unique<WidgetImpl> (parent)) {}
Widget::~Widget () {}

const Widget *Widget::parent () const { return m_pimpl->parent; }
Widget *Widget::parent () { return m_pimpl->parent; }

void Widget::setGeometry (int x, int y, int width, int height)
{
  int oldWidth = m_pimpl->width;
  int oldHeight = m_pimpl->height;

  m_pimpl->x = x;
  m_pimpl->y = y;
  m_pimpl->width = width;
  m_pimpl->height = height;

  resizeEvent (ResizeEvent (oldWidth, oldHeight, width, height));
}

int Widget::x () const { return m_pimpl->x; }
int Widget::y () const { return m_pimpl->y; }
int Widget::width () const { return m_pimpl->width; }
int Widget::height () const { return m_pimpl->height; }

void Widget::resizeEvent (const ResizeEvent & /*event*/) {}
void Widget::keyEvent (const KeyEvent & /*event*/) {}
void Widget::mouseEvent (const MouseEvent & /*event*/) {}
void Widget::renderEvent () {}
}
