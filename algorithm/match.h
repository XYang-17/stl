#ifndef _YXXX_MATCH_H_
#define _YXXX_MATCH_H_

#include "../container/array.h"

/* 搜索/匹配 */
namespace stl{
    template <typename _ForwardIter, typename _Equal>
    inline bool range_equal(_ForwardIter _begin_1, _ForwardIter _end_1,
        _ForwardIter _begin_2, const _Equal& _equal){
        while(_begin_1 != _end_1){
            if(!_equal(*_begin_1, *_begin_2)) return false;
            ++_begin_1; ++_begin_2;
        }
        return true;
    }

    template <typename _RandomIter, typename _ValueType, typename _Comp>
    _RandomIter find(_RandomIter _begin, _RandomIter _end,
        const _ValueType& target, const _Comp& _comp){
        if(_end == _begin) return _end;
        if(_comp(target, *_begin) or _comp(*(_end-1), target)) return _end;
        if(!_comp(*_begin, target)) return _begin;
        if(!_comp(target, *(_end-1))) return _end-1;

        _RandomIter _left = _begin, _right = _end - 1;
        _RandomIter _it = _end;
        while(_left < _right){
            _it = _left + ((_right - _left)>>1);
            if(_comp(*_it, target)) _left = _it;
            else if(_comp(target, *_it)) _right = _it;
            else return _it;
        }
        return _end;
    }
    template <typename _RandomIter, typename _UnaryPred>
    _RandomIter partition_find(_RandomIter _begin,
        _RandomIter _end, const _UnaryPred& _pred){
        if(_end == _begin) return _end;
        if(_pred(_begin)) return _begin;
        if(!_pred(_end-1)) return _end;

        _RandomIter _left = _begin, _right = _end - 1;
        _RandomIter _it = _end;
        while(_left < _right){
            _it = _left + ((_right - _left) >> 1);
            if(_pred(_it)) _right = _it;
            else _left = _it;
        }
        return _right;
    }

    /* 创建最大相同前后缀数组 */
    template <typename _BiIter, typename _Equal>
    array<_BiIter> _next_array(
        _BiIter _begin, _BiIter _end, const _Equal& _equal){
        /* array[0]为_end，第一个元素匹配失败时，检测到特殊值
           array[i]表示在第i个元素匹配失败后，迭代器跳转至该位置继续 */
        array<_BiIter> _array{_end};
        if(_begin != _end) _array.push_back(_begin);
        for(_BiIter _cur = ++_BiIter{_begin}, _nxt = ++_BiIter{_cur};
            _nxt != _end; _cur = _nxt++){
            _BiIter _prefix_end = _cur, _suffix_begin = _begin;
            while(_prefix_end != _begin && !range_equal(
                    _begin, _prefix_end, _suffix_begin, _equal)){
                --_prefix_end;
                ++_suffix_begin;
            }
            _array.push_back(_prefix_end);
        }
        return _array;
    }
    
    template <typename _RandomIter1, typename _RandomIter2, typename _Equal>
    _RandomIter1 find(_RandomIter1 _begin, _RandomIter1 _end,
        _RandomIter2 _pattern_begin, _RandomIter2 _pattern_end,
        const _Equal& _equal){
        array<_RandomIter2> _array = _next_array(
            _pattern_begin, _pattern_end, _equal);
        _RandomIter2 _pattern_it = _pattern_begin;
        while(_begin < _end && _pattern_it < _pattern_end){
            if(_equal(*_begin, *_pattern_it)){
                ++_begin;
                ++_pattern_it;
            }
            else{
                _RandomIter2 _tmp = _array[_pattern_it - _pattern_begin];
                if(_pattern_end == _tmp)
                    ++_begin;
                else
                    _pattern_it = _tmp;
            }
        }
        return _pattern_it == _pattern_end ?
            _begin - (_pattern_end - _pattern_begin) :
            _end;
    }
};

#endif