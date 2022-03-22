/*
 * ILockableObjectInterface.h
 *
 *  Created on: 06.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef ILOCKABLEOBJECTINTERFACE_H_
#define ILOCKABLEOBJECTINTERFACE_H_

#include <softel/core/CObject.h>

namespace SF {
namespace CORE {
namespace LOCK {

/*
 *
 */
class ILockableObjectInterface: public virtual SF::CORE::CObject {
public:
	/**
	 * Blocks the current thread until the object lock can be obtained. The lock
	 * must not be owned by the current thread. (It is not recursive.)
	 */
	virtual void lock() = 0;

	/**
	 * Unlocks the object. The current thread must already own the lock.
	 */
	virtual void unlock() = 0;

	/**
	 * Determine if the current thread owns the lock.
	 */
	virtual bool isLocked() const = 0;

};

/**
 *----------------------------------------------------------------------
 *
 * interface WaitableObject
 *
 *     Interface for objects on which threads can wait and be
 *     signaled. Essentially combines the properties of a
 *     ILockableObjectInterface with an internal conditional variable.
 *
 *----------------------------------------------------------------------
 */

class WaitableObject : virtual public ILockableObjectInterface {
public:
   /**
    * Blocks the calling thread until another thread signals
    * this object.
    */
   virtual void wait() = 0;

   /**
    * TimedWait is like Wait but also will return after the
    * given number of microseconds even if the waitable object
    * is not signalled.
    *
    * @param  microseconds [in] Period before the call times out
    * @return 'true' if the condition was signalled and 'false'
    *         otherwise
    */
   virtual bool timedWait(long microseconds) = 0;

   /**
    * Signal wakes up a thread waiting on the waitable
    * object. Note that if no thread is waiting then this
    * operation has no effect. In particular, it will not
    * prevent a subsequent Wait from blocking.
    */
   virtual void signal() = 0;

   /**
    * Broadcast wakes up all threads waiting on this waitable
    * object. If no threads are waiting then Broadcast has no
    * effect.
    */
   virtual void broadcast() = 0;

};


/**
 *----------------------------------------------------------------------
 *
 * interface RWLockableObject
 *
 *      Interface for objects that can be locked
 *
 *----------------------------------------------------------------------
 */

class RWLockableObject : virtual public CObject {
public:
   /* Reader-writer locking mode */
   enum LockMode {
      /**
       * Allow readers to acquire the lock even if there's a pending
       * writer.  For a highly contended lock writers may starve.  To
       * avoid deadlock, writers must acquire only one lock of the
       * same type/rank, but readers may acquire multiple locks of the
       * same type/rank in any order.
       */
      LOCK_MODE_NON_FAIR,

      /**
       * Block readers that request the lock when a writer is pending.
       * For a highly contended lock this will prevent writer
       * starvation.  To avoid deadlock, readers and writers must
       * acquire locks of the same type/rank in the same order.
       * Writers and readers can both acquire multiple locks of the
       * same type/rank.
       */
      LOCK_MODE_FAIR,

      /** Allow the implementation to choose the lock mode. */
      LOCK_MODE_UNSPECIFIED,

      /**
       * Legacy implementation of the non-fair lock. It is meant to be
       * used temporary until all vmacore users migrate to the new
       * implementation.
       */
      LOCK_MODE_LEGACY_NON_FAIR,

      /**
       * Legacy implementation of the fair lock. It is meant to be
       * used temporary until all vmacore users migrate to the new
       * implementation.
       */
      LOCK_MODE_LEGACY_FAIR,

      /** Use the default mode for the legacy implementation. */
      LOCK_MODE_LEGACY_UNSPECIFIED,
   };

public:
   /**
    * Blocks the calling thread until the object read lock can be
    * obtained.
    *
    * It is legitimate for the current thread to already own the object read
    * lock. (Read locks are recursive.)
    */
   virtual void readLock() = 0;

   /**
    * Blocks the current thread until the object write lock
    * can be obtained.
    *
    * The current thread must not own a read lock or write lock on the
    * object. (Write locks are not recursive.)
    */
   virtual void writeLock() = 0;

   /**
    * Releases the current read lock or write lock.
    *
    * The current thread must own a read lock or write lock. Nested calls to
    * ReadLock must be balanced with an equivalent number of calls to Unlock.
    */
   virtual void unlock() = 0;

   /**
    * Converts the currently held read lock to a write lock. This
    * method will block until all other readers have released the
    * lock, except in the case when there is an outstanding upgrade in
    * progress.
    *
    * If two reader threads both call UpgradeReadLock, having both
    * calls block will lead to a deadlock situation, as each thread
    * would depend on the other to release the read lock first. That
    * is why, when this situation occurs, the second call to
    * UpgradeReadLock will fail by returning 'false'. In such
    * cirumstances the caller will be expected to first release the
    * read lock before ackquiring the write lock or simply assert that
    * such situation should not occur.
    *
    * The current thread must already own the lock for reading.
    */
   virtual bool upgradeReadLock() = 0;

   /**
    * Converts the currently held write lock to a read lock.
    *
    * Potential readers that are blocking to acquire the lock will be
    * signalled to do so and will be able to acquire the
    * lock. Potential writers waiting to acquire the lock will
    * continue to block without being able to acquire the lock.
    *
    * The current thread must already own the lock for writing
    */
   virtual void downgradeWriteLock() = 0;

   /**
    * Determines whether the object is read-locked by the current
    * thread.
    *
    * @return 'true' if the object is read-locked by the currentn
    *         thread and 'false' otherwise.
    */
   virtual bool isReadLocked() const = 0;

   /**
    * Determines whether the object is write-locked by the current
    * thread.
    *
    * @return 'true' if the object is write-locked by the current
    *         thread and 'false' otherwise.
    */
   virtual bool isWriteLocked() const = 0;

   /**
    * Determines whether the object is neither read-locked nor write-locked by
    * the current thread. Calling IsUnlocked is more efficient than calling both
    * IsWriteLocked and IsReadLocked. For example prefer:
    *      VmacoreVerify(IsUnlocked());
    *      VmacoreVerify(!IsUnlocked());
    * over
    *      VmacoreVerify(!IsWriteLocked() && !IsReadLocked());
    *      VmacoreVerify(IsWriteLocked() || IsReadLocked());
    *
    * NOTE that the result of this method is not determined by whether
    * the object is owned by any other thread. Therefore, a return
    * value of 'false' does not mean that there are no other owners of
    * this lock.
    *
    * @return 'true' if the current thread does not hold a read lock or a write
    *         lock on this object and false otherwise.
    */
   virtual bool isUnlocked() const = 0;
};


/**
 *----------------------------------------------------------------------
 *
 * interface RWLockableNotifiedObject
 *
 *      A variant of RWLockableObject that provides callback
 *      notifications as write locks are released. This idiom is
 *      useful for generating ordered notifications of changes.
 *
 *----------------------------------------------------------------------
 */

class RWLockableNotifiedObject : virtual public RWLockableObject {
public:
   /**
    * Called within a call to DowngradeWriteLock, after the write lock
    * has been released.  Also called within a call to Unlock if the
    * write lock is held. In that case the write lock is downgraded to
    * a read lock, the callback is called, and then the read lock is
    * released.
    *
    * This callback must not throw.
    */
   virtual void beforeWriteLockRelease() = 0;
};


/**
 *----------------------------------------------------------------------
 *
 * interface Mutex
 *
 *      A mutex abstraction used for synchronization between
 *      threads
 *
 *----------------------------------------------------------------------
 */

class Mutex : virtual public ILockableObjectInterface {};


/**
 *----------------------------------------------------------------------
 *
 * interface RWLock
 *
 *        Abstraction of a reader-writer lock.
 *
 *----------------------------------------------------------------------
 */

class RWLock : virtual public RWLockableObject {};


/**
 *----------------------------------------------------------------------
 *
 * interface Condition
 *
 *      A condition abstraction used for synchronization between
 *      threads
 *
 *----------------------------------------------------------------------
 */

class Condition : virtual public CObject {
public:
   /**
    * Wait assumes the caller holds the given mutex and
    * releases the mutex while waiting for another thread to
    * signal the condition variable.  Note that wait always
    * blocks, so the caller should acquire the mutex and check
    * the appropriate state before waiting.
    *
    * After returning, the caller is guaranteed to have
    * re-acquired the mutex.  However, a caller normally
    * cannot assume the state after awakening and should
    * typically re-check the condition.
    *
    * A common usage pattern is therefore:
    *
    *         mutex->Lock();
    *         while (needToWait) {
    *            cond->Wait(mutex);
    *         }
    *          ...
    *         mutex->Unlock();
    *
    * Please note that there is no guaranteed ordering of signaling and wakeups
    * when there are multiple signals and waiting threads. If ordering is
    * required it should be handled by some external means.
    *
    * @param  m [in] The mutex that should be released while waiting
    */
   virtual void wait(Mutex *m) = 0;

   /**
    * TimedWait is like Wait but also will return after the
    * given number of microseconds even if the condition is
    * not signalled.
    *
    * @return 'true' if the condition was signalled and 'false' otherwise
    */
   virtual bool timedWait(Mutex *m, long microseconds) = 0;

   /**
    * Signal wakes up a thread waiting on a condition. Note
    * that if no thread is waiting then this operation has no
    * effect. In particular, it will not prevent a subsequent
    * Wait from blocking.
    */
   virtual void signal() = 0;

   /**
    * Broadcast wakes up all threads waiting on a condition.
    * If no threads are waiting then Broadcast has no effect.
    */
   virtual void broadcast() = 0;
};

} /* namespace LOCK */
} /* namespace CORE */
} /* namespace SF */
#endif /* ILOCKABLEOBJECTINTERFACE_H_ */
