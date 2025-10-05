#include "Vector.h"

Vector::Vector() : data(new int[INITIAL_CAPACITY]), m_size(0), m_capacity(INITIAL_CAPACITY) { }

Vector::Vector(size_t capacity) : data(new int[capacity]), m_size(0), m_capacity(capacity) { }

Vector::Vector(const Vector& other) : data(nullptr), m_size(0), m_capacity(0) {
	copyFrom(other);
}

Vector::Vector(Vector&& other) : data(nullptr), m_size(0), m_capacity(0) {
	move(std::move(other));
}

Vector& Vector::operator=(const Vector& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}

Vector& Vector::operator=(Vector&& other) {
	if (this != &other) {
		free();
		move(std::move(other));
	}
	return *this;
}

Vector::~Vector() {
	free();
}

void Vector::assertIndex(size_t index) const {
	if (index >= m_size) {
		throw std::exception("Out of range");
	}
}

void Vector::upsizeIfNeeded() {
	if (m_size == m_capacity) {
		resize(m_capacity * RESIZE_COEF);
	}
}

void Vector::resize(size_t newCapacity) {
	m_capacity = newCapacity;
	int* temp = new int[m_capacity];

	if (m_size > newCapacity) {
		m_size = newCapacity;
	}

	for (size_t i = 0; i < m_size; i++) {
		temp[i] = data[i];
	}

	delete[] data;
	data = temp;
}

void Vector::move(Vector&& other) {
	m_size = other.m_size;
	m_capacity = other.m_capacity;
	data = other.data;
	other.data = nullptr;
	other.m_size = 0;
	other.m_capacity = 0;
}

void Vector::copyFrom(const Vector& other) {
	m_size = other.m_size;
	m_capacity = other.m_capacity;
	data = new int[m_capacity];

	for (size_t i = 0; i < other.m_size; i++) {
		data[i] = other.data[i];
	}
}

void Vector::free() {
	delete[] data;
	data = nullptr;
	m_size = m_capacity = 0;
}

// Public interface
size_t Vector::size() const {
	return m_size;
}

size_t Vector::capacity() const {
	return m_capacity;
}

void Vector::push_back(int element) {
	upsizeIfNeeded();
	data[m_size++] = element;
}

int Vector::pop_back() {
	if (empty()) {
		throw std::exception("Vector is empty");
	}
	return data[--m_size];
}

bool Vector::empty() const {
	return m_size == 0;
}

void Vector::clear() {
	m_size = 0;
}

void Vector::shrink_to_fit() {
	resize(m_size);
}

int& Vector::operator[](size_t index) {
	assertIndex(index);
	return data[index];
}

const int& Vector::operator[](size_t index) const {
	assertIndex(index);
	return data[index];
}

const int* Vector::data() const {
	return data;
}
