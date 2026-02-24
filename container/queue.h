#ifndef _YXXX_QUEUE_H_
#define _YXXX_QUEUE_H_

#include "deque.h"

namespace stl{
    template <typename _Type, typename _Alloc = _alloc::allocator<_Type>,
        typename _deque = deque<_Type>>
    class queue{
    public:
        typedef _deque            base_container;
        typedef typename base_container::allocator_type      allocator_type;
        typedef typename base_container::value_type          value_type;
        typedef typename base_container::pointer             pointer;
        typedef typename base_container::const_pointer       const_pointer;
        typedef typename base_container::reference           reference;
        typedef typename base_container::const_reference     const_reference;
        typedef typename base_container::size_type           size_type;
        typedef typename base_container::difference_type     difference_type;

        typedef typename base_container::iterator                iterator;
        typedef typename base_container::const_iterator          const_iterator;
        typedef typename base_container::reverse_iterator        reverse_iterator;
        typedef typename base_container::const_reverse_iterator  const_reverse_iterator;

        typedef queue        self_type;

        queue(size_type _capacity=0,
            const allocator_type& _alloc = allocator_type()) _YXXX_NOEXCEPT:
            _M_deque(_capacity, _alloc){}
        queue(const allocator_type& _alloc) _YXXX_NOEXCEPT:
            _M_deque(_alloc){}
        queue(const self_type& _q) _YXXX_NOEXCEPT: _M_deque(_q._M_deque){}
        queue(self_type&& _q) _YXXX_NOEXCEPT: _M_deque(std::move(_q._M_deque)){}
        queue(std::initializer_list<value_type>&& _il) _YXXX_NOEXCEPT:
            _M_deque(std::move(_il)){}
        template <typename _Iterator>
        queue(_Iterator _begin, _Iterator _end) _YXXX_NOEXCEPT:
            _M_deque(_begin, _end){}

        reference operator[](difference_type _index) _YXXX_NOEXCEPT{return _M_deque[_index];}
        const_reference operator[](difference_type _index) const _YXXX_NOEXCEPT{
            return _M_deque[_index];
        }
        self_type& operator=(const self_type& _q) _YXXX_NOEXCEPT{
            _M_deque = _q._M_deque;
            return *this;
        }
        self_type& operator=(self_type&& _q) _YXXX_NOEXCEPT{
            _M_deque = std::move(_q._M_deque);
            return *this;
        }
        void push_back(const value_type& _value){
            _M_deque.emplace_back(_value);
        }
        void push_back(value_type&& _value){
            _M_deque.emplace_back(std::move(_value));
        }
        template <typename ...Args>
        void emplace_back(Args ...args){
            _M_deque.emplace_back(std::forward<Args>(args)...);
        }
        void push_front(const value_type& _value){
            _M_deque.push_front(_value);
        }
        void push_front(value_type&& _value){
            _M_deque.push_front(std::move(_value));
        }
        void pop_back() _YXXX_NOEXCEPT{_M_deque.pop_back();}
        void pop_front() _YXXX_NOEXCEPT{_M_deque.pop_front();}
        reference back() _YXXX_NOEXCEPT{return _M_deque.back();}
        const_reference back() const _YXXX_NOEXCEPT{return _M_deque.back();}
        reference front() _YXXX_NOEXCEPT{return _M_deque.front();}
        const_reference front() const _YXXX_NOEXCEPT{return _M_deque.front();}

        iterator begin() _YXXX_NOEXCEPT{return _M_deque.begin();}
        const_iterator begin() const _YXXX_NOEXCEPT{return _M_deque.begin();}
        iterator end() _YXXX_NOEXCEPT{return _M_deque.end();}
        const_iterator end() const _YXXX_NOEXCEPT{return _M_deque.end();}
        const_iterator cbegin() const _YXXX_NOEXCEPT{return _M_deque.cbegin();}
        const_iterator cend() const _YXXX_NOEXCEPT{return _M_deque.cend();}
        reverse_iterator rbegin() _YXXX_NOEXCEPT{return _M_deque.rbegin();}
        const_reverse_iterator rbegin() const _YXXX_NOEXCEPT{return _M_deque.rbegin();}
        reverse_iterator rend() _YXXX_NOEXCEPT{return _M_deque.rend();}
        const_reverse_iterator rend() const _YXXX_NOEXCEPT{return _M_deque.rend();}
        const_reverse_iterator crbegin() const _YXXX_NOEXCEPT{return _M_deque.crbegin();}
        const_reverse_iterator crend() const _YXXX_NOEXCEPT{return _M_deque.crend();}

        friend void swap(queue& _q1, queue& _q2) _YXXX_NOEXCEPT{
            swap(_q1._M_deque, _q2._M_deque);
        }
        size_type size() const _YXXX_NOEXCEPT{return _M_deque.size();}
        bool empty() const _YXXX_NOEXCEPT{return _M_deque.empty();}
        bool full() const _YXXX_NOEXCEPT{return _M_deque.full();}
        void clear() _YXXX_NOEXCEPT{_M_deque.clear();}
        void capacity() const _YXXX_NOEXCEPT{return _M_deque.capacity();}
    protected:
        base_container _M_deque;
    };
};

#endif