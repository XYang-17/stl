#ifndef _YXXX_PRIORITY_QUEUE_H_
#define _YXXX_PRIORITY_QUEUE_H_

#include "array.h"
#include "heap.h"
#include "../function.h"

namespace stl{
    template <typename _Type, typename _Alloc = _alloc::allocator<_Type>,
        typename _Sequence = array<_Type>, typename _Comp = stl::less_than<_Type>>
    class priority_queue{
    public:
        typedef _Sequence                             base_container;
        typedef typename base_container::allocator_type      allocator_type;
        typedef typename base_container::value_type          value_type;
        typedef typename base_container::pointer             pointer;
        typedef typename base_container::const_pointer       const_pointer;
        typedef typename base_container::reference           reference;
        typedef typename base_container::const_reference     const_reference;
        typedef typename base_container::size_type           size_type;
        typedef typename base_container::difference_type     difference_type;

        priority_queue(size_type _capacity=0, const _Comp& _comp = _Comp{}) _YXXX_NOEXCEPT:
            _M_sequence(_capacity), _M_comp(_comp){}
        priority_queue(const priority_queue& _heap) _YXXX_NOEXCEPT:
            _M_sequence(_heap._M_sequence), _M_comp(_heap._M_comp){}
        priority_queue(priority_queue&& _heap) _YXXX_NOEXCEPT:
            _M_sequence(std::move(_heap._M_sequence)), _M_comp(_heap._M_comp){}
        priority_queue(const base_container& _sequence, 
            const _Comp& _comp = _Comp{}) _YXXX_NOEXCEPT:
            _M_sequence(_sequence), _M_comp(_comp){
            stl::make_heap(_M_sequence.begin(), _M_sequence.end(), _M_comp);
        }
        priority_queue(base_container&& _sequence, 
            const _Comp& _comp = _Comp{}) _YXXX_NOEXCEPT:
            _M_sequence(std::move(_sequence)), _M_comp(_comp){
            stl::make_heap(_M_sequence.begin(), _M_sequence.end(), _M_comp);
        }
        template <typename _Iterator>
        priority_queue(_Iterator _begin, _Iterator _end, 
            const _Comp& _comp = _Comp{}):
            _M_sequence(_begin, _end), _M_comp(_comp){
            stl::make_heap(_M_sequence.begin(), _M_sequence.end(), _M_comp);
        }
        priority_queue(std::initializer_list<value_type>&& _il)
            : priority_queue(_il.begin(), _il.end()){}

        priority_queue& operator=(const priority_queue& _heap) _YXXX_NOEXCEPT{
            _M_sequence = _heap._M_sequence;
            _M_comp = _heap._M_comp;
            return *this;
        }
        priority_queue& operator=(priority_queue&& _heap) _YXXX_NOEXCEPT{
            _M_sequence = std::move(_heap._M_sequence);
            _M_comp = _heap._M_comp;
            return *this;

        }
        void push(const_reference _data){
            _M_sequence.push_back(_data);
            _M_push_heap();
        }
        void push(value_type&& _data){
            _M_sequence.push_back(std::move(_data));
            _M_push_heap();
        }
        template <typename ...Args>
        void emplace(Args&& ...args){
            _M_sequence.emplace_back(std::forward<Args>(args)...);
            _M_push_heap();
        }
        void pop() _YXXX_NOEXCEPT{
            pop_heap(_M_sequence.begin(), _M_sequence.end(), _M_comp);
            _M_sequence.pop_back();
        }
        const_reference front() const _YXXX_NOEXCEPT{
            return _M_sequence.front();
        }
        reference front() _YXXX_NOEXCEPT{
            return _M_sequence.front();
        }

        friend void swap(priority_queue& _heap1, priority_queue& _heap2) _YXXX_NOEXCEPT{
            std::swap(_heap1._M_comp, _heap2._M_comp);
            swap(_heap1._M_sequence, _heap2._M_sequence);
        }
        bool empty() const _YXXX_NOEXCEPT{return _M_sequence.empty();}
        size_type size() const _YXXX_NOEXCEPT{return _M_sequence.size();}
        size_type capacity() const _YXXX_NOEXCEPT{return _M_sequence.capacity();}
        void clear() _YXXX_NOEXCEPT{_M_sequence.clear();}
        bool full() const _YXXX_NOEXCEPT{return _M_sequence.full();}
    private:
        void _M_push_heap() _YXXX_NOEXCEPT{
            push_heap(_M_sequence.begin(), _M_sequence.end(), _M_comp);
        }
    protected:
        _Comp _M_comp;
        base_container _M_sequence;
    };
};

#endif