#ifndef _YXXX_STRING_H_
#define _YXXX_STRING_H_

#include <memory>
#include "./_container_base.h"

namespace _char_traits{
    template <typename _CharType>
    struct _char_type{
        using int_type = unsigned long;
    };

    template <typename _CharType>
    struct char_traits{
        using char_type    = _CharType;
        using int_type     = typename _char_type<_CharType>::int_type;
    
        static _YXXX_CONSTEXPR void
        assign(char_type& _c1, const char_type& _c2){
            _c1 = _c2;
        }
        static _YXXX_CONSTEXPR void
        assign(char_type* _s, stl::size_t _n, const char_type& _c){
            for(stl::size_t _i = 0; _i < _n; ++_i)
                _s[_i] = _c;
        }
        static _YXXX_CONSTEXPR bool
        eq(const char_type& _c1, const char_type& _c2){
            return _c1 == _c2;
        }
        static _YXXX_CONSTEXPR bool
        eq(const int_type& _a1, const int_type& _a2){
            return _a1 == _a2;
        }
        static _YXXX_CONSTEXPR bool
        lt(const char_type& _c1, const char_type& _c2){
            return _c1 < _c2;
        }
        static _YXXX_CONSTEXPR int
        compare(const char_type* _s1, const char_type* _s2, stl::size_t _n){
            for(int _i = 0; _i < _n; ++_i){
                if(lt(_s1[_i], _s2[_i])) return -1;
                else if(lt(_s2[_i], _s1[_i])) return 1;
            }
            return 0;
        }
        static _YXXX_CONSTEXPR stl::size_t
        length(const char_type* _s){
            char_type null_char{};
            stl::size_t _l = 0;
            while(!eq(_s[_l], null_char))
                ++_l;
            return _l;
        }
        static _YXXX_CONSTEXPR const char_type*
        find(const char_type* _s, stl::size_t _n, const char_type& _t){
            for(stl::size_t _i = 0; _i < _n; ++_i)
                if(eq(_s[_i], _t)) return _s + _i;
            return 0;
        }
        static _YXXX_CONSTEXPR char_type* 
        copy(char_type* _s1, const char_type* _s2, stl::size_t _n){
            if(_n) memcpy(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static _YXXX_CONSTEXPR char_type*
        move(char_type* _s1, const char_type* _s2, stl::size_t _n){
            if(_n) memmove(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static _YXXX_CONSTEXPR char_type
        to_char(const int_type& _a){
            return static_cast<char_type>(_a);
        }
        static _YXXX_CONSTEXPR int_type
        to_int(const char_type& _c){
            return static_cast<int_type>(_c);
        }
    };
    template <>
    struct char_traits<char>{
        using char_type    = char;
        using int_type     = int;
        
        static _YXXX_CONSTEXPR void
        assign(char_type& _c1, const char_type& _c2)
        _YXXX_NOEXCEPT{
            _c1 = _c2;
        }
        static _YXXX_CONSTEXPR void
        assign(char_type* _s, stl::size_t _n, const char_type& _c)
        _YXXX_NOEXCEPT{
            for(stl::size_t _i = 0; _i < _n; ++_i)
                _s[_i] = _c;
        }
        static _YXXX_CONSTEXPR bool
        eq(const char_type& _c1, const char_type& _c2)
        _YXXX_NOEXCEPT{
            return _c1 == _c2;
        }
        static _YXXX_CONSTEXPR bool
        eq(const int_type& _a1, const int_type& _a2)
        _YXXX_NOEXCEPT{
            return _a1 == _a2;
        }
        static _YXXX_CONSTEXPR bool
        lt(const char_type& _c1, const char_type& _c2)
        _YXXX_NOEXCEPT{
            return static_cast<unsigned char>(_c1)
                < static_cast<unsigned char>(_c2);
        }
        static _YXXX_CONSTEXPR int
        compare(const char_type* _s1, const char_type* _s2, stl::size_t _n)
        _YXXX_NOEXCEPT{
            for(int _i = 0; _i < _n; ++_i){
                if(lt(_s1[_i], _s2[_i])) return -1;
                else if(lt(_s2[_i], _s1[_i])) return 1;
            }
            return 0;
        }
        static _YXXX_CONSTEXPR stl::size_t
        length(const char_type* _s) _YXXX_NOEXCEPT{
            char_type null_char{};
            stl::size_t _l = 0;
            while(!eq(_s[_l], null_char))
                ++_l;
            return _l;
        }
        static _YXXX_CONSTEXPR const char_type*
        find(const char_type* _s, stl::size_t _n, const char_type& _t)
        _YXXX_NOEXCEPT{
            for(stl::size_t _i = 0; _i < _n; ++_i)
                if(eq(_s[_i], _t)) return _s + _i;
            return 0;
        }
        static _YXXX_CONSTEXPR char_type* 
        copy(char_type* _s1, const char_type* _s2, stl::size_t _n)
        _YXXX_NOEXCEPT{
            if(_n) memcpy(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static _YXXX_CONSTEXPR char_type*
        move(char_type* _s1, const char_type* _s2, stl::size_t _n)
        _YXXX_NOEXCEPT{
            if(_n) memmove(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static _YXXX_CONSTEXPR char_type
        to_char(const int_type& _a) _YXXX_NOEXCEPT{
            return static_cast<char_type>(_a);
        }
        static _YXXX_CONSTEXPR int_type
        to_int(const char_type& _c) _YXXX_NOEXCEPT{
            return static_cast<int_type>(static_cast<unsigned char>(_c));
        }
    };
    
    #if _GLIBCXX_USE_WCHAR_T
    template <>
    struct char_traits<wchar_t>{
        using char_type = wchar_t;
        using int_type  = wint_t;

        static _YXXX_CONSTEXPR void
        assign(char_type& _c1, const char_type& _c2){
            _c1 = _c2;
        }
        static _YXXX_CONSTEXPR void
        assign(char_type* _s, stl::size_t _n, const char_type& _c){
            for(stl::size_t _i = 0; _i < _n; ++_i)
                _s[_i] = _c;
        }
        static _YXXX_CONSTEXPR bool
        eq(const char_type& _c1, const char_type& _c2){
            return _c1 == _c2;
        }
        static _YXXX_CONSTEXPR bool
        eq(const int_type& _a1, const int_type& _a2){
            return _a1 == _a2;
        }
        static _YXXX_CONSTEXPR bool
        lt(const char_type& _c1, const char_type& _c2){
            return _c1 < _c2;
        }
        static _YXXX_CONSTEXPR int
        compare(const char_type* _s1, const char_type* _s2, stl::size_t _n){
            for(int _i = 0; _i < _n; ++_i){
                if(lt(_s1[_i], _s2[_i])) return -1;
                else if(lt(_s2[_i], _s1[_i])) return 1;
            }
            return 0;
        }
        static _YXXX_CONSTEXPR stl::size_t
        length(const char_type* _s){
            char_type null_char{};
            stl::size_t _l = 0;
            while(!eq(_s[_l], null_char))
                ++_l;
            return _l;
        }
        static _YXXX_CONSTEXPR const char_type*
        find(const char_type* _s, stl::size_t _n, const char_type& _t){
            for(stl::size_t _i = 0; _i < _n; ++_i)
                if(eq(_s[_i], _t)) return _s + _i;
            return 0;
        }
        static _YXXX_CONSTEXPR char_type* 
        copy(char_type* _s1, const char_type* _s2, stl::size_t _n){
            if(_n) memcpy(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static _YXXX_CONSTEXPR char_type*
        move(char_type* _s1, const char_type* _s2, stl::size_t _n){
            if(_n) memmove(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static _YXXX_CONSTEXPR char_type
        to_char(const int_type& _a){
            return static_cast<char_type>(_a);
        }
        static _YXXX_CONSTEXPR int_type
        to_int(const char_type& _c){
            return static_cast<int_type>(_c);
        }
    };
    #endif

    #if __cplusplus >= 201103L
    template <>
    struct char_traits<char16_t>{};
    template <>
    struct char_traits<char32_t>{};
    #endif
};
namespace stl{
    template <typename _CharType, typename _Traits,
        typename _Alloc>
    class _string{
    public:
        using char_type        = _CharType;
        using traits_type      = _Traits;
        using alloctor_type    = typename _alloc::alloc_rebind<_Alloc, _CharType>;
    protected:
        using _alloc_traits = typename _alloc::_alloc_traits<alloctor_type>;
    public:
        using value_type       = typename _alloc_traits::value_type;
        using pointer          = typename _alloc_traits::pointer;
        using const_pointer    = typename _alloc_traits::const_pointer;
        using reference        = typename _alloc_traits::reference;
        using const_reference  = typename _alloc_traits::const_reference;
        using size_type        = typename _alloc_traits::size_type;
        using difference_type  = typename _alloc_traits::difference_type;

        using iterator         = typename _iterator::normal_iterator<pointer>;
        using const_iterator   = typename _iterator::normal_iterator<const_pointer>;
        using reverse_iterator = typename _iterator::reverse_iterator<iterator>;
        using const_reverse_iterator = typename
            _iterator::reverse_iterator<const_iterator>;

        using self_type        = _string;
        using insert_return_type  = iterator;

        static const size_type npos = static_cast<size_type>(-1);

        _string() _YXXX_NOEXCEPT:
            _M_allocator(alloctor_type{}), _M_begin(nullptr),
            _M_size(0), _M_capacity(0){}
        _string(const char_type* _s, size_type _n = npos,
            const char_type& _e = char_type{}):
            _string(){
            _M_copy_or_assign(_s, _n, _e);
        }
        _string(const _string& _str):
            _string(){
            _M_copy_or_assign(_str.begin(), _str.end());
        }
        _string(const _string& _str, difference_type _begin,
            size_type _n = npos):
            _string(){
            if(_begin < _str._M_size){
                if(npos == _n || _begin + _n >= _str._M_size)
                    _M_copy_or_assign(_str.begin()+_begin, _str.end());
                else
                    _M_copy_or_assign(_str.begin()+_begin,
                        _str.begin()+_begin+_n);
            }
        }
        _string(_string&& _str):
            _string(){
            _M_move_or_assign(_str.begin(), _str.end());
        }
        _string(std::initializer_list<char_type>&& _il):
            _string(){
            _M_move_or_assign(_il.begin(), _il.end());
        }
        template <typename _InputIter>
        _string(_InputIter _begin, _InputIter _end):
            _string(){
            _M_copy_or_assign(_begin, _end);
        }

        _string(const alloctor_type& _alloc) _YXXX_NOEXCEPT:
            _M_allocator(_alloc), _M_begin(nullptr),
            _M_size(0), _M_capacity(0){}
        _string(const char_type* _s, size_type _n = npos,
            const char_type& _e = char_type{},
            const alloctor_type& _alloc):
            _string(_alloc){
            _M_copy_or_assign(_s, _n, _e);
        }
        _string(const _string& _str, const alloctor_type& _alloc):
            _string(_alloc){
            _M_copy_or_assign(_str.begin(), _str.end());
        }
        _string(const _string& _str, difference_type _begin,
            size_type _n = npos, const alloctor_type& _alloc):
            _string(_alloc){
            if(_begin < _str._M_size){
                if(npos == _n || _begin + _n >= _str._M_size)
                    _M_copy_or_assign(_str.begin()+_begin, _str.end());
                else
                    _M_copy_or_assign(_str.begin()+_begin,
                        _str.begin()+_begin+_n);
            }
        }
        _string(_string&& _str, const alloctor_type& _alloc):
            _string(_alloc){
            _M_move_or_assign(_str.begin(), _str.end());
        }
        _string(std::initializer_list<char_type>&& _il,
            const alloctor_type& _alloc):
            _string(_alloc){
            _M_move_or_assign(_il.begin(), _il.end());
        }
        template <typename _InputIter>
        _string(_InputIter _begin, _InputIter _end,
            const alloctor_type& _alloc):
            _string(_alloc){
            _M_copy_or_assign(_begin, _end);
        }

        ~_string() _YXXX_NOEXCEPT{
            _M_clear();
        };

        self_type& operator=(){}
        self_type& operator+=(){}
        friend self_type
        operator*(const _string& _str, size_type _n){}
        friend self_type
        operator*(size_type _n, const _string& _str){
            return _str * _n;
        }
        self_type& operator*=(size_type _n){}

        friend bool
        operator>(const _string& _str1, const _string& _str2){}
        friend bool
        operator>=(const _string& _str1, const _string& _str2){}
        friend bool
        operator<(const _string& _str1, const _string& _str2){}
        friend bool
        operator<=(const _string& _str1, const _string& _str2){}
        friend bool
        operator==(const _string& _str1, const _string& _str2){}
        friend bool
        operator!=(const _string& _str1, const _string& _str2){}

        void assign(){}

        insert_return_type
        push_back(const char_type& _c){
            _M_may_expand();
            _M_begin[_M_size++] = _c;
            return {_M_begin + _M_size - 1};
        }
        insert_return_type
        push_back(char_type&& _c){
            _M_may_expand();
            _M_begin[_M_size++] = std::move(_c);
            return {_M_begin + _M_size - 1};
        }
        insert_return_type
        append(const _string& _str, difference_type _index = 0, size_type _n = npos){
            _n = _str._M_len(_index, _n);
            return _M_insert(_M_size, _str._M_begin + _index, _n);
        }
        insert_return_type
        append(const char_type* _s, size_type _n = npos, const char_type& _e = char_type{}){
            _n = _M_len(_s, _n, _e);
            return _M_insert(_M_size, _s, _n);
        }
        template <typename Iter>
        insert_return_type
        append(Iter _begin, Iter _end){
            difference_type _r = difference_type(_M_size);
            while(_begin != _end) push_back(*_begin++);
            return {_M_begin + _r};
        }

        insert_return_type
        insert(difference_type _index, const char_type& _c){
            if(!valid(_index)) return end();
            return _M_insert(_index, _c);
        }
        insert_return_type
        insert(difference_type _index, const char_type& _c,
            size_type _n){
            if(!valid(_index)) return end();
            return _M_insert(_index, _c, _n);
        }
        insert_return_type
        insert(difference_type _index, char_type&& _c){
            if(!valid(_index)) return end();
            return _M_insert(_index, std::move(_c));
        }
        insert_return_type
        insert(difference_type _index,
            const char_type* _s, size_type _n = npos,
            const char_type& _e = char_type{}){
            if(!valid(_index)) return end();
            _n = _M_len(_s, _n, _e);
            return _M_insert(_index, _s, _n);
        }
        insert_return_type
        insert(difference_type _index1, const _string& _str,
            difference_type _index2, size_type _n = npos){
            if(!valid(_index1) || !_str.valid(_index2)) return end();
            _n = _str._M_len(_index2, _n);
            return _M_insert(_index1, _str._M_begin + _index2, _n);
        }
        template <typename Iter>
        insert_return_type
        insert(difference_type _index, Iter _begin, Iter _end){
            if(!valid(_index)) return end();
            difference_type _i = _index;
            while(_begin != _end){
                insert(_i++, *_begin);
            }
            return {_M_begin + _index};
        }
        insert_return_type
        insert(iterator _pos, const char_type& _c){
            if(!valid(_pos)) return end();
            return _M_insert(_pos - begin(), _c);
        }
        insert_return_type
        insert(iterator _pos, const char_type& _c, size_type _n){
            if(!valid(_pos)) return end();
            return _M_insert(_pos - begin(), _c, _n);
        }
        insert_return_type
        insert(iterator _pos, char_type&& _c){
            if(!valid(_pos)) return end();
            return _M_insert(_pos - begin(), std::move(_c));
        }
        insert_return_type
        insert(iterator _pos, const char_type* _s,
            size_type _n = npos, const char_type& _e = char_type{}){
            if(!valid(_pos)) return end();
            _n = _M_len(_s, _n, _e);
            return _M_insert(_pos - begin(), _s, _n);
        }
        insert_return_type
        insert(iterator _pos, const _string& _str,
            difference_type _index, size_type _n = npos){
            if(!valid(_pos)) return end();
            _n = _str._M_len(_index, _n);
            return _M_insert(_pos - beign(), _str._M_begin + _index, _n);
        }
        template <typename Iter>
        insert_return_type
        insert(iterator _pos, Iter _begin, Iter _end){
            if(!valid(_pos)) return end();
            return _M_insert(_pos - begin(), _begin, _end);
        }

        void pop_back() _YXXX_NOEXCEPT{
            if(!empty()) --_M_size;
        }
        bool erase(difference_type _index){
            if(!valid(_index)) return false;
            return _M_erase(_index);
        }
        bool erase(iterator _pos){
            if(!valid(_pos)) return false;
            return _M_erase(_pos - begin());
        }
        bool remove(difference_type _index,
            size_type _n = npos){
            if(!valid(_index)) return false;
            return _M_remove(_index, _n);
        }
        bool remove(iterator _begin, iterator _end = end()){
            if(!valid(_begin) || !valid_end(_end)) return false;
            return _M_remove(_begin - begin(), _end - _begin);
        }
        void clear() _YXXX_NOEXCEPT{
            _M_size = 0;
        }

        insert_return_type
        replace(difference_type _index, const char_type& _c){
            if(!valid(_index)) return end();
            return _M_replace(_M_begin + _index, _c);
        }
        insert_return_type
        replace(difference_type _index, char_type&& _c){
            if(!valid(_index)) return end();
            return _M_replace(_M_begin + _index, std::move(_c));
        }
        insert_return_type
        replace(difference_type _index, size_type _n, const char_type& _c){
            if(!valid(_index)) return end();
            if(_index + _n > _M_size) _n = _M_size - size_type(_index);
            return _M_replace(_M_begin + _index, _M_begin + _index + _n, _c);
        }
        insert_return_type
        replace(difference_type _index1, size_type _n1, const _string& _str,
            difference_type _index2 = 0, size_type _n2 = npos){
            if(!valid(_index1) || !_str.valid(_index2)) return end();
            _n1 = _M_len(_index1, _n1);
            _n2 = _str._M_len(_index2, _n2);
            return _M_replace(_index1, _n1, _str._M_begin + _index2, _n2);
        }
        insert_return_type
        replace(difference_type _index, size_type _n1,
            const char_type* _s, size_type _n2 = npos,
            const char_type& _end_char = char_type{}){
            if(!valid(_index)) return end();
            _n1 = _M_len(_index, _n1);
            _n2 = _M_len(_s, _n2, _end_char);
            return _M_replace(_index, _n1, _s, _n2);
        }
        template <typename Iter>
        replace(difference _index, size_type _n,
            Iter _begin, Iter _end){
            if(!valid(_index)) return end();
            _n = _M_len(_index, _n);
            return _M_replace(
                iterator{_M_begin + _index},
                iterator{_M_begin + _index + _n},
                _begin, _end
            );
        }
        insert_return_type
        replace(iterator _pos, const value_type& _c){
            if(!valid(_pos)) return end();
            return _M_replace(_pos.base(), _c);
        }
        insert_return_type
        replace(iterator _pos, value_type&& _c){
            if(!valid(_pos)) return end();
            return _M_replace(_pos.base(), std::move(_c));
        }
        insert_return_type
        replace(iterator _begin, iterator _end, const value_type& _c){
            if(!valid(_begin) || !valid_end(_end) || _end <= _begin) return end();
            return _M_replace(_begin.base(), _end.base(), _c);
        }
        template <typename Iter>
        insert_return_type
        replace(iterator _pos, Iter _it){
            if(!valid(_pos)) return end();
            return _M_replace(_pos.base(), *_it);
        }
        insert_return_type
        replace(iterator _begin, iterator _end, const _string& _str,
            difference_type _index = 0, size_type _n = npos){
            if(!valid(_begin) || !valid_end(_end) || _end <= _begin) return end();
            _n = _str._M_len(_index, _n);
            return _M_replace(
                difference_type(_begin - begin()),
                size_type(_end - _begin),
                _str._M_begin + _index, _n);
        }
        insert_return_type
        replace(iterator _begin, iterator _end,
            const char_type* _s, size_type _n = npos,
            const char_type& _end_char = char_type{}){
            if(!valid(_begin) || !valid_end(_end) || _end <= _begin) return end();
            _n = _M_len(_s, _n, _end_char);
            return _M_replace(_begin.base() - _M_begin, _end - _begin, _s, _n);
        }
        template <typename Iter>
        insert_return_type
        replace(iterator _begin1, iterator _end1,
            Iter _begin2, Iter _end2){
            if(!valid(_begin1) || !valid_end(_end1) || _end1 <= _begin1) return end();
            return _M_replace(_begin1, _end1, _begin2, _end2);
        }

        friend swap(_strint& _str1, _string& _str2)
            _YXXX_NOEXCEPT{
            std::swap(_str1._M_allocator, _str2._M_allocator);
            std::swap(_str1._M_begin, _str2._M_begin);
            std::swap(_str1._M_size, _str2._M_size);
            std::swap(_str1._M_capacity, _str2._M_capacity);
        }
        friend int compare(_strint& _str1, _string& _str2){
            
        }

        iterator find(){}
        iterator rfind(){}
        iterator find_first_of(){}
        iterator find_last_of(){}
        iterator find_first_not_of(){}
        iterator find_last_not_of(){}

        const char_type* c_str() const _YXXX_NOEXCEPT{}
        void copy_to(char_type* _s){}

        iterator
        begin() _YXXX_NOEXCEPT{
            return _M_begin;
        }
        const_iterator
        begin() const _YXXX_NOEXCEPT{
            return _M_begin;
        }
        iterator
        end() _YXXX_NOEXCEPT{
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
            return _M_begin + _M_size;
        }
        reverse_iterator
        rbegin() _YXXX_NOEXCEPT{
            return end();
        }
        const_reverse_iterator
        rbegin() const _YXXX_NOEXCEPT{
            return end();
        }
        reverse_iterator
        rend() _YXXX_NOEXCEPT{
            return begin();
        }
        const_reverse_iterator
        rend() const _YXXX_NOEXCEPT{
            return begin();
        }
        const_reverse_iterator
        crbegin() const _YXXX_NOEXCEPT{
            return cend();
        }
        const_reverse_iterator
        cend() const _YXXX_NOEXCEPT{
            return cbegin();
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

    protected:
        static size_type _M_len(const char_type* _s, size_type _n,
            const char_type& _end_char){
            size_type _len = 0;
            while(_len < _n){
                if(_end_char == _s[_len]) break;
                ++_len;
            }
            return _len;
        }
        size_type _M_len(difference_type _index, size_type _len){
            if(_M_size - size_type(_index) < _len) return _M_size - _index;
            return _len;
        }
        bool valid(difference_type _index)
        const _YXXX_NOEXCEPT{
            return _index < _M_size && _index >= 0;
        }
        bool valid(iterator _it)
        const _YXXX_NOEXCEPT{
            return _it >= begin() && _it < end();
        }
        bool valid_end(iterator _it)
        const _YXXX_NOEXCEPT{
            return _it >= begin() && _it <= end();
        }

        void _M_copy_or_assign(const char_type* _s, size_type _n,
            const char_type& _end_char){
            size_type _len = _M_len(_s, _n, _end_char);
            
            if(_M_size) _M_clear();
            _M_capacity = 1.5 * _len;
            _M_begin = _M_allocator->allocate(_M_capacity);
            memcpy(_M_begin, _s, _len * sizeof(char_type));
            _M_size = _len;
        }
        template <typename _InputIter>
        void _M_copy_or_assign(_InputIter _begin, _InputIter _end){
            if(_M_size) _M_clear();
            for(_InputIter _it = _begin; _it != _end; ++_it)
                push_back(*_it);
        }
        template <typename _InputIter>
        void _M_move_or_assign(_InputIter _begin, _InputIter _end){
            if(_M_size) _M_clear();
            for(_InputIter _it = _begin; _it != _end; ++_it)
                push_back(std::move(*_it));
        }
        void _M_clear() _YXXX_NOEXCEPT{
            _M_allocator->deallocate(_M_begin);
            _M_begin = nullptr;
            _M_size = _M_capacity = 0;
        }

        void _M_expand() _YXXX_NOEXCEPT{
            size_type new_capacity = _M_capacity * 1.5 + 1;
            pointer new_value = _M_allocator.allocate(new_capacity);
            memmove(new_value, _M_begin, _M_capacity * sizeof(value_type));
            _M_allocator.deallocate(_M_begin, sizeof(value_type) * _M_capacity);
             _M_begin = new_value;
            _M_capacity = new_capacity;
        }
        void _M_may_expand() _YXXX_NOEXCEPT{
            if(full()) _M_expand();
        }
        void _M_may_expand(size_type _n) _YXXX_NOEXCEPT{
            if(_M_size + _n > _M_capacity) _M_expand();
        }
        
        void _M_expand(difference_type _index1, difference_type _index2,
            size_type _expand_size, const char_type* _s = nullptr){
            size_type _new_capacity = _M_capacity + _expand_size;
            char_type* _new_begin = _M_allocator.allocate(
                _M_begin, _new_capacity);
            
            memmove(_new_begin, _M_begin,
                size_type(_index1) * sizeof(char_type));
            if(nullptr != _s)
                memcpy(_new_begin + _index1, _s,
                    (size_type(_index2 - _index1) + _expand_size) * sizeof(char_type));
            if(_M_size > _index2)
                memmove(_new_begin + _index2 + _expand_size,
                    _M_begin + _index2,
                    (_M_size - size_type(_index2)) * sizeof(char_type));

            _M_allocator.deallocate(_M_begin, _M_capacity);
            _M_begin = _new_begin;
            _M_capacity = _new_capacity;
            _M_size += _expand_size;
        }

        void _M_before_insert(difference_type _index, size_type _n = 1){
            _M_may_expand(_n);
            _M_size += _n;
            _index += difference_type(_n);
            for(difference_type i = difference_type(_M_size) - 1;
                    i >= _index;--i) 
                _M_begin[i] = std::move(_M_begin[i-difference_type(_n)]);
        }

        insert_return_type
        _M_insert(difference_type _index, const char_type& _c){
            _M_before_insert(_index);
            return _M_replace(_M_begin + _index, _c);
        }
        insert_return_type
        _M_insert(difference_type _index, const char_type& _c,
            size_type _n){
            _M_before_insert(_index, _n);
            return _M_replace(_M_begin + _index,
                _M_begin + _index + _n, _c);
        }
        insert_return_type
        _M_insert(difference_type _index, char_type&& _c){
            _M_before_insert(_index);
            return _M_replace(_M_begin + _index, std::move(_c));
        }
        insert_return_type
        _M_insert(difference_type _index,
            const char_type* _s, size_type _n){
            if(_M_capacity - _M_size < _n)
                _M_expand(_index, _index, _n, _s);
            else{
                _M_before_insert(_index, _n);
                memcpy(_M_begin + _index, _s, _n * sizeof(char_type));
            }
            return {_M_begin + _index};
        }
        template <typename Iter>
        insert_return_type
        _M_insert(difference_type _index, Iter _begin, Iter _end){
            difference_type _i = _index;
            while(_begin != _end){
                insert(_i++, *_begin);
            }
            return {_M_begin + _index};
        }
        
        bool _M_erase(difference_type _index){
            for(int i = _index;i < _M_size;){
                _M_begin[i] = _M_begin[++i];
            }
            --_M_size;
            return true;
        }
        bool _M_remove(difference_type _index, size_type _n){
            if(npos == _n | _M_size - size_type(_index) >= _n){
                _M_size = _index;
            }
            else{
                difference_type _idx = difference_type(_index + _n);
                while(_idx < _M_size){
                    _M_begin[_idx - difference_type(_n)]
                        = std::move(_M_begin[_idx]);
                    ++_idx;
                }
                _M_size = size_type(_idx) - _n + 1;
            }
            return true;
        }

        insert_return_type
        _M_replace(char_type* _pos, const char_type& _c){
            *_pos = _c;
            return _pos;
        }
        insert_return_type
        _M_replace(char_type* _pos, char_type&& _c){
            *_pos = std::move(_c);
            return _pos;
        }
        insert_return_type
        _M_replace(char_type* _begin, char_type* _end,
            const char_type& _c){
            char_type* _ptr = _begin;
            while(_ptr < _end)
                *_ptr++ = _c;
        }
        insert_return_type
        _M_replace(difference_type _index, size_type _n1,
            const char_type* _s, size_type _n2){
            if(_n1 < _n2){
                size_type _expand_size = _n2 - _n1;
                if(_M_capacity - _M_size < _expand_size){
                    _M_expand(_index, _index + difference_type(_n1), _expand_size, _s);
                }
                else{
                    _M_before_insert(_index + _n1, _expand_size);
                    memcpy(_M_begin + _index, _s, _n2 * sizeof(char_type));
                }
            }
            else{
                difference_type _i = 0;
                while(_i < _n1 && _i < _n2){
                    _M_begin[_index + _i] = _s[_i];
                    ++_i;
                }
                if(_n1 > _n2) remove(_index + _n2, _n1 - _n2);
            }
            return {_M_begin + _index};
        }
        template <typename Iter>
        insert_return_type
        _M_replace(iterator _begin1, iterator _end1,
            Iter _begin2, Iter _end2){
            iterator _it = _begin1;
            while(_it != _end1 && _begin2 != _end2){
                *_it++ = *_begin2++;
            }
            if(_begin2 != _end2)
                insert(_it - begin(), _begin2, _end2);
            else if(_it != _end1)
                remove(_it, _end1);
            return _begin1;
        }

        alloctor_type _M_allocator;
        char_type* _M_begin;
        size_type _M_size;
        size_type _M_capacity;
    };

    template <typename _CharType = char,
        typename _Traits = _char_traits::char_traits<_CharType>>
    using string = _string<_CharType, _Traits, _alloc::allocator<_CharType>>;
};

#endif