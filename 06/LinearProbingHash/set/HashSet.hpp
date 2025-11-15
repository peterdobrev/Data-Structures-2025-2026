#pragma once

#include <vector>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>
#include <functional>

template <class KeyType, class Hasher = std::hash<KeyType>>
class HashSet {
 public:
  class ConstIterator {
   public:
    const KeyType& operator*() const;
    ConstIterator operator++(int);
    ConstIterator& operator++();
    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;

   private:
    ConstIterator(int index, const HashSet<KeyType, Hasher>& context);
    int index;
    const HashSet<KeyType, Hasher>& context;
    friend class HashSet<KeyType, Hasher>;
  };

  explicit HashSet(size_t table_size = 10, size_t probe_step = 3);
  
  void add(const KeyType& key);
  void remove(const KeyType& key);
  ConstIterator get(const KeyType& key) const;
  ConstIterator cbegin() const;
  ConstIterator cend() const;
  size_t getSize() const;

 private:
  struct Data {
    std::optional<KeyType> data;
    bool tombstone = false;
  };

  std::vector<Data> data;
  size_t size;
  size_t k;
  double max_load_factor = 0.8;
  Hasher hasher;

  bool containsElementAtIndex(size_t index) const {
    return data[index].data.has_value() && !data[index].tombstone;
  }

  void resize(size_t newSize);
};

template <class KeyType, class Hasher>
HashSet<KeyType, Hasher>::HashSet(size_t table_size, size_t probe_step)
    : size(0), k(probe_step) {
  data.resize(table_size);
  for (auto& d : data) {
    d.data.reset();
    d.tombstone = false;
  }
}

template <class KeyType, class Hasher>
void HashSet<KeyType, Hasher>::add(const KeyType& key) {
  double load_factor = static_cast<double>(size) / data.size();
  if (load_factor > max_load_factor) {
    resize(data.size() * 2);
  }

  int index_to_put = hasher(key) % data.size();
  while (containsElementAtIndex(index_to_put)) {
    if (*data[index_to_put].data == key) {
      throw std::logic_error("Key already exists in the set");
    }
    index_to_put = (index_to_put + k) % data.size();
  }

  data[index_to_put].data = key;
  data[index_to_put].tombstone = false;
  size++;
}

template <class KeyType, class Hasher>
void HashSet<KeyType, Hasher>::remove(const KeyType& key) {
  int index = hasher(key) % data.size();
  int start = index;

  while (data[index].data.has_value()) {
    if (!data[index].tombstone && *data[index].data == key) {
      data[index].tombstone = true;
      size--;
      return;
    }
    index = (index + k) % data.size();
    if (index == start) {
      return;  
    }
  }
}

template <class KeyType, class Hasher>
typename HashSet<KeyType, Hasher>::ConstIterator
HashSet<KeyType, Hasher>::get(const KeyType& key) const {
  int index = hasher(key) % data.size();
  int start = index;

  while (data[index].data.has_value()) {
    if (!data[index].tombstone && *data[index].data == key) {
      return ConstIterator(index, *this);
    }
    index = (index + k) % data.size();
    if (index == start) {
      break;  
    }
  }

  return cend();
}

template <class KeyType, class Hasher>
typename HashSet<KeyType, Hasher>::ConstIterator
HashSet<KeyType, Hasher>::cbegin() const {
  if (size == 0) {
    return cend();
  }

  for (int i = 0; i < static_cast<int>(data.size()); ++i) {
    if (containsElementAtIndex(i)) {
      return ConstIterator(i, *this);
    }
  }

  return cend();
}

template <class KeyType, class Hasher>
typename HashSet<KeyType, Hasher>::ConstIterator
HashSet<KeyType, Hasher>::cend() const {
  return ConstIterator(-1, *this);
}

template <class KeyType, class Hasher>
size_t HashSet<KeyType, Hasher>::getSize() const {
  return size;
}

template <class KeyType, class Hasher>
void HashSet<KeyType, Hasher>::resize(size_t new_size) {
  std::vector<Data> old_data = data;
  data.clear();
  data.resize(new_size);

  for (auto& d : data) {
    d.data.reset();
    d.tombstone = false;
  }

  size = 0;
  for (auto& d : old_data) {
    if (d.data.has_value() && !d.tombstone) {
      add(*d.data);
    }
  }
}

template <class KeyType, class Hasher>
HashSet<KeyType, Hasher>::ConstIterator::ConstIterator(
    int index, const HashSet<KeyType, Hasher>& context)
    : index(index), context(context) {}

template <class KeyType, class Hasher>
const KeyType& HashSet<KeyType, Hasher>::ConstIterator::operator*() const {
  return *context.data[index].data;
}

template <class KeyType, class Hasher>
typename HashSet<KeyType, Hasher>::ConstIterator
HashSet<KeyType, Hasher>::ConstIterator::operator++(int) {
  int old_index = index;
  do {
    index++;
  } while (index < static_cast<int>(context.data.size()) &&
           !context.containsElementAtIndex(index));

  if (index >= static_cast<int>(context.data.size())) {
    index = -1;
  }

  return ConstIterator(old_index, context); 
}

template <class KeyType, class Hasher>
typename HashSet<KeyType, Hasher>::ConstIterator&
HashSet<KeyType, Hasher>::ConstIterator::operator++() {
  do {
    index++;
  } while (index < static_cast<int>(context.data.size()) &&
           !context.containsElementAtIndex(index));

  if (index >= static_cast<int>(context.data.size())) {
    index = -1;
  }

  return *this;
}

template <class KeyType, class Hasher>
bool HashSet<KeyType, Hasher>::ConstIterator::operator==(
    const ConstIterator& other) const {
  return index == other.index;
}

template <class KeyType, class Hasher>
bool HashSet<KeyType, Hasher>::ConstIterator::operator!=(
    const ConstIterator& other) const {
  return index != other.index;
}
