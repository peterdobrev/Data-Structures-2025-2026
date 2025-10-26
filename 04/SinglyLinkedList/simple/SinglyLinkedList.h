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


    class Iterator {
      private:
        Node* currentElementPtr;

        friend class SinglyLinkedList;

      public:
        typedef std::forward_iterator_tag iterator_category;

        Iterator(Node* currentElementPtr = nullptr) : currentElementPtr(currentElementPtr) {}

        int& operator*();
        int* operator->();

        Iterator& operator++();
        Iterator operator++(int);
        Iterator& operator+=(size_t increment);
        Iterator operator+(size_t increment) const;

        bool operator==(const Iterator& rhs) const;
        bool operator!=(const Iterator& lhs) const;
    };

    class ConstIterator {
      Node* currentElementPtr;

      friend class SinglyLinkedList;

      public:
        typedef std::forward_iterator_tag iterator_category;

        ConstIterator(const Iterator& nonConstIter);
        ConstIterator(Node* currentElementPtr = nullptr) : currentElementPtr(currentElementPtr) {}

        const int& operator*();
        const int* operator->();

        ConstIterator& operator++();
        ConstIterator operator++(int);
        ConstIterator& operator+=(size_t increment);
        ConstIterator operator+(size_t increment) const;

        bool operator==(const ConstIterator& rhs) const;
        bool operator!=(const ConstIterator& rhs) const;
    };

    Iterator begin();
    Iterator end();

    ConstIterator cbegin() const;
    ConstIterator cend() const;

    Iterator insertAfter(int element, const ConstIterator& it);
    Iterator removeAfter(const ConstIterator& it);
};
