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

		explicit Node(const T& value) : data(value), next(nullptr) {}
	};

	Node* head = nullptr;
	Node* tail = nullptr;
	size_t size = 0;

public:
	SinglyLinkedList() = default;
	SinglyLinkedList(const SinglyLinkedList<T>& other);
	SinglyLinkedList(SinglyLinkedList<T>&& other) noexcept;
	
	SinglyLinkedList<T>& operator=(const SinglyLinkedList<T>& other);
	SinglyLinkedList<T>& operator=(SinglyLinkedList<T>&& other) noexcept;
	~SinglyLinkedList();

	void pushFront(const T& value);
	void pushBack(const T& value);
	// TODO: Add move semantics for push operations

	void popFront();

	const T& front() const;
	const T& back() const;
	T& front();
	T& back();

	size_t getSize() const;
	bool isEmpty() const;

	template <typename U>
	friend SinglyLinkedList<U> concat(SinglyLinkedList<U>& lhs, SinglyLinkedList<U>& rhs);

	void print() const;

	// ==================== Iterator Classes ====================
	
	class Iterator
	{
	private:
		Node* currentNode;
		friend class SinglyLinkedList;

	public:
		typedef std::forward_iterator_tag iterator_category;

		explicit Iterator(Node* node = nullptr) : currentNode(node) {}

		T& operator*() { return currentNode->data; }
		T* operator->() { return &currentNode->data; }

		Iterator& operator++()
		{
			if (currentNode)
				currentNode = currentNode->next;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator temp(*this);
			++(*this);
			return temp;
		}

		Iterator& operator+=(size_t offset)
		{
			while (offset--)
				++(*this);
			return *this;
		}

		Iterator operator+(int offset) const
		{
			Iterator temp(*this);
			return temp += offset;
		}

		bool operator==(const Iterator& other) const 
		{ 
			return currentNode == other.currentNode; 
		}

		bool operator!=(const Iterator& other) const 
		{ 
			return !(*this == other); 
		}
	};

	class ConstIterator
	{
	private:
		Node* currentNode;
		friend class SinglyLinkedList;

	public:
		typedef std::forward_iterator_tag iterator_category;

		explicit ConstIterator(Node* node = nullptr) : currentNode(node) {}
		ConstIterator(const Iterator& iter) : currentNode(iter.currentNode) {}

		const T& operator*() const { return currentNode->data; }
		const T* operator->() const { return &currentNode->data; }

		ConstIterator& operator++()
		{
			if (currentNode)
				currentNode = currentNode->next;
			return *this;
		}

		ConstIterator operator++(int)
		{
			ConstIterator temp(*this);
			++(*this);
			return temp;
		}

		ConstIterator& operator+=(size_t offset)
		{
			while (offset--)
				++(*this);
			return *this;
		}

		ConstIterator operator+(int offset) const
		{
			ConstIterator temp(*this);
			return temp += offset;
		}

		bool operator==(const ConstIterator& other) const 
		{ 
			return currentNode == other.currentNode; 
		}

		bool operator!=(const ConstIterator& other) const 
		{ 
			return !(*this == other); 
		}
	};

	Iterator begin() { return Iterator(head); }
	Iterator end() { return Iterator(nullptr); }

	ConstIterator cbegin() const { return ConstIterator(head); }
	ConstIterator cend() const { return ConstIterator(nullptr); }

	Iterator insertAfter(const T& value, const ConstIterator& position);
	Iterator removeAfter(const ConstIterator& position);

private:
	void copyFrom(const SinglyLinkedList<T>& other);
	void moveFrom(SinglyLinkedList<T>&& other) noexcept;
	void free();
};

template <typename T>
void SinglyLinkedList<T>::pushFront(const T& value)
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

template <typename T>
void SinglyLinkedList<T>::pushBack(const T& value)
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

template <typename T>
void SinglyLinkedList<T>::popFront()
{
	if (!head)
		throw std::logic_error("Cannot pop from an empty list!");

	if (head == tail)
	{
		delete head;
		head = tail = nullptr;
	}
	else
	{
		Node* nodeToDelete = head;
		head = head->next;
		delete nodeToDelete;
	}
	size--;
}

template <typename T>
const T& SinglyLinkedList<T>::front() const
{
	if (!head)
		throw std::logic_error("Cannot access front of an empty list!");

	return head->data;
}

template <typename T>
const T& SinglyLinkedList<T>::back() const
{
	if (!tail)
		throw std::logic_error("Cannot access back of an empty list!");

	return tail->data;
}

template <typename T>
T& SinglyLinkedList<T>::front()
{
	if (!head)
		throw std::logic_error("Cannot access front of an empty list!");

	return head->data;
}

template <typename T>
T& SinglyLinkedList<T>::back()
{
	if (!tail)
		throw std::logic_error("Cannot access back of an empty list!");

	return tail->data;
}

template <typename T>
bool SinglyLinkedList<T>::isEmpty() const
{
	return size == 0;
}

template <typename T>
size_t SinglyLinkedList<T>::getSize() const
{
	return size;
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList<T>& other)
{
	copyFrom(other);
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList<T>&& other) noexcept
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
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(SinglyLinkedList<T>&& other) noexcept
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
void SinglyLinkedList<T>::copyFrom(const SinglyLinkedList<T>& other)
{
	Node* current = other.head;

	while (current)
	{
		pushBack(current->data);
		current = current->next;
	}
}

template <typename T>
void SinglyLinkedList<T>::moveFrom(SinglyLinkedList<T>&& other) noexcept
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
	Node* current = head;

	while (current)
	{
		Node* nodeToDelete = current;
		current = current->next;
		delete nodeToDelete;
	}

	head = tail = nullptr;
	size = 0;
}

template <typename T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::insertAfter(
	const T& value, 
	const typename SinglyLinkedList<T>::ConstIterator& position)
{
	if (position == end())
		return end();

	Node* newNode = new Node(value);
	Node* positionNode = position.currentNode;

	newNode->next = positionNode->next;
	positionNode->next = newNode;
	size++;

	return Iterator(newNode);
}

template <typename T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::removeAfter(
	const typename SinglyLinkedList<T>::ConstIterator& position)
{
	if (position == end() || getSize() == 1)
		return end();

	Node* nodeToDelete = (position + 1).currentNode;
	Node* nextNode = nodeToDelete->next;

	position.currentNode->next = nextNode;
	size--;

	if (nodeToDelete == tail)
		tail = position.currentNode;

	delete nodeToDelete;

	return Iterator(nextNode);
}

template <typename T>
SinglyLinkedList<T> concat(SinglyLinkedList<T>& lhs, SinglyLinkedList<T>& rhs)
{
	SinglyLinkedList<T> result;

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

	result.size = lhs.size + rhs.size;
	lhs.size = rhs.size = 0;
	lhs.head = lhs.tail = rhs.head = rhs.tail = nullptr;

	return result;
}

template <typename T>
void SinglyLinkedList<T>::print() const
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
