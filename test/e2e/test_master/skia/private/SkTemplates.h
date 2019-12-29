/*
 * Copyright 2006 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef SkTemplates_DEFINED
#  define SkTemplates_DEFINED
#  include "include/core/SkTypes.h"
#  include "include/private/SkMalloc.h"
#  include "include/private/SkTLogic.h"
#  include <string.h>
#  include <array>
#  include <cstddef>
#  include <memory>
#  include <new>
#  include <utility>
/** \file SkTemplates.h

    This file contains light-weight template classes for type-safe and exception-safe
    resource management.
*/

/**
 *  Marks a local variable as known to be unused (to avoid warnings).
 *  Note that this does *not* prevent the local variable from being optimized away.
 */
template <typename T>
inline void sk_ignore_unused_variable(const T&)
{
}
/**
 *  Returns a pointer to a D which comes immediately after S[count].
 */
template <typename D, typename S>
static D* SkTAfter(S* ptr, size_t count = 1)
{
  return reinterpret_cast<D*>(ptr + count);
}
/**
 *  Returns a pointer to a D which comes byteOffset bytes after S.
 */
template <typename D, typename S>
static D* SkTAddOffset(S* ptr, size_t byteOffset)
{
  return reinterpret_cast<D*>(reinterpret_cast<sknonstd::same_cv_t<char, D>*>(ptr) + byteOffset);
}
template <typename T, T* P>
struct SkFunctionWrapper
{
  template <typename... Args>
  auto operator()(Args&& args) const
  {
    return P(std::forward<Args>(args));
  }
};
/** \class SkAutoTCallVProc

    Call a function when this goes out of scope. The template uses two
    parameters, the object, and a function that is to be called in the destructor.
    If release() is called, the object reference is set to null. If the object
    reference is null when the destructor is called, we do not call the
    function.
*/
/*
template <typename T, void (*P)(T*)> class SkAutoTCallVProc
    : public std::unique_ptr<T, SkFunctionWrapper<skstd::remove_pointer_t<decltype(P)>, P>> {
public:
    SkAutoTCallVProc(T* obj)
        : std::unique_ptr<T, SkFunctionWrapper<skstd::remove_pointer_t<decltype(P)>, P>>(obj) {}

    operator T*() const { return this->get(); }
};
*/

/** Allocate an array of T elements, and free the array in the destructor
 */
template <typename T>
class SkAutoTArray
{
public:
  SkAutoTArray()
  {
  }
    /** Allocate count number of T elements
     */
  explicit SkAutoTArray(int count)
  {
    SkASSERT(count >= 0);
    if (count)
    {
      fArray.reset(new T[count]);
    }
    SkDEBUGCODE(fCount = count;)
  }
  SkAutoTArray(SkAutoTArray&& other)
    : fArray(std::move(other.fArray))
  {
    SkDEBUGCODE(fCount = other.fCount; other.fCount = 0;)
  }
  SkAutoTArray& operator=(SkAutoTArray&& other)
  {
    if (this != &other)
    {
      fArray = std::move(other.fArray);
      SkDEBUGCODE(fCount = other.fCount; other.fCount = 0;)
    }
    return *this;
  }
    /** Reallocates given a new count. Reallocation occurs even if new count equals old count.
     */
  void reset(int count)
  {
    *this = SkAutoTArray(count);
  }
    /** Return the array of T elements. Will be NULL if count == 0
     */
  T* get() const
  {
    return fArray.get();
  }
    /** Return the nth element in the array
     */
  T& operator[](int index) const
  {
    SkASSERT((unsigned) index < (unsigned) fCount);
    return fArray[index];
  }
private:
  std::unique_ptr<T[]> fArray;
  SkDEBUGCODE(int fCount = 0;)
};
/** Wraps SkAutoTArray, with room for kCountRequested elements preallocated.
 */
template <int kCountRequested, typename T>
class SkAutoSTArray
{
public:
  SkAutoSTArray(SkAutoSTArray&&) = delete;
  SkAutoSTArray(const SkAutoSTArray&) = delete;
  SkAutoSTArray& operator=(SkAutoSTArray&&);
  SkAutoSTArray& operator=(const SkAutoSTArray&);
    /** Initialize with no objects */
  SkAutoSTArray()
  {
    fArray = nullptr;
    fCount = 0;
  }
    /** Allocate count number of T elements
     */
  SkAutoSTArray(int count)
  {
    fArray = nullptr;
    fCount = 0;
    this->reset(count);
  }
  ~SkAutoSTArray()
  {
    this->reset(0);
  }
    /** Destroys previous objects in the array and default constructs count number of objects */
  void reset(int count)
  {
    T* start = fArray;
    T* iter = start + fCount;
    while (iter > start)
    {
      (--iter)->~T();
    }
    SkASSERT(count >= 0);
    if (fCount != count)
    {
      if (fCount > kCount)
      {
        SkASSERT((T*) fStorage != fArray);
        sk_free(fArray);
      }
      if (count > kCount)
      {
        fArray = (T*) sk_malloc_throw(count, sizeof(T));
      }
      else 
      {
        if (count > 0)
        {
          fArray = (T*) fStorage;
        }
        else 
        {
          fArray = nullptr;
        }
      }
      fCount = count;
    }
    iter = fArray;
    T* stop = fArray + count;
    while (iter < stop)
    {
      iter++  T;
    }
  }
    /** Return the number of T elements in the array
     */
  int count() const
  {
    return fCount;
  }
    /** Return the array of T elements. Will be NULL if count == 0
     */
  T* get() const
  {
    return fArray;
  }
  T* begin()
  {
    return fArray;
  }
  const T* begin() const
  {
    return fArray;
  }
  T* end()
  {
    return fArray + fCount;
  }
  const T* end() const
  {
    return fArray + fCount;
  }
    /** Return the nth element in the array
     */
  T& operator[](int index) const
  {
    SkASSERT(index < fCount);
    return fArray[index];
  }
#  if  defined(SK_BUILD_FOR_GOOGLE3)
private:
  static const int kMaxBytes = 4 * 1024;
  static const int kCount = kCountRequested * sizeof(T) > kMaxBytes ? kMaxBytes / sizeof(T) : kCountRequested;
#  else 
  static const int kCount = kCountRequested;
#  endif
  int fCount;
  T* fArray;
  char fStorage[kCount * sizeof(T)];
};
/** Manages an array of T elements, freeing the array in the destructor.
 *  Does NOT call any constructors/destructors on T (T must be POD).
 */
template <typename T>
class SkAutoTMalloc
{
public:
    /** Takes ownership of the ptr. The ptr must be a value which can be passed to sk_free. */
  explicit SkAutoTMalloc(T* ptr = nullptr)
    : fPtr(ptr)
  {
  }
    /** Allocates space for 'count' Ts. */
  explicit SkAutoTMalloc(size_t count)
    : fPtr(count ? (T*) sk_malloc_throw(count, sizeof(T)) : nullptr)
  {
  }
  SkAutoTMalloc(SkAutoTMalloc&&);
  SkAutoTMalloc& operator=(SkAutoTMalloc&&);
    /** Resize the memory area pointed to by the current ptr preserving contents. */
  void realloc(size_t count)
  {
    fPtr.reset(count ? (T*) sk_realloc_throw(fPtr.release(), count * sizeof(T)) : nullptr);
  }
    /** Resize the memory area pointed to by the current ptr without preserving contents. */
  T* reset(size_t count = 0)
  {
    fPtr.reset(count ? (T*) sk_malloc_throw(count, sizeof(T)) : nullptr);
    return this->get();
  }
  T* get() const
  {
    return fPtr.get();
  }
  operator T*()
  {
    return fPtr.get();
  }
  operator const T*() const
  {
    return fPtr.get();
  }
  T& operator[](int index)
  {
    return fPtr.get()[index];
  }
  const T& operator[](int index) const
  {
    return fPtr.get()[index];
  }
    /**
     *  Transfer ownership of the ptr to the caller, setting the internal
     *  pointer to NULL. Note that this differs from get(), which also returns
     *  the pointer, but it does not transfer ownership.
     */
  T* release()
  {
    return fPtr.release();
  }
private:
  std::unique_ptr<T, SkFunctionWrapper<void(void*), sk_free>> fPtr;
};
template <size_t kCountRequested, typename T>
class SkAutoSTMalloc
{
public:
  SkAutoSTMalloc()
    : fPtr(fTStorage)
  {
  }
  SkAutoSTMalloc(size_t count)
  {
    if (count > kCount)
    {
      fPtr = (T*) sk_malloc_throw(count, sizeof(T));
    }
    else 
    {
      if (count)
      {
        fPtr = fTStorage;
      }
      else 
      {
        fPtr = nullptr;
      }
    }
  }
  SkAutoSTMalloc(SkAutoSTMalloc&&) = delete;
  SkAutoSTMalloc(const SkAutoSTMalloc&) = delete;
  SkAutoSTMalloc& operator=(SkAutoSTMalloc&&);
  SkAutoSTMalloc& operator=(const SkAutoSTMalloc&);
  ~SkAutoSTMalloc()
  {
    if (fPtr != fTStorage)
    {
      sk_free(fPtr);
    }
  }
  T* reset(size_t count)
  {
    if (fPtr != fTStorage)
    {
      sk_free(fPtr);
    }
    if (count > kCount)
    {
      fPtr = (T*) sk_malloc_throw(count, sizeof(T));
    }
    else 
    {
      if (count)
      {
        fPtr = fTStorage;
      }
      else 
      {
        fPtr = nullptr;
      }
    }
    return fPtr;
  }
  T* get() const
  {
    return fPtr;
  }
  operator T*()
  {
    return fPtr;
  }
  operator const T*() const
  {
    return fPtr;
  }
  T& operator[](int index)
  {
    return fPtr[index];
  }
  const T& operator[](int index) const
  {
    return fPtr[index];
  }
  void realloc(size_t count)
  {
    if (count > kCount)
    {
      if (fPtr == fTStorage)
      {
        fPtr = (T*) sk_malloc_throw(count, sizeof(T));
        memcpy(fPtr, fTStorage, kCount * sizeof(T));
      }
      else 
      {
        fPtr = (T*) sk_realloc_throw(fPtr, count, sizeof(T));
      }
    }
    else 
    {
      if (count)
      {
        if (fPtr != fTStorage)
        {
          fPtr = (T*) sk_realloc_throw(fPtr, count, sizeof(T));
        }
      }
      else 
      {
        this->reset(0);
      }
    }
  }
private:
  static const size_t kCountWithPadding = SkAlign4(kCountRequested * sizeof(T)) / sizeof(T);
#  if  defined(SK_BUILD_FOR_GOOGLE3)
  static const size_t kMaxBytes = 4 * 1024;
  static const size_t kCount = kCountRequested * sizeof(T) > kMaxBytes ? kMaxBytes / sizeof(T) : kCountWithPadding;
#  else 
  static const size_t kCount = kCountWithPadding;
#  endif
  T* fPtr;
  union 
  {
    uint32_t fStorage32[SkAlign4(kCount * sizeof(T)) >> 2];
    T fTStorage[1];
  };
};
//////////////////////////////////////////////////////////////////////////////////////////////////

/**
 *  Pass the object and the storage that was offered during SkInPlaceNewCheck, and this will
 *  safely destroy (and free if it was dynamically allocated) the object.
 */
template <typename T>
void SkInPlaceDeleteCheck(T* obj, void* storage)
{
  if (storage == obj)
  {
    obj->~T();
  }
  else 
  {
    delete obj;
  }
}
/**
 *  Allocates T, using storage if it is large enough, and allocating on the heap (via new) if
 *  storage is not large enough.
 *
 *      obj = SkInPlaceNewCheck<Type>(storage, size);
 *      ...
 *      SkInPlaceDeleteCheck(obj, storage);
 */
template <typename T, typename... Args>
T* SkInPlaceNewCheck(void* storage, size_t size, Args&& args)
{
  return (sizeof(T) <= size) ? storage  T(std::forward<Args>(args)) : new T(std::forward<Args>(args));
}
/**
 * Reserves memory that is aligned on double and pointer boundaries.
 * Hopefully this is sufficient for all practical purposes.
 */
template <size_t N>
class SkAlignedSStorage
{
public:
  SkAlignedSStorage()
  {
  }
  SkAlignedSStorage(SkAlignedSStorage&&) = delete;
  SkAlignedSStorage(const SkAlignedSStorage&) = delete;
  SkAlignedSStorage& operator=(SkAlignedSStorage&&);
  SkAlignedSStorage& operator=(const SkAlignedSStorage&);
  size_t size() const
  {
    return N;
  }
  void* get()
  {
    return fData;
  }
  const void* get() const
  {
    return fData;
  }
private:
  union 
  {
    void* fPtr;
    double fDouble;
    char fData[N];
  };
};
/**
 * Reserves memory that is aligned on double and pointer boundaries.
 * Hopefully this is sufficient for all practical purposes. Otherwise,
 * we have to do some arcane trickery to determine alignment of non-POD
 * types. Lifetime of the memory is the lifetime of the object.
 */
template <int N, typename T>
class SkAlignedSTStorage
{
public:
  SkAlignedSTStorage()
  {
  }
  SkAlignedSTStorage(SkAlignedSTStorage&&) = delete;
  SkAlignedSTStorage(const SkAlignedSTStorage&) = delete;
  SkAlignedSTStorage& operator=(SkAlignedSTStorage&&);
  SkAlignedSTStorage& operator=(const SkAlignedSTStorage&);
    /**
     * Returns void* because this object does not initialize the
     * memory. Use placement new for types that require a cons.
     */
  void* get()
  {
    return fStorage.get();
  }
  const void* get() const
  {
    return fStorage.get();
  }
private:
  SkAlignedSStorage<sizeof(T)*N> fStorage;
};
using SkAutoFree = std::unique_ptr<void, SkFunctionWrapper<void(void*), sk_free>>;
template <typename C, std::size_t... Is>
std::array<skstd::result_of_t<C(std::size_t)>, sizeof...(Is)> SkMakeArrayFromIndexSequence(C c, skstd::index_sequence<Is...>)
{
  return {{c(Is)}};
}
template <size_t N, typename C>
std::array<skstd::result_of_t<C(std::size_t)>, N> SkMakeArray(C c)
{
  return SkMakeArrayFromIndexSequence(c, skstd::make_index_sequence < N > {});
}
#endif