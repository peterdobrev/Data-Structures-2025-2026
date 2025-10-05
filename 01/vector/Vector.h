#pragma once
#include <exception>

class Vector {
private:
	static const short INITIAL_CAPACITY = 4;
	static const short RESIZE_COEF = 2;

	int* data;
	size_t m_size;
	size_t m_capacity;

	void resize(size_t newCapacity);
	void assertIndex(size_t index) const;
	void upsizeIfNeeded();
	void move(Vector&& other);
	void copyFrom(const Vector& other);
	void free();

public:
	Vector();
	Vector(size_t capacity);
	Vector(const Vector& other);
	Vector(Vector&& other);
	Vector& operator=(const Vector& other);
	Vector& operator=(Vector&& other);
	~Vector();

	size_t size() const;
	size_t capacity() const;

	void push_back(int element);
	int pop_back();

	bool empty() const;
	void clear();
	void shrink_to_fit();

	int& operator[](size_t index);
	const int& operator[](size_t index) const;

	const int* data() const;
};
