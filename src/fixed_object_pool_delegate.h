//
// Created by hudong on 7/17/15.
//

#ifndef FIXEDOBJECTPOOL_FIXED_OBJECT_POOL_DELEGATE_H_H_
#define FIXEDOBJECTPOOL_FIXED_OBJECT_POOL_DELEGATE_H_H_

#include "fixed_object_pool.h"

template <typename T>
class FixedObjectPoolDelegate {
 public:
  FixedObjectPoolDelegate(size_t size)
      : pool_(FixedObjectPool<T>::CreatePool(size)) {
  }

  ~FixedObjectPoolDelegate() { delete pool_; }

  template <class R, class F, class... Args>
  R Run(F&& function, Args&&... args) {
    T *t = nullptr;
    pool_->WaitUntilPop(&t);
    R r = std::bind(function, t, args...)();
    pool_->Push(t);
    return r;
  };

 private:
  FixedObjectPool<T> *pool_;
};

#endif //FIXEDOBJECTPOOL_FIXED_OBJECT_POOL_DELEGATE_H_H_
