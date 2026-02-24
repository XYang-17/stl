#ifndef _YXXX_HASH_MAP_H_
#define _YXXX_HASH_MAP_H_

#include "hashtable.h"

namespace stl{
    template <bool _UniqueKey>
    using _map_traits = _hash::_hashtable_traits<true, _UniqueKey, false>;

    template <typename _KeyType, typename _ValueType,
        typename _Hash = std::hash<_KeyType>,
        typename _Equal = stl::equal_to<_KeyType>,
        typename _ExtractKey = stl::_first_arg,
        typename _TruncateHash = stl::modulus<std::size_t>,
        typename _Rehash = _hash::_prime_rehash,
        typename _Traits = _map_traits<true>,
        typename _Alloc = _alloc::allocator<std::pair<const _KeyType, _ValueType>>>
    class hash_map{
        using _hashtable = _hash::_hashtable<_KeyType, std::pair<const _KeyType, _ValueType>,
            _Hash, _Equal, _ExtractKey, _TruncateHash, _Rehash, _Traits, _Alloc>;
    public:
        using key_type           = typename _hashtable::key_type;
        using value_type         = typename _hashtable::value_type;
        using mapped_type        = typename _hashtable::mapped_type;
        using hasher             = typename _hashtable::hasher;
        using key_equal          = typename _hashtable::key_equal;
        using hash_code          = typename _hashtable::hash_code;

        using pointer            = typename _hashtable::pointer;
        using const_pointer      = typename _hashtable::const_pointer;
        using reference          = typename _hashtable::reference;
        using const_reference    = typename _hashtable::const_reference;
        using size_type          = typename _hashtable::size_type;
        using difference_type    = typename _hashtable::difference_type;

        using allocator_type     = typename _hashtable::allocator_type;
        
        using self_type        = hash_map;
        using iterator         = typename _hashtable::iterator;
        using const_iterator   = typename _hashtable::const_iterator;
        using local_iterator   = typename _hashtable::local_iterator;
        using const_local_iterator = typename _hashtable::const_local_iterator;

        hash_map(
            size_type _bkt_count = 47,
            float _max_load = 0.75,
            const hasher& _hash = hasher(),
            const key_equal& _equal = key_equal(),
            const allocator_type& _alloc = allocator_type()
        ): _M_hashtable(_bkt_count, _max_load, _hash, _equal, _alloc){}
        hash_map(const hash_map& _ht) _YXXX_NOEXCEPT: _M_hashtable(_ht._M_hashtable){}
        template <typename InputIter>
        hash_map(InputIter _first, InputIter _last): _M_hashtable(_first, _last){}
        hash_map(std::initializer_list<value_type>&& _ilist):
            _M_hashtable(std::move(_ilist)){}
        hash_map(hash_map&& _ht) _YXXX_NOEXCEPT:_M_hashtable(std::move(_ht._M_hashtable)){}

        mapped_type& operator[](const key_type& _key){return _M_hashtable[_key];}
        mapped_type& operator[](key_type&& _key){return _M_hashtable[std::move(_key)];}
        std::pair<iterator, bool> insert(const value_type& _val) _YXXX_NOEXCEPT{
            return _M_hashtable.insert(_val);
        }
        std::pair<iterator, bool> insert(value_type&& _val) _YXXX_NOEXCEPT{
            return _M_hashtable.insert(std::move(_val));
        }
        template <typename InputIter>
        void insert(InputIter _first, InputIter _last){
            _M_hashtable.insert(_first, _last);
        }
        template <typename Key, typename ...Args>
        std::pair<iterator, bool> emplace(Key&& _key, Args&&... _args){
            return _M_hashtable.emplace(
                std::forward<Key>(_key),
                std::forward<Args>(_args)...
            );
        }
        std::pair<local_iterator, local_iterator>
        equal_range(const key_type& _key) _YXXX_NOEXCEPT{
            return _M_hashtable.equal_range(_key);
        }
        std::pair<const_local_iterator, const_local_iterator>
        equal_range(const key_type& _key) const _YXXX_NOEXCEPT{
            return _M_hashtable.equal_range(_key);
        }
        void erase(const key_type& _key) _YXXX_NOEXCEPT{_M_hashtable.erase(_key);}
        iterator find(const key_type& _key) _YXXX_NOEXCEPT{return _M_hashtable.find(_key);}
        const_iterator find(const key_type& _key) const _YXXX_NOEXCEPT{
            return _M_hashtable.find(_key);
        }
        void rehash(size_type _bucket_count) _YXXX_NOEXCEPT{_M_hashtable.rehash(_bucket_count);}

        iterator begin() _YXXX_NOEXCEPT{return _M_hashtable.begin();}
        const_iterator begin() const _YXXX_NOEXCEPT{return _M_hashtable.begin();}
        iterator end() _YXXX_NOEXCEPT{return _M_hashtable.end();}
        const_iterator end() const _YXXX_NOEXCEPT{return _M_hashtable.end();}
        const_iterator cbegin() const _YXXX_NOEXCEPT{return _M_hashtable.cbegin();}
        const_iterator cend() const _YXXX_NOEXCEPT{return _M_hashtable.cend();}
        local_iterator begin(size_type _bkt_idx){return _M_hashtable.begin(_bkt_idx);}
        const_local_iterator begin(size_type _bkt_idx) const{
            return _M_hashtable.begin(_bkt_idx);
        }
        local_iterator end(size_type _bkt_idx){
            return _M_hashtable.end(_bkt_idx);
        }
        const_local_iterator end(size_type _bkt_idx) const{
            return _M_hashtable.end(_bkt_idx);
        }
        const_local_iterator cbegin(size_type _bkt_idx) const{
            return _M_hashtable.cbegin(_bkt_idx);
        }
        const_local_iterator cend(size_type _bkt_idx) const{
            return _M_hashtable.cend(_bkt_idx);
        }

        bool empty() const _YXXX_NOEXCEPT{return _M_hashtable.empty();}
        bool empty(difference_type _n) const{return _M_hashtable.empty(_n);}
        size_type size() const _YXXX_NOEXCEPT{return _M_hashtable.size();}
        size_type size(difference_type _bkt_index) const _YXXX_NOEXCEPT{
            return _M_hashtable.size(_bkt_index);
        }
        size_type count(const key_type& _key) const _YXXX_NOEXCEPT{
            return _M_hashtable.count(_key);
        }
        size_type bucket_count() const _YXXX_NOEXCEPT{return _M_hashtable.bucket_count();}
        float load_factor() const _YXXX_NOEXCEPT{return _M_hashtable.load_factor();}
        float max_load_factor() const _YXXX_NOEXCEPT{return _M_hashtable.max_load_factor();}
        void max_load_factor(float _mlf) _YXXX_NOEXCEPT{_M_hashtable.max_load_factor(_mlf);}
        void clear() _YXXX_NOEXCEPT{return _M_hashtable.clear();}
        friend void swap(self_type& _lh, self_type& _rh) _YXXX_NOEXCEPT{
            swap(_lh._M_hashtable, _rh._M_hashtable);
        }
    protected:
        _hashtable _M_hashtable;
    };
    template <typename _KeyType, typename _ValueType,
        typename _Hash = std::hash<_KeyType>,
        typename _Equal = stl::equal_to<_KeyType>,
        typename _ExtractKey = stl::_first_arg,
        typename _TruncateHash = stl::modulus<std::size_t>,
        typename _Rehash = _hash::_prime_rehash,
        typename _Traits = _map_traits<false>,
        typename _Alloc = _alloc::allocator<std::pair<const _KeyType, _ValueType>>>
    class hash_multimap{
        using _hashtable = _hash::_hashtable<_KeyType, std::pair<const _KeyType, _ValueType>,
            _Hash, _Equal, _ExtractKey, _TruncateHash, _Rehash, _Traits, _Alloc>;
    public:
        using key_type           = typename _hashtable::key_type;
        using value_type         = typename _hashtable::value_type;
        using mapped_type        = typename _hashtable::mapped_type;
        using hasher             = typename _hashtable::hasher;
        using key_equal          = typename _hashtable::key_equal;
        using hash_code          = typename _hashtable::hash_code;

        using pointer            = typename _hashtable::pointer;
        using const_pointer      = typename _hashtable::const_pointer;
        using reference          = typename _hashtable::reference;
        using const_reference    = typename _hashtable::const_reference;
        using size_type          = typename _hashtable::size_type;
        using difference_type    = typename _hashtable::difference_type;

        using allocator_type     = typename _hashtable::allocator_type;
        
        using self_type        = hash_multimap;
        using iterator         = typename _hashtable::iterator;
        using const_iterator   = typename _hashtable::const_iterator;
        using local_iterator   = typename _hashtable::local_iterator;
        using const_local_iterator = typename _hashtable::const_local_iterator;

        hash_multimap(
            size_type _bkt_count = 47,
            float _max_load = 0.75,
            const hasher& _hash = hasher(),
            const key_equal& _equal = key_equal(),
            const allocator_type& _alloc = allocator_type()
        ): _M_hashtable(_bkt_count, _max_load, _hash, _equal, _alloc){}
        hash_multimap(const hash_multimap& _ht) _YXXX_NOEXCEPT: _M_hashtable(_ht._M_hashtable){}
        template <typename InputIter>
        hash_multimap(InputIter _first, InputIter _last): _M_hashtable(_first, _last){}
        hash_multimap(std::initializer_list<value_type>&& _ilist):
            _M_hashtable(std::move(_ilist)){}
        hash_multimap(hash_multimap&& _ht) _YXXX_NOEXCEPT:_M_hashtable(std::move(_ht._M_hashtable)){}

        std::pair<iterator, bool> insert(const value_type& _val) _YXXX_NOEXCEPT{
            return _M_hashtable.insert(_val);
        }
        std::pair<iterator, bool> insert(value_type&& _val) _YXXX_NOEXCEPT{
            return _M_hashtable.insert(std::move(_val));
        }
        template <typename InputIter>
        void insert(InputIter _first, InputIter _last){
            _M_hashtable.insert(_first, _last);
        }
        template <typename Key, typename ...Args>
        std::pair<iterator, bool> emplace(Key&& _key, Args&&... _args){
            return _M_hashtable.emplace(
                std::forward<Key>(_key), 
                std::forward<Args>(_args)...
            );
        }
        std::pair<local_iterator, local_iterator>
        equal_range(const key_type& _key) _YXXX_NOEXCEPT{
            return _M_hashtable.equal_range(_key);
        }
        std::pair<const_local_iterator, const_local_iterator>
        equal_range(const key_type& _key) const _YXXX_NOEXCEPT{
            return _M_hashtable.equal_range(_key);
        }
        void erase(const key_type& _key) _YXXX_NOEXCEPT{_M_hashtable.erase(_key);}
        iterator find(const key_type& _key) _YXXX_NOEXCEPT{return _M_hashtable.find(_key);}
        const_iterator find(const key_type& _key) const _YXXX_NOEXCEPT{
            return _M_hashtable.find(_key);
        }
        void rehash(size_type _bucket_count) _YXXX_NOEXCEPT{_M_hashtable.rehash(_bucket_count);}

        iterator begin() _YXXX_NOEXCEPT{return _M_hashtable.begin();}
        const_iterator begin() const _YXXX_NOEXCEPT{return _M_hashtable.begin();}
        iterator end() _YXXX_NOEXCEPT{return _M_hashtable.end();}
        const_iterator end() const _YXXX_NOEXCEPT{return _M_hashtable.end();}
        const_iterator cbegin() const _YXXX_NOEXCEPT{return _M_hashtable.cbegin();}
        const_iterator cend() const _YXXX_NOEXCEPT{return _M_hashtable.cend();}
        local_iterator begin(size_type _bkt_idx){return _M_hashtable.begin(_bkt_idx);}
        const_local_iterator begin(size_type _bkt_idx) const{
            return _M_hashtable.begin(_bkt_idx);
        }
        local_iterator end(size_type _bkt_idx){
            return _M_hashtable.end(_bkt_idx);
        }
        const_local_iterator end(size_type _bkt_idx) const{
            return _M_hashtable.end(_bkt_idx);
        }
        const_local_iterator cbegin(size_type _bkt_idx) const{
            return _M_hashtable.cbegin(_bkt_idx);
        }
        const_local_iterator cend(size_type _bkt_idx) const{
            return _M_hashtable.cend(_bkt_idx);
        }

        bool empty() const _YXXX_NOEXCEPT{return _M_hashtable.empty();}
        bool empty(difference_type _n) const{return _M_hashtable.empty(_n);}
        size_type size() const _YXXX_NOEXCEPT{return _M_hashtable.size();}
        size_type size(difference_type _bkt_index) const _YXXX_NOEXCEPT{
            return _M_hashtable.size(_bkt_index);
        }
        size_type count(const key_type& _key) const _YXXX_NOEXCEPT{
            return _M_hashtable.count(_key);
        }
        size_type bucket_count() const _YXXX_NOEXCEPT{return _M_hashtable.bucket_count();}
        float load_factor() const _YXXX_NOEXCEPT{return _M_hashtable.load_factor();}
        float max_load_factor() const _YXXX_NOEXCEPT{return _M_hashtable.max_load_factor();}
        void max_load_factor(float _mlf) _YXXX_NOEXCEPT{_M_hashtable.max_load_factor(_mlf);}
        void clear() _YXXX_NOEXCEPT{return _M_hashtable.clear();}
        friend void swap(self_type& _lh, self_type& _rh) _YXXX_NOEXCEPT{
            swap(_lh._M_hashtable, _rh._M_hashtable);
        }
    protected:
        _hashtable _M_hashtable;
    };
};

#endif