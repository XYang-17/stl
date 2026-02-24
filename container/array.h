#ifndef _YXXX_ARRAY_H_
#define _YXXX_ARRAY_H_

#include "_container_base.h"

namespace stl{
    template <typename _Type, typename _Alloc = _alloc::allocator<_Type>>
    class array{
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
        
        using iterator = _iterator::normal_iterator<pointer>;
        using const_iterator = _iterator::normal_iterator<const_pointer>;
        using reverse_iterator = _iterator::reverse_iterator<iterator>;
        using const_reverse_iterator = _iterator::reverse_iterator<const_iterator>;

        array(size_type _capacity=0,
            const allocator_type& _alloc = allocator_type())
        _YXXX_NOEXCEPT{
            _M_init(0, _capacity, _alloc);
        }
        array(const allocator_type& _alloc) _YXXX_NOEXCEPT{
            _M_init(0, 0, _alloc);
        }
        array(const array& _array) _YXXX_NOEXCEPT{
            _M_init(_array._M_size,
                _array._M_capacity,
                _array._M_allocator);
            _M_copy_or_assign(_array.begin(), _array.end());
        }
        array(array&& _array) _YXXX_NOEXCEPT{_M_move(_array);}
        array(std::initializer_list<value_type>&& _list)
        _YXXX_NOEXCEPT: array(_list.size()){
            _M_move_or_assign(_list.begin(), _list.end());
        }
        template <typename _Iterator>
        array(_Iterator _begin, _Iterator _end,
            const allocator_type& _alloc = allocator_type())
        _YXXX_NOEXCEPT{
            _M_init(0, 0, _alloc);
            _M_copy_or_assign(_begin, _end);
        }
        ~array() _YXXX_NOEXCEPT{
            _M_allocator.deallocate(_M_begin);
            _M_begin = nullptr;
        }

        const value_type&
        operator[](difference_type _index)
        const _YXXX_NOEXCEPT{
            return _M_begin[_index];
        }
        value_type&
        operator[](difference_type _index)
        _YXXX_NOEXCEPT{
            return _M_begin[_index];
        }
        array& operator=(const array& _array)
        _YXXX_NOEXCEPT{
            if(&_array == this) return *this;
            _M_allocator = _array._M_allocator;
            _M_copy_or_assign(_array.begin(), _array.end());
            return *this;
        }
        array& operator=(array&& _array)
        _YXXX_NOEXCEPT{
            if(&_array == this) return *this;
            _M_allocator.deallocate(_M_begin);
            _M_move(_array);
            return *this;
        }
        void push_back(const value_type& _data){
            emplace_back(_data);
        }
        void push_back(value_type&& _data){
            emplace_back(std::move(_data));
        }
        void insert(difference_type _index, 
            const value_type& _value){
            emplace(_index, _value);
        }
        void insert(difference_type _index, value_type&& _value){
            emplace(_index, std::move(_value));
        }
        template <typename ...Args>
        void emplace_back(Args&& ..._args){
            _M_may_expand();
             _M_begin[_M_size++] = value_type(
                std::forward<Args>(_args)...
            );
        }
        template <typename ...Args>
        void emplace(difference_type _index, Args&& ..._args){
            _index = _M_before_insert(_index);
             _M_begin[_index] = value_type(
                std::forward<Args>(_args)...
            );
        }
        void pop_back() _YXXX_NOEXCEPT{
            if(!empty()) --_M_size;
        }
        void erase(difference_type _index)
        _YXXX_NOEXCEPT{
            for(int i = _index;i < _M_size;){
                 _M_begin[i] = _M_begin[++i];
            }
            --_M_size;
        }
        const value_type&
        front() const _YXXX_NOEXCEPT{
            return _M_begin[0];
        }
        value_type&
        front() _YXXX_NOEXCEPT{
            return _M_begin[0];
        }
        const value_type&
        back() const _YXXX_NOEXCEPT{
            return _M_begin[_M_size - 1];
        }
        value_type&
        back() _YXXX_NOEXCEPT{
            return _M_begin[_M_size - 1];
        }

        iterator begin() _YXXX_NOEXCEPT{
            return _M_begin;
        }
        const_iterator
        begin() const _YXXX_NOEXCEPT{
            return _M_begin;
        }
        iterator end() _YXXX_NOEXCEPT{
            return _M_begin + _M_size;
        }
        const_iterator
        end() const _YXXX_NOEXCEPT{
            return _M_begin + _M_size;
        }
        const_iterator
        cbegin() const _YXXX_NOEXCEPT{
            return _M_begin;
        }
        const_iterator
        cend() const _YXXX_NOEXCEPT{
            return _M_begin+_M_size;
        }
        reverse_iterator
        rbegin() _YXXX_NOEXCEPT{
            return _M_begin+_M_size;
        }
        const_reverse_iterator
        rbegin() const _YXXX_NOEXCEPT{
            return _M_begin+_M_size;
        }
        reverse_iterator
        rend() _YXXX_NOEXCEPT{
            return _M_begin;
        }
        const_reverse_iterator
        rend() const _YXXX_NOEXCEPT{
            return _M_begin;
        }
        const_reverse_iterator
        crbegin() const _YXXX_NOEXCEPT{
            return _M_begin+_M_size;
        }
        const_reverse_iterator
        crend() const _YXXX_NOEXCEPT{
            return _M_begin;
        }
        
        friend void swap(array& _array1, array& _array2)
        _YXXX_NOEXCEPT{
            std::swap(_array1._M_size, _array2._M_size);
            std::swap(_array1._M_capacity, _array2._M_capacity);
            std::swap(_array1._M_begin, _array2._M_begin);
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
        void _M_init(size_type _size, size_type _capacity,
            const allocator_type& _alloc)
        _YXXX_NOEXCEPT{
            _M_allocator = _alloc;
            _M_size = _size;
            _M_capacity = _capacity;
            _M_begin = _M_allocator.allocate(_M_capacity);
        }
        void _M_move(array& _array) _YXXX_NOEXCEPT{
            _M_before_move();
            _M_allocator = std::move(_array._M_allocator);
            _M_size = _array._M_size;
            _M_capacity = _array._M_capacity;
            _M_begin = _array._M_begin;
            _array._M_after_move();
        }
        void _M_before_move() _YXXX_NOEXCEPT{
            _M_allocator.deallocate(_M_begin);
            _M_begin = nullptr;
        }
        void _M_after_move() _YXXX_NOEXCEPT{
            _M_size = 0;
            _M_capacity = 0;
            _M_begin = nullptr;
        }

        template <typename _Iterator>
        void _M_copy_or_assign(_Iterator _begin, _Iterator _end)
        _YXXX_NOEXCEPT{
            if(!empty()){
                pointer _ptr = _M_begin, _end_ptr = _M_begin + _M_capacity;
                while(_ptr != _end_ptr && _begin != _end){
                    *(_ptr++) = *(_begin++);
                }
                if(_begin == _end){
                    _M_size = _ptr - _M_begin;
                }
            }
            while(_begin != _end){
                push_back(*(_begin++));
            }
        }
        template <typename _Iterator>
        void _M_move_or_assign(_Iterator _begin, _Iterator _end)
        _YXXX_NOEXCEPT{
            if(!empty()){
                pointer _ptr = _M_begin, _end_ptr = _M_begin + _M_size;
                while(_ptr != _end_ptr && _begin != _end){
                    *(_ptr++) = std::move(*(_begin++));
                }
                if(_begin == _end){
                    _M_size = _ptr - _M_begin;
                }
            }
            while(_begin != _end){
                push_back(std::move(*(_begin++)));
            }
        }
        void _M_expand() _YXXX_NOEXCEPT{
            size_type new_capacity = _M_capacity * 1.5 + 1;
            pointer new_value = _M_allocator.allocate(new_capacity);
            memmove(new_value, _M_begin, _M_capacity * sizeof(value_type));
            _M_allocator.deallocate(_M_begin);
             _M_begin = new_value;
            _M_capacity = new_capacity;
        }
        void _M_may_expand() _YXXX_NOEXCEPT{
            if(full()) _M_expand();
        }
        difference_type _M_before_insert(difference_type _index){
            if(_index > _M_size) _index = _M_size;
            _M_may_expand();
            for(int i = _M_size++;i > _index;--i) 
                _M_begin[i] = _M_begin[i-1];
            return _index;
        }
    protected:
        allocator_type _M_allocator;
        size_type _M_size;
        size_type _M_capacity;
        pointer _M_begin;
    };
};

#endif