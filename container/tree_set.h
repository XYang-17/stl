#ifndef _YXXX_TREE_SET_H_
#define _YXXX_TREE_SET_H_

#include <bits/stl_pair.h>
#include "./tree/tree_base.h"
#include "../function.h"

namespace stl{
    template<typename _ValueType,
        typename _ExtractKey = stl::_self,
        typename _KeyComp = stl::less_than<_ValueType>,
        typename _Alloc = _alloc::allocator<_ValueType>>
    class tree_set{
    protected:
        using _tree_type    = _tree::_Rb_tree<
            _ValueType, _ValueType,
            _ExtractKey, _KeyComp, true, true, _Alloc>;
        _tree_type _M_tree;
    public:
        using key_type       = typename _tree_type::key_type;
        using value_type     = typename _tree_type::value_type;
        using key_compare    = typename _tree_type::key_compare;
        using allocator_type = typename _tree_type::allocator_type;
        
        using pointer         = typename _tree_type::pointer;
        using const_pointer   = typename _tree_type::const_pointer;
        using reference       = typename _tree_type::reference;
        using const_reference = typename _tree_type::const_reference;
        using size_type       = typename _tree_type::size_type;
        using difference_type = typename _tree_type::difference_type;
        
        using iterator             = typename _tree_type::iterator;
        using const_iterator       = typename _tree_type::const_iterator;
        using reverse_iterator     = typename _tree_type::reverse_iterator;
        using const_reverse_iterator = typename _tree_type::const_reverse_iterator;

        using self_type       = tree_set;
        using insert_type     = typename _tree_type::insert_type;

        tree_set(const allocator_type& __a) _YXXX_NOEXCEPT:
            _M_tree(__a){}
        tree_set(const key_compare& __comp = key_compare(),
            const allocator_type& __a = allocator_type())
        _YXXX_NOEXCEPT: _M_tree(__comp, __a){}
        tree_set(self_type&& _t) _YXXX_NOEXCEPT:
            _M_tree(_t._M_tree){}
        tree_set(std::initializer_list<value_type>&& _il) _YXXX_NOEXCEPT:
            _M_tree(std::move(_il)){}
        
        insert_type insert(const value_type& _val){
            return _M_tree.insert(_val);
        }
        insert_type insert(value_type&& _val){
            return _M_tree.insert(std::move(_val));
        }
        template <typename _Iter>
        insert_type insert(_Iter _it){
            return _M_tree.insert(_it);
        }
        template <typename _Iter>
        void insert(_Iter _begin, _Iter _end){
            _M_tree.insert(_begin, _end);
        }
        template <typename ..._Args>
        insert_type emplace(_Args ..._args){
            return _M_tree.emplace(std::forward<_Args>(_args)...);
        }

        void erase(const key_type& _key) _YXXX_NOEXCEPT{
            _M_tree.erase(_key);
        }
        void erase(iterator _it) _YXXX_NOEXCEPT{
            _M_tree.erase(_it);
        }
        void erase(iterator _begin, iterator _end) _YXXX_NOEXCEPT{
            _M_tree.erase(_begin, _end);
        }

        template <typename _Func>
        void preorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_tree.preorder(std::forward<_Func>(_f));
        }
        template <typename _Func>
        void inorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_tree.inorder(std::forward<_Func>(_f));
        }
        template <typename _Func>
        void postorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_tree.postorder(std::forward<_Func>(_f));
        }
        template <typename _Func>
        void levelorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_tree.levelorder(std::forward<_Func>(_f));
        }

        iterator begin() _YXXX_NOEXCEPT{
            return _M_tree.begin();
        }
        const_iterator begin() const _YXXX_NOEXCEPT{
            return _M_tree.begin();
        }
        iterator end() _YXXX_NOEXCEPT{
            return _M_tree.end();
        }
        const_iterator end() const _YXXX_NOEXCEPT{
            return _M_tree.end();
        }
        const_iterator cbegin() const _YXXX_NOEXCEPT{
            return _M_tree.cbegin();
        }
        const_iterator cend() const _YXXX_NOEXCEPT{
            return _M_tree.cend();
        }
        reverse_iterator rbegin() _YXXX_NOEXCEPT{
            return _M_tree.rbegin();
        }
        const_reverse_iterator rbegin() const _YXXX_NOEXCEPT{
            return _M_tree.rbegin();
        }
        reverse_iterator rend() _YXXX_NOEXCEPT{
            return _M_tree.rend();
        }
        const_reverse_iterator rend() const _YXXX_NOEXCEPT{
            return _M_tree.rend();
        }
        const_reverse_iterator crbegin() const _YXXX_NOEXCEPT{
            return _M_tree.crbegin();
        }
        const_reverse_iterator crend() const _YXXX_NOEXCEPT{
            return _M_tree.crend();
        }

        key_compare key_comp() const _YXXX_NOEXCEPT{
            return _M_tree.key_comp();
        }

        iterator find(const key_type& _key) _YXXX_NOEXCEPT{
            return _M_tree.find(_key);
        }
        const_iterator find(const key_type& _key) const _YXXX_NOEXCEPT{
            return _M_tree.find(_key);
        }
        std::pair<iterator, iterator>
        equal_range(const key_type& _key) _YXXX_NOEXCEPT{
            return _M_tree.equal_range(_key);
        }
        std::pair<const_iterator, const_iterator>
        equal_range(const key_type& _key) const _YXXX_NOEXCEPT{
            return _M_tree.equal_range(_key);
        }

        bool empty() const _YXXX_NOEXCEPT{return _M_tree.empty();}
        size_type size() const _YXXX_NOEXCEPT{return _M_tree.size();}
        size_type count(const key_type& _key) const _YXXX_NOEXCEPT{
            return _M_tree.count(_key);
        }
        void clear() _YXXX_NOEXCEPT{
            _M_tree.clear();
        }

        friend void
        swap(self_type& _lt, self_type& _rt) _YXXX_NOEXCEPT{
            swap(_lt._M_tree, _rt._M_tree);
        }
    };
    template<typename _ValueType,
        typename _ExtractKey = stl::_self,
        typename _KeyComp = stl::less_than<_ValueType>,
        typename _Alloc = _alloc::allocator<_ValueType>>
    class tree_multiset{
    protected:
        using _tree_type    = _tree::_Rb_tree<
            _ValueType, _ValueType,
            _ExtractKey, _KeyComp, false, true, _Alloc>;
        _tree_type _M_tree;
    public:
        using key_type       = typename _tree_type::key_type;
        using value_type     = typename _tree_type::value_type;
        using key_compare    = typename _tree_type::key_compare;
        using allocator_type = typename _tree_type::allocator_type;
        
        using pointer         = typename _tree_type::pointer;
        using const_pointer   = typename _tree_type::const_pointer;
        using reference       = typename _tree_type::reference;
        using const_reference = typename _tree_type::const_reference;
        using size_type       = typename _tree_type::size_type;
        using difference_type = typename _tree_type::difference_type;
        
        using iterator             = typename _tree_type::iterator;
        using const_iterator       = typename _tree_type::const_iterator;
        using reverse_iterator     = typename _tree_type::reverse_iterator;
        using const_reverse_iterator = typename _tree_type::const_reverse_iterator;

        using self_type       = tree_multiset;
        using insert_type     = typename _tree_type::insert_type;

        tree_multiset(const allocator_type& __a) _YXXX_NOEXCEPT:
            _M_tree(__a){}
        tree_multiset(const key_compare& __comp = key_compare(),
            const allocator_type& __a = allocator_type())
        _YXXX_NOEXCEPT: _M_tree(__comp, __a){}
        tree_multiset(self_type&& _t) _YXXX_NOEXCEPT:
            _M_tree(_t._M_tree){}
        tree_multiset(std::initializer_list<value_type>&& _il) _YXXX_NOEXCEPT:
            _M_tree(std::move(_il)){}
        
        insert_type insert(const value_type& _val){
            return _M_tree.insert(_val);
        }
        insert_type insert(value_type&& _val){
            return _M_tree.insert(std::move(_val));
        }
        template <typename _Iter>
        insert_type insert(_Iter _it){
            return _M_tree.insert(_it);
        }
        template <typename _Iter>
        void insert(_Iter _begin, _Iter _end){
            _M_tree.insert(_begin, _end);
        }
        template <typename ..._Args>
        insert_type emplace(_Args ..._args){
            return _M_tree.emplace(std::forward<_Args>(_args)...);
        }

        void erase(const key_type& _key) _YXXX_NOEXCEPT{
            _M_tree.erase(_key);
        }
        void erase(iterator _it) _YXXX_NOEXCEPT{
            _M_tree.erase(_it);
        }
        void erase(iterator _begin, iterator _end) _YXXX_NOEXCEPT{
            _M_tree.erase(_begin, _end);
        }

        template <typename _Func>
        void preorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_tree.preorder(std::forward<_Func>(_f));
        }
        template <typename _Func>
        void inorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_tree.inorder(std::forward<_Func>(_f));
        }
        template <typename _Func>
        void postorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_tree.postorder(std::forward<_Func>(_f));
        }
        template <typename _Func>
        void levelorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_tree.levelorder(std::forward<_Func>(_f));
        }

        iterator begin() _YXXX_NOEXCEPT{
            return _M_tree.begin();
        }
        const_iterator begin() const _YXXX_NOEXCEPT{
            return _M_tree.begin();
        }
        iterator end() _YXXX_NOEXCEPT{
            return _M_tree.end();
        }
        const_iterator end() const _YXXX_NOEXCEPT{
            return _M_tree.end();
        }
        const_iterator cbegin() const _YXXX_NOEXCEPT{
            return _M_tree.cbegin();
        }
        const_iterator cend() const _YXXX_NOEXCEPT{
            return _M_tree.cend();
        }
        reverse_iterator rbegin() _YXXX_NOEXCEPT{
            return _M_tree.rbegin();
        }
        const_reverse_iterator rbegin() const _YXXX_NOEXCEPT{
            return _M_tree.rbegin();
        }
        reverse_iterator rend() _YXXX_NOEXCEPT{
            return _M_tree.rend();
        }
        const_reverse_iterator rend() const _YXXX_NOEXCEPT{
            return _M_tree.rend();
        }
        const_reverse_iterator crbegin() const _YXXX_NOEXCEPT{
            return _M_tree.crbegin();
        }
        const_reverse_iterator crend() const _YXXX_NOEXCEPT{
            return _M_tree.crend();
        }

        key_compare key_comp() const _YXXX_NOEXCEPT{
            return _M_tree.key_comp();
        }

        iterator find(const key_type& _key) _YXXX_NOEXCEPT{
            return _M_tree.find(_key);
        }
        const_iterator find(const key_type& _key) const _YXXX_NOEXCEPT{
            return _M_tree.find(_key);
        }
        std::pair<iterator, iterator>
        equal_range(const key_type& _key) _YXXX_NOEXCEPT{
            return _M_tree.equal_range(_key);
        }
        std::pair<const_iterator, const_iterator>
        equal_range(const key_type& _key) const _YXXX_NOEXCEPT{
            return _M_tree.equal_range(_key);
        }

        bool empty() const _YXXX_NOEXCEPT{return _M_tree.empty();}
        size_type size() const _YXXX_NOEXCEPT{return _M_tree.size();}
        size_type count(const key_type& _key) const _YXXX_NOEXCEPT{
            return _M_tree.count(_key);
        }
        void clear() _YXXX_NOEXCEPT{
            _M_tree.clear();
        }

        friend void
        swap(self_type& _lt, self_type& _rt) _YXXX_NOEXCEPT{
            swap(_lt._M_tree, _rt._M_tree);
        }
    };
};

#endif