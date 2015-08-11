#ifndef AVR_BOUNDING_TCC
#define AVR_BOUNDING_TCC

#ifdef __cplusplus

using std::cout;
using std::cin;
using std::endl;

namespace avr {

template <typename Tp> BoundingRect<Tp>::BoundingRect() : data() {}
template <typename Tp> BoundingRect<Tp>::BoundingRect(Tp x, Tp y, Tp w, Tp h) : data(x, y, w, h) {}
template <typename Tp> BoundingRect<Tp>::BoundingRect(const Point2<Tp>& p, const Size<Tp>& s) : data(p, s) {}
template <typename Tp> BoundingRect<Tp>::BoundingRect(const Point2<Tp>& p1, const Point2<Tp>& p2) : data(p1, p2) {}
template <typename Tp> BoundingRect<Tp>::BoundingRect(const BoundingRect<Tp>& rect) : data(rect.data) {}
template <typename Tp> BoundingRect<Tp>::BoundingRect(const Rect<Tp>& _data) : data(_data) {}

template <typename Tp> template <typename _Tp> BoundingRect<Tp>::BoundingRect(const BoundingRect<_Tp>& rect) : data(rect.data) {}
template <typename Tp> template <typename _Tp> BoundingRect<Tp>::BoundingRect(const Rect<_Tp>& _data) : data(_data) {}

template <typename Tp> Tp BoundingRect<Tp>::GetArea() const { return (data.width * data.height); }
template <typename Tp> Point2f BoundingRect<Tp>::GetCenter() const { return Point2f((data.width - data.x)/2.f, (data.height - data.y)/2.f); }
template <typename Tp> Size<Tp> BoundingRect<Tp>::GetSize() const { return data.size(); }

template <typename Tp> bool BoundingRect<Tp>::Contains(const Point2<Tp>& p) const { return data.contains(p); }
template <typename Tp> bool BoundingRect<Tp>::Intersects(const BoundingRect<Tp>& rect) const {
   Rect<Tp> in = this->data & rect();
   return (in.width != 0);
}

template <typename Tp> BoundingRect<Tp>& BoundingRect<Tp>::operator = (const BoundingRect<Tp>& rect) { this->data = rect(); return * this; }
template <typename Tp> bool BoundingRect<Tp>::operator == (const BoundingRect<Tp>& rect) { return (this->data == rect()); }
template <typename Tp> bool BoundingRect<Tp>::operator != (const BoundingRect<Tp>& rect) { return (this->data != rect()); }

template <typename Tp> BoundingRect<Tp>& BoundingRect<Tp>::operator ()(Tp x, Tp y, Tp w, Tp h) {
   data.x = x; data.width = w;
   data.y = y; data.height = h;
   return * this;
}
template <typename Tp> BoundingRect<Tp>& BoundingRect<Tp>::operator ()(const Point2<Tp>& p, const Size<Tp>& s) {
   data.x = p.x; data.width = s.width;
   data.y = p.y; data.height = s.height;
   return * this;
}
template <typename Tp> BoundingRect<Tp>& BoundingRect<Tp>::operator ()(const Point2<Tp>& p1, const Point2<Tp>& p2) {
   data.x = std::min(p1.x, p2.x); data.width = std::max(p1.x, p2.x) - data.x;
   data.y = std::min(p1.y, p2.y); data.height = std::max(p1.y, p2.y) - data.y;
   return * this;
}

} // namespace avr

#endif // __cplusplus

#endif // AVR_BOUNDING_TCC
