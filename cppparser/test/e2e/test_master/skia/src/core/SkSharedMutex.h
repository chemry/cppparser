/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef SkSharedLock_DEFINED
#  define SkSharedLock_DEFINED
#  include "include/core/SkTypes.h"
#  include "include/private/SkMacros.h"
#  include "include/private/SkSemaphore.h"
#  include "include/private/SkThreadAnnotations.h"
#  include <atomic>
#  ifdef SK_DEBUG
#    include "include/private/SkMutex.h"
#    include <memory>
#  endif
// There are two shared lock implementations one debug the other is high performance. They implement
// an interface similar to pthread's rwlocks.
// This is a shared lock implementation similar to pthreads rwlocks. The high performance
// implementation is cribbed from Preshing's article:
// http://preshing.com/20150316/semaphores-are-surprisingly-versatile/
//
// This lock does not obey strict queue ordering. It will always alternate between readers and
// a single writer.
class SK_CAPABILITY("mutex") SkSharedMutex
{
public:
  SkSharedMutex();
  ~SkSharedMutex();
    // Acquire lock for exclusive use.
  void acquire();
    // Release lock for exclusive use.
  void release();
    // Fail if exclusive is not held.
  void assertHeld() const;
    // Acquire lock for shared use.
  void acquireShared();
    // Release lock for shared use.
  void releaseShared();
    // Fail if shared lock not held.
  void assertHeldShared() const;
private:
#  ifdef SK_DEBUG
  class ThreadIDSet;
  std::unique_ptr<ThreadIDSet> fCurrentShared;
  std::unique_ptr<ThreadIDSet> fWaitingExclusive;
  std::unique_ptr<ThreadIDSet> fWaitingShared;
  int fSharedQueueSelect{0};
  mutable SkMutex fMu;
  SkSemaphore fSharedQueue[2];
  SkSemaphore fExclusiveQueue;
#  else 
  std::atomic<int32_t> fQueueCounts;
  SkSemaphore fSharedQueue;
  SkSemaphore fExclusiveQueue;
#  endif
};
#  ifndef SK_DEBUG
inline void SkSharedMutex::assertHeld() const
{
}
inline void SkSharedMutex::assertHeldShared() const
{
}
#  endif
class SK_SCOPED_CAPABILITY SkAutoSharedMutexExclusive
{
public:
  explicit SkAutoSharedMutexExclusive(SkSharedMutex& lock)
    : fLock(lock)
  {
    lock.acquire();
  }
  ~SkAutoSharedMutexExclusive()
  {
    fLock.release();
  }
private:
  SkSharedMutex& fLock;
};
#  define SkAutoSharedMutexExclusive(...) SK_REQUIRE_LOCAL_VAR(SkAutoSharedMutexExclusive)
class SK_SCOPED_CAPABILITY SkAutoSharedMutexShared
{
public:
  explicit SkAutoSharedMutexShared(SkSharedMutex& lock)
    : fLock(lock)
  {
    lock.acquireShared();
  }
    // You would think this should be SK_RELEASE_SHARED_CAPABILITY, but SK_SCOPED_CAPABILITY
    // doesn't fully understand the difference between shared and exclusive.
    // Please review https://reviews.llvm.org/D52578 for more information.
  ~SkAutoSharedMutexShared()
  {
    fLock.releaseShared();
  }
private:
  SkSharedMutex& fLock;
};
#  define SkAutoSharedMutexShared(...) SK_REQUIRE_LOCAL_VAR(SkAutoSharedMutexShared)
#endif
