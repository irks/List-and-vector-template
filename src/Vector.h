#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#define START_SIZE 10

#include <cstddef>
#include <initializer_list>
#include <stdexcept>



namespace aisdi
{

template <typename Type>
class Vector
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  Vector() {
    size = 0;
    capacity = START_SIZE;
    buffer = new Type[START_SIZE+1]; // +1 to have element after last data element
  }

  Vector(std::initializer_list<Type> l) : Vector() {
    for(auto it = l.begin(); it != l.end(); ++it)
      append(*it);
  }

  Vector(const Vector& other) : Vector() {
    for(const_iterator it = other.cbegin(); it != other.cend(); it++ )
      append(*it);
  }

  Vector(Vector&& other) {
    size = other.size;
    capacity = other.capacity;
    buffer = other.buffer;

    //reinitiliaze
    other.size = 0;
    other.capacity = START_SIZE;
    other.buffer = new Type[START_SIZE+1];

  }

  ~Vector() {
    delete [] buffer;
  }

  Vector& operator=(const Vector& other) {
    if(this == &other)
      return *this;
    if(!isEmpty())
      size =0; //faster than erase(cbegin,cend)
    for(const_iterator it = other.cbegin();it != other.cend() ; ++it )
      append(*it);
    return *this;
  }

  Vector& operator=(Vector&& other) {
    if(this == &other)
      return *this;
    Type* toDel = buffer;

    size = other.size;
    capacity = other.capacity;

    buffer = other.buffer;

    other.size = 0;
    other.capacity = START_SIZE;
    other.buffer = new Type[START_SIZE+1];

    delete [] toDel;
    return *this;
  }

  bool isEmpty() const {
    return !size;
  }

  size_type getSize() const {
    return size;
  }

  void append(const Type& item) {
    if(size == capacity) { //if vector is full
      Type* tmp = resize(); // 2 times bigger
      Type* toDel = buffer;
      Type* helper = tmp;

      for(const_iterator it = cbegin();it!=cend();++it, ++tmp)
        *tmp = *it;

      buffer = helper;
      delete [] toDel;
    }

    buffer[size] = item;
    ++size;
  }

  void prepend(const Type& item) {
    if(size == capacity) {
      Type* tmp = resize();
      Type* toDel = buffer;

      *tmp = item;
      Type* helper = tmp;
      const_iterator it = cbegin();
      ++tmp;
      for(; it!= cend(); ++tmp, ++it)
        *tmp = *it;
      buffer = helper;
      delete [] toDel;
      ++size;
    }
    else {
      rightShift(cbegin());
      *buffer = item;
    }

  }

  void insert(const const_iterator& insertPosition, const Type& item) {
    if(insertPosition == cend()) {
      append(item);
      return;
    }
    if(size == capacity) {
      Type* tmp = resize();
      Type* toDel = buffer;

      Type* helper = tmp;
      const_iterator it = cbegin();
      for(; it != insertPosition; ++tmp, ++it)
        *tmp = *it;
      *tmp = item;
      for(++tmp; it != cend(); ++tmp, ++it)
        *tmp = *it;
      buffer = helper;
      delete [] toDel;
      ++size;
    }
    else { //right shift
      rightShift(insertPosition);
      *( (iterator)insertPosition )  = item;
    }

  }

  Type popFirst() {
    if(isEmpty())
      throw std::logic_error("Attempt to pop first in empty vector");

    Type tmp = *buffer;
    leftShift(cbegin(), cbegin()+1);
    --size;
    return tmp;
  }

  Type popLast() {
    if(isEmpty())
      throw std::logic_error("Attempt to pop last in empty vector");
    --size;
    return buffer[size];
  }

  void erase(const const_iterator& position) {
    if(isEmpty())
      throw std::out_of_range("attempt to erase empty vector");
    if(position == cend())
      throw std::out_of_range("attempt to erase at end iterator");
    if(position == (cend() -1)) {
      popLast();
      return;
    }
    leftShift(position, (position+1));

    --size;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded) {
    if(isEmpty())
      throw std::out_of_range("attempt to erase empty vector");
    if(firstIncluded == lastExcluded)
      return;
    if(lastExcluded == cend()) {
      size -= (lastExcluded.index - firstIncluded.index);
      return;
    }
    leftShift(firstIncluded, lastExcluded);
    size -= (lastExcluded.index - firstIncluded.index);
  }

  iterator begin() {
    return iterator(0, this);
  }

  iterator end() {
    return iterator(size, this);
  }

  const_iterator cbegin() const {
    return const_iterator(0, this);
  }

  const_iterator cend() const {
    return const_iterator(size, this);
  }

  const_iterator begin() const {
    return cbegin();
  }

  const_iterator end() const {
    return cend();
  }

  protected:

    Type* resize() { //use if vector is full
      capacity *= 2;
      return new Type[capacity+1];
    }

    void leftShift(const const_iterator& positionTo, const const_iterator& positionFrom) {
      iterator to = iterator(positionTo.index, this);
      const_iterator from = positionFrom;

      for(; from != cend(); ++to, ++from)
        *to = *from;
      return;
    }
    void rightShift(const const_iterator& position) {
      if(isEmpty()) {
        ++size;
        return;
      }
      iterator it = end() - 1;
      ++size;
      for(; it != position; --it)
        *(it+1) = *it;

      *(it+1) = *it;
      return;
    }


    Type* buffer;
    int size;
    int capacity;

};

template <typename Type>
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;

  explicit ConstIterator(int i = 0, const Vector<Type>* v = nullptr ) : index(i), vec(v)  {}

  ConstIterator(const ConstIterator& iter) : index(iter.index), vec(iter.vec)  {}

  reference operator*() const {
    if(index >= 0 && index < vec->size)
      return vec->buffer[index];
    else
      throw std::out_of_range("Attempt to dereference end iterator");

  }

  ConstIterator& operator++() {
    if(index != (vec->size)) {
      ++index;
      return *this;
    }
    else
      throw std::out_of_range("Attempt to increment end iterator"); // before end THE LAST

  }

  ConstIterator operator++(int) {
    if(index != (vec->size)) {
      const_iterator tmp(this->index, vec);
      ++index;
      return tmp;
    }
    else
      throw std::out_of_range("Attempt to increment end iterator"); // before end THE LAST
  }

  ConstIterator& operator--() {
    if(index > 0) {
      --index;
      return *this;
    }
    throw std::out_of_range("Attempt to decrement begin iterator");
  }

  ConstIterator operator--(int) {
    if(index > 0) {
      const_iterator tmp(this->index, vec);
      --index;
      return tmp;
    }
    else
      throw std::out_of_range("Attempt to decrement begin iterator");
  }

  ConstIterator operator+(difference_type d) const {
    if(index + d > vec-> size )
      throw std::out_of_range("Attempt to add out of vector range");

    return const_iterator(index + d, vec);
  }

  ConstIterator operator-(difference_type d) const {
    if(index - d < 0 )
      throw std::out_of_range("Attempt to substract out of vector range");

    return const_iterator(index - d, vec);
  }

  bool operator==(const ConstIterator& other) const {
   return ( (vec->buffer == other.vec->buffer) && (index == other.index) );
  }

  bool operator!=(const ConstIterator& other) const {
    return ( !operator==(other) );
  }

  protected:
    int index;
    const Vector<value_type>* vec;

    friend void aisdi::Vector<Type>::erase(const const_iterator&, const const_iterator&);
    friend void aisdi::Vector<Type>::leftShift(const const_iterator&, const const_iterator&);
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator(int i, const Vector<value_type>* v) : const_iterator(i, v) {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other) {}

  Iterator& operator++() {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int) {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--() {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int) {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const {
    return ConstIterator::operator-(d);
  }

  reference operator*() const {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_VECTOR_H
