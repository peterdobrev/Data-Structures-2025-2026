#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility>
#include <list>

template <typename Key, typename T, typename Hasher = std::hash<Key>>
class UnorderedMap {
 public:
  class ConstUnorderedMapIterator {
   public:
    ConstUnorderedMapIterator();
    ConstUnorderedMapIterator& operator++();
    ConstUnorderedMapIterator operator++(int);
    const std::pair<Key, T>& operator*() const;
    const std::pair<Key, T>* operator->() const;
    bool operator==(const ConstUnorderedMapIterator& other) const;
    bool operator!=(const ConstUnorderedMapIterator& other) const;

   private:
    ConstUnorderedMapIterator(
        typename std::list<std::pair<Key, T>>::const_iterator it);
    typename std::list<std::pair<Key, T>>::const_iterator curr_element;
    friend class UnorderedMap;
  };

  explicit UnorderedMap(size_t init_hash_size = 16);

  std::pair<bool, ConstUnorderedMapIterator> insert(const Key& key,
                                                     const T& value);
  ConstUnorderedMapIterator find(const Key& key) const;
  bool remove(const Key& key);
  bool remove(const ConstUnorderedMapIterator& iter);
  void clear();
  bool empty() const;
  size_t size() const;
  ConstUnorderedMapIterator cbegin() const;
  ConstUnorderedMapIterator cend() const;

 private:
  std::list<std::pair<Key, T>> data;
  std::vector<std::pair<typename std::list<std::pair<Key, T>>::iterator,
                        size_t>>
      hash_table;
  size_t element_count = 0;
  double load_factor_threshold = 0.75;
  Hasher hasher;

  typename std::list<std::pair<Key, T>>::iterator getElementByChain(
      size_t chain_index, const Key& key);
  typename std::list<std::pair<Key, T>>::const_iterator getElementByChain(
      size_t chain_index, const Key& key) const;
  void rehash(size_t new_size);
};

template <typename Key, typename T, typename Hasher>
UnorderedMap<Key, T, Hasher>::UnorderedMap(size_t init_hash_size)
    : hash_table(init_hash_size, std::make_pair(data.end(), 0)) {}

template <typename Key, typename T, typename Hasher>
std::pair<bool, typename UnorderedMap<Key, T, Hasher>::ConstUnorderedMapIterator>
UnorderedMap<Key, T, Hasher>::insert(const Key& key, const T& value) {
  if (hash_table.empty()) {
    hash_table.resize(16, std::make_pair(data.end(), 0));
  }

  size_t bucket_index = hasher(key) % hash_table.size();
  auto found_it = getElementByChain(bucket_index, key);

  if (found_it != data.end()) {
    return std::make_pair(false, ConstUnorderedMapIterator(found_it));
  }

  auto& chain_info = hash_table[bucket_index];

  if (chain_info.second == 0) {
    data.push_front(std::make_pair(key, value));
    chain_info.first = data.begin();
  } else {
    auto new_it = data.insert(chain_info.first, std::make_pair(key, value));
    chain_info.first = new_it;
  }

  chain_info.second++;
  element_count++;

  double current_load_factor =
      static_cast<double>(element_count) / hash_table.size();
  if (current_load_factor > load_factor_threshold) {
    rehash(hash_table.size() * 2);
    size_t new_bucket_index = hasher(key) % hash_table.size();
    auto new_pos = getElementByChain(new_bucket_index, key);
    return std::make_pair(true, ConstUnorderedMapIterator(new_pos));
  }

  return std::make_pair(true, ConstUnorderedMapIterator(chain_info.first));
}

template <typename Key, typename T, typename Hasher>
typename UnorderedMap<Key, T, Hasher>::ConstUnorderedMapIterator
UnorderedMap<Key, T, Hasher>::find(const Key& key) const {
  if (hash_table.empty()) {
    return cend();
  }

  size_t bucket_index = hasher(key) % hash_table.size();
  auto found_it = getElementByChain(bucket_index, key);

  if (found_it == data.cend()) {
    return cend();
  }

  return ConstUnorderedMapIterator(found_it);
}

template <typename Key, typename T, typename Hasher>
bool UnorderedMap<Key, T, Hasher>::remove(const Key& key) {
  if (hash_table.empty()) {
    return false;
  }

  size_t bucket_index = hasher(key) % hash_table.size();
  auto found_it = getElementByChain(bucket_index, key);

  if (found_it == data.end()) {
    return false;
  }

  auto& chain_info = hash_table[bucket_index];

  if (found_it == chain_info.first) {
    auto next_it = found_it;
    ++next_it;
    chain_info.first = next_it;
  }

  data.erase(found_it);
  chain_info.second--;
  element_count--;

  return true;
}

template <typename Key, typename T, typename Hasher>
bool UnorderedMap<Key, T, Hasher>::remove(
    const ConstUnorderedMapIterator& iter) {
  if (iter == cend() || hash_table.empty()) {
    return false;
  }

  const Key& key = iter.curr_element->first;
  return remove(key);
}

template <typename Key, typename T, typename Hasher>
void UnorderedMap<Key, T, Hasher>::clear() {
  data.clear();
  hash_table.clear();
  element_count = 0;
}

template <typename Key, typename T, typename Hasher>
bool UnorderedMap<Key, T, Hasher>::empty() const {
  return element_count == 0;
}

template <typename Key, typename T, typename Hasher>
size_t UnorderedMap<Key, T, Hasher>::size() const {
  return element_count;
}

template <typename Key, typename T, typename Hasher>
typename UnorderedMap<Key, T, Hasher>::ConstUnorderedMapIterator
UnorderedMap<Key, T, Hasher>::cbegin() const {
  return ConstUnorderedMapIterator(data.cbegin());
}

template <typename Key, typename T, typename Hasher>
typename UnorderedMap<Key, T, Hasher>::ConstUnorderedMapIterator
UnorderedMap<Key, T, Hasher>::cend() const {
  return ConstUnorderedMapIterator(data.cend());
}

template <typename Key, typename T, typename Hasher>
typename std::list<std::pair<Key, T>>::iterator
UnorderedMap<Key, T, Hasher>::getElementByChain(size_t chain_index,
                                                const Key& key) {
  size_t chain_size = hash_table[chain_index].second;
  if (chain_size == 0) {
    return data.end();
  }

  auto curr_it = hash_table[chain_index].first;
  for (size_t i = 0; i < chain_size; ++i) {
    if (curr_it->first == key) {
      return curr_it;
    }
    ++curr_it;
  }

  return data.end();
}

template <typename Key, typename T, typename Hasher>
typename std::list<std::pair<Key, T>>::const_iterator
UnorderedMap<Key, T, Hasher>::getElementByChain(size_t chain_index,
                                                const Key& key) const {
  size_t chain_size = hash_table[chain_index].second;
  if (chain_size == 0) {
    return data.cend();
  }

  auto curr_it = hash_table[chain_index].first;
  for (size_t i = 0; i < chain_size; ++i) {
    if (curr_it->first == key) {
      return curr_it;
    }
    ++curr_it;
  }

  return data.cend();
}

template <typename Key, typename T, typename Hasher>
void UnorderedMap<Key, T, Hasher>::rehash(size_t new_size) {
  std::vector<std::pair<Key, T>> old_elements;
  old_elements.reserve(element_count);

  for (auto it = data.begin(); it != data.end(); ++it) {
    old_elements.push_back(*it);
  }

  data.clear();
  hash_table.clear();
  hash_table.resize(new_size, std::make_pair(data.end(), 0));
  element_count = 0;

  for (auto& elem : old_elements) {
    insert(elem.first, elem.second);
  }
}

template <typename Key, typename T, typename Hasher>
UnorderedMap<Key, T, Hasher>::ConstUnorderedMapIterator::
    ConstUnorderedMapIterator() {}

template <typename Key, typename T, typename Hasher>
UnorderedMap<Key, T, Hasher>::ConstUnorderedMapIterator::
    ConstUnorderedMapIterator(
        typename std::list<std::pair<Key, T>>::const_iterator it)
    : curr_element(it) {}

template <typename Key, typename T, typename Hasher>
typename UnorderedMap<Key, T, Hasher>::ConstUnorderedMapIterator&
UnorderedMap<Key, T, Hasher>::ConstUnorderedMapIterator::operator++() {
  ++curr_element;
  return *this;
}

template <typename Key, typename T, typename Hasher>
typename UnorderedMap<Key, T, Hasher>::ConstUnorderedMapIterator
UnorderedMap<Key, T, Hasher>::ConstUnorderedMapIterator::operator++(int) {
  ConstUnorderedMapIterator temp = *this;
  ++(*this);
  return temp;
}

template <typename Key, typename T, typename Hasher>
const std::pair<Key, T>&
UnorderedMap<Key, T, Hasher>::ConstUnorderedMapIterator::operator*() const {
  return *curr_element;
}

template <typename Key, typename T, typename Hasher>
const std::pair<Key, T>*
UnorderedMap<Key, T, Hasher>::ConstUnorderedMapIterator::operator->() const {
  return &(*curr_element);
}

template <typename Key, typename T, typename Hasher>
bool UnorderedMap<Key, T, Hasher>::ConstUnorderedMapIterator::operator==(
    const ConstUnorderedMapIterator& other) const {
  return curr_element == other.curr_element;
}

template <typename Key, typename T, typename Hasher>
bool UnorderedMap<Key, T, Hasher>::ConstUnorderedMapIterator::operator!=(
    const ConstUnorderedMapIterator& other) const {
  return curr_element != other.curr_element;
}
