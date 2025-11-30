#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <utility>

template <typename Key, typename Hasher = std::hash<Key>>
class UnorderedSet {
 public:
  class ConstUnorderedSetIterator {
   public:
    ConstUnorderedSetIterator();
    ConstUnorderedSetIterator& operator++();
    ConstUnorderedSetIterator operator++(int);
    const Key& operator*() const;
    const Key* operator->() const;
    bool operator==(const ConstUnorderedSetIterator& other) const;
    bool operator!=(const ConstUnorderedSetIterator& other) const;

   private:
    ConstUnorderedSetIterator(typename std::list<Key>::const_iterator it);
    typename std::list<Key>::const_iterator curr_element;
    friend class UnorderedSet;
  };

  explicit UnorderedSet(size_t init_hash_size = 16);
  
  std::pair<bool, ConstUnorderedSetIterator> insert(const Key& element);
  ConstUnorderedSetIterator find(const Key& element) const;
  bool remove(const Key& element);
  bool remove(const ConstUnorderedSetIterator& iter);
  void clear();
  bool empty() const;
  size_t size() const;
  ConstUnorderedSetIterator cbegin() const;
  ConstUnorderedSetIterator cend() const;

 private:
  std::list<Key> data;
  std::vector<std::pair<typename std::list<Key>::iterator, size_t>> hash_table;
  size_t element_count = 0;
  double load_factor_threshold = 0.75;
  Hasher hasher;

  typename std::list<Key>::iterator getElementByChain(size_t chain_index,
                                                      const Key& element);
  typename std::list<Key>::const_iterator getElementByChain(
      size_t chain_index, const Key& element) const;
  void rehash(size_t new_size);
};

template <typename Key, typename Hasher>
UnorderedSet<Key, Hasher>::UnorderedSet(size_t init_hash_size)
    : hash_table(init_hash_size, std::make_pair(data.end(), 0)) {}

template <typename Key, typename Hasher>
std::pair<bool, typename UnorderedSet<Key, Hasher>::ConstUnorderedSetIterator>
UnorderedSet<Key, Hasher>::insert(const Key& element) {
  if (hash_table.empty()) {
    hash_table.resize(16, std::make_pair(data.end(), 0));
  }

  size_t bucket_index = hasher(element) % hash_table.size();
  typename std::list<Key>::iterator found_it =
      getElementByChain(bucket_index, element);

  if (found_it != data.end()) {
    return std::make_pair(false, ConstUnorderedSetIterator(found_it));
  }

  std::pair<typename std::list<Key>::iterator, size_t>& chain_info =
      hash_table[bucket_index];

  if (chain_info.second == 0) {
    data.push_front(element);
    chain_info.first = data.begin();
  } else {
    typename std::list<Key>::iterator new_it =
        data.insert(chain_info.first, element);
    chain_info.first = new_it;
  }

  chain_info.second++;
  element_count++;

  double current_load_factor =
      static_cast<double>(element_count) / hash_table.size();
  if (current_load_factor > load_factor_threshold) {
    rehash(hash_table.size() * 2);
    size_t new_bucket_index = hasher(element) % hash_table.size();
    typename std::list<Key>::iterator new_pos =
        getElementByChain(new_bucket_index, element);
    return std::make_pair(true, ConstUnorderedSetIterator(new_pos));
  }

  return std::make_pair(true, ConstUnorderedSetIterator(chain_info.first));
}

template <typename Key, typename Hasher>
typename UnorderedSet<Key, Hasher>::ConstUnorderedSetIterator
UnorderedSet<Key, Hasher>::find(const Key& element) const {
  if (hash_table.empty()) {
    return cend();
  }

  size_t bucket_index = hasher(element) % hash_table.size();
  typename std::list<Key>::const_iterator found_it =
      getElementByChain(bucket_index, element);

  if (found_it == data.cend()) {
    return cend();
  }

  return ConstUnorderedSetIterator(found_it);
}

template <typename Key, typename Hasher>
bool UnorderedSet<Key, Hasher>::remove(const Key& element) {
  if (hash_table.empty()) {
    return false;
  }

  size_t bucket_index = hasher(element) % hash_table.size();
  typename std::list<Key>::iterator found_it =
      getElementByChain(bucket_index, element);

  if (found_it == data.end()) {
    return false;
  }

  std::pair<typename std::list<Key>::iterator, size_t>& chain_info =
      hash_table[bucket_index];

  if (found_it == chain_info.first) {
    typename std::list<Key>::iterator next_it = found_it;
    ++next_it;
    chain_info.first = next_it;
  }

  data.erase(found_it);
  chain_info.second--;
  element_count--;

  return true;
}

template <typename Key, typename Hasher>
bool UnorderedSet<Key, Hasher>::remove(
    const ConstUnorderedSetIterator& iter) {
  if (iter == cend() || hash_table.empty()) {
    return false;
  }

  const Key& element = *iter;
  return remove(element);
}

template <typename Key, typename Hasher>
void UnorderedSet<Key, Hasher>::clear() {
  data.clear();
  hash_table.clear();
  element_count = 0;
}

template <typename Key, typename Hasher>
bool UnorderedSet<Key, Hasher>::empty() const {
  return (element_count == 0);
}

template <typename Key, typename Hasher>
size_t UnorderedSet<Key, Hasher>::size() const {
  return element_count;
}

template <typename Key, typename Hasher>
typename UnorderedSet<Key, Hasher>::ConstUnorderedSetIterator
UnorderedSet<Key, Hasher>::cbegin() const {
  return ConstUnorderedSetIterator(data.cbegin());
}

template <typename Key, typename Hasher>
typename UnorderedSet<Key, Hasher>::ConstUnorderedSetIterator
UnorderedSet<Key, Hasher>::cend() const {
  return ConstUnorderedSetIterator(data.cend());
}

template <typename Key, typename Hasher>
typename std::list<Key>::iterator
UnorderedSet<Key, Hasher>::getElementByChain(size_t chain_index,
                                             const Key& element) {
  size_t chain_size = hash_table[chain_index].second;
  if (chain_size == 0) {
    return data.end();
  }

  typename std::list<Key>::iterator curr_it = hash_table[chain_index].first;
  for (size_t i = 0; i < chain_size; ++i) {
    if (*curr_it == element) {
      return curr_it;
    }
    ++curr_it;
  }

  return data.end();
}

template <typename Key, typename Hasher>
typename std::list<Key>::const_iterator
UnorderedSet<Key, Hasher>::getElementByChain(size_t chain_index,
                                             const Key& element) const {
  size_t chain_size = hash_table[chain_index].second;
  if (chain_size == 0) {
    return data.cend();
  }

  typename std::list<Key>::const_iterator curr_it =
      hash_table[chain_index].first;
  for (size_t i = 0; i < chain_size; ++i) {
    if (*curr_it == element) {
      return curr_it;
    }
    ++curr_it;
  }

  return data.cend();
}

template <typename Key, typename Hasher>
void UnorderedSet<Key, Hasher>::rehash(size_t new_size) {
  std::vector<Key> old_elements;
  old_elements.reserve(element_count);

  typename std::list<Key>::iterator it = data.begin();
  while (it != data.end()) {
    old_elements.push_back(*it);
    ++it;
  }

  data.clear();
  hash_table.clear();
  hash_table.resize(new_size, std::make_pair(data.end(), 0));
  element_count = 0;

  typename std::vector<Key>::iterator vec_it = old_elements.begin();
  while (vec_it != old_elements.end()) {
    insert(*vec_it);
    ++vec_it;
  }
}

template <typename Key, typename Hasher>
UnorderedSet<Key, Hasher>::ConstUnorderedSetIterator::
    ConstUnorderedSetIterator() {}

template <typename Key, typename Hasher>
UnorderedSet<Key, Hasher>::ConstUnorderedSetIterator::
    ConstUnorderedSetIterator(typename std::list<Key>::const_iterator it)
    : curr_element(it) {}

template <typename Key, typename Hasher>
typename UnorderedSet<Key, Hasher>::ConstUnorderedSetIterator&
UnorderedSet<Key, Hasher>::ConstUnorderedSetIterator::operator++() {
  ++curr_element;
  return *this;
}

template <typename Key, typename Hasher>
typename UnorderedSet<Key, Hasher>::ConstUnorderedSetIterator
UnorderedSet<Key, Hasher>::ConstUnorderedSetIterator::operator++(int) {
  ConstUnorderedSetIterator temp = *this;
  ++(*this);
  return temp;
}

template <typename Key, typename Hasher>
const Key& UnorderedSet<Key, Hasher>::ConstUnorderedSetIterator::operator*()
    const {
  return *curr_element;
}

template <typename Key, typename Hasher>
const Key*
UnorderedSet<Key, Hasher>::ConstUnorderedSetIterator::operator->() const {
  return &(*curr_element);
}

template <typename Key, typename Hasher>
bool UnorderedSet<Key, Hasher>::ConstUnorderedSetIterator::operator==(
    const ConstUnorderedSetIterator& other) const {
  return curr_element == other.curr_element;
}

template <typename Key, typename Hasher>
bool UnorderedSet<Key, Hasher>::ConstUnorderedSetIterator::operator!=(
    const ConstUnorderedSetIterator& other) const {
  return curr_element != other.curr_element;
}
