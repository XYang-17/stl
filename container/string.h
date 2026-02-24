#ifndef _YXXX_STRING_H_
#define _YXXX_STRING_H_

#include "_container_base.h"

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
        assign(char_type* _s, std::size_t _n, const char_type& _c){
            for(std::size_t _i = 0; _i < _n; ++_i)
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
        compare(const char_type* _s1, const char_type* _s2, std::size_t _n){
            for(int _i = 0; _i < _n; ++_i){
                if(lt(_s1[_i], _s2[_i])) return -1;
                else if(lt(_s2[_i], _s1[_i])) return 1;
            }
            return 0;
        }
        static _YXXX_CONSTEXPR std::size_t
        length(const char_type* _s){
            char_type null_char{};
            std::size_t _l = 0;
            while(!eq(_s[_l], null_char))
                ++_l;
            return _l;
        }
        static _YXXX_CONSTEXPR const char_type*
        find(const char_type* _s, std::size_t _n, const char_type& _t){
            for(std::size_t _i = 0; _i < _n; ++_i)
                if(eq(_s[_i], _t)) return _s + _i;
            return 0;
        }
        static _YXXX_CONSTEXPR char_type* 
        copy(char_type* _s1, const char_type* _s2, std::size_t _n){
            if(_n) memcpy(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static _YXXX_CONSTEXPR char_type*
        move(char_type* _s1, const char_type* _s2, std::size_t _n){
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
        assign(char_type* _s, std::size_t _n, const char_type& _c)
        _YXXX_NOEXCEPT{
            for(std::size_t _i = 0; _i < _n; ++_i)
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
        compare(const char_type* _s1, const char_type* _s2, std::size_t _n)
        _YXXX_NOEXCEPT{
            for(int _i = 0; _i < _n; ++_i){
                if(lt(_s1[_i], _s2[_i])) return -1;
                else if(lt(_s2[_i], _s1[_i])) return 1;
            }
            return 0;
        }
        static _YXXX_CONSTEXPR std::size_t
        length(const char_type* _s) _YXXX_NOEXCEPT{
            char_type null_char{};
            std::size_t _l = 0;
            while(!eq(_s[_l], null_char))
                ++_l;
            return _l;
        }
        static _YXXX_CONSTEXPR const char_type*
        find(const char_type* _s, std::size_t _n, const char_type& _t)
        _YXXX_NOEXCEPT{
            for(std::size_t _i = 0; _i < _n; ++_i)
                if(eq(_s[_i], _t)) return _s + _i;
            return 0;
        }
        static _YXXX_CONSTEXPR char_type* 
        copy(char_type* _s1, const char_type* _s2, std::size_t _n)
        _YXXX_NOEXCEPT{
            if(_n) memcpy(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static _YXXX_CONSTEXPR char_type*
        move(char_type* _s1, const char_type* _s2, std::size_t _n)
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
        assign(char_type* _s, std::size_t _n, const char_type& _c){
            for(std::size_t _i = 0; _i < _n; ++_i)
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
        compare(const char_type* _s1, const char_type* _s2, std::size_t _n){
            for(int _i = 0; _i < _n; ++_i){
                if(lt(_s1[_i], _s2[_i])) return -1;
                else if(lt(_s2[_i], _s1[_i])) return 1;
            }
            return 0;
        }
        static _YXXX_CONSTEXPR std::size_t
        length(const char_type* _s){
            char_type null_char{};
            std::size_t _l = 0;
            while(!eq(_s[_l], null_char))
                ++_l;
            return _l;
        }
        static _YXXX_CONSTEXPR const char_type*
        find(const char_type* _s, std::size_t _n, const char_type& _t){
            for(std::size_t _i = 0; _i < _n; ++_i)
                if(eq(_s[_i], _t)) return _s + _i;
            return 0;
        }
        static _YXXX_CONSTEXPR char_type* 
        copy(char_type* _s1, const char_type* _s2, std::size_t _n){
            if(_n) memcpy(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static _YXXX_CONSTEXPR char_type*
        move(char_type* _s1, const char_type* _s2, std::size_t _n){
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

        static const size_type npos = static_cast<size_type>(-1);

        _string(const alloctor_type& _alloc = alloctor_type{})
        _YXXX_NOEXCEPT: _M_alloc(_alloc), _M_begin(nullptr), _M_size(0){}
        _string(const char_type* _s, size_type _n = npos,
            const alloctor_type& _alloc = alloctor_type{}):
            _string(_alloc){}
        _string(const _string& _str){}
        _string(const _string& _str,
            difference_type _begin = 0, size_type _n = npos){}
        _string(_string&& _str){}
        _string(std::initializer_list<char_type>&& _il){}
        template <typename _InputIter>
        _string(_InputIter _begin, _InputIter _end){}
        ~_string() _YXXX_NOEXCEPT{}
    protected:
        alloctor_type _M_alloc;
        char_type* _M_begin;
        size_type _M_size;
    };

    template <typename _CharType = char,
        typename _Traits = _char_traits::char_traits<_CharType>>
    using string = _string<_CharType, _Traits, _alloc::allocator<_CharType>>;
};

#endif