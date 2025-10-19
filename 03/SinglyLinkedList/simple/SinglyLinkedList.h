#pragma once
#include <iostream>

class SinglyLinkedList {
  
  private:

    struct Node {
      int data;
      Node* next;

      Node(int data) : data(data), next(nullptr) { }
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    size_t size = 0;

  public:
    SinglyLinkedList() = default;
    ~SinglyLinkedList();

    void push_back(int value);
    void push_front(int value);

    void pop_front();
    void pop_back();

    int front() const;
    int back() const;

    size_t getSize() const;
    bool isEmpty() const;

    void print() const;
    void clear();
};
