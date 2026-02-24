 #ifndef _YXXX_HEAP_H_
 #define _YXXX_HEAP_H_

#include <type_traits>
#include "../iterator.h"

namespace stl{
    namespace _heap{
        template <typename _RandomAccessIterator, typename _Comp>
        typename std::enable_if_t<
            std::is_base_of<
                stl::random_access_iterator_tag,
                typename stl::iterator_traits<_RandomAccessIterator>::iterator_category
            >::value,
            _RandomAccessIterator
        >
        _adjust_heap(_RandomAccessIterator _begin, _RandomAccessIterator _iter,
                _RandomAccessIterator _end, const _Comp& _comp){
            _RandomAccessIterator comp_iter = _iter + (_iter - _begin) + 1;
            if(comp_iter >= _end) return _end;
            if(comp_iter + 1 < _end && _comp(*(comp_iter+1), *comp_iter)) ++comp_iter;
            if(_comp(*comp_iter, *_iter)) std::swap(*_iter, *comp_iter);

            while(comp_iter < _end){
                comp_iter = _adjust_heap(_begin, comp_iter, _end, _comp);
            }
            return comp_iter;
        }

        template <typename _RandomAccessIterator, typename _Comp>
        std::enable_if_t<
            std::is_base_of<
                stl::random_access_iterator_tag,
                typename stl::iterator_traits<_RandomAccessIterator>::iterator_category
            >::value,
            void
        >
        inline make_heap(_RandomAccessIterator _begin, _RandomAccessIterator _end, const _Comp& _comp){
            if(_end - _begin < 2) return;
            _RandomAccessIterator _iter = _begin + ((_end - _begin - 1) >> 1);
            while(_iter >= _begin){
                _adjust_heap(_begin, _iter, _end, _comp);
                --_iter;
            }
        }

        template <typename _RandomAccessIterator, typename _Comp>
        std::enable_if_t<
            std::is_base_of<
                stl::random_access_iterator_tag,
                typename stl::iterator_traits<_RandomAccessIterator>::iterator_category
            >::value,
            void
        >
        inline push_heap(_RandomAccessIterator _begin, _RandomAccessIterator _end, const _Comp& _comp){
            if(_end - _begin < 2) return;
            typename _RandomAccessIterator::difference_type _offset;
            _RandomAccessIterator _iter = _end-1, comp_iter;
            while(_iter > _begin){
                _offset = (_iter - _begin - 1) >> 1;
                comp_iter = _begin + _offset;
                if(!_comp(*comp_iter, *_iter)) break;
                std::swap(*_iter, *comp_iter);
                _iter = comp_iter;
            }
        }

        
        template <typename _RandomAccessIterator, typename _Comp>
        std::enable_if_t<
            std::is_base_of<
                random_access_iterator_tag,
                typename iterator_traits<_RandomAccessIterator>::iterator_category
            >::value,
            void
        >
        inline pop_heap(_RandomAccessIterator _begin, _RandomAccessIterator _end, const _Comp& _comp){
            std::swap(*_begin, *(_end-1));
            _adjust_heap(_begin, _begin, _end-1, _comp);
        }

        template <typename _RandomAccessIterator, typename _Comp>
        std::enable_if_t<
            std::is_base_of<
                random_access_iterator_tag,
                typename iterator_traits<_RandomAccessIterator>::iterator_category
            >::value,
            bool
        >
        inline is_heap(_RandomAccessIterator _begin, _RandomAccessIterator _end, const _Comp& _comp){
            _RandomAccessIterator _iter = _begin;
            _RandomAccessIterator _over = _begin + ((_end - _begin - 1) >> 1);
            _RandomAccessIterator comp_iter = _begin;
            while(true){
                comp_iter = _iter + (_iter - _begin) + 1;
                if(comp_iter < _end){
                    if(!(_comp(*_iter, *comp_iter) 
                        || !_comp(*comp_iter, *_iter)))
                        return false;
                }
                if(comp_iter+1 < _end){
                    if(!(_comp(*_iter, *(comp_iter+1)) 
                        || !_comp(*(comp_iter+1), *_iter)))
                        return false;
                }
                ++_iter;
                if(_iter >= _over) return true;
            }
        }
    };
    using _heap::make_heap;
    using _heap::push_heap;
    using _heap::pop_heap;
};

#endif