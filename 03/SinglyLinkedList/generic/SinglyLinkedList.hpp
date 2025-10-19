#pragma once
#include <iostream>

template <typename T>
class SinglyLinkedList
{
private:
	struct Node
	{
		T data;
		Node* next;

		Node(const T& data) : data(data), next(nullptr) {}
	};

	Node* head = nullptr;
	Node* tail = nullptr;
	size_t size = 0;

public:
	SinglyLinkedList() = default;
	SinglyLinkedList(const SinglyLinkedList<T>& other);
	SinglyLinkedList(SinglyLinkedList<T>&& other);
	SinglyLinkedList<T>& operator=(const SinglyLinkedList<T>& other);
	SinglyLinkedList<T>& operator=(SinglyLinkedList<T>&& other);
	~SinglyLinkedList();

	void pushFront(const T& el);
	void pushBack(const T& el);
	void popFront();

	const T& front() const;
	const T& back() const;
	T& front();
	T& back();

	size_t getSize() const;
	bool isEmpty() const;

	void print() const;

	template <typename U>
	friend SinglyLinkedList<U> concat(SinglyLinkedList<U>& lhs, SinglyLinkedList<U>& rhs);

private:
	void copyFrom(const SinglyLinkedList<T>& other);
	void moveFrom(SinglyLinkedList<T>&& other);
	void free();
};

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList<T>& other)
{
	copyFrom(other);
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList<T>&& other)
{
	moveFrom(std::move(other));
}

template <typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(const SinglyLinkedList<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

template <typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(SinglyLinkedList<T>&& other)
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList()
{
	free();
}

template <typename T>
void SinglyLinkedList<T>::pushFront(const T& el)
{
	Node* newNode = new Node(el);

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

template <typename T>
void SinglyLinkedList<T>::pushBack(const T& el)
{
	Node* newNode = new Node(el);

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

template <typename T>
void SinglyLinkedList<T>::popFront()
{
	if (!head)
		throw std::logic_error("Empty list!");

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

template <typename T>
const T& SinglyLinkedList<T>::front() const
{
	if (!head)
		throw std::logic_error("Empty list!");
	return head->data;
}

template <typename T>
const T& SinglyLinkedList<T>::back() const
{
	if (!tail)
		throw std::logic_error("Empty list!");
	return tail->data;
}

template <typename T>
T& SinglyLinkedList<T>::front()
{
	if (!head)
		throw std::logic_error("Empty list!");
	return head->data;
}

template <typename T>
T& SinglyLinkedList<T>::back()
{
	if (!tail)
		throw std::logic_error("Empty list!");
	return tail->data;
}

template <typename T>
size_t SinglyLinkedList<T>::getSize() const
{
	return size;
}

template <typename T>
bool SinglyLinkedList<T>::isEmpty() const
{
	return getSize() == 0;
}

template <typename T>
void SinglyLinkedList<T>::print() const
{
	Node* iter = head;

	while (iter)
	{
		std::cout << iter->data << ' ';
		if (iter->next)
			std::cout << "-> ";
		iter = iter->next;
	}
	std::cout << std::endl;
}

template <typename T>
void SinglyLinkedList<T>::copyFrom(const SinglyLinkedList<T>& other)
{
	Node* iter = other.head;
	while (iter)
	{
		pushBack(iter->data);
		iter = iter->next;
	}
}

template <typename T>
void SinglyLinkedList<T>::moveFrom(SinglyLinkedList<T>&& other)
{
	head = other.head;
	tail = other.tail;
	size = other.size;

	other.head = other.tail = nullptr;
	other.size = 0;
}

template <typename T>
void SinglyLinkedList<T>::free()
{
	Node* iter = head;

	while (iter)
	{
		Node* toDelete = iter;
		iter = iter->next;
		delete toDelete;
	}
	head = tail = nullptr;
	size = 0;
}

template <typename U>
SinglyLinkedList<U> concat(SinglyLinkedList<U>& lhs, SinglyLinkedList<U>& rhs)
{
	SinglyLinkedList<U> result;

	if (!lhs.head)
	{
		result.head = rhs.head;
		result.tail = rhs.tail;
	}
	else if (!rhs.head)
	{
		result.head = lhs.head;
		result.tail = lhs.tail;
	}
	else
	{
		lhs.tail->next = rhs.head;
		result.head = lhs.head;
		result.tail = rhs.tail;
	}

	result.size = rhs.size + lhs.size;
	rhs.size = lhs.size = 0;
	rhs.head = rhs.tail = lhs.head = lhs.tail = nullptr;

	return result;
}
