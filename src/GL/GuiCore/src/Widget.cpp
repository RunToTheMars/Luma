// #include "GL/Widget.h"
// #include "GL/KeyEvent.h"
// #include <algorithm>

// namespace GL
// {

// class WidgetImpl
// {
// public:
//   WidgetImpl (Window *parentWindow);
//   WidgetImpl ();
//   WidgetImpl ();

//   Widget *parentWidget = nullptr;
//   Window *parentWindow = nullptr;
// };

// Widget::Widget (): mPimpl (std::make_unique<WidgetImpl> ())
// {
// }

// Widget::Widget (Window &parentWindow): mPimpl (std::make_unique<WidgetImpl> ())
// {
// }

// Widget::Widget (Widget &parentWidget): mPimpl (std::make_unique<WidgetImpl> ())
// {
// }

// Widget::~Widget ()
// {
// }

// bool Widget::isVisible () const
// {
//   const Widget *widget = this;

//   while (widget)
//     {
//       if (!widget->mIsVisible)
//         return false;

//       widget = widget->mParent;
//     }

//   return true;
// }

// void Widget::setVisible (bool visible)
// {
//   mIsVisible = visible;
// }

// Geometry::Rect Widget::rect () const
// {
//   return mRect;
// }

// void Widget::setRect (const Geometry::Rect &rect)
// {
//   if (mRect == rect)
//     return;

//   mRect = rect;
//   rectChanged.notify (rect);
// }

// void Widget::addWidget (std::unique_ptr<Widget> widget)
// {
//   if (!widget)
//     return;

//   widget->mParent = this;
//   mChildrens.emplace_back (std::move (widget));
// }

// bool Widget::hasWidget (const Widget *widget) const
// {
//   auto it = std::find_if (mChildrens.begin (), mChildrens.end (), [&] (const std::unique_ptr<Widget> &children) { return children.get () == widget; });
//   return it != mChildrens.end ();
// }

// std::unique_ptr<Widget> Widget::releaseWidget (const Widget *widget)
// {
//   auto it = std::find_if (mChildrens.begin (), mChildrens.end (), [&] (const std::unique_ptr<Widget> &children) { return children.get () == widget; });
//   std::unique_ptr<Widget> releasedWidget = std::move (*it);
//   mChildrens.erase (it);
//   return releasedWidget;
// }

// void Widget::keyEventPrivate (const KeyEvent &event)
// {
//   if (!isVisible ())
//     return;

//   keyEvent (event);
// }

// // bool Widget::mouseEventPrivate (const MouseEvent &event)
// // {
// //   if (!mIsVisible)
// //     return false;

// //   if (!mRect.contains (event.position ().toPoint ()))
// //     return false;

// //   if (mouseEvent (event))
// //     return true;

// //   for (const std::unique_ptr<Widget> &children : mChildrens)
// //     {
// //       if (children->mouseEventPrivate (event))
// //         return true;
// //     }

// //   return false;
// // }

// void Widget::renderEventPrivate ()
// {
//   if (!isVisible ())
//     return;

//   renderEvent ();
//   for (const std::unique_ptr<Widget> &children : mChildrens)
//     children->renderEvent ();
// }

// // Geometry::Size Widget::size () const
// // {
// //   return mSize;
// // }

// // void Widget::setParent (Widget *parent)
// // {
// //   mParent = parent;
// // }

// // GL::Window *Widget::window () const
// // {
// //   if (mParent)
// //     return mParent->window ();

// //   return nullptr;
// // }

// // void Widget::resizeEvent (const GL::ResizeEvent &event)
// // {
// //   mSize = event.size ();
// // }

// void Widget::keyEvent (const GL::KeyEvent & /*event*/)
// {
// }

// // bool Widget::mouseEvent (const GL::MouseEvent & /*event*/)
// // {
// //   return false;
// // }

// void Widget::renderEvent ()
// {
// }
// }  // namespace GL
