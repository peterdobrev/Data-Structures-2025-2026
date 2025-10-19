#include "SinglyLinkedList.h"

SinglyLinkedList::~SinglyLinkedList() {
  clear();
}

void SinglyLinkedList::push_front(int value) {
  Node* newNode = new Node(value);

  if (isEmpty()) {
    head = tail = newNode;
  } else {
    newNode->next = head;
    head = newNode;
  }

  size++;
}

void SinglyLinkedList::push_back(int value) {
  Node* newNode = new Node(value);

  if (isEmpty()) {
    head = tail = newNode;
  } else {
    tail->next = newNode;
    tail = newNode;
  }

  size++;
}

void SinglyLinkedList::pop_front() {
  if (isEmpty()) {
    throw std::logic_error("Linked list is empty");
  }

  Node* toDelete = head;
  head = head->next;
  delete toDelete;
  size--;

  if(isEmpty()) {
    tail = nullptr;
  }
}

void SinglyLinkedList::pop_back() {
  if (isEmpty()) {
    throw std::logic_error("Linked list is empty");
  }

  if(head == tail) {
    Node* toDelete = head;
    delete toDelete;
    size--;
    head = tail = nullptr;
    return;
  }

  Node* toDelete = tail;

  Node* iter = head;
  while(iter && iter->next != tail) {
    iter = iter->next;
  }

  iter->next = nullptr;
  tail = iter;

  delete toDelete;
  size--;
}

int SinglyLinkedList::front() const {
  if (isEmpty()) {
    throw std::logic_error("Linked list is empty");
  }

  return head->data;
}

int SinglyLinkedList::back() const {
  if (isEmpty()) {
    throw std::logic_error("Linked list is empty");
  }

  return tail->data;
}

size_t SinglyLinkedList::getSize() const {
  return size;
}

bool SinglyLinkedList::isEmpty() const {
  return size == 0;
}

void SinglyLinkedList::print() const {
  Node* iter = head;

  while (iter) {
    std::cout << iter->data;
    if(iter->next) {
      std::cout << " -> ";
    }
    iter = iter->next;
  }
  std::cout << std::endl;
}

void SinglyLinkedList::clear() {
  Node* iter = head;

  while (iter) {
    Node* toDelete = iter;
    iter = iter->next;
    delete toDelete;
  }

  head = tail = nullptr;
  size = 0;
}
