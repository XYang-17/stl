#ifndef _YXXX_DEQUE_H_
#define _YXXX_DEQUE_H_

#include "_container_base.h"

namespace stl{
    namespace _deque{
        #ifndef _DEQUE_BLOCK_SIZE
        #define _DEQUE_BLOCK_SIZE 64
        #endif

        _YXXX_CONSTEXPR inline std::size_t _deque_block_size(std::size_t _size) _YXXX_NOEXCEPT{
            return (_size < _DEQUE_BLOCK_SIZE ?
                std::size_t(_DEQUE_BLOCK_SIZE / _size) :
                std::size_t(1));
        }

        template <typename _Type, typename _Reference, typename _Pointer>
        class _deque_iterator{
        public:
            typedef _Type*                       _element_pointer;
            typedef _Type**                      _map_pointer;
            typedef _deque_iterator<_Type, const _Type&,  const _Type*>  const_iterator;

            typedef random_access_iterator_tag  iterator_category;
            typedef _Type              value_type;
            typedef _Pointer           pointer;
            typedef _Reference         reference;
            typedef std::ptrdiff_t     difference_type;

            typedef _deque_iterator   self_type;

        private:
            _YXXX_CONSTEXPR inline std::size_t _M_block_size() _YXXX_NOEXCEPT{
                return _deque_block_size(sizeof(_Type));
            }
            inline void _set_block(_map_pointer _new_block){
                _M_block = _new_block;
                _M_first = *_M_block;
                _M_last = _M_first + difference_type(_M_block_size());
            }
        public:

            _deque_iterator(_element_pointer _value, _map_pointer _block) _YXXX_NOEXCEPT:
                _M_iter(_value), _M_first(*_block),
                _M_last(*_block + _M_block_size()), _M_block(_block){}
            _deque_iterator(const self_type& _iterator) _YXXX_NOEXCEPT:
                _M_iter(_iterator._M_iter), _M_first(_iterator._M_first),
                _M_last(_iterator._M_last), _M_block(_iterator._M_block){}

            operator const_iterator(){
                return const_iterator(_M_iter, _M_block);
            }

            reference operator*() const _YXXX_NOEXCEPT{return *_M_iter;}
            pointer operator->() const _YXXX_NOEXCEPT{return _M_iter;}
            reference operator[](difference_type _offset) const _YXXX_NOEXCEPT{
                return *(*this + _offset);
            }
            
            self_type operator++(int) _YXXX_NOEXCEPT{
                self_type res{*this};
                ++*this;
                return res;
            }
            self_type& operator++() _YXXX_NOEXCEPT{
                if(_M_iter + 1 == _M_last){
                    _set_block(_M_block + 1);
                    _M_iter = _M_first;
                }
                else{
                    ++_M_iter;
                }
                return *this;
            }
            self_type operator--(int) _YXXX_NOEXCEPT{
                self_type res{*this};
                --*this;
                return res;
            }
            self_type& operator--() _YXXX_NOEXCEPT{
                if(_M_iter == _M_first){
                    _set_block(_M_block - 1);
                    _M_iter = _M_last;
                }
                else{
                    --_M_iter;
                }
                return *this;
            }
            self_type& operator+=(difference_type _offset) _YXXX_NOEXCEPT{
                difference_type offset_to_first = _M_iter - _M_first + _offset;
                if(offset_to_first >= 0 && offset_to_first < _M_block_size()){
                    _M_iter = _M_first + offset_to_first;
                }
                else{
                    difference_type block_offset = 
                        offset_to_first > 0 ?
                        offset_to_first / difference_type(_M_block_size()) :
                        -((-offset_to_first - 1) / difference_type(_M_block_size())) - 1;
                    _set_block(_M_block + block_offset);
                    _M_iter = _M_first + (_offset - 
                        block_offset * difference_type(_M_block_size()));
                }
                return *this;
            }
            self_type& operator-=(difference_type _offset) _YXXX_NOEXCEPT{
                return *this += -_offset;
            }
        
            self_type operator+(difference_type _offset) const _YXXX_NOEXCEPT{
                self_type res{this};
                res += _offset;
                return res;
            }
            friend self_type
            operator+(difference_type _offset, const self_type& _iter) _YXXX_NOEXCEPT{
                return _iter + (-_offset);
            }
            self_type operator-(difference_type _offset) const _YXXX_NOEXCEPT{
                self_type res{this};
                res -= _offset;
                return res;
            }

            friend bool operator==(const _deque_iterator& _l_ite,
                const _deque_iterator& _r_ite) _YXXX_NOEXCEPT{
                return _l_ite. _M_iter == _r_ite._M_iter;
            }
            friend bool operator!=(const _deque_iterator& _l_ite,
                const _deque_iterator& _r_ite) _YXXX_NOEXCEPT{
                return _l_ite. _M_iter != _r_ite._M_iter;
            }
            friend bool operator>=(const _deque_iterator& _l_ite,
                const _deque_iterator& _r_ite) _YXXX_NOEXCEPT{
                return _l_ite._M_block == _r_ite._M_block ?
                    _l_ite._M_iter >= _r_ite._M_iter :
                    _l_ite._M_block >= _r_ite._M_block;
            }
            friend bool operator<=(const _deque_iterator& _l_ite,
                const _deque_iterator& _r_ite) _YXXX_NOEXCEPT{
                return _l_ite._M_block == _r_ite._M_block ?
                    _l_ite._M_iter <= _r_ite._M_iter :
                    _l_ite._M_block <= _r_ite._M_block;
            }
            friend bool operator>(const _deque_iterator& _l_ite,
                const _deque_iterator& _r_ite) _YXXX_NOEXCEPT{
                return _l_ite._M_block == _r_ite._M_block ?
                    _l_ite._M_iter > _r_ite._M_iter :
                    _l_ite._M_block > _r_ite._M_block;
            }
            friend bool operator<(const _deque_iterator& _l_ite,
                const _deque_iterator& _r_ite) _YXXX_NOEXCEPT{
                return _l_ite._M_block == _r_ite._M_block ?
                    _l_ite._M_iter < _r_ite._M_iter :
                    _l_ite._M_block < _r_ite._M_block;
            }
            friend difference_type operator-(const _deque_iterator& _l_ite,
                const _deque_iterator& _r_ite) _YXXX_NOEXCEPT{
                if(_l_ite > _r_ite){
                    return _l_ite._M_block == _r_ite._M_block ?
                        _l_ite._M_iter - _r_ite._M_iter :
                        (_l_ite._M_iter - _r_ite._M_first)
                        + (_l_ite._M_block - _r_ite._M_block - 1)
                        * difference_type(_M_block_size());
                        + (_r_ite._M_last - _r_ite._M_first);
                }
                else return -(_r_ite - _l_ite);
            }
        protected:
            _element_pointer _M_iter;
            _element_pointer _M_first;
            _element_pointer _M_last;
            _map_pointer _M_block;
        };
    };

    template <typename _Type, typename _Alloc = _alloc::allocator<_Type>>
    class deque{
    public:
        typedef _Type          value_type;
        typedef _Alloc              allocator_type;
        typedef _alloc::_alloc_traits<allocator_type>  _alloc_traits;

        typedef typename _alloc_traits::pointer          pointer;
        typedef typename _alloc_traits::const_pointer    const_pointer;
        typedef typename _alloc_traits::reference        reference;
        typedef typename _alloc_traits::const_reference  const_reference;
        typedef typename _alloc_traits::size_type        size_type;
        typedef typename _alloc_traits::difference_type  difference_type;

        typedef _deque::_deque_iterator<
            value_type, reference, pointer>                  iterator;
        typedef typename iterator::const_iterator            const_iterator;
        typedef _iterator::reverse_iterator<iterator>        reverse_iterator;
        typedef _iterator::reverse_iterator<const_iterator>  const_reverse_iterator;

        typedef _alloc::alloc_rebind<allocator_type, pointer>  _map_alloc;
        typedef typename iterator::_map_pointer                _map_pointer;

        typedef deque           self_type;

        deque(size_type _capacity=0,
            const allocator_type& _alloc = allocator_type()) _YXXX_NOEXCEPT{
            _M_init(_capacity, _alloc);
        }
        deque(const allocator_type& _alloc) _YXXX_NOEXCEPT{_M_init(0, _alloc);}
        deque(const self_type& _deq) _YXXX_NOEXCEPT{
            _M_init(_deq.capacity(), _deq._M_allocator);
            _M_copy_or_assign(_deq.begin(), _deq.end());
        }
        deque(self_type&& _deq) _YXXX_NOEXCEPT{_M_move(_deq);}
        deque(std::initializer_list<value_type>&& _list) _YXXX_NOEXCEPT:
            deque(_list.size()){
            _M_move_or_assign(_list.begin(), _list.end());
        }
        template <typename _Iterator>
        deque(_Iterator _first, _Iterator _last,
            const allocator_type& _alloc = allocator_type()) _YXXX_NOEXCEPT{
            _M_init(0, _alloc);
            _M_copy_or_assign(_first, _last);
        }
        ~deque() _YXXX_NOEXCEPT{
            for(_map_pointer _map_ptr = _M_start; _map_ptr != _M_over; ++_map_ptr){
                _M_allocator.deallocate(*_map_ptr);
                *_map_ptr = nullptr;
            }
            _map_alloc(_M_allocator).deallocate(_M_map);
            _M_map_capacity = 0;
            _M_map = _M_start = _M_over = nullptr;
            _M_begin = _M_end = nullptr;
        }

        reference operator[](difference_type _index) _YXXX_NOEXCEPT{
            _index += _M_begin - *_M_start;
            difference_type _map_offset = difference_type(_index / _M_block_size());
            return *(*(_M_start + _map_offset) + _index % _M_block_size());
        }
        const_reference operator[](difference_type _index) const _YXXX_NOEXCEPT{
            _index += _M_begin - *_M_start;
            difference_type _map_offset = difference_type(_index / _M_block_size());
            return *(*(_M_start + _map_offset) + _index % _M_block_size());
        }
        self_type& operator=(const self_type& _deq) _YXXX_NOEXCEPT{
            _M_copy_or_assign(_deq.begin(), _deq.end());
            return *this;
        }
        self_type& operator=(self_type&& _deq) _YXXX_NOEXCEPT{
            _M_move(_deq);
            return *this;
        }
        void push_back(const value_type& _value){
            emplace_back(_value);
        }
        void push_back(value_type&& _value){
            emplace_back(std::move(_value));
        }
        void push_front(const value_type& _value){
            emplace_front(_value);
        }
        void push_front(value_type&& _value){
            emplace_front(std::move(_value));
        }
        template <typename ...Args>
        void emplace_back(Args&& ...args){
            _M_before_push_back();
            *(_M_end++) = value_type(std::forward<Args>(args)...);
        }
        template <typename ...Args>
        void emplace_front(Args&& ...args){
            _M_before_push_back();
            *(--_M_begin) = value_type(std::forward<Args>(args)...);
        }
        void pop_back() _YXXX_NOEXCEPT{
            if(_M_end == _M_begin) return;
            --_M_end;
            if(_M_end == *(_M_over - 1)){
                _M_end = *((--_M_over) - 1) + _M_block_size();
            }
        }
        void pop_front() _YXXX_NOEXCEPT{
            if(_M_end == _M_begin) return;
            ++_M_begin;
            if(_M_begin == *_M_start + _M_block_size()){
                _M_begin = *(++_M_start);
            }
        }
        reference front() _YXXX_NOEXCEPT{return *_M_begin;}
        const_reference front() const _YXXX_NOEXCEPT{return *_M_begin;}
        reference back() _YXXX_NOEXCEPT{return *(_M_end - 1);}
        const_reference back() const _YXXX_NOEXCEPT{return *(_M_end - 1);}

        iterator begin() _YXXX_NOEXCEPT{return iterator(_M_begin, _M_start);}
        const_iterator begin() const _YXXX_NOEXCEPT{return const_iterator(_M_begin, _M_start);}
        iterator end() _YXXX_NOEXCEPT{return iterator(_M_end, _M_over - 1);}
        const_iterator end() const _YXXX_NOEXCEPT{return const_iterator(_M_end, _M_over - 1);}
        const_iterator cbegin() const _YXXX_NOEXCEPT{return const_iterator(_M_begin, _M_start);}
        const_iterator cend() const _YXXX_NOEXCEPT{return const_iterator(_M_end, _M_over - 1);}
        reverse_iterator rbegin() _YXXX_NOEXCEPT{return reverse_iterator(end());}
        const_reverse_iterator rbegin() const _YXXX_NOEXCEPT{return const_reverse_iterator(end());}
        reverse_iterator rend() _YXXX_NOEXCEPT{return reverse_iterator(begin());}
        const_reverse_iterator rend() const _YXXX_NOEXCEPT{return const_reverse_iterator(begin());}
        const_reverse_iterator crbegin() const _YXXX_NOEXCEPT{return const_reverse_iterator(end());}
        const_reverse_iterator crend() const _YXXX_NOEXCEPT{return const_reverse_iterator(begin());}

        friend void swap(deque& _deq1, deque& _deq2) _YXXX_NOEXCEPT{
            std::swap(_deq1._M_map_capacity, _deq2._M_map_capacity);
            std::swap(_deq1._M_map, _deq2._M_map);
            std::swap(_deq1._M_start, _deq2._M_start);
            std::swap(_deq1._M_over, _deq2._M_over);
            std::swap(_deq1._M_begin, _deq2._M_begin);
            std::swap(_deq1._M_end, _deq2._M_end);
        }
        bool empty() const _YXXX_NOEXCEPT{return _M_begin == _M_end;}
        bool full() const _YXXX_NOEXCEPT{
            return _M_begin == *_M_start
                && _M_end == *(_M_over - 1) + _M_block_size();
        }
        size_type size() const _YXXX_NOEXCEPT{
            if(empty()) return 0;
            if(_M_start + 1 == _M_over) return _M_end - _M_begin;
            return (*_M_start + _M_block_size() - _M_begin)
                + (_M_over - _M_start - 1) * _M_block_size()
                + (_M_end - *(_M_over - 1));
        }
        size_type capacity() const _YXXX_NOEXCEPT{return _M_map_capacity * _M_block_size();}
        void clear() _YXXX_NOEXCEPT{
            _M_start = _M_map;
            _M_over = _M_start + 1;
            _M_end = _M_begin = *_M_start;
        }
        
        // std::ostream& show_memory(std::ostream& os) const _YXXX_NOEXCEPT{
        //     for(_map_pointer _map_iter = _M_map; _map_iter < _M_map + _M_map_capacity; ++_map_iter){
        //         if(_map_iter == _M_start)     os << "start -> [";
        //         else if(_map_iter == _M_over) os << "over  -> [";
        //         else                          os << "      -> [";
        //         if(*_map_iter != nullptr){
        //             for(pointer _ptr = *_map_iter; _ptr < (*_map_iter) + _M_block_size(); ++_ptr){
        //                 if(_ptr == _M_begin)
        //                     os << ">" << *_ptr << " ";
        //                 else if(_ptr + 1 == _M_end)
        //                     os << " " << *_ptr << "<";
        //                 else
        //                     os << " " << *_ptr << " ";
        //             }
        //         }
        //         os << "]\n";
        //     }
        //     return os;
        // }
    private:
        inline _YXXX_CONSTEXPR size_type _M_block_size() const _YXXX_NOEXCEPT{
            return _deque::_deque_block_size(sizeof(value_type));
        }

        void _M_init(size_type _capacity, const allocator_type& _alloc) _YXXX_NOEXCEPT{
            _M_allocator = _alloc;
            _M_map_capacity = _capacity % _M_block_size() ?
                              _capacity / _M_block_size() + 1 :
                              (_capacity ? _capacity / _M_block_size() : 1);
            _M_map = _map_alloc(_M_allocator).allocate(_M_map_capacity);
            for(_map_pointer _map_ptr = _M_map; _map_ptr < _M_map + _M_map_capacity; ++_map_ptr){
                *_map_ptr = _M_allocator.allocate(_M_block_size());
            }
            _M_start = _M_map;
            _M_over = _M_start + 1;
            _M_begin = _M_end = *_M_map;
        }
        void _M_move(self_type& _deq) _YXXX_NOEXCEPT{
            _M_before_move();
            _M_allocator = _deq._M_allocator;
            _M_map_capacity = _deq._M_map_capacity;
            _M_map = _deq._M_map;
            _M_start = _deq._M_start;
            _M_over = _deq._M_over;
            _M_begin = _deq._M_begin;
            _M_end = _deq._M_end;
            _deq._M_after_move();
        }
        void _M_before_move() _YXXX_NOEXCEPT{
            _M_begin = _M_end = nullptr;
            _M_start = _M_over = nullptr;
            for(_map_pointer _map_ptr = _M_map; _map_ptr < _M_map + _M_map_capacity; ++_map_ptr){
                _M_allocator.deallocate(*_map_ptr);
            }
            _map_alloc(_M_allocator).deallocate(_M_map);
            _M_map = nullptr;
        }
        void _M_after_move() _YXXX_NOEXCEPT{
           _M_end = nullptr;
           _M_begin = nullptr;
           _M_over = nullptr;
           _M_start = nullptr;
           _M_map = nullptr;
           _M_map_capacity = 0;
        }

        template <typename _Iterator>
        std::tuple<pointer, _Iterator>
        _M_copy_to_block(pointer _begin, pointer _end,
            _Iterator _src_begin, _Iterator _src_end) _YXXX_NOEXCEPT{
            while(_begin != _end && _src_begin != _src_end)
                *(_begin++) = *(_src_begin++);
            return std::tuple<pointer, _Iterator>{_begin, _src_begin};
        }
        template <typename _Iterator>
        void _M_copy_or_assign(_Iterator _begin, _Iterator _end) _YXXX_NOEXCEPT{
            clear();
            _map_pointer _map_iter = _M_start, _map_end = _M_map + _M_map_capacity;
            while(_begin != _end){
                _M_may_expand_back();
                while(_map_iter != _map_end && _begin != _end){
                    std::tie(_M_end, _begin) = _M_copy_to_block(
                        *_map_iter, *(_map_iter) + _M_block_size(), _begin, _end);
                    _M_over = ++_map_iter;
                }
            }
        }
        template <typename _Iterator>
        std::tuple<pointer, _Iterator>
        _M_move_to_block(pointer _begin, pointer _end,
            _Iterator _src_begin, _Iterator _src_end) _YXXX_NOEXCEPT{
            while(_begin != _end && _src_begin != _src_end)
                *(_begin++) = std::move(*(_src_begin++));
            return std::tuple<pointer, _Iterator>{_begin, _src_begin};
        }
        template <typename _Iterator>
        void _M_move_or_assign(_Iterator _begin, _Iterator _end) _YXXX_NOEXCEPT{
            clear();
            _map_pointer _map_iter = _M_start, _map_end = _M_map + _M_map_capacity;
            while(_begin != _end){
                _M_may_expand_back();
                while(_map_iter != _map_end && _begin != _end){
                    std::tie(_M_end, _begin) = _M_move_to_block(
                        *_map_iter, *(_map_iter) + _M_block_size(), _begin, _end);
                    _M_over = ++_map_iter;
                }
            }
        }
        void _M_expand_front() _YXXX_NOEXCEPT{
            size_type _new_map_capacity = _M_map_capacity * 1.5 + 1;
            _map_pointer _new_map = _map_alloc(_M_allocator).allocate(_new_map_capacity);
            _map_pointer _tmp = _new_map + _new_map_capacity - _M_map_capacity;
            memmove(_tmp, _M_map, _M_map_capacity * sizeof(pointer));
            for(_map_pointer _map_iter = _new_map;
                    _map_iter < _tmp; ++_map_iter){
                *_map_iter = _M_allocator.allocate(_M_block_size());
            }

            _M_start = _tmp + (_M_start - _M_map);
            _M_over = _tmp + (_M_over - _M_map);
            _map_alloc(_M_allocator).deallocate(_M_map);
            _M_map = _new_map;
            _M_map_capacity = _new_map_capacity;
        }
        void _M_expand_back() _YXXX_NOEXCEPT{
            size_type _new_map_capacity = _M_map_capacity * 1.5 + 1;
            _map_pointer _new_map = _map_alloc(_M_allocator).allocate(_new_map_capacity);
            memmove(_new_map, _M_map, _M_map_capacity * sizeof(pointer));

            _M_start = _new_map + (_M_start - _M_map);
            _M_over = _new_map + (_M_over - _M_map);
            _map_alloc(_M_allocator).deallocate(_M_map);

            for(_map_pointer _map_iter = _new_map + _M_map_capacity;
                    _map_iter < _new_map + _new_map_capacity; ++_map_iter){
                *_map_iter = _M_allocator.allocate(_M_block_size());
            }
            _M_map = _new_map;
            _M_map_capacity = _new_map_capacity;
        }
        void _M_may_expand_back() _YXXX_NOEXCEPT{
            if(_M_over == _M_map + _M_map_capacity
                && _M_end == *(_M_over - 1) + _M_block_size())
                _M_expand_back();
        }
        void _M_before_push_back() _YXXX_NOEXCEPT{
            if(_M_end == *(_M_over - 1) + _M_block_size()){
                if(_M_over == _M_map + _M_map_capacity)
                    _M_expand_back();
                _M_end = *(_M_over++);
            }
        }
        void _M_before_push_front() _YXXX_NOEXCEPT{
            if(_M_begin == *_M_start){
                if(_M_start == _M_map)
                    _M_expand_front();
                _M_begin = *(--_M_start) + _M_block_size();
            }
        }

    protected:
        allocator_type _M_allocator;
        size_type _M_map_capacity;
        _map_pointer _M_map;
        _map_pointer _M_start;
        _map_pointer _M_over;
        pointer _M_begin;
        pointer _M_end;
    };
};

#endif