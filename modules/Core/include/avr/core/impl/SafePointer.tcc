#ifndef AVR_SAFE_POINTER_TCC
#define AVR_SAFE_POINTER_TCC

#ifndef AVR_CORE_HANDLING_HPP
    #include <avr/core/Handling.hpp>
#endif // AVR_CORE_HANDLING_HPP

#ifdef __cplusplus

#ifndef NULL
   #define NULL  0x0
#endif // NULL

#include <typeinfo>
#include <string>

namespace avr {

using std::string;

template <class T> SPtr<T>::SPtr(typename SPtr<T>::TypePtr* _obj) : obj(_obj), refCount(NULL) { this->newReference(); }
template <class T> SPtr<T>::SPtr(const SPtr<T>& ptr) : obj(ptr.obj), refCount(ptr.refCount) { this->newReference(); }
template <class T> template <class Dt> SPtr<T>::SPtr(const SPtr<Dt>& ptr) : obj(NULL), refCount(ptr.refCount) { this->create(ptr.obj); }
template <class T> template <class DTypePtr> SPtr<T>::SPtr(DTypePtr* _obj) : obj(NULL), refCount(NULL) { this->create(_obj); }
template <class T> SPtr<T>::~SPtr() { this->releases(); }

template <class T> inline SPtr<T>& SPtr<T>::operator = (typename SPtr<T>::TypePtr* _obj) {
   this->releases();
   this->obj = _obj;
   this->refCount = new size_t(1);
   return * this;
}
template <class T> inline SPtr<T>& SPtr<T>::operator = (const SPtr<T>& ptr) {
   this->releases();
   this->obj = ptr.obj;
   this->refCount = ptr.refCount;
   this->newReference();
   return * this;
}
template <class T> template <class Dt> inline SPtr<T>& SPtr<T>::operator = (const SPtr<Dt>& ptr) {
   this->releases();
   this->refCount = ptr.refCount;
   this->create(ptr.obj);
   return * this;
}
template <class T> template <class DTypePtr> inline SPtr<T>& SPtr<T>::operator = (DTypePtr* _obj) {
   this->releases();
   this->create(_obj);
   return * this;
}

template <class T> inline typename SPtr<T>::TypePtr& SPtr<T>::operator * () {
   if(this->null())
      AVR_ERROR(Cod::NullPointer, "Cannot access a null pointer");
   return * this->obj;
}
template <class T> inline typename SPtr<T>::TypePtr* SPtr<T>::operator -> () {
   if(this->null())
      AVR_ERROR(Cod::NullPointer, "Cannot access a null pointer");
   return this->obj;
}

template <class T> inline const typename SPtr<T>::TypePtr& SPtr<T>::operator * () const {
   if(this->null())
      AVR_ERROR(Cod::NullPointer, "Cannot access a null pointer");
   return * this->obj;
}
template <class T> inline const typename SPtr<T>::TypePtr* SPtr<T>::operator -> () const {
   if(this->null())
      AVR_ERROR(Cod::NullPointer, "Cannot access a null pointer");
   return this->obj;
}

//! Unary operator of negation
template <class T> inline bool SPtr<T>::operator ! () const { return !this->obj; }

//! Boolean operators with receive a object
template <class T> inline bool SPtr<T>::operator == (const SPtr<T>& p) const { return this->obj == p.obj; }
template <class T> inline bool SPtr<T>::operator != (const SPtr<T>& p) const { return this->obj != p.obj; }
template <class T> inline bool SPtr<T>::operator <= (const SPtr<T>& p) const { return this->obj <= p.obj; }
template <class T> inline bool SPtr<T>::operator >= (const SPtr<T>& p) const { return this->obj >= p.obj; }
template <class T> inline bool SPtr<T>::operator <  (const SPtr<T>& p) const { return this->obj < p.obj; }
template <class T> inline bool SPtr<T>::operator >  (const SPtr<T>& p) const { return this->obj > p.obj; }
//! Boolean operators with receive a generic c-pointer
template <class T> inline bool SPtr<T>::operator == (typename SPtr<T>::TypePtr* p) const { return this->obj == p; }
template <class T> inline bool SPtr<T>::operator != (typename SPtr<T>::TypePtr* p) const { return this->obj != p; }
template <class T> inline bool SPtr<T>::operator <= (typename SPtr<T>::TypePtr* p) const { return this->obj <= p; }
template <class T> inline bool SPtr<T>::operator >= (typename SPtr<T>::TypePtr* p) const { return this->obj >= p; }
template <class T> inline bool SPtr<T>::operator <  (typename SPtr<T>::TypePtr* p) const { return this->obj < p; }
template <class T> inline bool SPtr<T>::operator >  (typename SPtr<T>::TypePtr* p) const { return this->obj > p; }

//! Gets
template <class T> inline typename SPtr<T>::TypePtr* SPtr<T>::Get() { return this->obj; }
template <class T> inline const typename SPtr<T>::TypePtr* SPtr<T>::Get() const { return this->obj; }
template <class T> inline size_t SPtr<T>::Refs() const { return * this->refCount; }

//! Stream operator
template <class T> static inline std::ostream& operator << (std::ostream& out, const SPtr<T>& p) {
   if(p.null()) return (out << "|null; no reference|");
   return (out << "|" << p.Get() << "; " << p.Refs() << " references|");
}

//! Private methods implementation
template <class T> template <class DTypePtr> inline void SPtr<T>::create(DTypePtr* _obj) {
   if(_obj == NULL) return;

   this->obj = dynamic_cast<T*>(_obj);
   if(!this->obj)
      AVR_FMT_ERROR(Cod::BadCasting, "invalid cast from `%s' to `%s'", typeid(*_obj).name(), typeid(T).name());

   this->newReference();
}
template <class T> inline void SPtr<T>::releases() {
   if(this->noReference()) {
      delete this->obj;       this->obj = NULL;
      delete this->refCount;  this->refCount = NULL;
   } else {
      this->obj = NULL;
      (*this->refCount)--;
   }
}
template <class T> inline void SPtr<T>::newReference() {
   if(this->obj && this->refCount)
      (*this->refCount)++;
   else if(this->obj)
      this->refCount = new size_t(1);
}
template <class T> inline bool SPtr<T>::noReference() const {
   return (!this->refCount) || (*this->refCount) == 1;
}

} // namespace avr

#endif // __cplusplus

#endif // AVR_SAFE_POINTER_HPP
