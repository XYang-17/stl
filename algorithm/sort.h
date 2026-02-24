#ifndef _YXXX_SORT_H_
#define _YXXX_SORT_H_

#include <type_traits>
#include <algorithm>
#include "../container/heap.h"

/* 排序算法(从小到大排序为例) */
/* 冒泡排序：遍历未排序区间，相邻元素两两比较，更大者向后移动
     O(n)->O(n^2) O(1) 稳定(相同元素的相对顺序保持不变) */
/* 选择排序：遍历未排序区间，搜索最小值，移动未排序区间首位
     O(n^2)->O(n^2) O(1) 不稳定 */
/* 插入排序：遍历元素，将其插入已排序区间
     O(n)->O(n^2) O(1) 稳定
     当数据趋于有序，插入排序效率最高 */
/* 希尔排序：交叉分组，组内插入排序，逐步减少组数直到归为一组
     O(n)->O(n^2) O(1) 不稳定
     利用插入排序在趋于有序时的高效加速排序过程 */
/* 快速排序：两端依次向中遍历元素，根据相对于基准数的大小调整元素位置
     O(nlogn)->O(n^2) O(logn)->O(n)(递归的栈内存开销) 不稳定
     后期元素趋于有序，转为插入排序
     取首中尾三数的中位数为基准数，尽量使左右分支平衡 */
/* 归并排序：元素递归分组，有序合并至一组
      O(nlogn) O(n)(临时内存空间开销) 稳定 */
/* 堆排序：使用堆的操作处理元素，在未排序区间维护大顶堆
     1.从后往前依次处理所有非叶子节点，上浮最大元素，同时维护大顶堆；
     2.堆顶元素与未排序区间末尾元素交换，更新已排序区间；
     3.下沉堆顶元素(同时会上浮当前未排序区间的最大元素)；
     4.重复步骤2，3，直到排序完毕。
     O(nlogn) O(1) 不稳定 */
/* 基数排序(桶排序)：根据元素基数设置等量的桶，从低位至高位，每轮将元素依次放入对应桶内，再按序取出
     O(nd) O(n) 稳定
     实际使用的限制较大，如小数、负数等都需要特殊调整 */
/* 注意：
     快速排序和归并排序按顺序遍历元素，有利于CPU数据访问，因而快于堆排序
       此外，堆排序过程中执行了大量无效的下沉操作
     仅有归并排序能够执行外排序(内存无法容纳全部元素，每次只能加载部分元素)
       将数据拆分，每个部分独立排序后，按序重新放回原位置 */
namespace stl{
    template <typename _BiIter, typename _UnaryPred>
    _BiIter partition(_BiIter _begin, _BiIter _end, const _UnaryPred& _pred){
        auto _tmp = std::move(*_begin);
        while(_begin < _end){
            while(_begin < _end && !_pred(*_end)) --_end;
            if(_begin != _end) *_begin = std::move(*_end);
            while(_begin < _end && _pred(*_begin)) ++_begin;
            if(_begin != _end) *_end = std::move(*_begin);
        }
        *_begin = std::move(_tmp);
        if(_pred(*_begin)) ++_begin;
        return _begin;
    }
    
    template <typename  _BiIter, typename _Comp>
    void bubble_sort(_BiIter _begin, _BiIter _end, _Comp&& _comp){
        _BiIter _it = _end, _nxt = _end;
        bool flag = true;
        for(_BiIter _it_end=_end; _it_end != _begin; --_it_end){
            flag = true;
            _it = _begin;
            _nxt = ++_BiIter{_it};
            while(_it != _it_end){
                if(!_comp(*_it, *_nxt)){
                    std::swap(*_it, *_nxt);
                    flag = false;
                }
                ++_it;
                ++_nxt;
            }
            if(flag) return; // 此轮无交换，提前结束退出
        }
    }
    template <typename  _ForwardIter, typename _Comp>
    void selection_sort(_ForwardIter _begin, _ForwardIter _end, _Comp&& _comp){
        _ForwardIter _min = _end, _nxt = ++_ForwardIter{_begin};
        for(_ForwardIter _it_begin = _begin; _nxt != _end; _it_begin = _nxt++){
            _min = _it_begin;
            for(_ForwardIter _it = _it_begin; _it != _end; ++_it)
                if(_comp(*_it, *_min)) _min = _it;
            if(_min != _it_begin)
                std::swap(*_it_begin, *_min);
        }
    }
    template <typename _BiIter, typename Comp>
    void insertion_sort(_BiIter _begin, _BiIter _end, Comp&& _comp){
        _BiIter loc = _end;
        for(_BiIter cur=++_BiIter{_begin}; cur != _end; ++cur){
            loc  = cur;
            while(loc != _begin && _comp(*cur, *(--_BiIter{loc}))) --loc;
            if(loc != cur){
                // 将元素插入已排序区间，rotate算法对不同容器的迭代器进行了优化
                // 对于基于链表的容器，只需修改区间端点的next指针
                std::rotate(loc, cur, cur+1);
            }
        }
    }
    template <typename _RandomIter, typename Comp>
    void shell_sort(_RandomIter _begin, _RandomIter _end, Comp&& _comp){
        using index_type = typename _RandomIter::difference_type;
        index_type loc = 0, size = _end - _begin;
        _RandomIter _cur = _end;
        // 分组
        for(index_type groups=size>>1; groups > 0; groups>>=1){
            // 组内插入排序
            for(index_type cur=groups; cur < size; ++cur){
                loc = cur;
                _cur = _begin+loc;
                while(loc >= groups && _comp(*_cur, *(_cur-groups))){
                    std::swap(*_cur, *(_cur-groups));
                    loc -= groups;
                }
            }
        }
    }
    template <typename _RandomIter, typename Comp>
    
    void quick_sort(_RandomIter _begin, _RandomIter _end, Comp&& _comp){
        using index_type = typename _RandomIter::difference_type;
        using value_type = typename std::__detail::__first_t<decltype(*_begin)>;
        index_type size = _end - _begin;
        if(size <= 1) return;
        if(size < 50){
            stl::insertion_sort(_begin, _end, std::forward<Comp>(_comp));
            return;
        }

        _RandomIter left=_begin, right=_end-1, mid = _begin + (size >> 1);
        {
            _RandomIter max = left, min = right;
            if(*max < *min) std::swap(max, min);
            if(*mid > *max){
                if(max!=left) std::swap(*left, *right);
            }
            else if(*mid < *min){
                if(min!=left) std::swap(*left, *right);
            }
            else{
                std::swap(*left, *mid);
            }
        }
        auto base = *_begin;
        left = stl::partition(left, right,
            [&_comp, &base](const value_type& _v){return _comp(_v, base);});

        stl::quick_sort(_begin, left, std::forward<Comp>(_comp));
        stl::quick_sort(left+1, _end, std::forward<Comp>(_comp));
    }
    template <typename _RandomIter, typename Comp>
    void merge_sort(_RandomIter _begin, _RandomIter _end, Comp&& _comp){
        using indx_type = typename _RandomIter::difference_type;
        indx_type size = _end - _begin;
        if(size <= 1) return;
        if(size == 2){
            if(!_comp(*_begin, *(_begin+1))) std::swap(*_begin, *(_begin+1));
            return;
        }
        indx_type half_size = size >> 1;
        stl::merge_sort(_begin, _begin+half_size, std::forward<Comp>(_comp));
        stl::merge_sort(_begin+half_size, _end, std::forward<Comp>(_comp));

        _RandomIter mid = _begin + half_size;
        // // 后段整体小于前段
        // if(_comp(*(_end-1), *_begin)){
        //     std::rotate(_begin, mid, _end);
        //     return;
        // }
        // // 后段尾元素 与 前段首元素 均不小于对方
        // if(!_comp(*_begin, *(_end-1))){
        //     int size_in_first = 0, size_in_second = 0;
        //     Iter loc = _begin;
        //     while(loc!=mid && !_comp(*(_end-1), *loc)){++loc;++size_in_first;}
        //     loc = _end;
        //     while(loc!=mid && !_comp(*loc, *_begin)){--loc;--size_in_second;}

        //     std::rotate(_begin, mid, _end);
        //     loc = _begin + (_end - mid);
        //     std::rotate(loc-size_in_second, loc, loc + size_in_first);
        //     return;
        // }

        // 合并
        // 常规通用做法应是开辟临时空间，复制到临时空间后，依次有序复制到原空间/目标地址
        _RandomIter loc = _begin;
        for(_RandomIter cur=mid; cur < _end; ++cur){
            if(!_comp(*cur, *(cur-1))) return; // 当*cur不小于*(cur-1)，直接退出
            while(loc != cur && !_comp(*cur, *loc)) ++loc;
            std::rotate(loc, cur, cur+1);
            ++loc;
        }
    }
    template<typename _RandomIter, typename _Comp>
    void heap_sort(_RandomIter _begin, _RandomIter _end, _Comp&& _comp){
        stl::make_heap(_begin, _end, _comp);
        for(_RandomIter _heap_end = _end; _heap_end > _begin; --_heap_end)
            stl::pop_heap(_begin, _heap_end, _comp);
    }
};

#endif