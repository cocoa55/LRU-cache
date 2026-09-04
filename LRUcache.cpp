#include <cstddef>
#include <list>
#include <optional>
#include <unordered_map>
#include <utility>

template <typename Key, typename Value> class LRUCache {
private:
  std::size_t _capacity;
  std::list<std::pair<Key, Value>> _cache;
  std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator>
      _map;

  void touch(typename std::list<std::pair<Key, Value>>::iterator itr) {
    _cache.splice(_cache.begin(), _cache, itr);
  }

public:
  LRUCache(std::size_t capacity) : _capacity(capacity) {}

  void put(const Key &key, const Value &value) { // this function "uses" a cache
    auto itr = _map.find(key);
    if (itr != _map.end()) {
      touch(itr->second);
      itr->second->second = value;
      return;
    }
    if (_cache.size() == _capacity) { // if the cache is full, we need to
                                      // remove,
      auto &back = _cache.back();
      _map.erase(back.first); // remove the cache from the map
      _cache.pop_back();      // remove the least recently used
    }
    _cache.emplace_front(key, value); // place new cache in the line head,

    _map[key] = _cache.begin(); // keep track of it via map
  }

  std::optional<Value> get(const Key &key) { // this function "uses" a cache

    auto add = _map.find(key);

    if (add != _map.end()) {
      touch(add->second);
      return add->second->second;
    } else {
      return std::nullopt;
    }
  }

  bool contains(const Key &key) const { return _map.contains(key); }

  bool erase(const Key &key) {
    auto remove = _map.find(key);

    if (remove != _map.end()) {
      _cache.erase(remove->second); //
      _map.erase(remove);           // remove the ma
      return true;
    }
    return false;
  }

  [[nodiscard]] std::size_t size() const { return _cache.size(); }

  std::size_t capacity() const { return _capacity; }
};
