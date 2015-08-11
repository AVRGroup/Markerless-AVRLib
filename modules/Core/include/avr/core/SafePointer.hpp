#ifndef AVR_SAFE_POINTER_HPP
#define AVR_SAFE_POINTER_HPP

#ifdef __cplusplus

namespace avr {

template <class T>
class SafePointer {
   public:
      typedef T TypePtr;

      //! Default and initialize constructor
      SafePointer(TypePtr* = 0x0);
      //! Copy construtor
      SafePointer(const SafePointer&);
      //! Copy constructor with conversion @pre template type Dt must be convertible to T
      template <class Dt> SafePointer(const SafePointer<Dt>&);
      //! Conversion constructor @pre template type DTypePtr must be convertible to TypePtr
      template <class DTypePtr> SafePointer(DTypePtr*);
      //! Destructor, deletes the object if there is not another reference for it
      ~SafePointer();

      //! Assign operator
      SafePointer& operator = (TypePtr*);
      //! Copy operator
      SafePointer& operator = (const SafePointer&);
      //! Copy operator with conversion @pre template type Dt must be convertible to T
      template <class Dt> SafePointer& operator = (const SafePointer<Dt>&);
      //! Conversion operator by assignment @pre template type Dt must be convertible to T
      template <class DTypePtr> SafePointer& operator = (DTypePtr*);

      //! Dereference operators, access pointer's content in same style of C (*ptr)
      TypePtr& operator * ();
      const TypePtr& operator * () const;
      //! Dereference member operators, access a member of the object in same style of C (ptr->x)
      TypePtr* operator -> ();
      const TypePtr* operator -> () const;

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

      //! Getters
      TypePtr* Get();
      const TypePtr* Get() const;
      //! @return the number of references to same objetc
      size_t Refs() const;

      template <class Dt> friend class SafePointer;

   private:
      TypePtr* obj;        // pointed object
      size_t*  refCount;   // reference counter for the object

      template <class DTypePtr>
      void create(DTypePtr*);
      void releases();
      void newReference();
      bool noReference() const;
};

#if __cplusplus > 199711L // C++11
   template <class T> using SPtr = SafePointer<T>;
#endif

} //namespace avr

#endif // __cplusplus

#include "impl/SafePointer.tcc"

#endif // AVR_SAFE_POINTER_HPP
