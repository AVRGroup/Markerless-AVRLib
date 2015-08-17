#ifndef AVR_SAFE_POINTER_HPP
#define AVR_SAFE_POINTER_HPP

#ifdef __cplusplus

namespace avr {

template <class T>
class SPtr {
   public:
      typedef T TypePtr;

      //! Default and initialize constructor
      SPtr(TypePtr* = 0x0);
      //! Copy construtor
      SPtr(const SPtr&);
      //! Copy constructor with conversion @pre template type Dt must be convertible to T
      template <class Dt> SPtr(const SPtr<Dt>&);
      //! Conversion constructor @pre template type DTypePtr must be convertible to TypePtr
      template <class DTypePtr> SPtr(DTypePtr*);
      //! Destructor, deletes the object if there is not another reference for it
      virtual ~SPtr();

      //! Assign operator
      SPtr& operator = (TypePtr*);
      //! Copy operator
      SPtr& operator = (const SPtr&);
      //! Copy operator with conversion @pre template type Dt must be convertible to T
      template <class Dt> SPtr& operator = (const SPtr<Dt>&);
      //! Conversion operator by assignment @pre template type Dt must be convertible to T
      template <class DTypePtr> SPtr& operator = (DTypePtr*);

      //! Dereference operators, access pointer's content in same style of C (*ptr)
      TypePtr& operator * ();
      const TypePtr& operator * () const;
      //! Dereference member operators, access a member of the object in same style of C (ptr->x)
      TypePtr* operator -> ();
      const TypePtr* operator -> () const;

      //! Checks if pointer is null
      inline bool Null() const { return this->obj == 0x0; }

      //! Unary operator of negation
      bool operator ! () const;
      //! Boolean operators that receive a object
      bool operator == (const SPtr&) const;
      bool operator != (const SPtr&) const;
      bool operator <= (const SPtr&) const;
      bool operator >= (const SPtr&) const;
      bool operator <  (const SPtr&) const;
      bool operator >  (const SPtr&) const;
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

      // all classes of this template are friends with each other
      template <class Dt> friend class SPtr;

   private:
      TypePtr* obj;        // pointed object
      size_t*  refCount;   // reference counter for the object

      template <class DTypePtr>
      void create(DTypePtr*);
      void releases();
      void newReference();
      bool noReference() const;
};

} //namespace avr

#endif // __cplusplus

#include "impl/SafePointer.tcc"

#endif // AVR_SAFE_POINTER_HPP
