#ifndef _YXXX_BIT_MAP_H_
#define _YXXX_BIT_MAP_H_

#include "../allocator/allocator.h"

namespace stl{
    template <stl::size_t _Size,
        typename _Alloc = _alloc::allocator<unsigned long>>
    class bit_map{
    protected:
        using _type  = unsigned long;
    public:
        using allocator_type = _Alloc;

        bit_map(const allocator_type& _alloc = allocator_type())
            _YXXX_NOEXCEPT: _M_allocator(_alloc),
            _M_map(_M_allocator.allocate(_Size)){
            reset();
        }
        ~bit_map() _YXXX_NOEXCEPT{
            _M_allocator.deallocate(_M_map);
        }

        bool operator[](stl::size_t _pos){
            auto _loc = _M_locate(_pos);
            _type _tmp = _M_map[_loc.first];
            return (_tmp >> _loc.second) & 1;
        }
        void set() _YXXX_NOEXCEPT{
            for(stl::size_t _n = 0; _n < _Size; ++_n)
                _M_map[_n] = ~_type(0);
        }
        void set(stl::size_t _pos, bool _val = true){
            auto _loc = _M_locate(_pos);
            if(_val)
                _M_map[_loc.first] |= (1 << _loc.second);
            else
                _M_map[_loc.first] &= ~(1 << _loc.second);
        }
        void reset() _YXXX_NOEXCEPT{
            for(stl::size_t _n = 0; _n < _Size; ++_n)
                _M_map[_n] = _type(0);
        }
        void reset(stl::size_t _pos){
            set(_pos, false);
        }
        void flip() _YXXX_NOEXCEPT{
            for(stl::size_t _n; _n < _Size; ++_n)
                _M_map[_n] = ~_M_map[_n];
        }
        void flip(stl::size_t _pos){
            auto _loc = _M_locate(_pos);
            _M_map[_loc.first] ^= (1 << _loc.second);
        }
        _YXXX_CONSTEXPR stl::size_t max_bit() const _YXXX_NOEXCEPT{
            return 8 * sizeof(_type) * _Size;
        }

        friend bit_map operator&(
            const bit_map& _l, const bit_map& _r) _YXXX_NOEXCEPT{
            bit_map _res;
            for(stl::size_t _n; _n < _Size; ++_n)
                _res = _l._M_map[_n] & _l._M_map[_n];
        }
        friend bit_map operator|(
            const bit_map& _l, const bit_map& _r) _YXXX_NOEXCEPT{
            bit_map _res;
            for(stl::size_t _n; _n < _Size; ++_n)
                _res = _l._M_map[_n] | _l._M_map[_n];
        }
        friend bit_map operator~(const bit_map& _l) _YXXX_NOEXCEPT{
            bit_map _res;
            for(stl::size_t _n; _n < _Size; ++_n)
                _res = ~(_l._M_map[_n]);
        }
        friend bit_map operator^(
            const bit_map& _l, const bit_map& _r) _YXXX_NOEXCEPT{
            bit_map _res;
            for(stl::size_t _n; _n < _Size; ++_n)
                _res = _l._M_map[_n] ^ _l._M_map[_n];
        }
        friend bit_map operator&=(
            const bit_map& _l, const bit_map& _r) _YXXX_NOEXCEPT{
            for(stl::size_t _n; _n < _Size; ++_n)
                _l._M_map[_n] &= _l._M_map[_n];
        }
        friend bit_map operator|=(
            const bit_map& _l, const bit_map& _r) _YXXX_NOEXCEPT{
            for(stl::size_t _n; _n < _Size; ++_n)
                _l._M_map[_n] |= _l._M_map[_n];
        }
        friend bit_map operator^=(
            const bit_map& _l, const bit_map& _r) _YXXX_NOEXCEPT{
            for(stl::size_t _n; _n < _Size; ++_n)
                _l._M_map[_n] ^= _l._M_map[_n];
        }
        friend void swap(bit_map& _l, bit_map& _r) _YXXX_NOEXCEPT{
            std::swap(_l._M_map, _r._M_map);
            std::swap(_l._M_allocator, _r._M_allocator);
        }

    private:
        std::pair<stl::size_t, stl::size_t>
        _M_locate(stl::size_t _pos) const{
            _YXXX_CONSTEXPR stl::size_t _byte_size = 8 * sizeof(_type);
            stl::size_t _idx = _pos / _byte_size;
            if(_Size <= _idx) throw std::out_of_range("invalid position.");
            return {_idx, _pos % _byte_size};
        }
    protected:
        allocator_type _M_allocator;
        _type* _M_map;
    };
};

#endif