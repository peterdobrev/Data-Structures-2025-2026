#include "SinglyLinkedList.h"

SinglyLinkedList::~SinglyLinkedList()
{
  clear();
}

void SinglyLinkedList::push_back(int value)
{
  Node* newNode = new Node(value);

  if (isEmpty())
  {
    head = tail = newNode;
  }
  else
  {
    tail->next = newNode;
    tail = newNode;
  }
  size++;
}

void SinglyLinkedList::push_front(int value)
{
  Node* newNode = new Node(value);

  if (isEmpty())
  {
    head = tail = newNode;
  }
  else
  {
    newNode->next = head;
    head = newNode;
  }
  size++;
}

void SinglyLinkedList::pop_front()
{
  if (isEmpty())
    throw std::logic_error("Cannot pop from empty list!");

  if (head == tail)
  {
    delete head;
    head = tail = nullptr;
  }
  else
  {
    Node* toDelete = head;
    head = head->next;
    delete toDelete;
  }
  size--;
}

void SinglyLinkedList::pop_back()
{
  if (isEmpty())
    throw std::logic_error("Cannot pop from empty list!");

  if (head == tail)
  {
    delete head;
    head = tail = nullptr;
  }
  else
  {
    Node* current = head;
    while (current->next != tail)
      current = current->next;

    delete tail;
    tail = current;
    tail->next = nullptr;
  }
  size--;
}

int SinglyLinkedList::front() const
{
  if (isEmpty())
    throw std::logic_error("List is empty!");

  return head->data;
}

int SinglyLinkedList::back() const
{
  if (isEmpty())
    throw std::logic_error("List is empty!");

  return tail->data;
}

size_t SinglyLinkedList::getSize() const
{
  return size;
}

bool SinglyLinkedList::isEmpty() const
{
  return size == 0;
}

void SinglyLinkedList::print() const
{
  Node* current = head;

  while (current)
  {
    std::cout << current->data << ' ';
    if (current->next)
      std::cout << "-> ";
    current = current->next;
  }
  std::cout << std::endl;
}

void SinglyLinkedList::clear()
{
  Node* current = head;

  while (current)
  {
    Node* toDelete = current;
    current = current->next;
    delete toDelete;
  }
  head = tail = nullptr;
  size = 0;
}


int& SinglyLinkedList::Iterator::operator*()
{
  return currentElementPtr->data;
}

int* SinglyLinkedList::Iterator::operator->()
{
  return &currentElementPtr->data;
}

SinglyLinkedList::Iterator& SinglyLinkedList::Iterator::operator++()
{
  if (currentElementPtr)
    currentElementPtr = currentElementPtr->next;
  return *this;
}

SinglyLinkedList::Iterator SinglyLinkedList::Iterator::operator++(int)
{
  Iterator copy(*this);
  ++(*this);
  return copy;
}

SinglyLinkedList::Iterator& SinglyLinkedList::Iterator::operator+=(size_t s)
{
  while (s--)
    ++(*this);
  return *this;
}

SinglyLinkedList::Iterator SinglyLinkedList::Iterator::operator+(int i) const
{
  Iterator res(*this);
  return res += i;
}

bool SinglyLinkedList::Iterator::operator==(const Iterator& rhs) const
{
  return currentElementPtr == rhs.currentElementPtr;
}

bool SinglyLinkedList::Iterator::operator!=(const Iterator& rhs) const
{
  return !(rhs == *this);
}

SinglyLinkedList::ConstIterator::ConstIterator(const Iterator& nonConstIter)
  : currentElementPtr(nonConstIter.currentElementPtr) {}

SinglyLinkedList::ConstIterator::ConstIterator(Node* currentElementPtr)
  : currentElementPtr(currentElementPtr) {}

const int& SinglyLinkedList::ConstIterator::operator*() const
{
  return currentElementPtr->data;
}

const int* SinglyLinkedList::ConstIterator::operator->() const
{
  return &currentElementPtr->data;
}

SinglyLinkedList::ConstIterator& SinglyLinkedList::ConstIterator::operator++()
{
  if (currentElementPtr)
    currentElementPtr = currentElementPtr->next;
  return *this;
}

SinglyLinkedList::ConstIterator SinglyLinkedList::ConstIterator::operator++(int)
{
  ConstIterator copy(*this);
  ++(*this);
  return copy;
}

SinglyLinkedList::ConstIterator& SinglyLinkedList::ConstIterator::operator+=(size_t s)
{
  while (s--)
    ++(*this);
  return *this;
}

SinglyLinkedList::ConstIterator SinglyLinkedList::ConstIterator::operator+(int i) const
{
  ConstIterator res(*this);
  return res += i;
}

bool SinglyLinkedList::ConstIterator::operator==(const ConstIterator& rhs) const
{
  return currentElementPtr == rhs.currentElementPtr;
}

bool SinglyLinkedList::ConstIterator::operator!=(const ConstIterator& rhs) const
{
  return !(rhs == *this);
}

SinglyLinkedList::Iterator SinglyLinkedList::begin()
{
  return Iterator(head);
}

SinglyLinkedList::Iterator SinglyLinkedList::end()
{
  return Iterator(nullptr);
}

SinglyLinkedList::ConstIterator SinglyLinkedList::cbegin() const
{
  return ConstIterator(head);
}

SinglyLinkedList::ConstIterator SinglyLinkedList::cend() const
{
  return ConstIterator(nullptr);
}

SinglyLinkedList::Iterator SinglyLinkedList::insertAfter(int element, const ConstIterator& it)
{
  if (it == cend())
    return end();

  Node* newNode = new Node(element);
  Node* itNode = it.currentElementPtr;

  newNode->next = itNode->next;
  itNode->next = newNode;
  size++;

  if (itNode == tail)
    tail = newNode;

  return Iterator(newNode);
}

SinglyLinkedList::Iterator SinglyLinkedList::removeAfter(const ConstIterator& it)
{
  if (it == cend() || getSize() == 1)
    return end();

  Node* toDelete = (it + 1).currentElementPtr;
  Node* newNext = toDelete->next;

  it.currentElementPtr->next = newNext;
  size--;

  if (toDelete == tail)
    tail = it.currentElementPtr;

  delete toDelete;

  return Iterator(newNext);
}
