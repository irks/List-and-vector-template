#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>

#include "Vector.h"
#include "LinkedList.h"


namespace
{

template <typename T>
using LinearCollection = aisdi::LinkedList<T>;

void perfomTest ()
{
  std::chrono::time_point<std::chrono::system_clock> start, end;
  std::chrono::duration<double> timeDifference;

  aisdi::LinkedList<int> list;
  aisdi::Vector<int> vector1;
  aisdi::Vector<int> vector2;
  const int size_n = 10000;

  start = std::chrono::system_clock::now();
  for(int i = 0; i < size_n; i++)
    list.append(4);
  end = std::chrono::system_clock::now();
  timeDifference = end - start;
  std::cout << "LinkedList: append " << size_n << " elements:   " << timeDifference.count() << std::endl;

  start = std::chrono::system_clock::now();
  for(int i=0; i < size_n; i++)
    list.prepend(5);
  end = std::chrono::system_clock::now();
  timeDifference = end-start;
  std::cout << "LinkedList: prepend " << size_n << " elements:  " << timeDifference.count() << std::endl;

  start = std::chrono::system_clock::now();
  for(int i = 0; i < size_n; i++)
    list.popLast();
  end = std::chrono::system_clock::now();
  timeDifference = end-start;
  std::cout << "LinkedList: popLast " << size_n << " elements:  " << timeDifference.count() << std::endl;

  start = std::chrono::system_clock::now();
  for(int i = 0; i < size_n; i++)
    list.popFirst();
  end = std::chrono::system_clock::now();
  timeDifference = end-start;
  std::cout << "LinkedList: popFirst " << size_n << " elements: " << timeDifference.count() << std::endl;

  for(int i = 0; i < size_n; i++)
    list.append(5);
  start = std::chrono::system_clock::now();
    list.erase(list.cbegin(), list.cend());
  end = std::chrono::system_clock::now();
  timeDifference = end-start;
  std::cout << "LinkedList: erase from begin to end " << size_n << " elements: " << timeDifference.count() << std::endl;

  for(int i = 0; i < size_n; i++)
    list.append(5);
  start = std::chrono::system_clock::now();
  list.insert(list.cbegin() + 10, 6);
  end = std::chrono::system_clock::now();
  timeDifference = end-start;
  std::cout << "LinkedList: insert inside of list " << timeDifference.count() << std::endl;

  start = std::chrono::system_clock::now();
  list.erase(list.cbegin() + 10);
  end = std::chrono::system_clock::now();
  timeDifference = end-start;
  std::cout << "LinkedList: erase inside of list  " << timeDifference.count() << std::endl<< std::endl;

//=========================================================
//        VECTOR
// =============================================


  start = std::chrono::system_clock::now();
  for(int i = 0; i < size_n; i++)
    vector1.prepend(5);
  end = std::chrono::system_clock::now();
  timeDifference = end-start;
  std::cout << "Vector: prepend " << size_n << " elements:  " << timeDifference.count() << std::endl;

  start = std::chrono::system_clock::now();
  for(int i=0; i < size_n; i++)
    vector2.append(2);
  end = std::chrono::system_clock::now();
  timeDifference = end-start;
  std::cout << "Vector: append " << size_n << " elements:   " << timeDifference.count() << std::endl;

  start = std::chrono::system_clock::now();
  for(int i=0; i < size_n; i++)
    vector1.popFirst();
  end = std::chrono::system_clock::now();
  timeDifference = end-start;
  std::cout << "Vector: popFirst " << size_n << " elements: " << timeDifference.count() << std::endl;

  start = std::chrono::system_clock::now();
  for(int i=0; i < size_n; i++)
    vector2.popLast();
  end = std::chrono::system_clock::now();
  timeDifference = end-start;
  std::cout << "Vector: popLast " << size_n << " elements:  " << timeDifference.count() << std::endl;

  for(int i=0; i < size_n; i++)
    vector2.append(2);
  start = std::chrono::system_clock::now();
  vector2.erase(vector2.cbegin(), vector2.cend());
  end = std::chrono::system_clock::now();
  timeDifference = end-start;
  std::cout << "Vector: erase from begin to end " << size_n << " elements: " << timeDifference.count() << std::endl;

  for(int i = 0; i < size_n; i++)
    vector2.append(5);
  start = std::chrono::system_clock::now();
  vector2.insert(vector2.cbegin() + 10, 6);
  end = std::chrono::system_clock::now();
  timeDifference = end-start;
  std::cout << "Vector: insert inside of vector  "  << timeDifference.count() << std::endl;

  start = std::chrono::system_clock::now();
  vector2.erase(vector2.cbegin() + 10);
  end = std::chrono::system_clock::now();
  timeDifference = end-start;
  std::cout << "Vector: erase inside of vector   " << timeDifference.count() << std::endl<< std::endl;


}

} // namespace

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest();
  return 0;
}

