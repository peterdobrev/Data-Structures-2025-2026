#pragma once
#include <exception>
#include <utility>

template<typename T>
class GenericVector {
private:
	static const short INITIAL_CAPACITY = 4;
	static const short RESIZE_COEF = 2;

	T* data;
	size_t m_size;
	size_t m_capacity;

	void resize(size_t newCapacity);
	void assertIndex(size_t index) const;
	void upsizeIfNeeded();
	void move(GenericVector<T>&& other);
	void copyFrom(const GenericVector<T>& other);
	void free();

public:
	GenericVector();
	GenericVector(size_t capacity);
	GenericVector(const GenericVector<T>& other);
	GenericVector(GenericVector<T>&& other);
	GenericVector<T>& operator=(const GenericVector<T>& other);
	GenericVector<T>& operator=(GenericVector<T>&& other);
	~GenericVector();

	size_t size() const;
	size_t capacity() const;

	void push_back(const T& element);
	void push_back(T&& element);
	T pop_back();

	bool empty() const;
	void clear();
	void shrink_to_fit();

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	const T* data() const;
};

// Implementation

// Constructors
template<typename T>
GenericVector<T>::GenericVector() : data(new T[INITIAL_CAPACITY]), m_size(0), m_capacity(INITIAL_CAPACITY) { }

template<typename T>
GenericVector<T>::GenericVector(size_t capacity) : data(new T[capacity]), m_size(0), m_capacity(capacity) { }

template<typename T>
GenericVector<T>::GenericVector(const GenericVector<T>& other) : data(nullptr), m_size(0), m_capacity(0) {
	copyFrom(other);
}

template<typename T>
GenericVector<T>::GenericVector(GenericVector<T>&& other) : data(nullptr), m_size(0), m_capacity(0) {
	move(std::move(other));
}

template<typename T>
GenericVector<T>& GenericVector<T>::operator=(const GenericVector<T>& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}

template<typename T>
GenericVector<T>& GenericVector<T>::operator=(GenericVector<T>&& other) {
	if (this != &other) {
		free();
		move(std::move(other));
	}
	return *this;
}

template<typename T>
GenericVector<T>::~GenericVector() {
	free();
}

// Private helper functions
template<typename T>
void GenericVector<T>::assertIndex(size_t index) const {
	if (index >= m_size) {
		throw std::exception("Out of range");
	}
}

template<typename T>
void GenericVector<T>::upsizeIfNeeded() {
	if (m_size == m_capacity) {
		resize(m_capacity * RESIZE_COEF);
	}
}

template<typename T>
void GenericVector<T>::resize(size_t newCapacity) {
	m_capacity = newCapacity;
	T* temp = new T[m_capacity];

	if (m_size > newCapacity) {
		m_size = newCapacity;
	}

	for (size_t i = 0; i < m_size; i++) {
		temp[i] = std::move(data[i]);
	}

	delete[] data;
	data = temp;
}

template<typename T>
void GenericVector<T>::move(GenericVector<T>&& other) {
	m_size = other.m_size;
	m_capacity = other.m_capacity;
	data = other.data;
	other.data = nullptr;
	other.m_size = 0;
	other.m_capacity = 0;
}

template<typename T>
void GenericVector<T>::copyFrom(const GenericVector<T>& other) {
	m_size = other.m_size;
	m_capacity = other.m_capacity;
	data = new T[m_capacity];

	for (size_t i = 0; i < other.m_size; i++) {
		data[i] = other.data[i];
	}
}

template<typename T>
void GenericVector<T>::free() {
	delete[] data;
	data = nullptr;
	m_size = m_capacity = 0;
}

// Public interface
template<typename T>
size_t GenericVector<T>::size() const {
	return m_size;
}

template<typename T>
size_t GenericVector<T>::capacity() const {
	return m_capacity;
}

template<typename T>
void GenericVector<T>::push_back(const T& element) {
	upsizeIfNeeded();
	data[m_size++] = element;
}

template<typename T>
void GenericVector<T>::push_back(T&& element) {
	upsizeIfNeeded();
	data[m_size++] = std::move(element);
}

template<typename T>
T GenericVector<T>::pop_back() {
	if (empty()) {
		throw std::exception("Vector is empty");
	}
	return std::move(data[--m_size]);
}

template<typename T>
bool GenericVector<T>::empty() const {
	return m_size == 0;
}

template<typename T>
void GenericVector<T>::clear() {
	m_size = 0;
}

template<typename T>
void GenericVector<T>::shrink_to_fit() {
	resize(m_size);
}

template<typename T>
T& GenericVector<T>::operator[](size_t index) {
	assertIndex(index);
	return data[index];
}

template<typename T>
const T& GenericVector<T>::operator[](size_t index) const {
	assertIndex(index);
	return data[index];
}

template<typename T>
const T* GenericVector<T>::data() const {
	return data;
}
