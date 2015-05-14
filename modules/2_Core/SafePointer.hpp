#ifndef AVR_SAFE_POINTER_HPP
#define AVR_SAFE_POINTER_HPP

#ifdef __cplusplus

#include <stdexcept>

namespace avr {

template <class T>
class SafePointer {
   public:
      typedef T TypePtr;

      //! Default and initialize constructor
      SafePointer(TypePtr* = 0x0);
      //! Copy construtor
      SafePointer(const SafePointer&);
      //! Copy constructor with conversion @pre template type Dt must be convertible to T (otherwise throws an exception)
      template <class Dt>
      SafePointer(SafePointer<Dt>&) throw(std::runtime_error&);
      //! Destructor, deletes the object if there is not another reference for it
      ~SafePointer();

      //! Assign operator
      SafePointer& operator = (TypePtr*);
      //! Copy operator
      SafePointer& operator = (const SafePointer&);
      //! Copy operator with conversion @pre template type Dt must be convertible to T (otherwise throws an exception)
      template <class Dt>
      SafePointer& operator = (SafePointer<Dt>&) throw (std::runtime_error&);

      //! Dereference operators, access pointer's content in same style of C (*ptr)
      TypePtr& operator * () throw(std::logic_error&);
      const TypePtr& operator * () const throw(std::logic_error&);
      //! Dereference member operators, access a member of the object in same style of C (ptr->x)
      TypePtr* operator -> () throw(std::logic_error&);
      const TypePtr* operator -> () const throw(std::logic_error&);

      //! Checks if pointer is null
      inline bool null() const { return this->obj == 0x0; }

      //! Unary operator of negation
      bool operator ! () const;
      //! Boolean operators that receive a object
      bool operator == (const SafePointer&) const;
      bool operator != (const SafePointer&) const;
      bool operator <= (const SafePointer&) const;
      bool operator >= (const SafePointer&) const;
      bool operator <  (const SafePointer&) const;
      bool operator >  (const SafePointer&) const;
      //! Boolean operators that receive a generic c-pointer
      bool operator == (TypePtr* p) const;
      bool operator != (TypePtr* p) const;
      bool operator <= (TypePtr* p) const;
      bool operator >= (TypePtr* p) const;
      bool operator <  (TypePtr* p) const;
      bool operator >  (TypePtr* p) const;

      inline TypePtr* Get() { return this->obj; }
      inline const TypePtr* Get() const { return this->obj; }
      inline const size_t* GetRefCount() const { return this->refCount; }

   private:
      TypePtr* obj;        // pointed object
      size_t*  refCount;   // reference counter for the object

      bool noReference() const;
      void newReference();
      void releases();
};

#if __cplusplus > 199711L
   template <class T> using SPtr = SafePointer<T>;
#endif

} //namespace avr

#endif // __cplusplus

#include "SafePointer.tcc"

#endif // AVR_SAFE_POINTER_HPP
