#ifndef _YXXX_a_H_
#define _YXXX_a_H_

#include <stdexcept>
#include "./_container_base.h"

namespace stl{
    template <typename _Type, typename _Alloc = _alloc::allocator<_Type>>
    class array{
    protected:
        typedef _alloc::_alloc_traits<_Alloc>  _alloc_traits;
    public:
        typedef _Type          value_type;
        typedef _Alloc              allocator_type;

        typedef typename _alloc_traits::pointer          pointer;
        typedef typename _alloc_traits::const_pointer    const_pointer;
        typedef typename _alloc_traits::reference        reference;
        typedef typename _alloc_traits::const_reference  const_reference;
        typedef typename _alloc_traits::size_type        size_type;
        typedef typename _alloc_traits::difference_type  difference_type;
        
        using iterator               = _iterator::normal_iterator<pointer>;
        using const_iterator         = _iterator::normal_iterator<const_pointer>;
        using reverse_iterator       = _iterator::reverse_iterator<iterator>;
        using const_reverse_iterator = _iterator::reverse_iterator<const_iterator>;

        using return_type    = iterator;

        array() _YXXX_NOEXCEPT:
            _M_allocator(allocator_type{}), _M_size(0),
            _M_capacity(0), _M_data(nullptr){}
        array(size_type _size):
            _M_allocator(allocator_type{}),
            _M_size(_size), _M_capacity(_size),
            _M_data(_M_allocator.allocate(_M_capacity)){}
        array(const allocator_type& _alloc):
            _M_allocator(_alloc), _M_size(0),
            _M_capacity(0), _M_data(nullptr){}
        array(allocator_type&& _alloc):
            _M_allocator(std::move(_alloc)), _M_size(0),
            _M_capacity(0), _M_data(nullptr){}
        array(size_type _size, const allocator_type& _alloc):
            _M_allocator(_alloc),
            _M_size(_size), _M_capacity(_size),
            _M_data(_M_allocator.allocate(_M_capacity)){}
        array(size_type _size, allocator_type&& _alloc):
            _M_allocator(std::move(_alloc)),
            _M_size(_size), _M_capacity(_size),
            _M_data(_M_allocator.allocate(_M_capacity)){}

        array(const array& _a):
            array(_a._M_size, _a._M_allocator){
            _M_copy_or_assign(_a.begin(), _a.end());
        }
        array(const array& _a, const allocator_type& _alloc):
            array(_a._M_size, _alloc){
            _M_copy_or_assign(_a.begin(), _a.end());
        }
        array(const array& _a, allocator_type&& _alloc):
            array(_a._M_size, std::move(_alloc)){
            _M_copy_or_assign(_a.begin(), _a.end());
        }

        array(array&& _a) _YXXX_NOEXCEPT:
            _M_allocator(std::move(_a._M_allocator)){
            _M_move_data(_a);
        }
        array(array&& _a, const allocator_type& _alloc)
        _YXXX_NOEXCEPT: _M_allocator(_alloc){
            _M_move_data(_a);
        }
        array(array&& _a, allocator_type&& _alloc)
        _YXXX_NOEXCEPT: _M_allocator(std::move(_alloc)){
            _M_move_data(_a);
        }

        template <typename _Iterator,
            typename = std::enable_if_t<stl::is_iterator_v<_Iterator>>>
        array(_Iterator _begin, _Iterator _end): array(){
            _M_copy_or_assign(_begin, _end);
        }
        template <typename _Iterator,
            typename = std::enable_if_t<stl::is_iterator_v<_Iterator>>>
        array(_Iterator _begin, _Iterator _end,
            const allocator_type& _alloc): array(_alloc){
            _M_copy_or_assign(_begin, _end);
        }
        template <typename _Iterator,
            typename = std::enable_if_t<stl::is_iterator_v<_Iterator>>>
        array(_Iterator _begin, _Iterator _end,
            allocator_type&& _alloc): array(std::move(_alloc)){
            _M_copy_or_assign(_begin, _end);
        }

        array(std::initializer_list<value_type>&& _il): array(){
            _M_copy_or_assign(_il.begin(), _il.end());
        }
        array(std::initializer_list<value_type>&& _il,
            const allocator_type& _alloc): array(_alloc){
            _M_copy_or_assign(_il.begin(), _il.end());
        }
        array(std::initializer_list<value_type>&& _il,
            allocator_type&& _alloc): array(std::move(_alloc)){
            _M_copy_or_assign(_il.begin(), _il.end());
        }

        ~array() _YXXX_NOEXCEPT{
            _M_allocator.deallocate(_M_data, sizeof(value_type) * _M_capacity);
            _M_data = nullptr;
        }

        const value_type&
        operator[](difference_type _index)
        const _YXXX_NOEXCEPT{
            return _M_data[_index];
        }
        value_type&
        operator[](difference_type _index)
        _YXXX_NOEXCEPT{
            return _M_data[_index];
        }
        const value_type&
        at(difference_type _index) const{
            if(!_M_valid(_index)) throw std::out_of_range();
            return _M_data[_index];
        }
        value_type&
        at(difference_type _index){
            if(!_M_valid(_index)) throw std::out_of_range();
            return _M_data[_index];
        }
        const value_type&
        front() const _YXXX_NOEXCEPT{
            return _M_data[0];
        }
        value_type&
        front() _YXXX_NOEXCEPT{
            return _M_data[0];
        }
        const value_type&
        back() const _YXXX_NOEXCEPT{
            return _M_data[_M_size - 1];
        }
        value_type&
        back() _YXXX_NOEXCEPT{
            return _M_data[_M_size - 1];
        }

        array& operator=(const array& _a){
            if(&_a == this) return *this;
            _M_allocator = _a._M_allocator;
            _M_copy_or_assign(_a.begin(), _a.end());
            return *this;
        }
        array& operator=(array&& _a)
        _YXXX_NOEXCEPT{
            if(&_a == this) return *this;
            _M_move(std::move(_a));
            return *this;
        }

        // assign
        void assign(size_type _n, const value_type& _val){
            if(_M_capacity < _n)
                _M_expand(_M_size, _n - _M_capacity, false);
            _M_size = _n;
            if(_n) std::fill_n(_M_data, _n, _val);
        }
        template <typename _Iterator>
        std::enable_if_t<
            stl::is_iterator_v<_Iterator>,
            void>
        assign(_Iterator _beg, _Iterator _end){
            _M_copy_or_assign(_beg, _end);
        }
        void
        assign(std::initializer_list<value_type>&& _il){
            _M_copy_or_assign(_il.begin(), _il.end());
        }

        // push_back/emplace_back
        return_type
        push_back(const value_type& _data){
            return _M_emplace(_M_size, _data);
        }
        return_type
        push_back(value_type&& _data){
            return _M_emplace(_M_size, std::move(_data));
        }
        template <typename ...Args>
        return_type
        emplace_back(Args&& ..._args){
            return _M_emplace(_M_size, std::forward<Args>(_args)...);
        }

        // insert/emplace
        return_type
        insert(difference_type _index, const value_type& _value){
            if(!_M_valid_insert(_index)) return end();
            _M_emplace(_index, _value);
        }
        return_type
        insert(iterator _pos, const value_type& _value){
            if(!_M_valid_insert(_pos)) return end();
            _M_emplace(_M_idx(_pos), _value);
        }

        return_type
        insert(difference_type _index, value_type&& _value){
            if(!_M_valid_insert(_index)) return end();
            return _M_emplace(_index, std::move(_value));
        }
        return_type
        insert(iterator _pos, value_type&& _value){
            if(!_M_valid_insert(_pos)) return end();
            return _M_emplace(_M_idx(_pos), std::move(_value));
        }

        return_type
        insert(difference_type _index, size_type _n,
            const value_type& _value){
            if(0 == _n || !_M_valid_insert(_index))
                return end();
            return _M_emplace(_index, _n, _value);
        }
        return_type
        insert(iterator _begin, size_type _n,
            const value_type& _value){
            if(0 == _n || !_M_valid_insert(_begin))
                return end();
            return _M_emplace(_begin.base() - _M_data, _n, _value);
        }

        return_type
        insert(difference_type _index, iterator _begin, iterator _end){
            if(_begin == _end || !_M_valid_insert(_index))
                return end();
            size_type _n = _end - _begin;
            _M_before_insert(_index, _n);
            memcpy(_M_data + _index, _begin.base(), _n);
            return _M_it(_index);
        }
        return_type
        insert(iterator _pos, iterator _begin, iterator _end){
            if(_begin == _end || !_M_valid_insert(_pos))
                return end();
            difference_type _index = _M_idx(_pos);
            size_type _n = _end - _begin;
            _M_before_insert(_index, _n);
            memcpy(_M_data + _index, _begin.base(), _n);
            return _M_it(_index);
        }

        template <typename _Iterator>
        std::enable_if_t<
            stl::is_iterator_v<_Iterator>,
            return_type>
        insert(difference_type _index, _Iterator _begin, _Iterator _end){
            if(_begin == _end || !_M_valid_insert(_index))
                return end();

            if constexpr(stl::is_random_access_iterator_v<_Iterator>){
                _M_before_insert(_index, _end - _begin);
                difference_type _idx = _index;
                for(_Iterator _it = _begin; _it != _end; ++_it, ++_idx){
                    _M_data[_idx] = *_it;
                }
            }
            else{
                difference_type _idx = _index;
                while(_begin != _end){
                    _M_emplace(_idx++, *_begin++);
                }
            }
            return _M_it(_index);
        }
        template <typename _Iterator>
        std::enable_if_t<
            stl::is_iterator_v<_Iterator>,
            return_type>
        insert(iterator _pos, _Iterator _begin, _Iterator _end){
            if(_begin == _end || !_M_valid_insert(_pos))
                return end();

            difference_type _index = _M_idx(_pos);
            if constexpr(stl::is_random_access_iterator_v<_Iterator>){
                _M_before_insert(_index, _end - _begin);
                difference_type _idx = _index;
                for(_Iterator _it = _begin; _it != _end; ++_it, ++_idx){
                    _M_data[_idx] = *_it;
                }
            }
            else{
                difference_type _idx = _index;
                while(_begin != _end){
                    _M_emplace(_idx++, *_begin++);
                }
            }
            return _M_it(_index);
        }

        template <typename ...Args>
        return_type emplace(difference_type _index, Args&& ..._args){
            if(!_M_valid_insert(_index)) return end();
            return _M_emplace(_index, std::forward<Args>(_args)...);
        }
        template <typename ...Args>
        return_type emplace(iterator _pos, Args&& ..._args){
            if(!_M_valid_insert(_pos)) return end();
            return _M_emplace(
                _M_idx(_pos),
                std::forward<Args>(_args)...
            );
        }

        // pop_back/erase
        void pop_back() _YXXX_NOEXCEPT{
            if(!empty()) --_M_size;
        }

        return_type erase(difference_type _index){
            if(!_M_valid(_index)) end();
            return _M_erase(_index, 1);
        }
        return_type erase(iterator _pos){
            if(!_M_valid(_pos)) end();
            return _M_erase(_M_idx(_pos), 1);
        }

        return_type erase(difference_type _index, size_type _n){
            if(!_M_valid(_index)) return end();
            return _M_erase(_index, _n);
        }
        return_type erase(iterator _begin, iterator _end){
            if(!_M_valid_range(_begin, _end))
                return end();
            return _M_erase(_M_idx(_begin), _end - _begin);
        }

        iterator begin() _YXXX_NOEXCEPT{
            return _M_data;
        }
        const_iterator
        begin() const _YXXX_NOEXCEPT{
            return _M_data;
        }
        iterator end() _YXXX_NOEXCEPT{
            return _M_data + _M_size;
        }
        const_iterator
        end() const _YXXX_NOEXCEPT{
            return _M_data + _M_size;
        }
        const_iterator
        cbegin() const _YXXX_NOEXCEPT{
            return _M_data;
        }
        const_iterator
        cend() const _YXXX_NOEXCEPT{
            return _M_data+_M_size;
        }
        reverse_iterator
        rbegin() _YXXX_NOEXCEPT{
            return end();
        }
        const_reverse_iterator
        rbegin() const _YXXX_NOEXCEPT{
            return end();
        }
        reverse_iterator
        rend() _YXXX_NOEXCEPT{
            return begin();
        }
        const_reverse_iterator
        rend() const _YXXX_NOEXCEPT{
            return begin();
        }
        const_reverse_iterator
        crbegin() const _YXXX_NOEXCEPT{
            return cend();
        }
        const_reverse_iterator
        crend() const _YXXX_NOEXCEPT{
            return cbegin();
        }
        
        friend void swap(array& _a1, array& _a2){
            std::swap(_a1._M_size, _a2._M_size);
            std::swap(_a1._M_capacity, _a2._M_capacity);
            std::swap(_a1._M_data, _a2._M_data);
            std::swap(_a1._M_allocator, _a2._M_allocator);
        }

        void shrink_to_fit(){
            reserve(_M_size);
        }
        void reserve(size_type _n){
            if(nullptr != _M_data){
                pointer _new_begin = _M_allocator.allocate(_n);
                if(_n < _M_size) _M_size = _n;
                memmove(_new_begin, _M_data, _M_size * sizeof(value_type));
                _M_allocator.deallocate(_M_data, _M_capacity);
                _M_data = _new_begin;
            }
            else{
                _M_size = _n;
                _M_data = _M_allocator.allocate(_M_size);
            }
            _M_capacity = _n;
        }

        bool empty() const _YXXX_NOEXCEPT{
            return 0 == _M_size;
        }
        bool full() const _YXXX_NOEXCEPT{
            return _M_size == _M_capacity;
        }
        size_type size() const _YXXX_NOEXCEPT{
            return _M_size;
        }
        size_type capacity() const _YXXX_NOEXCEPT{
            return _M_capacity;
        }
        void clear() _YXXX_NOEXCEPT{
            _M_size = 0;
        }
    private:
        bool _M_valid(difference_type _idx)
        const _YXXX_NOEXCEPT{
            return size_type(_idx) < _M_size;
        }
        bool _M_valid(iterator _pos)
        const _YXXX_NOEXCEPT{
            return _M_valid(_M_idx(_pos));
        }
        bool _M_valid_insert(difference_type _idx)
        const _YXXX_NOEXCEPT{
            return size_type(_idx) <= _M_size;
        }
        bool _M_valid_insert(iterator _pos)
        const _YXXX_NOEXCEPT{
            return _M_valid_insert(_M_idx(_pos));
        }
        bool _M_valid_range(iterator _beg, iterator _end)
        const _YXXX_NOEXCEPT{
            if(_end <= _beg || !_M_valid(_beg))
                return false;
            return size_type(_M_idx(_end)) <= _M_size;
        }

        iterator _M_it(difference_type _idx)
        const _YXXX_NOEXCEPT{
            return _M_data + _idx;
        }
        difference_type _M_idx(iterator _pos) const{
            return _pos.base() - _M_data;
        }

        void _M_destroy(difference_type _index, size_type _n){
            while(_n--) _M_allocator.destroy(_M_data + (_index++));
        }

        void _M_move(array&& _a) _YXXX_NOEXCEPT{
            _M_before_move();
            _M_allocator = std::move(_a._M_allocator);
            _M_move_data(_a);
        }
        void _M_move_data(array& _a) _YXXX_NOEXCEPT{
            _M_data = _a._M_data;
            _M_size = _a._M_size;
            _M_capacity = _a._M_capacity;
            _a._M_after_move();
        }
        void _M_before_move() _YXXX_NOEXCEPT{
            if(nullptr != _M_data){
                _M_destroy(0, _M_size);
                _M_allocator.deallocate(
                    _M_data, sizeof(value_type) * _M_capacity);
                _M_data = nullptr;
            }
            // _M_size = _M_capacity = 0;
        }
        void _M_after_move() _YXXX_NOEXCEPT{
            if(nullptr != _M_data){
                _M_data = nullptr;
            }
            _M_size = _M_capacity = 0;
        }

        void _M_assign_n(size_type _n){
            if(nullptr == _M_data)
                _M_allocator = allocator_type{};
            if(_M_capacity < _n)
                _M_expand(_M_size, _n - _M_capacity, false, true);
            _M_size = _n;
        }
        
        void _M_expand(difference_type _index, size_type _n,
            bool _move = true, bool _shrank = false){
            size_type _new_capacity = _M_capacity + _n;
            if(!_shrank) _new_capacity = _new_capacity * 1.5 + 1;
            pointer _new_begin = _M_allocator.allocate(_new_capacity);
            
            if(_move){
                memmove(_new_begin, _M_data, size_type(_index) * sizeof(value_type));
                if(size_type(_index) < _M_size)
                    memmove(
                        _new_begin + _index + difference_type(_n),
                        _M_data + _index,
                        (_M_size - size_type(_index)) * sizeof(value_type)
                    );
            }

            _M_allocator.deallocate(_M_data, _M_capacity * sizeof(value_type));
            _M_data = _new_begin;
            _M_capacity = _new_capacity;
        }
        
        void _M_copy_or_assign(iterator _begin, iterator _end){
            size_type _size = size_type(_end - _begin);
            if(_M_capacity < _size){
                _M_expand(_M_size, _size - _M_capacity, false);
            }
            memcpy(_M_data, _begin.base(), _size * sizeof(value_type));
            _M_size = _size;
        }
        template <typename _Iterator>
        std::enable_if_t<
            stl::is_iterator_v<_Iterator>,
            void>
        _M_copy_or_assign(_Iterator _begin, _Iterator _end){
            if constexpr(stl::is_random_access_iterator_v<_Iterator>){
                size_type _size = size_type(_end - _begin);
                if(_M_capacity < _size){
                    _M_expand(_M_size, _size - _M_capacity, false);
                }
                for(pointer _ptr = _M_data; _begin != _end; ){
                    *(_ptr++) = *(_begin++);
                }
                _M_size = _size;
            }
            else{
                if(!empty()){
                    pointer _ptr = _M_data, _end_ptr = _M_data + _M_capacity;
                    while(_ptr != _end_ptr && _begin != _end){
                        *(_ptr++) = *(_begin++);
                    }
                    if(_begin == _end){
                        _M_size = _ptr - _M_data;
                    }
                }
                while(_begin != _end){
                    push_back(*(_begin++));
                }
            }
        }
        
        void _M_before_insert(difference_type _index, size_type _n){
            if(_M_capacity - _M_size < _n)
                _M_expand(_index, _n);
            else{
                for(difference_type _idx = difference_type(_M_size - 1);
                    _idx >= _index; --_idx){
                    _M_data[_idx+_n] = std::move(_M_data[_idx]);
                }
            }
            _M_size += _n;
        }

        template <typename ...Args>
        return_type
        _M_emplace(difference_type _index, Args&& ..._args){
            _M_before_insert(_index, 1);
            _M_data[_index] = value_type(
                std::forward<Args>(_args)...
            );
            return _M_it(_index);
        }
        return_type
        _M_emplace(difference_type _index,
            size_type _n, const value_type& _value){
            _M_before_insert(_index, _n);
            std::fill_n(_M_data + _index, _n, _value);
            return _M_it(_index);
        }

        return_type _M_erase(difference_type _index, size_type _n){
            _M_destroy(_index, _n);
            if(_M_size - size_type(_index) > _n){
                difference_type _dst_idx = _index, _src_idx = _dst_idx + difference_type(_n);
                while(_M_size - size_type(_src_idx) > _n){
                    memmove(_M_data+_dst_idx, _M_data+_src_idx, _n * sizeof(value_type));
                    _dst_idx = _src_idx;
                    _src_idx += difference_type(_n);
                }
                memmove(_M_data+_dst_idx, _M_data+_src_idx,
                    (_M_size - size_type(_src_idx)) * sizeof(value_type)
                );
                _M_size -= _n;
            }
            else{
                _M_size = _index;
            }
            return _M_it(_index);
        }
        
    protected:
        allocator_type _M_allocator;
        size_type _M_size;
        size_type _M_capacity;
        pointer _M_data;
    };
};

#endif