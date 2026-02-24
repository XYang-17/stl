#ifndef _YXXX_STACK_H_
#define _YXXX_STACK_H_

#include "array.h"

namespace stl{
    // std::stack使用std::deque作为默认的底层序列容器，有以下优势：
    //   1.分散存储，更加灵活，且避免大规模的数据移动，保证性能稳定
    //   3.元素的指针/引用不会失效
    template <typename _Type, typename _Alloc = _alloc::allocator<_Type>,
        typename _Sequence = array<_Type, _Alloc>>
    class stack{
    public:
        typedef _Sequence                                base_container;
        typedef typename base_container::allocator_type      allocator_type;
        typedef typename base_container::value_type          value_type;
        typedef typename base_container::pointer             pointer;
        typedef typename base_container::const_pointer       const_pointer;
        typedef typename base_container::reference           reference;
        typedef typename base_container::const_reference     const_reference;
        typedef typename base_container::size_type           size_type;
        typedef typename base_container::difference_type     difference_type;
        
        using iterator               = typename base_container::iterator;
        using const_iterator         = typename base_container::const_iterator;
        using reverse_iterator       = typename base_container::reverse_iterator;
        using const_reverse_iterator  = typename base_container::const_reverse_iterator;
        
        stack(size_type _capacity = 0)
        _YXXX_NOEXCEPT: _M_sequence(_capacity){}
        stack(const base_container& _sequence)
        _YXXX_NOEXCEPT:
            _M_sequence(_sequence._M_sequence){}
        stack(base_container&& _sequence) _YXXX_NOEXCEPT:
            _M_sequence(std::move(_sequence._M_sequence)){}
        stack(std::initializer_list<value_type>&& _il)
        _YXXX_NOEXCEPT: _M_sequence(std::move(_il)){}
        template <typename _Iterator>
        stack(_Iterator _begin, _Iterator _end)
        _YXXX_NOEXCEPT: _M_sequence(_begin, _end){}

        stack& operator=(const stack& _stack)
        _YXXX_NOEXCEPT{
            _M_sequence = _stack._M_sequence;
            return *this;
        }
        stack& operator=(stack&& _stack)
        _YXXX_NOEXCEPT{
            _M_sequence = std::move(_stack._M_sequence);
            return *this;
        }
        void push(const value_type& _data){
            _M_sequence.push_back(_data);
        }
        void push(value_type&& _data){
            _M_sequence.push_back(std::move(_data));
        }
        template <typename ...Args>
        void emplace(Args&& ...args){
            _M_sequence.emplace_back(
                std::forward<Args>(args)...
            );
        }
        void pop() _YXXX_NOEXCEPT{
            _M_sequence.pop_back();
        }
        const value_type& peek() const{
            return _M_sequence.back();
        }
        value_type& peek(){
            return _M_sequence.back();
        }

        friend void swap(stack& _stack1, stack& _stack2)
        _YXXX_NOEXCEPT{
            swap(_stack1._M_sequence, _stack2._M_sequence);
        }
        bool empty() const _YXXX_NOEXCEPT{
            return _M_sequence.empty();
        }
        size_type size() const _YXXX_NOEXCEPT{
            return _M_sequence.size();
        }
        size_type capacity() const _YXXX_NOEXCEPT{
            return _M_sequence.capacity();
        }
        void clear() _YXXX_NOEXCEPT{
            _M_sequence.clear();
        }
        bool full() const _YXXX_NOEXCEPT{
            return _M_sequence.full();
        }
    protected:
        base_container _M_sequence;
    };
};

#endif