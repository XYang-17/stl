#ifndef _YXXX_FUNCTION_H_
#define _YXXX_FUNCTION_H_

#include <bits/c++config.h>
#include "adaptive_c++config.h"

namespace stl{
    template <typename _Tp, typename _Result>
    class unary_function{};
    template <typename _Tp, typename _Up, typename _Result>
    class binary_function{};
    template <typename _Tp>
    class unary_prediction:
        public unary_function<_Tp, bool>{};
    template <typename _Tp, typename _Up>
    class binary_prediction:
        public binary_function<_Tp, _Up, bool>{};

    template <typename _Tp>
    class plus: binary_function<_Tp, _Tp, _Tp>{
    public:
        _Tp operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x + _y;
        }
    };
    template <typename _Tp>
    class minus: public binary_function<_Tp, _Tp, _Tp>{
    public:
        _Tp operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x - _y;
        }
    };
    template <typename _Tp>
    class multiplies: public binary_function<_Tp, _Tp, _Tp>{
    public:
        _Tp operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x * _y;
        }
    };
    template <typename _Tp>
    class divides: public binary_function<_Tp, _Tp, _Tp>{
    public:
        _Tp operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x / _y;
        }
    };
    template <typename _Tp>
    class modulus: public binary_function<_Tp, _Tp, _Tp>{
    public:
        _Tp operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x % _y;
        }
    };
    template <typename _Tp>
    class negate: public unary_function<_Tp, _Tp>{
    public:
        _Tp operator()(const _Tp& _x) const{
            return -_x;
        }
    };

    template <typename _Tp>
    class equal_to: public binary_prediction<_Tp, _Tp>{
    public:
        bool operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x == _y;
        }
    };
    template <typename _Tp>
    class not_equal_to: public binary_prediction<_Tp, _Tp>{
    public:
        bool operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x != _y;
        }
    };
    template <typename _Tp>
    class less_than: public binary_prediction<_Tp, _Tp>{
    public:
        bool operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x < _y;
        }
    };
    template <typename _Tp>
    class less_equal: public binary_prediction<_Tp, _Tp>{
    public:
        bool operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x <= _y;
        }
    };
    template <typename _Tp>
    class greater_than: public binary_prediction<_Tp, _Tp>{
    public:
        bool operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x > _y;
        }
    };
    template <typename _Tp>
    class greater_equal: public binary_prediction<_Tp, _Tp>{
    public:
        bool operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x >= _y;
        }
    };

    template <typename _Tp>
    class logical_and: public binary_function<_Tp, _Tp, _Tp>{
    public:
        _Tp operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x && _y;
        } 
    };
    template <typename _Tp>
    class logical_or: public binary_function<_Tp, _Tp, _Tp>{
    public:
        _Tp operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x || _y;
        } 
    };
    template <typename _Tp>
    class logical_not: public unary_function<_Tp, _Tp>{
    public:
        _Tp operator()(const _Tp& _x)
        const{
            return !_x;
        } 
    };

    template <typename _Tp>
    class bit_and: public binary_function<_Tp, _Tp, _Tp>{
    public:
        _Tp operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x & _y;
        } 
    };
    template <typename _Tp>
    class bit_or: public binary_function<_Tp, _Tp, _Tp>{
    public:
        _Tp operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x | _y;
        } 
    };
    template <typename _Tp>
    class bit_not: public unary_function<_Tp, _Tp>{
    public:
        _Tp operator()(const _Tp& _x)
        const{
            return ~_x;
        } 
    };
    template <typename _Tp>
    class bit_xor: public binary_function<_Tp, _Tp, _Tp>{
    public:
        _Tp operator()(const _Tp& _x, const _Tp& _y)
        const{
            return _x ^ _y;
        } 
    };
};
namespace stl{
    struct _first_arg{
        template <typename _Tp> auto
        operator()(_Tp&& _x){
            return std::forward<_Tp>(_x).first;
        }
    };

    struct _self{
        template <typename _Tp> _Tp&&
        operator()(_Tp&& _x) const _YXXX_NOEXCEPT{
            return std::forward<_Tp>(_x);
        }
    };
};

namespace stl{
    bool is_prime(std::size_t _n) _YXXX_NOEXCEPT{
        if(_n <= 1) return false;
        if(_n <= 3) return true;
        if(!(_n & 1) || 0 == _n % 3) return false;
        
        for(std::size_t i = 5; i * i <= _n; i += 6) {
            if (_n % i == 0 || _n % (i + 2) == 0) {
                return false;
            }
        }
        return true;
    }
    std::size_t min_prime(std::size_t _n)
    _YXXX_NOEXCEPT{
        while(!stl::is_prime(_n)) ++_n;
        return _n;
    }
};

#endif