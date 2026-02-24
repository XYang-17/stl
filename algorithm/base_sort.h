#ifndef _YXXX_BASE_SORT_H_
#define _YXXX_BASE_SORT_H_

#include <type_traits>
#include "../container/hash_map.h"
#include "../container/queue.h"

namespace stl{
    template<typename _RandomIter, typename Comp>
    void base_sort(_RandomIter _begin, _RandomIter _end, Comp&& _comp){
        using index_type = typename _RandomIter::difference_type;
        using value_type = typename std::__detail::__first_t<decltype(*_begin)>;
        
        int _base = 10;
        stl::hash_map<int, stl::queue<value_type>*> _buckets;
        for(int i = 0; i < _base; ++i){
            _buckets.emplace(i, new stl::queue<value_type>{_base});
        }

        int last_base_num = 1;
        int base_num = last_base_num * _base;
        stl::queue<value_type>* bucket = nullptr;
        _RandomIter iter = _begin;
        while(true){
            iter = _begin;
            while(iter < _end){
                _buckets[(*iter) % base_num / last_base_num]->push(*iter);
                ++iter;
            }
            if(_buckets[0]->size() == _end - _begin) break;

            iter = _begin;
            for(int i = 0; i < _base; ++i){
                bucket = _buckets[i];
                while(!bucket->empty()){
                    *iter = bucket->front();
                    bucket->pop();
                    ++iter;
                }
            }
            last_base_num = base_num;
            base_num *= _base;
        }
    }
};

#endif