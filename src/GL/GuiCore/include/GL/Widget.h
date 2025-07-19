// #pragma once

// #include "Common/Signal.h"
// #include "Geometry/Rect.h"
// #include <memory>
// #include <vector>

// namespace GL
// {
// class KeyEvent;
// class MouseEvent;

// class Widget
// {
// public:
//   Widget () noexcept;
//   virtual ~Widget () noexcept;

//   Widget (const Widget &) = delete;
//   Widget &operator= (const Widget &) = delete;

//   Widget (Widget &&) = delete;
//   Widget &operator= (Widget &&) = delete;

//   bool isVisible () const;
//   void setVisible (bool visible);

//   Geom::RectI rect () const;
//   void setRect (const Geom::RectI &rect);

//   void addWidget (std::unique_ptr<Widget> widget);
//   bool hasWidget (const Widget *widget) const;
//   std::unique_ptr<Widget> releaseWidget (const Widget *widget);

// public:
//   virtual void renderEvent ();
//   virtual void enterEvent ();
//   virtual void leaveEvent ();
//   virtual void hoverEvent (const Geom::Vec2D &pos);
//   virtual void keyEvent (const GL::KeyEvent &event);
//   virtual void keyEvent (const GL::MouseEvent &event);

// private:
//   Widget *mParent = nullptr;
//   bool mIsVisible = true;

//   Geom::RectI mRect;
//   std::vector<std::unique_ptr<Widget>> mChildrens;
// };

// }  // namespace GL
