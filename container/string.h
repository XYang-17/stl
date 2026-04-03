#ifndef _YXXX_STRING_H_
#define _YXXX_STRING_H_

#include <memory>
#include <stdexcept>
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
    
        static  void
        assign(char_type& _c1, const char_type& _c2){
            _c1 = _c2;
        }
        static  void
        assign(char_type* _s, stl::size_t _n, const char_type& _c){
            for(stl::size_t _i = 0; _i < _n; ++_i)
                _s[_i] = _c;
        }
        static  bool
        eq(const char_type& _c1, const char_type& _c2){
            return _c1 == _c2;
        }
        static  bool
        eq(const int_type& _a1, const int_type& _a2){
            return _a1 == _a2;
        }
        static  bool
        lt(const char_type& _c1, const char_type& _c2){
            return _c1 < _c2;
        }
        static  int
        compare(const char_type* _s1, const char_type* _s2, stl::size_t _n){
            for(int _i = 0; _i < _n; ++_i){
                if(lt(_s1[_i], _s2[_i])) return -1;
                else if(lt(_s2[_i], _s1[_i])) return 1;
            }
            return 0;
        }
        static  stl::size_t
        length(const char_type* _s){
            char_type null_char{};
            stl::size_t _l = 0;
            while(!eq(_s[_l], null_char))
                ++_l;
            return _l;
        }
        static  const char_type*
        find(const char_type* _s, stl::size_t _n, const char_type& _t){
            for(stl::size_t _i = 0; _i < _n; ++_i)
                if(eq(_s[_i], _t)) return _s + _i;
            return 0;
        }
        static  char_type* 
        copy(char_type* _s1, const char_type* _s2, stl::size_t _n){
            if(_n) memcpy(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static  char_type*
        move(char_type* _s1, const char_type* _s2, stl::size_t _n){
            if(_n) memmove(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static  char_type
        to_char(const int_type& _a){
            return static_cast<char_type>(_a);
        }
        static  int_type
        to_int(const char_type& _c){
            return static_cast<int_type>(_c);
        }
    };
    template <>
    struct char_traits<char>{
        using char_type    = char;
        using int_type     = int;
        
        static  void
        assign(char_type& _c1, const char_type& _c2)
        _YXXX_NOEXCEPT{
            _c1 = _c2;
        }
        static  void
        assign(char_type* _s, stl::size_t _n, const char_type& _c)
        _YXXX_NOEXCEPT{
            for(stl::size_t _i = 0; _i < _n; ++_i)
                _s[_i] = _c;
        }
        static  bool
        eq(const char_type& _c1, const char_type& _c2)
        _YXXX_NOEXCEPT{
            return _c1 == _c2;
        }
        static  bool
        eq(const int_type& _a1, const int_type& _a2)
        _YXXX_NOEXCEPT{
            return _a1 == _a2;
        }
        static  bool
        lt(const char_type& _c1, const char_type& _c2)
        _YXXX_NOEXCEPT{
            return static_cast<unsigned char>(_c1)
                < static_cast<unsigned char>(_c2);
        }
        static  int
        compare(const char_type* _s1, const char_type* _s2, stl::size_t _n)
        _YXXX_NOEXCEPT{
            for(int _i = 0; _i < _n; ++_i){
                if(lt(_s1[_i], _s2[_i])) return -1;
                else if(lt(_s2[_i], _s1[_i])) return 1;
            }
            return 0;
        }
        static  stl::size_t
        length(const char_type* _s) _YXXX_NOEXCEPT{
            char_type null_char{};
            stl::size_t _l = 0;
            while(!eq(_s[_l], null_char))
                ++_l;
            return _l;
        }
        static  const char_type*
        find(const char_type* _s, stl::size_t _n, const char_type& _t)
        _YXXX_NOEXCEPT{
            for(stl::size_t _i = 0; _i < _n; ++_i)
                if(eq(_s[_i], _t)) return _s + _i;
            return 0;
        }
        static  char_type* 
        copy(char_type* _s1, const char_type* _s2, stl::size_t _n)
        _YXXX_NOEXCEPT{
            if(_n) memcpy(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static  char_type*
        move(char_type* _s1, const char_type* _s2, stl::size_t _n)
        _YXXX_NOEXCEPT{
            if(_n) memmove(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static  char_type
        to_char(const int_type& _a) _YXXX_NOEXCEPT{
            return static_cast<char_type>(_a);
        }
        static  int_type
        to_int(const char_type& _c) _YXXX_NOEXCEPT{
            return static_cast<int_type>(static_cast<unsigned char>(_c));
        }
    };
    
    #if _GLIBCXX_USE_WCHAR_T
    template <>
    struct char_traits<wchar_t>{
        using char_type = wchar_t;
        using int_type  = wint_t;

        static  void
        assign(char_type& _c1, const char_type& _c2){
            _c1 = _c2;
        }
        static  void
        assign(char_type* _s, stl::size_t _n, const char_type& _c){
            for(stl::size_t _i = 0; _i < _n; ++_i)
                _s[_i] = _c;
        }
        static  bool
        eq(const char_type& _c1, const char_type& _c2){
            return _c1 == _c2;
        }
        static  bool
        eq(const int_type& _a1, const int_type& _a2){
            return _a1 == _a2;
        }
        static  bool
        lt(const char_type& _c1, const char_type& _c2){
            return _c1 < _c2;
        }
        static  int
        compare(const char_type* _s1, const char_type* _s2, stl::size_t _n){
            for(int _i = 0; _i < _n; ++_i){
                if(lt(_s1[_i], _s2[_i])) return -1;
                else if(lt(_s2[_i], _s1[_i])) return 1;
            }
            return 0;
        }
        static  stl::size_t
        length(const char_type* _s){
            char_type null_char{};
            stl::size_t _l = 0;
            while(!eq(_s[_l], null_char))
                ++_l;
            return _l;
        }
        static  const char_type*
        find(const char_type* _s, stl::size_t _n, const char_type& _t){
            for(stl::size_t _i = 0; _i < _n; ++_i)
                if(eq(_s[_i], _t)) return _s + _i;
            return 0;
        }
        static  char_type* 
        copy(char_type* _s1, const char_type* _s2, stl::size_t _n){
            if(_n) memcpy(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static  char_type*
        move(char_type* _s1, const char_type* _s2, stl::size_t _n){
            if(_n) memmove(_s1, _s2, _n * sizeof(char_type));
            return _s1;
        }
        static  char_type
        to_char(const int_type& _a){
            return static_cast<char_type>(_a);
        }
        static  int_type
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
        using allocator_type    = typename _alloc::alloc_rebind<_Alloc, _CharType>;
    protected:
        using _alloc_traits = typename _alloc::_alloc_traits<allocator_type>;
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

        // construction
        _string() _YXXX_NOEXCEPT:
            _M_allocator(allocator_type{}), _M_begin(nullptr),
            _M_size(0), _M_capacity(0){}
        _string(const allocator_type& _alloc):
            _M_allocator(_alloc), _M_begin(nullptr),
            _M_size(0), _M_capacity(0){}
        _string(allocator_type&& _alloc):
            _M_allocator(std::move(_alloc)), _M_begin(nullptr),
            _M_size(0), _M_capacity(0){}
        
        _string(const _string& _str):
            _string(_str.allocator()){
            _M_copy_or_assign(_str._M_begin, _str._M_size);
        }
        _string(const _string& _str, const allocator_type& _alloc):
            _string(_alloc){
            _M_copy_or_assign(_str._M_begin, _str._M_size);
        }
        _string(const _string& _str, allocator_type&& _alloc):
            _string(std::move(_alloc)){
            _M_copy_or_assign(_str._M_begin, _str._M_size);
        }

        _string(const _string& _str,
            difference_type _begin, size_type _n = npos){
            _str._M_check(_begin);
            _M_construct(_str, _begin,
                _M_len(_str, _begin, _n),
                _str.allocator());
        }
        _string(const _string& _str,
            difference_type _begin, const allocator_type& _alloc){
            _str._M_check(_begin);
            _M_construct(_str, _begin,
                _M_len(_str, _begin, npos),
                _alloc);
        }
        _string(const _string& _str,
            difference_type _begin, allocator_type&& _alloc){
            _str._M_check(_begin);
            _M_construct(_str, _begin,
                _M_len(_str, _begin, npos),
                std::move(_alloc));
        }
        _string(const _string& _str,
            difference_type _begin, size_type _n,
            const allocator_type& _alloc){
            _str._M_check(_begin);
            _M_construct(_str, _begin,
                _M_len(_str, _begin, _n),
                _alloc);
        }
        _string(const _string& _str,
            difference_type _begin, size_type _n,
            allocator_type&& _alloc){
            _str._M_check(_begin);
            _M_construct(_str, _begin,
                _M_len(_str, _begin, _n),
                std::move(_alloc));
        }

        _string(_string&& _str):
            _string(std::move(_str.allocator())){
            _M_move_data(std::move(_str));
        }
        _string(_string&& _str, const allocator_type& _alloc):
            _string(_alloc){
            _M_move_data(std::move(_str));
        }
        _string(_string&& _str, allocator_type && _alloc):
            _string(std::move(_alloc)){
            _M_move_data(std::move(_str));
        }

        template <typename _InputIter>
        _string(_InputIter _begin, _InputIter _end):
            _string(){
            _M_copy_or_assign(_begin, _end);
        }
        template <typename _InputIter>
        _string(_InputIter _begin, _InputIter _end,
            const allocator_type& _alloc):
            _string(_alloc){
            _M_copy_or_assign(_begin, _end);
        }
        template <typename _InputIter>
        _string(_InputIter _begin, _InputIter _end,
            allocator_type&& _alloc):
            _string(std::move(_alloc)){
            _M_copy_or_assign(_begin, _end);
        }

        _string(std::initializer_list<char_type>&& _il):
            _string(){
            _M_move_or_assign(_il.begin(), _il.end());
        }
        _string(std::initializer_list<char_type>&& _il,
            const allocator_type& _alloc):
            _string(_alloc){
            _M_move_or_assign(_il.begin(), _il.end());
        }
        _string(std::initializer_list<char_type>&& _il,
            allocator_type&& _alloc):
            _string(std::move(_alloc)){
            _M_move_or_assign(_il.begin(), _il.end());
        }

        _string(const char_type* _s):
            _string(){
            _M_copy_or_assign(_s, _M_len(_s, char_type{}));
        }
        _string(const char_type* _s,
            const allocator_type& _alloc):
            _string(_alloc){
            _M_copy_or_assign(_s, _M_len(_s, char_type{}));
        }
        _string(const char_type* _s,
            allocator_type&& _alloc):
            _string(std::move(_alloc)){
            _M_copy_or_assign(_s, _M_len(_s, char_type{}));
        }

        _string(const char_type* _s, size_type _n):
            _string(){
            _M_copy_or_assign(_s, _n);
        }
        _string(const char_type* _s, size_type _n,
            const allocator_type& _alloc):
            _string(_alloc){
            _M_copy_or_assign(_s, _n);
        }
        _string(const char_type* _s, size_type _n,
            allocator_type&& _alloc):
            _string(std::move(_alloc)){
            _M_copy_or_assign(_s, _n);
        }

        _string(size_type _n, const char_type& _c):
            _string(){
            _M_construct(_n, _c);
        }
        _string(size_type _n, const char_type& _c,
            const allocator_type& _alloc):
            _string(_alloc){
            _M_construct(_n, _c);
        }
        _string(size_type _n, const char_type& _c,
            allocator_type&& _alloc):
            _string(std::move(_alloc)){
            _M_construct(_n, _c);
        }

        ~_string() _YXXX_NOEXCEPT{
            _M_clear();
        };

        self_type& operator=(const _string& _str){
            _M_replace(0, _M_size, _str._M_begin, _str._M_size);
            return *this;
        }
        self_type& operator=(_string&& _str){
            _M_replace(0, _M_size, std::move(_str));
            return *this;
        }
        self_type& operator=(const char_type* _s){
            _M_replace(0, _M_size, _s);
            return *this;
        }
        self_type& operator=(const char_type& _c){
            reserve(1);
            *_M_begin = _c;
            return *this;
        }
        self_type& operator=(char_type&& _c){
            reserve(1);
            *_M_begin = std::move(_c);
            return *this;
        }

        self_type& operator+=(const _string& _str){
            append(_str);
            return *this;
        }
        self_type& operator+=(_string&& _str){
            append(std::move(_str));
            return *this;
        }
        self_type& operator+=(const char_type* _s){
            append(_s);
            return *this;
        }
        self_type& operator+=(const char_type& _c){
            append(_c);
            return *this;
        }
        self_type& operator+=(char_type&& _c){
            append(std::move(_c));
            return *this;
        }

        reference operator[](difference_type _idx){
            return *(_M_begin + _idx);
        }
        const_reference
        operator[](difference_type _idx) const{
            return *(_M_begin + _idx);
        }
        reference at(difference_type _idx){
            _M_check(_idx);
            return *(_M_begin + _idx);
        }
        const_reference
        at(difference_type _idx) const{
            _M_check(_idx);
            return *(_M_begin + _idx);
        }
        reference front(){
            return *_M_begin;
        }
        const_reference front() const{
            return *_M_begin;
        }
        reference back(){
            return *_M_begin;
        }
        const_reference back() const{
            return *(_M_begin + _M_size);
        }

        friend bool
        operator>(const _string& _str1, const _string& _str2)
        _YXXX_NOEXCEPT{
            return compare(_str1, _str2) > 0;
        }
        friend bool
        operator>=(const _string& _str1, const _string& _str2)
        _YXXX_NOEXCEPT{
            return compare(_str1, _str2) >= 0;
        }
        friend bool
        operator<(const _string& _str1, const _string& _str2)
        _YXXX_NOEXCEPT{
            return compare(_str1, _str2) < 0;
        }
        friend bool
        operator<=(const _string& _str1, const _string& _str2)
        _YXXX_NOEXCEPT{
            return compare(_str1, _str2) <=0;
        }
        friend bool
        operator==(const _string& _str1, const _string& _str2)
        _YXXX_NOEXCEPT{
            return compare(_str1, _str2) == 0;
        }
        friend bool
        operator!=(const _string& _str1, const _string& _str2)
        _YXXX_NOEXCEPT{
            return compare(_str1, _str2) != 0;
        }

        void assign(){}

        // push_back
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

        // append
        insert_return_type
        append(const _string& _str){
            return _M_insert(
                _M_size,
                _str._M_begin,
                _str._M_size);
        }
        insert_return_type
        append(const _string& _str,
            difference_type _index, size_type _n = npos){
            if(!_M_valid_insert(_index)) return end();
            return _M_insert(
                _M_size,
                _str._M_begin + _index,
                _M_len(_str, _index, _n));
        }
        insert_return_type
        append(_string&& _str){
            return _M_insert(_M_size, std::move(_str));
        }
        insert_return_type
        append(const char_type* _s){
            return _M_insert(_M_size, _s,
                _M_len(_s, char_type{}));
        }
        insert_return_type
        append(const char_type* _s, size_type _n){
            return _M_insert(_M_size, _s, _n);
        }
        template <typename Iter>
        insert_return_type
        append(Iter _begin, Iter _end){
            difference_type _r = difference_type(_M_size);
            while(_begin != _end) push_back(*_begin++);
            return {_M_begin + _r};
        }

        // insert
        insert_return_type
        insert(difference_type _index, const char_type& _c){
            if(!_M_valid_insert(_index)) return end();
            return _M_insert(_index, _c);
        }
        insert_return_type
        insert(iterator _pos, const char_type& _c){
            if(!_M_valid_insert(_pos)) return end();
            return _M_insert(_pos.base() - _M_begin, _c);
        }

        insert_return_type
        insert(difference_type _index, char_type&& _c){
            if(!_M_valid_insert(_index)) return end();
            return _M_insert(_index, std::move(_c));
        }
        insert_return_type
        insert(iterator _pos, char_type&& _c){
            if(!_M_valid_insert(_pos)) return end();
            return _M_insert(_pos.base() - _M_begin, std::move(_c));
        }

        insert_return_type
        insert(difference_type _index, const _string& _str){
            if(!_M_valid_insert(_index)) return end();
            return _M_insert(_index, _str._M_begin, _str._M_size);
        }
        insert_return_type
        insert(iterator _pos, const _string& _str){
            if(!_M_valid_insert(_pos)) return end();
            return _M_insert(
                _pos.base() - _M_begin,
                _str._M_begin,
                _str._M_size);
        }

        insert_return_type
        insert(difference_type _index, _string&& _str){
            if(!_M_valid_insert(_index)) return end();
            return _M_insert(_index, std::move(_str));
        }
        insert_return_type
        insert(iterator _pos, _string&& _str){
            if(!_M_valid_insert(_pos)) return end();
            return _M_insert(_pos.base() - _M_begin, std::move(_str));
        }
        
        insert_return_type
        insert(difference_type _index1, const _string& _str,
            difference_type _index2, size_type _n = npos){
            if(!_M_valid_insert(_index1) || !_str._M_valid(_index2))
                return end();
            return _M_insert(_index1,
                _str._M_begin + _index2,
                _M_len(_str, _index2, _n));
        }
        insert_return_type
        insert(iterator _pos, const _string& _str,
            difference_type _index, size_type _n = npos){
            if(!_M_valid_insert(_pos) || !_str._M_valid(_index))
                return end();
            return _M_insert(_pos.base() - _M_begin,
                _str._M_begin + _index,
                _M_len(_str, _index, _n));
        }

        insert_return_type
        insert(difference_type _index, size_type _n,
            const char_type& _c){
            if(!_M_valid_insert(_index)) return end();
            return _M_insert(_index, _c, _n);
        }
        insert_return_type
        insert(iterator _pos, size_type _n,
            const char_type& _c){
            if(!_M_valid_insert(_pos)) return end();
            return _M_insert(_pos.base() - _M_begin, _c, _n);
        }

        template <typename Iter>
        insert_return_type
        insert(difference_type _index, Iter _begin, Iter _end){
            if(!_M_valid_insert(_index)) return end();
            return _M_insert(_index, _begin, _end);
        }
        template <typename Iter>
        insert_return_type
        insert(iterator _pos, Iter _begin, Iter _end){
            if(!_M_valid_insert(_pos)) return end();
            return _M_insert(_pos.base() - _M_begin, _begin, _end);
        }

        insert_return_type
        insert(difference_type _index, const char_type* _s){
            if(!_M_valid_insert(_index)) return end();
            return _M_insert(_index, _s, _M_len(_s, char_type{}));
        }
        insert_return_type
        insert(iterator _pos, const char_type* _s){
            if(!_M_valid_insert(_pos)) return end();
            return _M_insert(
                _pos.base() - _M_begin, _s,
                _M_len(_s, char_type{}));
        }

        insert_return_type
        insert(difference_type _index, const char_type* _s,
            size_type _n){
            if(!_M_valid_insert(_index)) return end();
            return _M_insert(_index, _s, _n);
        }
        insert_return_type
        insert(iterator _pos, const char_type* _s,
            size_type _n){
            if(!_M_valid_insert(_pos)) return end();
            return _M_insert(_pos.base() - _M_begin, _s, _n);
        }

        // pop_back/erase/remove/clear
        void pop_back() _YXXX_NOEXCEPT{
            if(!empty()) --_M_size;
        }
        bool erase(difference_type _index){
            if(!_M_valid(_index)) return false;
            _M_erase(_index);
            return true;
        }
        bool erase(iterator _pos){
            if(!_M_valid(_pos)) return false;
            _M_erase(_pos.base() - _M_begin);
            return true;
        }
        bool remove(difference_type _index, size_type _n = npos){
            if(!_M_valid(_index)) return false;
            _M_remove(_index, _n);
            return true;
        }
        bool remove(iterator _begin){
            if(!_M_valid(_begin)) return false;
            _M_remove(_begin - begin(), npos);
            return true;
        }
        bool remove(iterator _begin, iterator _end){
            if(!_M_valid(_begin) || !_M_valid_end(_end)) return false;
            _M_remove(_begin - begin(), _end - _begin);
            return true;
        }
        void clear() _YXXX_NOEXCEPT{
            _M_size = 0;
        }

        // replace
        insert_return_type
        replace(difference_type _index, const char_type& _c){
            if(!_M_valid(_index)) return end();
            return _M_replace(_index, _c);
        }
        insert_return_type
        replace(iterator _pos, const char_type& _c){
            if(!_M_valid(_pos)) return end();
            return _M_replace(_pos.base() - _M_begin, _c);
        }

        insert_return_type
        replace(difference_type _index, char_type&& _c){
            if(!_M_valid(_index)) return end();
            return _M_replace(_index, std::move(_c));
        }
        insert_return_type
        replace(iterator _pos, char_type&& _c){
            if(!_M_valid(_pos)) return end();
            return _M_replace(_pos.base() - _M_begin, std::move(_c));
        }

        template <typename Iter>
        insert_return_type
        replace(iterator _pos, Iter _it){
            if(!_M_valid(_pos)) return end();
            return _M_replace(_pos.base() - _M_begin, *_it);
        }

        insert_return_type
        replace(difference_type _index, size_type _n, const char_type& _c){
            if(!_M_valid(_index)) return end();
            _n = _M_len(*this, _index, _n);
            return _M_replace(_index, _n, _c);
        }
        insert_return_type
        replace(iterator _begin, iterator _end, const char_type& _c){
            if(!_M_valid(_begin) || !_M_valid_end(_end) || _begin > _end)
                return end();
            return _M_replace(
                _begin.base() - _M_begin,
                _end - _begin,
                _c);
        }

        insert_return_type
        replace(difference_type _index, size_type _n, const _string& _str){
            if(!_M_valid(_index)) return end();
            _n = _M_len(*this, _index, _n);
            return _M_replace(_index, _n, _str._M_begin, _str._M_size);
        }
        insert_return_type
        replace(iterator _begin, iterator _end, const _string& _str){
            if(!_M_valid(_begin) || !_M_valid_end(_end)) return end();
            return _M_replace(
                _begin.base() - _M_begin,
                _end - _begin,
                _str._M_begin,
                _str._M_size);
        }

        insert_return_type
        replace(difference_type _index, size_t _n, _string&& _str){
            if(!_M_valid(_index)) return end();
            _n = _M_len(*this, _index, _n);
            return _M_replace(_index, _n, std::move(_str));
        }
        insert_return_type
        replace(iterator _begin, iterator _end, _string&& _str){
            if(!_M_valid(_begin) || !_M_valid_end(_end)) return end();
            return _M_replace(
                _begin.base() - _M_begin,
                _end - _begin,
                std::move(_str));
        }

        insert_return_type
        replace(difference_type _index1, size_type _n1, const _string& _str,
            difference_type _index2, size_type _n2 = npos){
            if(!_M_valid(_index1) || !_str._M_valid(_index2)) return end();
            _n1 = _M_len(*this, _index1, _n1);
            _n2 = _M_len(_str, _index2, _n2);
            return _M_replace(_index1, _n1, _str._M_begin + _index2, _n2);
        }
        insert_return_type
        replace(iterator _begin, iterator _end, const _string& _str,
            difference_type _index, size_type _n = npos){
            if(!_M_valid(_begin) || !_M_valid_end(_end)
                || !_str._M_valid(_index))
                return end();
            _n = _M_len(_str, _index, _n);
            return _M_replace(
                _begin.base() - _M_begin,
                _end - _begin,
                _str._M_begin + _index,
                _n);
        }

        insert_return_type
        replace(difference_type _index, size_type _n,
            const char_type* _s){
            if(!_M_valid(_index)) return end();
            _n = _M_len(*this, _index, _n);
            return _M_replace(_index, _n, _s, _M_len(_s, char_type{}));
        }
        insert_return_type
        replace(iterator _begin, iterator _end,
            const char_type* _s){
            if(!_M_valid(_begin) || !_M_valid_end(_end)) return end();
            return _M_replace(
                _begin.base() - _M_begin,
                _end - _begin,
                _s, _M_len(_s, char_type{}));
        }

        insert_return_type
        replace(difference_type _index, size_type _n1,
            const char_type* _s, size_type _n2){
            if(!_M_valid(_index)) return end();
            _n1 = _M_len(*this, _index, _n1);
            return _M_replace(_index, _n1, _s, _n2);
        }
        insert_return_type
        replace(iterator _begin, iterator _end,
            const char_type* _s, size_type _n){
            if(!_M_valid(_begin) || !_M_valid_end(_end)) return end();
            return _M_replace(
                _begin.base() - _M_begin,
                _end - _begin,
                _s, _n);
        }

        template <typename Iter>
        insert_return_type
        replace(iterator _begin1, iterator _end1,
            Iter _begin2, Iter _end2){
            if(!_M_valid(_begin1) || !_M_valid_end(_end1)
                || _end1 <= _begin1)
                return end();
            return _M_replace(_begin1, _end1, _begin2, _end2);
        }

        friend void swap(_string& _str1, _string& _str2)
            _YXXX_NOEXCEPT{
            std::swap(_str1._M_allocator, _str2._M_allocator);
            std::swap(_str1._M_begin, _str2._M_begin);
            std::swap(_str1._M_size, _str2._M_size);
            std::swap(_str1._M_capacity, _str2._M_capacity);
        }
        friend int compare(const _string& _str1, const _string& _str2)
        _YXXX_NOEXCEPT{
            using difference_type = typename _string::difference_type;
            difference_type _n1 = difference_type(_str1._M_size),
                _n2 = difference_type(_str2._M_size);
            if(_n1 < _n2){
                int _res = traits_type::compare(
                    _str1._M_begin, _str2._M_begin, _n1);
                return _res ? _res : -1;
            }
            else if(_n2 < _n1){
                int _res = traits_type::compare(
                    _str1._M_begin, _str2._M_begin, _n2);
                return _res ? _res : 1;
            }
            else
                return traits_type::compare(
                    _str1._M_begin, _str2._M_begin, _n1);
        }

        // iterator find(){}
        // iterator rfind(){}
        // iterator find_first_of(){}
        // iterator find_last_of(){}
        // iterator find_first_not_of(){}
        // iterator find_last_not_of(){}

        // const char_type* c_str() const _YXXX_NOEXCEPT{}
        // void copy_to(char_type* _s){}

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
        crend() const _YXXX_NOEXCEPT{
            return cbegin();
        }

        void shrink_to_fit(){
            reserve(_M_size);
        }
        void reserve(size_type _n){
            if(nullptr != _M_begin){
                char_type* _new_begin = _M_allocator.allocate(_n);
                if(_n < _M_size) _M_size = _n;
                memmove(_new_begin, _M_begin, _M_size * sizeof(char_type));
                _M_allocator.deallocate(_M_begin, _M_capacity);
                _M_begin = _new_begin;
            }
            else{
                _M_size = _n;
                _M_begin = _M_allocator.allocate(_M_size);
            }
            _M_capacity = _n;
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
        allocator_type allocator() const{
            return _M_allocator;
        }

    protected:
        static size_type _M_len(const char_type* _s,
            const char_type& _end_char){
            size_type _len = 0;
            while(_len < npos){
                if(_end_char == _s[_len]) break;
                ++_len;
            }
            return _len;
        }
        static size_type _M_len(const _string& _str,
            difference_type _index, size_type _len){
            // 默认valid(_index) == true
            return _str._M_size - size_type(_index) < _len ?
                _str._M_size - _index :
                _len;
        }
        bool _M_valid(difference_type _index)
        const _YXXX_NOEXCEPT{
            return size_type(_index) < _M_size;
        }
        bool _M_valid(iterator _it)
        const _YXXX_NOEXCEPT{
            return _M_valid(_it - begin());
        }
        bool _M_valid_insert(difference_type _index)
        const _YXXX_NOEXCEPT{
            return size_type(_index) <= _M_size;
        }
        bool _M_valid_insert(iterator _it)
        const _YXXX_NOEXCEPT{
            return _M_valid_insert(_it - begin());
        }
        bool _M_valid_end(difference_type _index)
        const _YXXX_NOEXCEPT{
            return _M_valid_insert(_index);
        }
        bool _M_valid_end(iterator _it)
        const _YXXX_NOEXCEPT{
            return _M_valid_insert(_it - begin());
        }

        void _M_check(size_type _index) const{
            if(!_M_valid(_index))
                throw std::runtime_error("invalid index");
        }

        template <typename _AllocType>
        void _M_construct(const _string& _str,
            difference_type _begin, size_type _n,
            _AllocType&& _alloc){
            _M_allocator = std::forward<_Alloc>(_alloc);
            _M_copy_or_assign(_str._M_begin + _begin, _n);
        }
        void _M_construct(size_type _n, const char_type& _c){
            reserve(_n);
            std::fill_n(_M_begin, _n, _c);
        }

        void _M_copy_or_assign(const char_type* _s, size_type _n){
            if(_n > _M_capacity){
                _M_allocator.deallocate(_M_begin, _M_capacity);
                // _M_begin = nullptr;
                _M_capacity = 1.5 * _n;
                _M_begin = _M_allocator.allocate(_M_capacity);
            }
            memcpy(_M_begin, _s, _n * sizeof(char_type));
            _M_size = _n;
        }
        template <typename _InputIter>
        void _M_copy_or_assign(_InputIter _begin, _InputIter _end){
            iterator _it = begin(), _it_end = end();
            while(_it != _it_end && _begin != _end){
                *_it++ = *_begin++;
            }
            if(_it == _it_end){
                while(_begin != _end)
                    push_back(*_begin++);
            }
            else{
                _M_size = _it - begin();
            }
        }
        void _M_move_or_assign(const char_type* _s, size_type _n){
            if(_n > _M_capacity){
                _M_allocator.deallocate(_M_begin, _M_capacity);
                // _M_begin = nullptr;
                _M_capacity = 1.5 * _n;
                _M_begin = _M_allocator.allocate(_M_capacity);
            }
            memmove(_M_begin, _s, _n * sizeof(char_type));
            _M_size = _n;
        }
        template <typename _InputIter>
        void _M_move_or_assign(_InputIter _begin, _InputIter _end){
            iterator _it = begin(), _it_end = end();
            while(_it != _it_end && _begin != _end){
                *_it++ = std::move(*_begin++);
            }
            if(_it == _it_end){
                while(_begin != _end)
                    push_back(std::move(*_begin++));
            }
            else{
                _M_size = _it - begin();
            }
        }

        void _M_move_data(_string&& _str){
            _M_move_or_assign(_str._M_begin, _str._M_size);
            _str._M_after_move_data();
        }
        void _M_after_move_data(){
            _M_size = 0;
        }
        
        void _M_clear(){
            _M_allocator.deallocate(_M_begin);
            _M_begin = nullptr;
            _M_size = _M_capacity = 0;
        }

        void _M_expand(){
            size_type _new_capacity = _M_capacity * 1.5 + 1;
            pointer _new_begin = _M_allocator.allocate(_new_capacity);
            memmove(_new_begin, _M_begin, _M_capacity * sizeof(value_type));
            _M_allocator.deallocate(_M_begin, sizeof(value_type) * _M_capacity);
             _M_begin = _new_begin;
            _M_capacity = _new_capacity;
        }
        void _M_may_expand(){
            if(full()) _M_expand();
        }
        void _M_expand(size_type _n){
            size_type _new_capacity = _n * 1.5 + 1;
            pointer _new_begin = _M_allocator.allocate(_new_capacity);
            memmove(_new_begin, _M_begin, _M_capacity * sizeof(value_type));
            _M_allocator.deallocate(_M_begin, sizeof(value_type) * _M_capacity);
             _M_begin = _new_begin;
            _M_capacity = _new_capacity;
        }
        void _M_may_expand(size_type _n){
            if(_M_capacity - _M_size < _n) _M_expand(_M_size + _n);
        }

        void _M_before_insert(difference_type _index, size_type _n = 1){
            _M_may_expand(_n);
            _M_size += _n;
            _index += difference_type(_n);
            for(difference_type i = difference_type(_M_size) - 1;
                    i >= _index;--i) 
                _M_begin[i] = std::move(_M_begin[i-difference_type(_n)]);
        }
        void _M_before_replace(difference_type _index1, difference_type _index2,
            size_type _n2){
            size_type _diff = size_type(_index2 - _index1);
            if(_M_capacity - _M_size < _n2){
                size_type _new_capacity = (_M_capacity + _n2 - _diff) * 1.5 + 1;
                char_type* _new_begin = _M_allocator.allocate(_new_capacity);

                memmove(_new_begin, _M_begin,
                    size_type(_index1) * sizeof(char_type));
                memmove(_new_begin + _index1 + _n2,
                    _M_begin + _index2,
                    (_M_size - size_type(_index2)) * sizeof(char_type));

                _M_allocator.deallocate(_M_begin, _M_capacity);
                _M_begin = _new_begin;
                _M_capacity = _new_capacity;
                _M_size += _n2 - _diff;
            }
            else if(_n2 < _diff){
                _M_remove(_index1 + _n2, _diff - _n2);
            }
            else if(_n2 > _diff){
                _M_before_insert(_index2, _n2 - _diff);
            }
        }

        insert_return_type
        _M_insert(difference_type _index, const char_type& _c){
            _M_before_insert(_index);
            return _M_replace(_index, _c);
        }
        insert_return_type
        _M_insert(difference_type _index, char_type&& _c){
            _M_before_insert(_index);
            return _M_replace( _index, std::move(_c));
        }
        insert_return_type
        _M_insert(difference_type _index, _string&& _str){
            _M_before_insert(_index, _str._M_size);
            memmove(_M_begin + _index, _str._M_begin, _str._M_size);
            _str._M_after_move_data();
            return _M_begin + _index;
        }
        insert_return_type
        _M_insert(difference_type _index, size_type _n,
            const char_type& _c){
            _M_before_insert(_index, _n);
            return _M_replace(_index, _n, _c);
        }
        insert_return_type
        _M_insert(difference_type _index,
            const char_type* _s, size_type _n){
            _M_before_insert(_index, _n);
            return _M_replace(_index, _n, _s, _n);
        }
        template <typename Iter>
        insert_return_type
        _M_insert(difference_type _index, Iter _begin, Iter _end){
            difference_type _i = _index;
            while(_begin != _end){
                _M_insert(_i++, *_begin++);
            }
            return {_M_begin + _index};
        }
        
        void _M_erase(difference_type _index){
            for(int i = _index;i < _M_size;){
                _M_begin[i] = std::move(_M_begin[++i]);
            }
            --_M_size;
        }
        void _M_remove(difference_type _index, size_type _n){
            if(npos == _n || _M_size - size_type(_index) <= _n){
                _M_size = _index + 1;
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
        }

        insert_return_type
        _M_replace(difference_type _index, const char_type& _c){
            char_type* _pos = _M_begin + _index;
            *_pos = _c;
            return _pos;
        }
        insert_return_type
        _M_replace(difference_type _index, char_type&& _c){
            char_type* _pos = _M_begin + _index;
            *_pos = std::move(_c);
            return _pos;
        }
        insert_return_type
        _M_replace(difference_type _index, size_type _n, _string&& _str){
            _M_before_replace(_index, _n, _str._M_size);
            memmove(_M_begin + _index, _str._M_begin, _str._M_size);
            _str._M_after_move_data();
            return _M_begin + _index;
        }
        insert_return_type
        _M_replace(difference_type _index, size_type _n,
            const char_type& _c){
            if(_M_capacity - _index < _n)
                _M_before_replace(_index, _M_size, _n);
            std::fill_n(_M_begin + _index, _n, _c);
            if(_M_size - _index < _n)
                _M_size = size_type(_index) + _n + 1;
            return {_M_begin + _index};
        }
        insert_return_type
        _M_replace(difference_type _index, size_type _n1,
            const char_type* _s, size_type _n2){
            _M_before_replace(_index, _index + _n1, _n2);
            memcpy(_M_begin + _index, _s, _n2 * sizeof(char_type));
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
                _M_insert(_it - begin(), _begin2, _end2);
            else if(_it != _end1)
                _M_remove(_it - begin(), _end1 - _it);
            return _begin1;
        }

        allocator_type _M_allocator;
        char_type* _M_begin;
        size_type _M_size;
        size_type _M_capacity;
    };

    template <typename _CharType,
        typename _Traits = _char_traits::char_traits<_CharType>>
    using string = _string<_CharType, _Traits, _alloc::allocator<_CharType>>;
};

#endif