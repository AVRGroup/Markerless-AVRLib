#ifndef AVR_SAFE_POINTER_HPP
   #include "SafePointer.hpp"
#endif // AVR_SAFE_POINTER_HPP

#ifdef __cplusplus

#ifndef NULL
   #define NULL  0x0
#endif // NULL

#define DEBUG 0

#if DEBUG
   #include <iostream>
   using namespace std;
#endif // DEBUG

#include <typeinfo>
#include <string>

namespace avr {

template <class T> SafePointer<T>::SafePointer(typename SafePointer<T>::TypePtr* _obj) : obj(_obj), refCount((_obj) ? new size_t(1) : NULL) {}
template <class T> SafePointer<T>::SafePointer(const SafePointer<T>& ptr) : obj(ptr.obj), refCount(ptr.refCount) { this->newReference(); }
template <class T> template <class Dt> SafePointer<T>::SafePointer(SafePointer<Dt>& ptr) throw(std::runtime_error&) : obj(NULL), refCount(NULL) {
   if(ptr.null()) return;

   this->obj = dynamic_cast<TypePtr*>(ptr.Get());
   if(!this->obj)
      throw std::runtime_error("avr::SafePointer.tcc: error: Invalid cast from type " + std::string(typeid(ptr.Get()).name()) + " to " + std::string(typeid(TypePtr*).name()) + " in constructor [SafePointer(SafePointer<Dt>&)]");

   this->refCount = (size_t*) ptr.GetRefCount();
   this->newReference();
}
template <class T> SafePointer<T>::~SafePointer() {
   #if DEBUG
      cout << "dtor\t" << ((this->null()) ? 0 : (*this->refCount)) << " references" << endl;
   #endif // DEBUG
   this->releases();
}

template <class T> inline SafePointer<T>& SafePointer<T>::operator = (typename SafePointer<T>::TypePtr* _obj) {
   this->releases();

   this->obj = _obj;
   this->refCount = new size_t(1);

   return * this;
}
template <class T> inline SafePointer<T>& SafePointer<T>::operator = (const SafePointer<T>& ptr) {
   this->releases();

   this->obj = ptr.obj;
   this->refCount = ptr.refCount;
   this->newReference();

   return * this;
}
template <class T> template <class Dt> inline SafePointer<T>& SafePointer<T>::operator = (SafePointer<Dt>& ptr) throw (std::runtime_error&) {
   this->releases();

   this->obj = dynamic_cast<TypePtr*>(ptr.Get());
   if(!this->obj)
      throw std::runtime_error("avr::SafePointer.tcc: error: Invalid cast from type " + std::string(typeid(ptr.Get()).name()) + " to " + std::string(typeid(TypePtr*).name()) + " in assign operator [= (avr::SafePointer<Dt>&)]");

   this->refCount = (size_t*) ptr.GetRefCount();
   this->newReference();

   return * this;
}

template <class T> inline typename SafePointer<T>::TypePtr& SafePointer<T>::operator * () throw(std::logic_error&) {
   if(this->null())
      throw std::logic_error("avr::SafePointer.tcc: error: Cannot access a null pointer in dereference operator [*()]\n");
   return * this->obj;
}
template <class T> inline typename SafePointer<T>::TypePtr* SafePointer<T>::operator -> () throw(std::logic_error&) {
   if(this->null())
      throw std::logic_error("avr::SafePointer.tcc: error: Cannot access a null pointer in dereference member operator [->()]\n");
   return this->obj;
}

template <class T> inline const typename SafePointer<T>::TypePtr& SafePointer<T>::operator * () const throw(std::logic_error&) {
   if(this->null())
      throw std::logic_error("avr::SafePointer.tcc: error: Cannot access a null pointer in const dereference operator [*()]\n");
   return * this->obj;
}
template <class T> inline const typename SafePointer<T>::TypePtr* SafePointer<T>::operator -> () const throw(std::logic_error&) {
   if(this->null())
      throw std::logic_error("avr::SafePointer.tcc: error: Cannot access a null pointer in const dereference member operator [->()]\n");
   return this->obj;
}

//! Unary operator of negation
template <class T> inline bool SafePointer<T>::operator ! () const { return !this->obj; }

//! Boolean operators with receive a object
template <class T> inline bool SafePointer<T>::operator == (const SafePointer<T>& p) const { return this->obj == p.obj; }
template <class T> inline bool SafePointer<T>::operator != (const SafePointer<T>& p) const { return this->obj != p.obj; }
template <class T> inline bool SafePointer<T>::operator <= (const SafePointer<T>& p) const { return this->obj <= p.obj; }
template <class T> inline bool SafePointer<T>::operator >= (const SafePointer<T>& p) const { return this->obj >= p.obj; }
template <class T> inline bool SafePointer<T>::operator <  (const SafePointer<T>& p) const { return this->obj < p.obj; }
template <class T> inline bool SafePointer<T>::operator >  (const SafePointer<T>& p) const { return this->obj > p.obj; }
//! Boolean operators with receive a generic c-pointer
template <class T> inline bool SafePointer<T>::operator == (typename SafePointer<T>::TypePtr* p) const { return this->obj == p; }
template <class T> inline bool SafePointer<T>::operator != (typename SafePointer<T>::TypePtr* p) const { return this->obj != p; }
template <class T> inline bool SafePointer<T>::operator <= (typename SafePointer<T>::TypePtr* p) const { return this->obj <= p; }
template <class T> inline bool SafePointer<T>::operator >= (typename SafePointer<T>::TypePtr* p) const { return this->obj >= p; }
template <class T> inline bool SafePointer<T>::operator <  (typename SafePointer<T>::TypePtr* p) const { return this->obj < p; }
template <class T> inline bool SafePointer<T>::operator >  (typename SafePointer<T>::TypePtr* p) const { return this->obj > p; }

//! Stream operator
template <class T> static inline std::ostream& operator << (std::ostream& out, const SafePointer<T>& p) {
   if(p.null()) return (out << "null  <- No reference");
   return (out << p.Get() << "  <- There are " << *(p.GetRefCount()) << " references");
}

//! Private methods implementation
template <class T> inline bool SafePointer<T>::noReference() const { return (!this->refCount) || (*this->refCount) == 1; }
template <class T> inline void SafePointer<T>::newReference()      { (*this->refCount)++; }
template <class T> inline void SafePointer<T>::releases() {
   if(this->noReference()) {
      #if DEBUG
         cout << "releses\n> no reference\n";
      #endif // DEBUG

      delete this->obj;       this->obj = NULL;
      delete this->refCount;  this->refCount = NULL;

      #if DEBUG
         cout << ">> deleted\n";
      #endif // DEBUG
   } else {
      #if DEBUG
         cout << "releses\n> has reference\n";
      #endif // DEBUG

      this->obj = NULL;
      (*this->refCount)--;

      #if DEBUG
         cout << ">> dereferenced\n";
      #endif // DEBUG
   }
}

} // namespace avr

#endif // __cplusplus
