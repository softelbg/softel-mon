#pragma once

namespace SF {
namespace CORE {
namespace UTIL {


template <class K, class V>
class MapBase
{
public:
    virtual ~MapBase() {}

    size_t size() const {
        return map_.size();
    }
    
    void clear() {
        map_.clear();
    }

    typename std::map<K, V>::const_iterator cbegin() const {
        return map_.cbegin();
    }
    typename std::map<K, V>::const_iterator cend() const {
        return map_.cend();
    }
    typename std::map<K, V>::const_iterator find(const K& key) const {
        return map_.find(key);
    }
    std::string& operator[] (const K& key) {
        return map_[key];
    }

protected:
    std::map<K, V> map_;
};


} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */
