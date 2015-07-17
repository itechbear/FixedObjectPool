//
// Created by hudong on 7/17/15.
//

#ifndef FIXEDOBJECTPOOL_FIXED_OBJECT_POOL_H_
#define FIXEDOBJECTPOOL_FIXED_OBJECT_POOL_H_

#include <cstddef>
#include <stack>
#include <mutex>
#include <condition_variable>

template <typename T>
class FixedObjectPoolDelegate;

template <typename T>
class FixedObjectPool {
 public:
  static FixedObjectPool *CreatePool(size_t size) {
    FixedObjectPool *pool = new FixedObjectPool(size);

    while (size > 0) {
      T *instance = new T();
      pool->stack_.push(instance);
      --size;
    }

    return pool;
  }

  // Try to get a pointer.
  // return false immediately on failure.
  bool TryPop(T **type) {
    std::lock_guard<std::mutex> lock_guard(mutex_);
    if (stack_.empty()) {
      return false;
    }
    *type = stack_.top();
    stack_.pop();
    return true;
  }

  // Try to get a pointer
  // wait until success.
  bool WaitUntilPop(T **type) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (stack_.empty()) {
      cond_var_.wait(lock);
    }
    *type = stack_.top();
    stack_.pop();
    lock.unlock();
    return true;
  }

  // Return this pointer to this pool.
  bool Push(T *type) {
    std::lock_guard<std::mutex> lock_guard(mutex_);
    stack_.push(type);
    cond_var_.notify_one();
    return true;
  }

 private:
  FixedObjectPool(size_t core_size)
      : core_size_(core_size), stack_(),
        mutex_(), cond_var_() {
  }

  ~FixedObjectPool() {
    while (!stack_.empty()) {
      delete stack_.top();
      stack_.pop();
    }
  }

  friend class FixedObjectPoolDelegate<T>;

  size_t core_size_;
  std::stack<T *> stack_;
  std::mutex mutex_;
  std::condition_variable cond_var_;
};

#endif //FIXEDOBJECTPOOL_FIXED_OBJECT_POOL_H_
