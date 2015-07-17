#include <iostream>

#include "fixed_object_pool_delegate.h"

using namespace std;

class Work {
 public:
  Work() { std::cout << "created!" << std::endl; }
  ~Work() { std::cout << "destructed!" << std::endl; }

  bool Run(int a, int b) {
    std::cout << "a: " << a << ", b: " << b << std::endl;
    return false;
  }
};

int main() {
  FixedObjectPoolDelegate<Work> delegate(3);

  bool result = delegate.Run<bool>(&Work::Run, 1, 2);
  std::cout << "result: " << result << std::endl;

  return 0;
}