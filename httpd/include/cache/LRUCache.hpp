#pragma once

#include <cassert>
#include <list>
#include <map>
#include <iostream>
#include "exception/cache_exception.hh"

namespace zia
{
    namespace io
    {
        template<
                typename key,
                typename value,
                template<typename...> class map
        >
        class LruCache
        {
        public:
            using key_type_t = key;
            using value_type_t = value;
            using key_container_t = std::list<key_type_t>;
            using key_to_value_container_t = map<key_type_t, std::pair<value_type_t, typename key_container_t::iterator>>;
        public:
            LruCache(std::size_t const cacheSize);
            void put(key_type_t const &k, value_type_t const &v);
            value_type_t &retrieveValue(key_type_t const &k);
            value_type_t &getFirstElement()
            {
                auto it = keyValueList_.find(keyList_.front());
                return it->second.first;
            }
        private:
            void insert(key_type_t const &k, value_type_t const &v);
            void evict();
        private:
            std::size_t const        cacheSize_;
            key_container_t          keyList_;
            key_to_value_container_t keyValueList_;
        };

        template<
                typename key,
                typename value,
                template<typename...> class map
        >
        LruCache<key, value, map>::LruCache(std::size_t const cacheSize) : cacheSize_{cacheSize}
        {
            assert(cacheSize_ > 0 && "LruCache forbid consruction with a null cache size");
        };

        template<
                typename k,
                typename v,
                template<typename...> class map
        >
        typename LruCache<k, v, map>::value_type_t &
        LruCache<k, v, map>::retrieveValue(LruCache<k, v, map>::key_type_t const &key)
        {
            auto it = keyValueList_.find(key);
            if (it == keyValueList_.end())
            {
                std::cout << "KEY = " << key << std::endl;
                throw zia::cache_exception{"Error, key not found in cache"};
            }
            keyList_.splice(keyList_.end(), keyList_, it->second.second);
            return it->second.first;
        }

        template<
                typename k,
                typename v,
                template<typename...> class map
        >
        void
        LruCache<k, v, map>::put(LruCache<k, v, map>::key_type_t const &key,
                                 LruCache<k, v, map>::value_type_t const &value)
        {
            auto itKey = keyValueList_.find(key);
            if (itKey != keyValueList_.end())
            {
                itKey->second.first = value;
                return;
            }
            insert(key, value);
        };

        template<
                typename k,
                typename v,
                template<typename...> class map
        >
        void LruCache<k, v, map>::insert(LruCache<k, v, map>::key_type_t const &key,
                                         LruCache<k, v, map>::value_type_t const &value)
        {
            assert(keyValueList_.find(key) == keyValueList_.end() && "LruCache forbid insertion with an existing key");
            if (keyList_.size() == cacheSize_)
            {
                evict();
            }
            auto it = keyList_.insert(keyList_.end(), key);
            keyValueList_[key] = std::make_pair(value, it);
        };

        template<
                typename k,
                typename v,
                template<typename...> class map
        >
        void LruCache<k, v, map>::evict()
        {
            assert(!keyList_.empty() && "LruCache forbid eviction on empty cache");
            auto const it = keyValueList_.find(keyList_.front());
            assert(it != keyValueList_.end() && "LruCache forbid eviction on empty cache");
            keyValueList_.erase(it);
            keyList_.pop_front();
        };
    }
}