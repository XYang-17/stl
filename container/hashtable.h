#ifndef _YXXX_HASHTABLE_H_
#define _YXXX_HASHTABLE_H_

#include "_container_base.h"
#include "../function.h"

namespace _hash{
    template <typename _ValueType, bool _Cache>
    struct _hash_node;
    template <typename _ValueType, bool _Cache>
    struct _hash_node_base{
        _hash_node<_ValueType, _Cache>* next = nullptr;
    };
    template <bool _Cache>
    struct _hash_node_code_cache{};
    template <>
    struct _hash_node_code_cache<true>{
        std::size_t& hash_code() _YXXX_NOEXCEPT{return _M_hash_code;}
        const std::size_t& hash_code() const _YXXX_NOEXCEPT{return _M_hash_code;}
    protected:
        std::size_t _M_hash_code;
    };
    template <typename _ValueType, bool _Cache>
    struct _hash_node:
        public _hash_node_base<_ValueType, _Cache>,
        public _hash_node_code_cache<_Cache>{
    public:
        using base_type           = _hash_node_base<_ValueType, _Cache>;
        using value_type          = _ValueType;
        using pointer             = _ValueType*;
        using const_pointer       = const _ValueType*;
        using reference           = _ValueType&;
        using const_reference     = const _ValueType&;
    protected:
        using _node_pointer       = _hash_node*;
    public:
        _hash_node() : base_type(), _M_value(){}
        template <typename ...Args>
        _hash_node(Args ...args) :
            _M_value{std::forward<Args>(args)...}{}
        _hash_node(const _hash_node& _other) = default;
        _hash_node(_hash_node&&) = delete;
    
        reference value() _YXXX_NOEXCEPT{return _M_value;}
        const_reference value() const _YXXX_NOEXCEPT{return _M_value;}
    protected:
        _ValueType _M_value;
    };

    template <typename _KeyType, typename _ValueType,
        typename _Hash, typename _ExtractKey,
        typename _TruncateHash>
    class _hash_code_base{
    public:
        using hasher              = _Hash;
        using key_type            = _KeyType;
        using value_type          = _ValueType;
        using hash_code           = std::size_t;
        
        using size_type           = std::size_t;
        using difference_type     = std::ptrdiff_t;

        _hash_code_base() _YXXX_NOEXCEPT = default;
        _hash_code_base(const _Hash& hash) _YXXX_NOEXCEPT: _M_hash(hash){}
        _hash_code_base(const _hash_code_base&) _YXXX_NOEXCEPT = default;
        _hash_code_base(_hash_code_base&&) _YXXX_NOEXCEPT = default;

        hasher hash_function() const _YXXX_NOEXCEPT{return _M_hash;}

    protected:
        hash_code _M_hash_code(const _KeyType& key) const{
            return _M_hash(key);
        }
        hash_code _M_hash_code(const _hash_node<_ValueType, true>& _node) const{
            return _node.hash_code();
        }
        hash_code _M_hash_code(const _hash_node<_ValueType, false>& _node) const{
            return _M_hash_code(_ExtractKey{}(_node.value()));
        }

        difference_type _M_bucket_index(
            const hash_code& _code,
            size_type _bkt_count) const{
            return _TruncateHash{}(_code, _bkt_count);
        }
        template <bool _Cache>
        difference_type _M_bucket_index(
            const _hash_node<_ValueType, _Cache>& _node,
            size_type _bkt_count) const{
            return _M_bucket_index(_M_hash_code(_node), _bkt_count);
        }

        _Hash _M_hash;
    };

    template <typename _KeyType, typename _ValueType,
        typename _Hash, typename _Equal, typename _ExtractKey,
        typename _TruncateHash, typename _Rehash,
        typename _Traits, typename _Alloc>
    class _hashtable;

    template <typename _KeyType, typename _ValueType,
        typename _Hash, typename _Equal, typename _ExtractKey,
        typename _TruncateHash, typename _Traits>
    class _hashtable_base:
        public _hash_code_base<_KeyType, _ValueType,
            _Hash, _ExtractKey, _TruncateHash>{
    protected:
        using _code_base        = _hash_code_base<_KeyType, _ValueType,
            _Hash, _ExtractKey, _TruncateHash>;

    public:
        using hasher            = typename _code_base::hasher;
        using key_type          = typename _code_base::key_type;
        using value_type        = typename _code_base::value_type;
        using hash_code         = typename _code_base::hash_code;
        using key_equal         = _Equal;
    protected:
        using _traits          = _Traits;
        using _cache_hash_code = typename _traits::_cache_hash_code;
        using _node_type       = _hash_node<value_type, _cache_hash_code::value>;
    public:
        using pointer           = typename _node_type::pointer;
        using const_pointer     = typename _node_type::const_pointer;
        using reference         = typename _node_type::reference;
        using const_reference   = typename _node_type::const_reference;
        using size_type         = typename _code_base::size_type;
        using difference_type   = typename _code_base::difference_type;

    public:
        _hashtable_base() _YXXX_NOEXCEPT = default;
        _hashtable_base(const _Hash& _hash, const key_equal& _equal) _YXXX_NOEXCEPT:
            _code_base(_hash), _M_eq(_equal){}
        _hashtable_base(const _hashtable_base& _other) _YXXX_NOEXCEPT = default;
        _hashtable_base(_hashtable_base&& _other) _YXXX_NOEXCEPT = default;

        const key_equal& key_eq() const _YXXX_NOEXCEPT{return _M_eq;}
    protected:
        bool _M_key_equal(const key_type& x, const _node_type& _node) const{
            return _M_eq(x, _ExtractKey{}(_node.value()));
        }
        bool _M_node_equal(const _node_type& _l_node, const _node_type& _r_node) const{
            if _YXXX_CONSTEXPR(_cache_hash_code::value){
                if(_l_node._M_hash_code != _r_node._M_hash_code)
                    return false;
            }
            return _M_key_equal(_ExtractKey{}(_l_node.value()), _r_node);
        }
        bool _M_equal(const key_type& _key, const hash_code& _code,
            const _node_type& _node) const{
            if _YXXX_CONSTEXPR(_cache_hash_code::value){
                if(_code != _node.hash_code())
                    return false;
            }
            return _M_key_equal(_key, _node);
        }
        key_equal _M_eq;
    };

    template <typename _KeyType, typename _ValueType,
        typename _Hash, typename _Equal, typename _ExtractKey,
        typename _TruncateHash, typename _Rehash,
        typename _Traits, typename _Alloc, bool _UniqueKey>
    class _hash_map_base{};
    template <typename _KeyType, typename _ValueType,
        typename _Hash, typename _Equal, typename _ExtractKey,
        typename _TruncateHash, typename _Rehash,
        typename _Traits, typename _Alloc>
    class _hash_map_base<_KeyType, std::pair<const _KeyType, _ValueType>,
        _Hash, _Equal, _ExtractKey, _TruncateHash,
        _Rehash, _Traits, _Alloc, false>{
    public:
        using mapped_type = _ValueType;
    };
    template <typename _KeyType, typename _ValueType,
        typename _Hash, typename _Equal, typename _ExtractKey,
        typename _TruncateHash, typename _Rehash,
        typename _Traits, typename _Alloc>
    class _hash_map_base<_KeyType, std::pair<const _KeyType, _ValueType>,
        _Hash, _Equal, _ExtractKey, _TruncateHash,
        _Rehash, _Traits, _Alloc, true>{
    protected:
        using _hashtable   = _hashtable<
            _KeyType, std::pair<const _KeyType, _ValueType>,
            _Hash, _Equal, _ExtractKey, _TruncateHash,
            _Rehash, _Traits, _Alloc>;

        using _hashtable_base = _hashtable_base<_KeyType,
            _ValueType, _Hash, _Equal, _ExtractKey,
            _TruncateHash, _Traits>;
    public:
        using key_type     = typename _hashtable_base::key_type;
        using hash_code    = typename _hashtable_base::hash_code;
        using mapped_type  = _ValueType;

        mapped_type& operator[](const key_type& _key);
        mapped_type& operator[](key_type&& _key);
    };

    struct _prime_rehash{
        _prime_rehash(float _f = 0) _YXXX_NOEXCEPT: _M_max_load_factor(_f){}
    
        float max_load_factor() const _YXXX_NOEXCEPT{return _M_max_load_factor;}
        void max_load_factor(float _f) _YXXX_NOEXCEPT{_M_max_load_factor = _f;}
        std::size_t first_bucket_count(std::size_t _n) const _YXXX_NOEXCEPT{
            return stl::min_prime(_n);
        }
        std::size_t bucket_for_n(std::size_t _n) const _YXXX_NOEXCEPT{
            if(_n <= 2) return _n + 1;
            return first_bucket_count(_n / double(_M_max_load_factor));
        }
        std::size_t need_rehash(
            std::size_t _n, std::size_t _bkt_n) const _YXXX_NOEXCEPT{
            return (_n / _bkt_n < _M_max_load_factor) ?
                0 : first_bucket_count(_bkt_n + 1);
        }

    protected:
        float _M_max_load_factor;
    };

    template <typename _NodeAlloc>
    class _hashtable_alloc{
    public:
        using node_alloc_type         = _NodeAlloc;
        using node_type               = typename _alloc::
            _alloc_traits<node_alloc_type>::value_type;
        using node_pointer            = node_type*;

        using node_base_type          = typename node_type::base_type;
        using node_base_pointer       = node_base_type*;
        using bucket_pointer          = node_base_pointer;
        using bucket_alloc_type
            = _alloc::alloc_rebind<node_alloc_type, node_base_type>;
        
        _hashtable_alloc() _YXXX_NOEXCEPT = default;
        _hashtable_alloc(const _hashtable_alloc& alloc) _YXXX_NOEXCEPT = default;
        _hashtable_alloc(_hashtable_alloc&& alloc) _YXXX_NOEXCEPT = default;
        _hashtable_alloc(_NodeAlloc&& alloc) _YXXX_NOEXCEPT: _M_node_alloc(std::move(alloc)){}

        const node_alloc_type& _M_node_allocator() const _YXXX_NOEXCEPT{
            return _M_node_alloc;
        }
        node_alloc_type& _M_node_allocator() _YXXX_NOEXCEPT{ 
            return _M_node_alloc;
        }
        void _M_deallocate_node(node_pointer _node) _YXXX_NOEXCEPT{
            _M_node_alloc.destroy(_node);
            _M_node_alloc.deallocate(_node);
        }
        void _M_deallocate_nodes(node_pointer _node) _YXXX_NOEXCEPT{
            node_pointer _tmp = nullptr;
            while(nullptr != _node){
                _tmp = _node;
                _node = node_pointer(_node->next);
                _M_deallocate_node(_tmp);
            }
        }
        node_pointer _M_allocate_node(){
            node_pointer _node = _M_node_alloc.allocate(1);
            try{
                _M_node_alloc.construct(_node);
                return _node;
            }
            catch(...){
                _M_deallocate_node(_node);
                throw;
            }
        }
        template <typename ..._Args>
        node_pointer _M_allocate_node(_Args&& ..._args){
            node_pointer _node = _M_node_alloc.allocate(1);
            try{
                _M_node_alloc.construct(
                    _node,
                    std::forward<_Args>(_args)...
                );
                return _node;
            }
            catch(...){
                _M_deallocate_node(_node);
                throw;
            }
        }
        node_pointer _M_nodes_copy(node_pointer _node){
            if(nullptr == _node) return nullptr;
            node_pointer _res = _M_allocate_node(_node);
            try{
                node_pointer _cur = _res;
                while(nullptr != _node->next){
                    _node = _node->next;
                    _cur->next = _M_allocate_node(_node);
                    _cur = _cur->next;
                }
            }
            catch(...){
                _M_deallocate_nodes(_res);
                throw;
            }
            return _res;
        }

        void _M_reset_buckets(bucket_pointer _bkt, std::size_t _n)_YXXX_NOEXCEPT{
            for(std::size_t _i = 0; _i < _n; ++_i){
                _M_deallocate_nodes(node_pointer(_bkt[_i].next));
                _bkt[_i].next = nullptr;
            }
        }
        bucket_pointer _M_allocate_buckets(std::size_t _bkt_count){
            bucket_alloc_type _alloc(_M_node_allocator());
            bucket_pointer _bkt = _alloc.allocate(_bkt_count);
            for(bucket_pointer _cur = _bkt; _cur < _bkt + _bkt_count; ++_cur)
                _alloc.construct(_cur);
            return _bkt;
        }
        void _M_deallocate_buckets(bucket_pointer _bkt) _YXXX_NOEXCEPT{
            bucket_alloc_type _alloc(_M_node_allocator());
            _alloc.deallocate(_bkt);
        }

    protected:
        _NodeAlloc _M_node_alloc;
    };

    template <bool _Cache_hash_code, bool _Unique_key, bool _Const_elements>
    struct _hashtable_traits{
        using _cache_hash_code = std::__bool_constant<_Cache_hash_code>;
        using _unique_key = std::__bool_constant<_Unique_key>;
        using _const_elements = std::__bool_constant<_Const_elements>;
    };
    
    /* 
        head->node0->node1->node2->node3->node4->node5->node6->node7->nullptr
            ⬆     ⬆             ⬆      ⬆             ⬆     ⬆       
        bkt0 begin(bkt0)     bkt1 begin(bkt1)    bkt2 begin(bkt1)
        */
    template <typename _KeyType, typename _ValueType,
        typename _Hash, typename _Equal, typename _ExtractKey,
        typename _TruncateHash, typename _Rehash,
        typename _Traits, typename _Alloc>
    class _hashtable:
        public _hashtable_base<_KeyType, _ValueType, _Hash, _Equal,
            _ExtractKey, _TruncateHash, _Traits>,
        public _hash_map_base<_KeyType, _ValueType, _Hash, _Equal,
            _ExtractKey, _TruncateHash, _Rehash, _Traits, _Alloc,
            _Traits::_unique_key::value>,
        private _hashtable_alloc<
            _alloc::alloc_rebind<
                _Alloc,
                typename _hashtable_base<_KeyType, _ValueType, _Hash,
                    _Equal, _ExtractKey, _TruncateHash, _Traits>::_node_type
            >>{
        using _hashtable_base = _hashtable_base<_KeyType, _ValueType, _Hash,
            _Equal, _ExtractKey, _TruncateHash, _Traits>;
        using _map_base = _hash_map_base<_KeyType, _ValueType, _Hash, _Equal,
            _ExtractKey, _TruncateHash, _Rehash, _Traits, _Alloc,
            _Traits::_unique_key::value>;

        using _node_type         = typename _hashtable_base::_node_type;
        using _node_alloc_type   = _alloc::alloc_rebind<_Alloc, _node_type>;
        using _hashtable_alloc   = _hashtable_alloc<_node_alloc_type>;
        using _node_pointer      = typename _hashtable_alloc::node_pointer;
        using _node_base_pointer = typename _hashtable_alloc::node_base_pointer;
        using _bucket_pointer    = typename _hashtable_alloc::bucket_pointer;

        using _traits            = _Traits;
        using _cache_hash_code   = typename _traits::_cache_hash_code;
        using _unique_key        = typename _traits::_unique_key;
        using _const_elements    = typename _traits::_const_elements;

        template <typename __KeyType, typename __ValueType, typename __Hash,
            typename __Equal, typename __ExtractKey, typename __TruncateHash,
            typename __Rehash, typename __Traits, typename __Alloc,
            bool __UniqueKey>
        friend class _hash_map_base;
    public:
        using key_type           = typename _hashtable_base::key_type;
        using value_type         = typename _hashtable_base::value_type;
        using hasher             = typename _hashtable_base::hasher;
        using key_equal          = typename _hashtable_base::key_equal;
        using hash_code          = typename _hashtable_base::hash_code;

        using pointer            = typename _hashtable_base::pointer;
        using const_pointer      = typename _hashtable_base::const_pointer;
        using reference          = typename _hashtable_base::reference;
        using const_reference    = typename _hashtable_base::const_reference;
        using size_type          = typename _hashtable_base::size_type;
        using difference_type    = typename _hashtable_base::difference_type;

        using allocator_type     = _Alloc;
        using self_type         = _hashtable;
    
    protected:
        template <bool _Const, bool _Local>
        class _hashtable_iterator{
        public:
            using pointer             = std::conditional_t<_Const, const value_type*, value_type*>;
            using reference           = std::conditional_t<_Const, const value_type&, value_type&>;
            using iterator_category   = stl::forward_iterator_tag;
            
            using self_type          = _hashtable_iterator;

            friend _hashtable;

            _hashtable_iterator() _YXXX_NOEXCEPT: _M_ptr(nullptr), _M_container(nullptr){}
            _hashtable_iterator(_node_pointer _ptr, const _hashtable* _container,
                difference_type _bucket_index) _YXXX_NOEXCEPT:
                _M_ptr(_ptr), _M_container(_container), _M_bucket_index(_bucket_index){}
            _hashtable_iterator(const _hashtable_iterator& _lit) _YXXX_NOEXCEPT:
                _M_ptr(_lit._M_ptr), _M_container(_lit._M_container),
                _M_bucket_index(_lit._M_bucket_index){}
            _hashtable_iterator(_hashtable_iterator&&) = delete;

            operator _hashtable_iterator<true, _Local>() const _YXXX_NOEXCEPT{
                return _hashtable_iterator<true, _Local>(
                    _M_ptr, _M_container, _M_bucket_index);
            }
            
            self_type& operator=(const _hashtable_iterator& _other) _YXXX_NOEXCEPT{
                if(*this == _other) return *this;
                _M_ptr = _other._M_ptr;
                _M_container = _other._M_container;
                _M_bucket_index = _other._M_bucket_index;
                return *this;
            }
            reference operator*() const _YXXX_NOEXCEPT{return _M_ptr->value();}
            pointer operator->() const _YXXX_NOEXCEPT{return std::addressof(operator*());}
            self_type& operator++() _YXXX_NOEXCEPT;
            self_type operator++(int) _YXXX_NOEXCEPT{
                self_type _tmp(*this);
                ++*this;
                return _tmp;
            }
            friend bool operator==(const _hashtable_iterator& _lhs,
                const _hashtable_iterator& _rhs) _YXXX_NOEXCEPT{
                return _lhs._M_ptr == _rhs._M_ptr
                    && _lhs._M_bucket_index == _rhs._M_bucket_index
                    && _lhs._M_container == _rhs._M_container;
            }
            friend bool operator!=(const _hashtable_iterator& _lhs,
                const _hashtable_iterator& _rhs) _YXXX_NOEXCEPT{
                return !(_lhs == _rhs);
            }
        protected:
            _node_pointer _M_ptr;
            const _hashtable* _M_container;
            difference_type _M_bucket_index;
        };
    public:
        using iterator          = _hashtable_iterator<_const_elements::value, false>;
        using const_iterator    = _hashtable_iterator<true, false>;
        using local_iterator    = _hashtable_iterator<_const_elements::value, true>;
        using const_local_iterator = _hashtable_iterator<true, true>;

        _hashtable(size_type _bkt_count = 47,
            float _max_load = 0.75,
            const hasher& _hash = hasher(),
            const key_equal& _equal = key_equal(),
            const allocator_type& _alloc = allocator_type()):
            _hashtable_base(_hash, _equal),
            _hashtable_alloc(_alloc){
            this->_M_init(_bkt_count, _max_load);
        }
        _hashtable(const _hashtable& _ht) _YXXX_NOEXCEPT:
            _hashtable_base(_ht),
            _hashtable_alloc(_ht){
            this->_M_copy_or_assign(_ht);
        }
        _hashtable(_hashtable&& _ht) _YXXX_NOEXCEPT:
            _hashtable_base(std::move(_ht)),
            _hashtable_alloc(std::move(_ht)){
            this->_M_move(_ht);
        }
        template <class _InputIterator>
        _hashtable(_InputIterator _first, _InputIterator _last):
            _hashtable(){insert(_first, _last);}
        _hashtable(std::initializer_list<value_type>&& _il):
            _hashtable(_il.size()){insert(_il.begin(), _il.end());}
        ~_hashtable() _YXXX_NOEXCEPT{
            clear();
            this->_M_deallocate_buckets(_M_buckets);
            _M_buckets = nullptr;
        }

        std::pair<iterator, bool> insert(const value_type& _val) _YXXX_NOEXCEPT{
            if _YXXX_CONSTEXPR(_Traits::_unique_key::value)
                return _M_emplace_unique(_val);
            else
                return _M_emplace_multi(_val);
        }
        std::pair<iterator, bool> insert(value_type&& _val) _YXXX_NOEXCEPT{
            if _YXXX_CONSTEXPR(_Traits::_unique_key::value)
                return _M_emplace_unique(std::move(_val));
            else
                return _M_emplace_multi(std::move(_val));
        }
        template <class _InputIterator>
        void insert(_InputIterator _first, _InputIterator _last){
            for(; _first != _last; ++_first){
                insert(*_first);
            }
        }

        template <typename ...Args>
        std::pair<iterator, bool> emplace(Args&&... _args){
            if(_Traits::_unique_key::value)
                return _M_emplace_unique(std::forward<Args>(_args)...);
            else
                return _M_emplace_multi(std::forward<Args>(_args)...);
        }

        std::pair<local_iterator, local_iterator>
        equal_range(const key_type& _key) _YXXX_NOEXCEPT{
            hash_code _code = this->_M_hash_code(_key);
            difference_type _bkt_idx = this->_M_bucket_index(_code, _M_bucket_count);

            _node_pointer _begin_ptr = this->_M_find(_key, _code, _bkt_idx);
            if(nullptr == _begin_ptr) return {end(_bkt_idx), end(_bkt_idx)};
            _node_pointer _end_ptr = _begin_ptr->next;
            while(nullptr != _end_ptr && this->_M_key_equal(_key, *_end_ptr))
                _end_ptr = _end_ptr->next;
            return {local_iterator(_begin_ptr, this, _bkt_idx),
                local_iterator(_end_ptr, this, _bkt_idx)};
        }
        std::pair<const_local_iterator, const_local_iterator>
        equal_range(const key_type& _key) const _YXXX_NOEXCEPT{
            hash_code _code = this->_M_hash_code(_key);
            difference_type _bkt_idx = this->_M_bucket_index(_code, _M_bucket_count);

            _node_pointer _begin_ptr = this->_M_find(_key, _code, _bkt_idx);
            if(nullptr == _begin_ptr) return {end(_bkt_idx), end(_bkt_idx)};
            _node_pointer _end_ptr = _begin_ptr->next;
            while(nullptr != _end_ptr && this->_M_key_equal(_key, *_end_ptr))
                _end_ptr = _end_ptr->next;
            return {const_local_iterator(_begin_ptr, this, _bkt_idx),
                const_local_iterator(_end_ptr, this, _bkt_idx)};
        }

        void erase(const key_type& _key) _YXXX_NOEXCEPT{
            hash_code _code = this->_M_hash_code(_key);
            difference_type _bkt_index = this->_M_bucket_index(_code, _M_bucket_count);
            this->_M_erase(_key, _code, _bkt_index);
        }
        // std::pair<iterator, iterator>
        // extract(const key_type& _key) _YXXX_NOEXCEPT{
        //     hash_code _code = this->_M_hash_code(_key);
        //     difference_type _bkt_idx = this->_M_bucket_index(_code, _M_bucket_count);

        //     _node_pointer _begin_ptr = this->_M_find(_key, _code, _bkt_idx);
        //     if(nullptr == _begin_ptr) return {end(_bkt_idx), end(_bkt_idx)};
        //     _node_pointer _last_ptr = _begin_ptr;
        //     while(nullptr != _last_ptr->next &&
        //         this->_M_key_equal(_key, *(_last_ptr->next)))
        //         _last_ptr = _last_ptr->next;
            
        // }

        iterator find(const key_type& _key) _YXXX_NOEXCEPT{
            hash_code _code = this->_M_hash_code(_key);
            difference_type _bkt_index = this->_M_bucket_index(_code, _M_bucket_count);
            _node_pointer _node_ptr = this->_M_find(_key, _code, _bkt_index);
            if(nullptr == _node_ptr) return end();
            else return {_node_ptr, this, _bkt_index};
        }
        const_iterator find(const key_type& _key) const _YXXX_NOEXCEPT{
            hash_code _code = this->_M_hash_code(_key);
            difference_type _bkt_index = this->_M_bucket_index(_code, _M_bucket_count);
            _node_pointer _node_ptr = this->_M_find(_key, _code, _bkt_index);
            if(nullptr == _node_ptr) return end();
            else return {_node_ptr, this, _bkt_index};
        }
        void rehash(size_type _bucket_count) _YXXX_NOEXCEPT{
            _bucket_count = _M_rehash.first_bucket_count(_bucket_count);
            if(_bucket_count == _M_bucket_count) return;
            _bucket_pointer _buckets = _M_buckets;
            _M_buckets = this->_M_allocate_buckets(_bucket_count);

            if(nullptr != _buckets){
                hash_code _code;
                difference_type _bkt_index;
                _node_pointer _tmp = nullptr;
                for(difference_type _idx = 0; _idx < _M_bucket_count; ++_idx){
                    _node_pointer _cur = _buckets[_idx].next;
                    while(nullptr != _cur){
                        _code = this->_M_hash_code(*_cur);
                        _bkt_index = this->_M_bucket_index(_code, _bucket_count);

                        _tmp = _cur->next;
                        _cur->next = _M_buckets[_bkt_index].next;
                        _M_buckets[_bkt_index].next = _cur;
                        _cur = _tmp;
                    }
                }
            }
            _M_bucket_count = _bucket_count;
        }

        iterator begin() _YXXX_NOEXCEPT{return _M_begin();}
        const_iterator begin() const _YXXX_NOEXCEPT{return _M_begin();}
        iterator end() _YXXX_NOEXCEPT{
            return {nullptr, this, difference_type(_M_bucket_count-1)};
        }
        const_iterator end() const _YXXX_NOEXCEPT{
            return {nullptr, this, difference_type(_M_bucket_count-1)};
        }
        const_iterator cbegin() const _YXXX_NOEXCEPT{return _M_begin();}
        const_iterator cend() const _YXXX_NOEXCEPT{
            return {nullptr, this, difference_type(_M_bucket_count-1)};
        }
        local_iterator begin(difference_type _bkt_idx){
            this->_M_check_valid_bucket(_bkt_idx);
            return {_M_buckets[_bkt_idx].next, this, _bkt_idx};
        }
        const_local_iterator begin(difference_type _bkt_idx) const{
            this->_M_check_valid_bucket(_bkt_idx);
            return {_M_buckets[_bkt_idx].next, this, _bkt_idx};
        }
        local_iterator end(difference_type _bkt_idx){
            this->_M_check_valid_bucket(_bkt_idx);
            return {nullptr, this, _bkt_idx};
        }
        const_local_iterator end(difference_type _bkt_idx) const{
            this->_M_check_valid_bucket(_bkt_idx);
            return {nullptr, this, _bkt_idx};
        }
        const_local_iterator cbegin(difference_type _bkt_idx) const{
            this->_M_check_valid_bucket(_bkt_idx);
            return {_M_buckets[_bkt_idx].next, this, _bkt_idx};
        }
        const_local_iterator cend(difference_type _bkt_idx) const{
            this->_M_check_valid_bucket(_bkt_idx);
            return {nullptr, this, _bkt_idx};
        }

        bool empty() const _YXXX_NOEXCEPT{return 0 == _M_element_count;}
        bool empty(difference_type _bkt_index) const{
            this->_M_check_valid_bucket(_bkt_index);
            return nullptr == _M_buckets[_bkt_index].next;
        }
        size_type size() const _YXXX_NOEXCEPT{return _M_element_count;}
        size_type size(difference_type _bkt_index) const _YXXX_NOEXCEPT{
            if(empty(_bkt_index)) return 0;
            size_type _count = 0;
            const_local_iterator _end = end(_bkt_index);
            for(const_local_iterator _it = begin(_bkt_index); _it != _end; ++_it)
                ++_count;
            return _count;
        }
        size_type count(const key_type& _key) const _YXXX_NOEXCEPT{
            difference_type _bkt_idx = this->_M_bucket_index(_key, _M_bucket_count);
            size_type _res = 0;
            const_local_iterator _it_end = end(_bkt_idx);
            for(const_local_iterator _it = begin(_bkt_idx); _it != _it_end; ++_it){
                if(this->_M_key_equal(_key, *(_it._M_ptr))) ++_res;
            }
            return _res;
        }
        difference_type bucket_index(iterator _it) const{
            return this->_M_bucket_index(*(_it._M_ptr), _M_bucket_count); 
        }
        size_type bucket_count() const _YXXX_NOEXCEPT{return _M_bucket_count;}
        float load_factor() const _YXXX_NOEXCEPT{
            return float(_M_element_count / _M_bucket_count);
        }
        float max_load_factor() const _YXXX_NOEXCEPT{
            return _M_rehash.max_load_factor();
        }
        void max_load_factor(float _mlf) _YXXX_NOEXCEPT{_M_rehash.max_load_factor(_mlf);}
        void clear() _YXXX_NOEXCEPT{
            if(!empty()){
                this->_M_reset_buckets(_M_buckets, _M_bucket_count);
                _M_element_count = 0;
            }
        }

        friend void swap(self_type& _lh, self_type& _rh) _YXXX_NOEXCEPT{
            std::swap(_lh._M_buckets, _rh._M_buckets);
            std::swap(_lh._M_bucket_count, _rh._M_bucket_count);
            std::swap(_lh._M_element_count, _rh._M_element_count);
            std::swap(_lh._M_rehash, _rh._M_rehash);
        }
    private:
        void _M_init(){
            _M_element_count = 0;
            _M_buckets = this->_M_allocate_buckets(_M_bucket_count);
        }
        void _M_init(size_type _bkt_count, float _f){
            _M_rehash.max_load_factor(_f);
            _M_bucket_count = _M_rehash.bucket_for_n(_bkt_count);
            _M_init();
        }
        void _M_before_move() _YXXX_NOEXCEPT{
            clear();
            this->_M_deallocate_buckets();
            _M_buckets = nullptr;
        }
        void _M_after_move() _YXXX_NOEXCEPT{
            _M_element_count = 0;
            _M_buckets = nullptr;
        }
        void _M_move(_hashtable&& _ht) _YXXX_NOEXCEPT{
            _M_before_move();
            _M_bucket_count = _ht._M_bucket_count;
            _M_rehash = _ht._M_rehash;
            _M_element_count = _ht._M_element_count;
            _M_buckets = _ht._M_buckets;
            _ht._M_after_move();
        }
        void _M_copy_or_assign(const _hashtable& _ht){
            _M_rehash.max_load_factor(_ht._f);
            if(_M_bucket_count != _ht._M_bucket_count){
                this->_M_deallocate_buckets(_M_buckets);
                _M_bucket_count = _ht._M_bucket_count;
                _M_buckets = this->_M_allocate_buckets(_M_bucket_count);
            }
            else if(nullptr = _M_buckets)
                _M_buckets = this->_M_allocate_buckets(_M_bucket_count);

            for(difference_type _bkt_idx = 0; _bkt_idx < _M_bucket_count; ++_bkt_idx){
                _M_buckets[_bkt_idx].next = this->_M_nodes_copy(
                        _ht._M_buckets[_bkt_idx].next);
            }
            _M_element_count = _ht._M_element_count;
        }
        iterator _M_begin() _YXXX_NOEXCEPT{
            if(empty()) return end();
            difference_type _bkt_idx = 0, _bkt_count = difference_type(_M_bucket_count);
            while(_bkt_idx < _bkt_count
                && nullptr == _M_buckets[_bkt_idx].next)
                ++_bkt_idx;
            return  {_M_buckets[_bkt_idx].next, this, _bkt_idx};
        }
        const_iterator _M_begin() const _YXXX_NOEXCEPT{
            if(empty()) return end();
            difference_type _bkt_idx = 0, _bkt_count = difference_type(_M_bucket_count);
            while(_bkt_idx < _bkt_count
                && nullptr == _M_buckets[_bkt_idx].next)
                ++_bkt_idx;
            return  {_M_buckets[_bkt_idx].next, this, _bkt_idx};
        }
        _node_pointer _M_find(const key_type& _key, hash_code _code,
            difference_type _bkt_index) const _YXXX_NOEXCEPT{
            if(empty(_bkt_index)) return nullptr;
            auto _bkt = _M_buckets[_bkt_index];
            _node_pointer _node_ptr = _bkt.next;
            while(nullptr != _node_ptr){
                if(this->_M_equal(_key, _code, *_node_ptr)) break;
                _node_ptr = _node_ptr->next;
            }
            return _node_ptr;
        }
        _node_base_pointer _M_previous(_node_pointer _node_ptr,
            difference_type _bkt_index) const _YXXX_NOEXCEPT{
            if(_M_buckets[_bkt_index].next == _node_ptr)
                return &_M_buckets[_bkt_index];
            _node_pointer _cur_ptr = _M_buckets[_bkt_index].next;
            while(_cur_ptr->next != _node_ptr)
                _cur_ptr = _cur_ptr->next;
            return _cur_ptr;
        }
        
        void _M_before_insert(_node_pointer _node_ptr,
            hash_code _code) _YXXX_NOEXCEPT{
            if(nullptr == _M_buckets) _M_init();
            if _YXXX_CONSTEXPR(_cache_hash_code::value)
                _node_ptr->hash_code() = _code;
        }
        void _M_after_insert(){
            ++_M_element_count;
            this->_M_maybe_rehash();
        }
        iterator _M_insert_after(_node_pointer _node_ptr,
            _node_base_pointer _prev_ptr, hash_code _code,
            difference_type _bkt_idx){
            this->_M_before_insert(_node_ptr, _code);

            _node_ptr->next = _prev_ptr->next;
            _prev_ptr->next = _node_ptr;

            this->_M_after_insert();
            return {_node_ptr, this, _bkt_idx};
        }
        
        
        template <typename ...Args>
        std::pair<iterator, bool> _M_emplace_unique(Args&& ...args){
            value_type _value{std::forward<Args>(args)...};
            key_type _key = _ExtractKey{}(_value);
            hash_code _code = this->_M_hash(_key);
            difference_type _bkt_idx = this->_M_bucket_index(_key, _M_bucket_count);
            if(_node_pointer _node_ptr = _M_find(_key, _code, _bkt_idx)){
                return {{_node_ptr, this, _bkt_idx}, false};
            }
            
            iterator _iter = this->_M_insert_after(
                this->_M_allocate_node(std::move(_value)),
                &_M_buckets[_bkt_idx], _code, _bkt_idx
            );
            return {_iter, true};
        }

        template <typename ...Args>
        std::pair<iterator, bool> _M_emplace_multi(Args&& ...args){
            value_type _value{std::forward<Args>(args)...};
            key_type _key = _ExtractKey{}(_value);
            hash_code _code = this->_M_hash(_key);
            difference_type _bkt_idx = this->_M_bucket_index(_key, _M_bucket_count);

            _node_base_pointer _prev_ptr = nullptr;
            if(_node_pointer _node_ptr = _M_find(_key, _code, _bkt_idx)){
                _prev_ptr = _M_previous(_node_ptr, _bkt_idx);
            }
            if(nullptr == _prev_ptr){
                iterator _iter = this->_M_insert_after(
                    this->_M_allocate_node(std::move(_value)),
                    &_M_buckets[_bkt_idx], _code, _bkt_idx
                );
                return {_iter, true};
            }
            else{
                iterator _iter = this->_M_insert_after(
                    this->_M_allocate_node(std::move(_value)),
                    _prev_ptr, _code, _bkt_idx
                );
                return {_iter, true};
            }
        }
        
        void _M_erase(const key_type& _key, hash_code _code,
            difference_type _bkt_index) _YXXX_NOEXCEPT{
            _node_pointer _node_ptr = this->_M_find(_key, _code, _bkt_index);
            if(nullptr == _node_ptr) return;
            _node_base_pointer _prev_ptr = this->_M_previous(_node_ptr, _bkt_index);

            while(this->_M_equal(_key, _code, *_node_ptr)){
                _prev_ptr->next = _node_ptr->next;
                --_M_element_count;
                this->_M_deallocate_node(_node_ptr);
            }
        }
        void _M_check_valid_bucket(difference_type& _bkt_idx) const{
            if(_bkt_idx >= difference_type(_M_bucket_count))
                throw std::out_of_range("A incorrect bucket index.");
        }
        void _M_maybe_rehash() _YXXX_NOEXCEPT{
            if(_M_rehash.need_rehash(_M_element_count, _M_bucket_count))
                rehash(_M_bucket_count*2);
        }
    protected:
        _bucket_pointer _M_buckets;
        size_type _M_bucket_count;
        size_type _M_element_count;
        _Rehash _M_rehash;
    };

    template <typename _KeyType, typename _ValueType,
        typename _Hash, typename _Equal, typename _ExtractKey,
        typename _TruncateHash, typename _Rehash,
        typename _Traits, typename _Alloc>
    typename _hash_map_base<_KeyType, std::pair<const _KeyType, _ValueType>,
        _Hash, _Equal, _ExtractKey, _TruncateHash, _Rehash, _Traits,
        _Alloc, true>::
    mapped_type&
    _hash_map_base<_KeyType, std::pair<const _KeyType, _ValueType>,
        _Hash, _Equal, _ExtractKey, _TruncateHash, _Rehash, _Traits,
        _Alloc, true>::
    operator[](const key_type& _key){
        _hashtable* _this = static_cast<_hashtable*>(this);
        hash_code _code = _this->_M_hash_code(_key);
        std::size_t _bkt_index = _this->_M_bucket_index(_code, _this->_M_bucket_count);
        if(auto _node_ptr = _this->_M_find(_key, _code, _bkt_index))
            return _node_ptr->value().second;
        
        auto _iter = _this->insert({_key, mapped_type()});
        return _iter.first->second;
    }
    template <typename _KeyType, typename _ValueType,
        typename _Hash, typename _Equal, typename _ExtractKey,
        typename _TruncateHash, typename _Rehash,
        typename _Traits, typename _Alloc>
    typename _hash_map_base<_KeyType, std::pair<const _KeyType, _ValueType>,
        _Hash, _Equal, _ExtractKey, _TruncateHash, _Rehash, _Traits,
        _Alloc, true>::
    mapped_type&
    _hash_map_base<_KeyType, std::pair<const _KeyType, _ValueType>,
        _Hash, _Equal, _ExtractKey, _TruncateHash, _Rehash, _Traits,
        _Alloc, true>::
    operator[](key_type&& _key){
        _hashtable* _this = static_cast<_hashtable*>(this);
        hash_code _code = _this->_M_hash_code(_key);
        std::size_t _bkt_index = _this->_M_bucket_index(_code, _this->_M_bucket_count);
        if(auto _node_ptr = _this->_M_find(_key, _code, _bkt_index))
            return _node_ptr->value().second;
            
        auto _iter = _this->insert({std::move(_key), mapped_type()});
        return _iter.first->second;
    }

    template <typename _KeyType, typename _ValueType,
        typename _Hash, typename _Equal, typename _ExtractKey,
        typename _TruncateHash, typename _Rehash,
        typename _Traits, typename _Alloc>
    template <bool _Const, bool _Local>
    _hashtable<_KeyType, _ValueType, _Hash, _Equal, _ExtractKey,
        _TruncateHash, _Rehash, _Traits, _Alloc>::
    _hashtable_iterator<_Const, _Local>&
    _hashtable<_KeyType, _ValueType, _Hash, _Equal, _ExtractKey,
        _TruncateHash, _Rehash, _Traits, _Alloc>::
        _hashtable_iterator<_Const, _Local>::
    operator++() _YXXX_NOEXCEPT{
        if(nullptr != _M_ptr) _M_ptr = _M_ptr->next;
        if(nullptr != _M_ptr) return *this;
        if _YXXX_CONSTEXPR(_Local) return *this;

        const difference_type _max_bkt_idx = _M_container->bucket_count()-1;
        ++_M_bucket_index;
        while(_M_bucket_index < _max_bkt_idx
            && _M_container->empty(_M_bucket_index))
            ++_M_bucket_index;
        _M_ptr = _M_container->_M_buckets[_M_bucket_index].next;
        return *this;
    }
};

#endif