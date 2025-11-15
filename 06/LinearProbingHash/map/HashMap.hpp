#pragma once

#include <vector>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>
#include <functional>

template <class KeyType, class ValueType, class Hasher = std::hash<KeyType>>
class HashMap {
 public:
  using element = std::pair<KeyType, ValueType>;

  class ConstIterator {
   public:
    const element& operator*() const;
    ConstIterator operator++(int);
    ConstIterator& operator++();
    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;

   private:
    ConstIterator(int idx, const HashMap& ctx);
    void advance();
    int index;
    const HashMap& context;
    friend class HashMap<KeyType, ValueType, Hasher>;
  };

  explicit HashMap(size_t table_size = 10, size_t probe_step = 3);

  void add(const KeyType& key, const ValueType& value);
  void remove(const KeyType& key);
  ConstIterator get(const KeyType& key) const;
  ConstIterator cbegin() const;
  ConstIterator cend() const;
  size_t getSize() const;

 private:
  struct Bucket {
    std::optional<std::pair<KeyType, ValueType>> entry;
    bool tombstone = false;
  };

  std::vector<Bucket> buckets;
  size_t size;
  size_t k;
  double max_load_factor = 0.8;
  Hasher hasher;

  bool isActive(const Bucket& b) const {
    return b.entry.has_value() && !b.tombstone;
  }

  void resize(size_t new_size);
};

template <class KeyType, class ValueType, class Hasher>
HashMap<KeyType, ValueType, Hasher>::HashMap(size_t table_size,
                                             size_t probe_step)
    : size(0), k(probe_step), buckets(table_size) {}

template <class KeyType, class ValueType, class Hasher>
void HashMap<KeyType, ValueType, Hasher>::add(const KeyType& key,
                                               const ValueType& value) {
  double load_factor = static_cast<double>(size) / buckets.size();
  if (load_factor > max_load_factor) {
    resize(buckets.size() * 2);
  }

  size_t idx = hasher(key) % buckets.size();
  while (isActive(buckets[idx])) {
    if (buckets[idx].entry->first == key) {
      throw std::logic_error("Key already exists in the map");
    }
    idx = (idx + k) % buckets.size();
  }

  buckets[idx].entry = std::make_pair(key, value);
  buckets[idx].tombstone = false;
  ++size;
}

template <class KeyType, class ValueType, class Hasher>
void HashMap<KeyType, ValueType, Hasher>::remove(const KeyType& key) {
  size_t idx = hasher(key) % buckets.size();
  size_t start = idx;

  while (true) {
    if (!buckets[idx].entry.has_value() && !buckets[idx].tombstone) {
      break;
    }
    if (isActive(buckets[idx]) && buckets[idx].entry->first == key) {
      buckets[idx].tombstone = true;
      buckets[idx].entry.reset();
      --size;
      break;
    }
    idx = (idx + k) % buckets.size();
    if (idx == start) {
      break;
    }
  }
}

template <class KeyType, class ValueType, class Hasher>
typename HashMap<KeyType, ValueType, Hasher>::ConstIterator
HashMap<KeyType, ValueType, Hasher>::get(const KeyType& key) const {
  size_t idx = hasher(key) % buckets.size();
  size_t start = idx;

  while (true) {
    if (!buckets[idx].entry.has_value() && !buckets[idx].tombstone) {
      return cend();
    }
    if (isActive(buckets[idx]) && buckets[idx].entry->first == key) {
      return ConstIterator(static_cast<int>(idx), *this);
    }
    idx = (idx + k) % buckets.size();
    if (idx == start) {
      return cend();
    }
  }
}

template <class KeyType, class ValueType, class Hasher>
typename HashMap<KeyType, ValueType, Hasher>::ConstIterator
HashMap<KeyType, ValueType, Hasher>::cbegin() const {
  if (size == 0) {
    return cend();
  }

  for (int i = 0; i < static_cast<int>(buckets.size()); ++i) {
    if (isActive(buckets[i])) {
      return ConstIterator(i, *this);
    }
  }

  return cend();
}

template <class KeyType, class ValueType, class Hasher>
typename HashMap<KeyType, ValueType, Hasher>::ConstIterator
HashMap<KeyType, ValueType, Hasher>::cend() const {
  return ConstIterator(-1, *this);
}

template <class KeyType, class ValueType, class Hasher>
size_t HashMap<KeyType, ValueType, Hasher>::getSize() const {
  return size;
}

template <class KeyType, class ValueType, class Hasher>
void HashMap<KeyType, ValueType, Hasher>::resize(size_t new_size) {
  std::vector<Bucket> old_buckets = std::move(buckets);
  buckets.resize(new_size);

  for (auto& b : buckets) {
    b.entry.reset();
    b.tombstone = false;
  }

  size = 0;
  for (auto& b : old_buckets) {
    if (isActive(b)) {
      add(b.entry->first, b.entry->second);
    }
  }
}

template <class KeyType, class ValueType, class Hasher>
HashMap<KeyType, ValueType, Hasher>::ConstIterator::ConstIterator(
    int idx, const HashMap& ctx)
    : index(idx), context(ctx) {}

template <class KeyType, class ValueType, class Hasher>
const typename HashMap<KeyType, ValueType, Hasher>::element&
HashMap<KeyType, ValueType, Hasher>::ConstIterator::operator*() const {
  return *context.buckets[index].entry;
}

template <class KeyType, class ValueType, class Hasher>
typename HashMap<KeyType, ValueType, Hasher>::ConstIterator
HashMap<KeyType, ValueType, Hasher>::ConstIterator::operator++(int) {
  ConstIterator old(*this);
  advance();
  return old;
}

template <class KeyType, class ValueType, class Hasher>
typename HashMap<KeyType, ValueType, Hasher>::ConstIterator&
HashMap<KeyType, ValueType, Hasher>::ConstIterator::operator++() {
  advance();
  return *this;
}

template <class KeyType, class ValueType, class Hasher>
bool HashMap<KeyType, ValueType, Hasher>::ConstIterator::operator==(
    const ConstIterator& other) const {
  return (&context == &other.context) && (index == other.index);
}

template <class KeyType, class ValueType, class Hasher>
bool HashMap<KeyType, ValueType, Hasher>::ConstIterator::operator!=(
    const ConstIterator& other) const {
  return !(*this == other);
}

template <class KeyType, class ValueType, class Hasher>
void HashMap<KeyType, ValueType, Hasher>::ConstIterator::advance() {
  do {
    ++index;
  } while (index >= 0 && index < static_cast<int>(context.buckets.size()) &&
           !context.isActive(context.buckets[index]));

  if (index < 0 || index >= static_cast<int>(context.buckets.size())) {
    index = -1;
  }
}
