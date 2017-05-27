#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList {

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

  LinkedList() {
    head = new Node();
    tail = new Node();
    head->next = tail;
    head->previous = nullptr;
    tail->previous = head;
    tail->next = nullptr;
    size = 0;
  }

  LinkedList(std::initializer_list<Type> l) : LinkedList() {
    for(auto it = l.begin(); it != l.end(); ++it)
      append(*it);
  }

  LinkedList(const LinkedList& other) : LinkedList() {
    for(const_iterator it = other.cbegin(); it!=other.cend(); ++it )
      append(*it);
  }

  LinkedList(LinkedList&& other) : LinkedList() {

    Node* tmp1 = head;
    Node* tmp2 = tail;

    head = other.head;
    tail = other.tail;
    size = other.size;

//reinitialize other
    other.head = tmp1;
    other.tail = tmp2;
    other.size = 0;
  }

  ~LinkedList() {
    Node* tmp = head;
    while(tmp != tail) {
      tmp = tmp->next;
      delete tmp->previous;
    }
    delete tail;
  }

  LinkedList& operator=(const LinkedList& other) {
    if(this == &other)
      return *this;
    if(!isEmpty())
      erase(cbegin(), cend());

    for(const_iterator it = other.cbegin(); it!= other.cend(); ++it)
      append(*it);
    return *this;
  }

  LinkedList& operator=(LinkedList&& other) {
    if(this == &other)
      return *this;
    if(!isEmpty())
      erase(cbegin(), cend());

    Node* tmp1 = head;
    Node* tmp2 = tail;

    head = other.head;
    tail = other.tail;
    size = other.size;

    other.head = tmp1;
    other.tail = tmp2;
    other.size = 0;

    return *this;
  }

  bool isEmpty() const {
    return !size;
  }

  size_type getSize() const {
    return size;
  }

  void append(const Type& item) {
    tail->previous = new DataNode(item, tail->previous, tail);
    tail->previous->previous->next = tail->previous;
    ++size;
  }

  void prepend(const Type& item) {
    head->next = new DataNode(item, head, head->next);
    head->next->next->previous = head->next;
    ++size;
  }

  void insert(const const_iterator& insertPosition, const Type& item) {
    DataNode* tmp = new DataNode(item, insertPosition.ptr->previous, insertPosition.ptr);
    tmp->previous->next = tmp;
    tmp->next->previous = tmp;
    ++size;
  }

  Type popFirst() {
    if(isEmpty())
      throw std::logic_error("empty list");
    value_type tmp = reinterpret_cast<DataNode*>(head->next)->data;
    Node* toDel = head->next;
    head->next = toDel->next;
    toDel->next->previous = head;
    delete toDel;
    --size;
    return tmp;
  }

  Type popLast() {
    if(isEmpty())
      throw std::logic_error("empty list");

    value_type tmp = reinterpret_cast<DataNode*>(tail->previous)->data;
    Node* toDel = tail->previous;
    toDel->previous->next = tail;
    tail->previous = toDel->previous;
    delete toDel;
    --size;
    return tmp;
  }

  void erase(const const_iterator& possition) {
    if(isEmpty())
      throw std::out_of_range("attempt to erase begin in empty list");
    Node* toDel = possition.ptr;
    if(!toDel->next)
      throw std::out_of_range("attempt to erase at end iterator");

    toDel->previous->next = toDel->next;
    toDel->next->previous = toDel->previous;
    delete toDel;
    --size;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded) {
    if(firstIncluded == lastExcluded)
      return;
    Node* beforeFirstINode = firstIncluded.ptr->previous;
    Node* lastExcludedNode = lastExcluded.ptr;
    Node* tmp = firstIncluded.ptr;
    for(tmp = tmp->next; tmp != lastExcludedNode; tmp = tmp->next) {
        delete tmp->previous;
      --size;
    }
    delete tmp->previous;
    --size;
    beforeFirstINode->next = lastExcludedNode;
    lastExcludedNode->previous = beforeFirstINode;
  }

  iterator begin() {
    return iterator(head->next);
  }

  iterator end() {
    return iterator(tail);
  }

  const_iterator cbegin() const {
    return const_iterator(head->next);
  }

  const_iterator cend() const {
    return const_iterator(const_cast<Node*>(tail));
  }

  const_iterator begin() const {
    return cbegin();
  }

  const_iterator end() const {
    return cend();
  }

protected:
  struct Node {
    Node* previous;
    Node* next;

    Node(Node* p = NULL, Node* n = NULL) : previous(p), next(n) {}
  };
  struct DataNode : Node {
    value_type data;

    DataNode(value_type d, Node* p, Node* n) {
      data = d;
      this->previous = p;
      this->next = n;
    }
  };
  Node* head;
  Node* tail;
  size_type size;

};

template <typename Type>
class LinkedList<Type>::ConstIterator {

public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;

  explicit ConstIterator(Node* p = NULL) : ptr(p) {}

  reference operator*() const {
    if(!ptr->next)
      throw std::out_of_range("Attempt to dereference end iterator");

    return reinterpret_cast<DataNode*>(ptr)->data;
  }

  ConstIterator& operator++() {
    if(!ptr->next)
      throw std::out_of_range("Attempt to increment end iterator");

    ptr = ptr->next;
    return *this;
  }

  ConstIterator operator++(int) {
    if(!ptr->next)
      throw std::out_of_range("Attempt to increment end iterator");

    const_iterator tmp = *this;
    ptr = ptr->next;
    return tmp;
  }

  ConstIterator& operator--() {
    if(!ptr->previous->previous)
      throw std::out_of_range("Attempt to decrement begin iterator");

    ptr = ptr->previous;
    return *this;
  }

  ConstIterator operator--(int) {
    if(!ptr->previous->previous )
      throw std::out_of_range("Attempt to decrement begin iterator");

    const_iterator tmp = *this;
    ptr = ptr->next;
    return tmp;
  }

  ConstIterator operator+(difference_type d) const {
    const_iterator tmp = *this;
    while(d--) {
      if(!tmp.ptr->next)
        throw std::out_of_range("Attempt to add out of list range");
      ++tmp;
    }
    return tmp;
  }

  ConstIterator operator-(difference_type d) const {
    const_iterator tmp = *this;
    while(d--) {
      if(!ptr->previous->previous)
        throw std::out_of_range("Attempt to substract out of list range");
      --tmp;
    }
    return tmp;
  }

  bool operator==(const ConstIterator& other) const {
    return ptr == other.ptr;
  }

  bool operator!=(const ConstIterator& other) const {
    return ptr != other.ptr;
  }

protected:
  Node* ptr;
  friend void aisdi::LinkedList<Type>::insert(const const_iterator&, const Type&);
  friend void aisdi::LinkedList<Type>::erase(const const_iterator& );
  friend void aisdi::LinkedList<Type>::erase(const const_iterator&, const const_iterator& );
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator(Node* p) : ConstIterator(p) {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

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

#endif // AISDI_LINEAR_LINKEDLIST_H
