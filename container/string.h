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

        static const size_type npos = static_cast<size_type>(-1);

        _string() _YXXX_NOEXCEPT:
            _M_allocator(alloctor_type{}), _M_begin(nullptr),
            _M_size(0), _M_capacity(0){}
        _string(const char_type* _s, size_type _n = npos):
            _string(){
            _M_copy_or_assign(_s, _n);
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
            const alloctor_type& _alloc):
            _string(_alloc){
            _M_copy_or_assign(_s, _n);
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
        void push_back(const value_type& _c){}
        void insert(){}
        void replace(){}
        void append(){}
        void erase(){}
        void pop_back(){}
        void clear(){}
        friend swap(_strint& _str1, _string& _str2)
            _YXXX_NOEXCEPT{
            std::swap(_str1._M_allocator, _str2._M_allocator);
            std::swap(_str1._M_begin, _str2._M_begin);
            std::swap(_str1._M_size, _str2._M_size);
            std::swap(_str1._M_capacity, _str2._M_capacity);
        }
        void compare(){}

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

    protected:
        void _M_copy_or_assign(const char_type* _s, size_type _n){
            size_type _len = _n;
            if(npos == _n){
                _YXXX_CONSTEXPR char_type null_char;
                _len = 0;
                while(_len < npos){
                    if(null_char == _s[_len]) break;
                    ++_len;
                }
            }
            
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