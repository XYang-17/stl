#ifndef _YXXX_RB_TREE_H_
#define _YXXX_RB_TREE_H_

#include "tree_base.h"
#include "../queue.h"

namespace _tree{
    template <typename _Type>
    struct _Rb_tree_node:
        public _tree_node_base<_Type>{
    public:
        enum _NodeColor{_Red = false, _Black = true};

        template <bool _Const, typename _TreeType>
        class _iterator{
        public:
            using iterator_category        = stl::bidirectional_iterator_tag;
            using value_type               = _Type;
            using pointer                  = std::conditional_t<
                _Const, const value_type*, value_type*>;
            using reference                = std::conditional_t<
                _Const, const value_type&, value_type&>;
            using difference_type          = std::ptrdiff_t;

            using node_pointer   = _Rb_tree_node*;
            using self_type      = _iterator;

            friend _TreeType;

            _iterator() _YXXX_NOEXCEPT:
                _M_root(nullptr), _M_ptr(nullptr){}
            _iterator(node_pointer _root, node_pointer _ptr)
                _YXXX_NOEXCEPT: _M_root(_root), _M_ptr(_ptr){}

            _YXXX_CONSTEXPR reference operator*() const _YXXX_NOEXCEPT{
                return _M_ptr->value();
            }
            _YXXX_CONSTEXPR pointer operator->() const _YXXX_NOEXCEPT{
                return std::addressof(operator*());
            }
            _YXXX_CONSTEXPR self_type operator++(int) _YXXX_NOEXCEPT{
                self_type _rtn = *this;
                ++*this;
                return _rtn;
            }
            _YXXX_CONSTEXPR self_type& operator++() _YXXX_NOEXCEPT{
                if(nullptr == _M_ptr)
                    _M_ptr = _M_root->leftmost();
                else
                    _M_ptr = node_pointer(_M_ptr->_M_successor());
                return *this;
            }
            _YXXX_CONSTEXPR self_type operator--(int) _YXXX_NOEXCEPT{
                self_type _rtn = *this;
                --*this;
                return _rtn;
            }
            _YXXX_CONSTEXPR self_type& operator--() _YXXX_NOEXCEPT{
                if(nullptr == _M_ptr)
                    _M_ptr = _M_root->rightmost();
                else
                    _M_ptr = node_pointer(_M_ptr->_M_precursor());
                return *this;
            }

            friend _YXXX_CONSTEXPR bool
            operator==(const self_type& _li,
                const self_type& _ri) _YXXX_NOEXCEPT{
                return _li._M_root == _ri._M_root
                    && _li._M_ptr == _ri._M_ptr;
            }
            friend _YXXX_CONSTEXPR bool
            operator!=(const self_type& _li,
                const self_type& _ri) _YXXX_NOEXCEPT{
                return !(_li == _ri);
            }
        protected:
            node_pointer _M_root, _M_ptr;
        };

        using base_type          = _tree_node_base<_Type>;

        typedef typename base_type::value_type    value_type;
        typedef typename base_type::pointer       pointer;
        typedef typename base_type::const_pointer const_pointer;
        typedef typename base_type::reference     reference;
        typedef typename base_type::const_reference  const_reference;
        typedef typename base_type::size_type     size_type;
        typedef typename base_type::difference_type  difference_type;

        using color_type         = _NodeColor;

        using self_type          = _Rb_tree_node;
        using self_pointer       = self_type*;

        struct _RotateMethod: public _Rb_tree_node::base_type::_RotateMethod{
            using base_type = typename _Rb_tree_node::base_type::_RotateMethod;

            _RotateMethod() _YXXX_NOEXCEPT: base_type(){}
            _RotateMethod(self_pointer _ptr, bool _left_rotate)
                _YXXX_NOEXCEPT: base_type(_ptr, _left_rotate){}

            self_pointer pivot() const _YXXX_NOEXCEPT{
                return self_pointer(base_type::pivot());
            }
            void pivot(self_pointer _ptr) _YXXX_NOEXCEPT{
                base_type::pivot(_ptr);
            }
            bool left_rotate() const _YXXX_NOEXCEPT{
                return self_pointer(base_type::left_rotate());
            }
            void left_rotate(bool _b) _YXXX_NOEXCEPT{
                base_type::left_rotate(_b);
            }

            self_pointer prev() const _YXXX_NOEXCEPT{
                return self_pointer(base_type::prev());
            }
            self_pointer left() const _YXXX_NOEXCEPT{
                return self_pointer(base_type::left());
            }
            self_pointer right() const _YXXX_NOEXCEPT{
                return self_pointer(base_type::right());
            }
            void right(self_pointer _ptr) _YXXX_NOEXCEPT{
                self_pointer(base_type::right(_ptr));
            }
            void left(self_pointer _ptr) _YXXX_NOEXCEPT{
                self_pointer(base_type::left(_ptr));
            }
            
            self_pointer inner_node() const _YXXX_NOEXCEPT{
                return self_pointer(base_type::inner_node());
            }
            self_pointer outer_node() const _YXXX_NOEXCEPT{
                return self_pointer(base_type::outer_node());
            }
            self_pointer rotate() _YXXX_NOEXCEPT{
                return self_pointer(base_type::rotate());
            }

            self_pointer like_inner(self_pointer _ptr) const _YXXX_NOEXCEPT{
                return self_pointer(base_type::like_inner(_ptr));
            }
            self_pointer like_outer(self_pointer _ptr) const _YXXX_NOEXCEPT{
                return self_pointer(base_type::like_outer(_ptr));
            }
        };

        _Rb_tree_node(const value_type& _data = value_type(),
            self_pointer _prev = nullptr,
            self_pointer _left = nullptr,
            self_pointer _right = nullptr) _YXXX_NOEXCEPT:
            base_type(_data, _prev, _left, _right),
            _M_color(_Red){}
        
        reference value() _YXXX_NOEXCEPT{return this->_M_value;}
        const_reference value() const _YXXX_NOEXCEPT{return this->_M_value;}
        self_pointer prev() const _YXXX_NOEXCEPT{return self_pointer(this->_M_prev);}
        self_pointer left() const _YXXX_NOEXCEPT{return self_pointer(this->_M_left);}
        self_pointer right() const _YXXX_NOEXCEPT{return self_pointer(this->_M_right);}
        color_type color() const _YXXX_NOEXCEPT{
            if(nullptr == this) return _Black;
            return _M_color;}

        void color(color_type _color) _YXXX_NOEXCEPT{_M_color = _color;}
        void left(self_pointer _ptr) _YXXX_NOEXCEPT{
            this->_M_set_left(_ptr);
        }
        void right(self_pointer _ptr) _YXXX_NOEXCEPT{
            this->_M_set_right(_ptr);
        }
        self_pointer leftmost() _YXXX_NOEXCEPT{
            return self_pointer(this->_M_leftmost());
        }
        self_pointer rightmost() _YXXX_NOEXCEPT{
            return self_pointer(this->_M_rightmost());
        }

        friend void swap_color(
            self_pointer _lptr, self_pointer _rptr) _YXXX_NOEXCEPT{
            std::swap(_lptr->_M_color, _rptr->_M_color);
        }
        friend void swap_value(
            self_pointer _lptr, self_pointer _rptr) _YXXX_NOEXCEPT{
            std::swap(_lptr->_M_value, _rptr->_M_value);
        }
    protected:
        color_type _M_color;
    };

    template<typename _KeyType, typename _ValueType,
        typename _ExtractKey, typename _KeyComp,
        bool _UniqueKey, bool _ConstElements, typename _Alloc>
    class _Rb_tree:
        public _tree_alloc_base<_Rb_tree_node<_ValueType>, _Alloc>{
    public:
        using key_type        = _KeyType;
        using value_type      = _ValueType;
        using key_compare     = _KeyComp;
        using allocator_type  = _Alloc;
    protected:
        using _node_type      = _Rb_tree_node<value_type>;
        using _alloc_base     = _tree_alloc_base<_node_type, _Alloc>;
        using _node_alloc     = typename _alloc_base::_node_alloc;
        using _node_base      = typename _alloc_base::_node_base;
        using _node_pointer   = typename _alloc_base::_node_pointer;
        using _base_pointer   = typename _alloc_base::_base_pointer;
        
        using _RotateMethod   = typename _node_type::_RotateMethod;
    public:
        using pointer         = typename _node_type::pointer;
        using const_pointer   = typename _node_type::const_pointer;
        using reference       = typename _node_type::reference;
        using const_reference = typename _node_type::const_reference;
        using size_type       = typename _node_type::size_type;
        using difference_type = typename _node_type::difference_type;

        using color_type     = typename _node_type::color_type;
        using self_type      = _Rb_tree;

        using iterator             = typename _node_type::_iterator<_ConstElements, self_type>;
        using const_iterator       = typename _node_type::_iterator<true, self_type>;
        using reverse_iterator     = _iterator::reverse_iterator<iterator>;
        using const_reverse_iterator = _iterator::reverse_iterator<reverse_iterator>;

        using insert_type    = std::pair<iterator, bool>;
    public:
        _Rb_tree(const allocator_type& __a) _YXXX_NOEXCEPT:
            _M_allocator(__a), _M_root(nullptr),
            _M_size(0), _M_comp(key_compare{}){}
        _Rb_tree(
            const key_compare& __comp = key_compare(),
            const allocator_type& __a = allocator_type())
        _YXXX_NOEXCEPT: self_type(__a){}
        _Rb_tree(const self_type& _t) _YXXX_NOEXCEPT:
            self_type(_t._M_comp, _t._M_allocator){
            _M_copy_or_assign(_t.begin(), _t.end());
        }
        _Rb_tree(self_type&& _t) _YXXX_NOEXCEPT{
            _M_comp = std::move(_t._M_comp);
            _M_allocator = std::move(_t._M_allocator);
            _M_release(_M_root);
            _M_root = _t._M_root;
            _t._M_root = nullptr;
            _M_size = _t._M_size;
            _t._M_size = 0;
        }
        _Rb_tree(std::initializer_list<value_type>&& _il) _YXXX_NOEXCEPT:
            _Rb_tree(){
            _M_copy_or_assign(_il.begin(), _il.end());
        }
        ~_Rb_tree() _YXXX_NOEXCEPT{clear();}

        insert_type insert(const value_type& _val){
            return _M_emplace(_val);
        }
        insert_type insert(value_type&& _val){
            return _M_emplace(std::move(_val));
        }
        template <typename _Iter>
        insert_type insert(_Iter _it){
            return _M_emplace(*_it);
        }
        template <typename _Iter>
        void insert(_Iter _begin, _Iter _end){
            while(_begin != _end)
                _M_emplace(*(_begin++));
        }
        template <typename ..._Args>
        insert_type emplace(_Args ..._args){
            return _M_emplace(
                std::forward<_Args>(_args)...
            );
        }

        void erase(const key_type& _key) _YXXX_NOEXCEPT{
            _M_erase(_key, _M_root);
        }
        void erase(iterator _it) _YXXX_NOEXCEPT{
            iterator _end = _it;
            _M_erase(_it, ++_end);
        }
        void erase(iterator _begin, iterator _end) _YXXX_NOEXCEPT{
            _M_erase(_begin, _end);
        }

        template <typename _Func>
        void preorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_preorder(_M_root, std::forward<_Func>(_f));
        }
        template <typename _Func>
        void inorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_inorder(_M_root, std::forward<_Func>(_f));
        }
        template <typename _Func>
        void postorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_postorder(_M_root, std::forward<_Func>(_f));
        }
        template <typename _Func>
        void levelorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_levelorder(_M_root, std::forward<_Func>(_f));
        }

        iterator begin() _YXXX_NOEXCEPT{
            return {_M_root, _M_root->leftmost()};
        }
        const_iterator begin() const _YXXX_NOEXCEPT{
            return {_M_root, _M_root->leftmost()};
        }
        iterator end() _YXXX_NOEXCEPT{
            return {_M_root, nullptr};
        }
        const_iterator end() const _YXXX_NOEXCEPT{
            return {_M_root, nullptr};
        }
        const_iterator cbegin() const _YXXX_NOEXCEPT{
            return {_M_root, _M_root->leftmost()};
        }
        const_iterator cend() const _YXXX_NOEXCEPT{
            return {_M_root, nullptr};
        }
        reverse_iterator rbegin() _YXXX_NOEXCEPT{
            return end();
        }
        const_reverse_iterator rbegin() const _YXXX_NOEXCEPT{
            return end();
        }
        reverse_iterator rend() _YXXX_NOEXCEPT{
            return begin();
        }
        const_reverse_iterator rend() const _YXXX_NOEXCEPT{
            return begin();
        }
        const_reverse_iterator crbegin() const _YXXX_NOEXCEPT{
            return cend();
        }
        const_reverse_iterator crend() const _YXXX_NOEXCEPT{
            return cbegin();
        }

        key_compare key_comp() const _YXXX_NOEXCEPT{
            return _M_comp;
        }

        iterator find(const key_type& _key) _YXXX_NOEXCEPT{
            return _M_find(_key, _M_root);
        }
        const_iterator find(const key_type& _key) const _YXXX_NOEXCEPT{
            return _M_find(_key, _M_root);
        }
        std::pair<iterator, iterator>
        equal_range(const key_type& _key) _YXXX_NOEXCEPT{
            iterator _begin = _M_find(_key, _M_root);
            if(end() == _begin) return {end(), end()};
            iterator _end = _begin;
            if _YXXX_CONSTEXPR(!_UniqueKey){
                iterator _tmp_it = end();
                while(end() != (_tmp_it = _M_find(
                    _key, _begin._M_ptr->left()))){
                    _begin = _tmp_it;
                }
                while(end() != (_tmp_it = _M_find(
                    _key, _end._M_ptr->right()))){
                    _end = _tmp_it;
                }
            }
            return {_begin, ++_end};
        }
        std::pair<const_iterator, const_iterator>
        equal_range(const key_type& _key) const _YXXX_NOEXCEPT{
            const_iterator _begin = _M_find(_key, _M_root);
            if(end() == _begin) return {end(), end()};
            const_iterator _end = _begin;
            if _YXXX_CONSTEXPR(!_UniqueKey){
                const_iterator _tmp_it = end();
                while(end() != (_tmp_it = _M_find(
                    _key, _begin._M_ptr->left()))){
                    _begin = _tmp_it;
                }
                while(end() != (_tmp_it = _M_find(
                    _key, _end._M_ptr->right()))){
                    _end = _tmp_it;
                }
            }
            return {_begin, ++_end};
        }

        bool empty() const _YXXX_NOEXCEPT{return 0 == _M_size;}
        size_type size() const _YXXX_NOEXCEPT{return _M_size;}
        size_type count(const key_type& _key) const _YXXX_NOEXCEPT{
            const_iterator _begin = _M_find(_key, _M_root);
            if(end() == _begin) return 0;
            if _YXXX_CONSTEXPR(_UniqueKey) return 1;
            size_type _count = 1;
            const_iterator _end = _begin;
            const_iterator _tmp_it = end();
            while(end() != (_tmp_it = _M_find(
                _key, _begin._M_ptr->left()))){
                _begin = _tmp_it;
                _count += _M_node_count(
                    _begin._M_ptr->right()) + 1;
            }
            while(end() != (_tmp_it = _M_find(
                _key, _end._M_ptr->right()))){
                _end = _tmp_it;
                _count += _M_node_count(
                    _end._M_ptr->left()) + 1;
            }
            return _count;
        }
        void clear() _YXXX_NOEXCEPT{
            _M_release(_M_root);
            _M_root = nullptr;
            _M_size = 0;
        }

        friend void
        swap(self_type& _lt, self_type& _rt) _YXXX_NOEXCEPT{
            std::swap(_lt._M_allocator, _rt._M_allocator);
            std::swap(_lt._M_comp, _rt._M_comp);
            std::swap(_lt._M_root, _rt._M_root);
            std::swap(_lt._M_size, _rt._M_size);
        }
    protected:
        template <typename ...Args>
        void _M_init_root(Args&& ..._args){
            _M_root = this->_M_construct_node(std::forward<Args>(_args)...);
            _M_root->color(color_type::_Black);
            ++_M_size;
        }
        void _M_release(_node_pointer _ptr) _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return;
            _M_release(_ptr->left());
            _M_release(_ptr->right());
            this->_M_destroy_node(_ptr);
            --_M_size;
        }

        template <typename _Iterator>
        void _M_copy_or_assign(_Iterator _iter, _Iterator _end){
            iterator _it = begin(), _it_end = end();
            while(_it != _it_end && _iter != _end)
                *(_it++) = *(_iter++);
            if(_it != _it_end) _M_erase(_it, _it_end);
            while(_iter != _end){
                _M_emplace(_iter);
                ++_iter;
            }
        }

        template <typename _Func>
        void _M_preorder(_node_pointer _ptr, _Func&& _f) const _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return;
            _M_preorder(_ptr->left(), _f);
            _f(_ptr->value());
            _M_preorder(_ptr->right(), _f);
        }
        template <typename _Func>
        void _M_postorder(_node_pointer _ptr, _Func&& _f) const _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return;
            _M_postorder(_ptr->left(), _f);
            _M_postorder(_ptr->right(), _f);
            _f(_ptr->value());
        }
        template <typename _Func>
        void _M_inorder(_node_pointer _ptr, _Func&& _f) const _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return;
            _M_inorder(_ptr->left(), _f);
            _f(_ptr->value());
            _M_inorder(_ptr->right(), _f);
        }
        template <typename _Func>
        void _M_levelorder(_node_pointer _ptr, _Func&& _f) const _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return;
            stl::queue<_node_pointer, _alloc::alloc_rebind<
                    allocator_type, _node_pointer>>
            _deq{_M_allocator};
            _deq.emplace_back(_ptr);
            _node_pointer _tmp = nullptr;
            while(!_deq.empty()){
                _tmp = _deq.front();
                _deq.pop_front();
                if(_tmp->left()) _deq.emplace_back(_tmp->left());
                if(_tmp->right()) _deq.emplace_back(_tmp->right());
                _f(_tmp->value());
            }
        }

        void _M_after_insert(_node_pointer _ptr) _YXXX_NOEXCEPT{
            if(_ptr->color() || !_ptr->prev()
                || _ptr->prev()->color()){
                _M_root->color(color_type::_Black);
                return;
            }
            // 根节点一定为黑，因此红色的父节点必不为根节点，必然存在父节点
            _node_pointer _pnt = _ptr->prev(), _gdp = _pnt->prev();
            _RotateMethod _gdp_rotate{_gdp, _gdp->right() == _pnt};
            _node_pointer _unc = _node_pointer(_gdp_rotate.inner_node());
            {
                _RotateMethod _pnt_rotate{_pnt, _pnt->right() == _ptr};
                if(!same_rotate(_pnt_rotate, _gdp_rotate)){
                    _pnt_rotate.rotate();
                    std::swap(_ptr, _pnt);
                }
            }
            swap_color(_pnt, _gdp);
            if(_unc->color()){
                _base_pointer _tmp = _gdp_rotate.rotate();
                if(_M_root == _gdp) _M_root = _node_pointer(_tmp);
                _M_root->color(color_type::_Black);
            }
            else{
                _unc->color(color_type::_Black);
                _M_after_insert(_gdp);
            }
        }
        insert_type _M_insert(const key_type& _key,
            _node_pointer _ist, _node_pointer _ptr) _YXXX_NOEXCEPT{
            if(nullptr == _ptr)
                return {iterator{_M_root, _ptr}, false};
            key_type _ptr_key = _ExtractKey{}(_ptr->value());
            if(_M_comp(_ptr_key, _key)){
                if(_ptr->right())
                    return _M_insert(_key, _ist, _ptr->right());
                _ptr->right(_ist);
                ++_M_size;
                _M_after_insert(_ist);
                return {iterator{_M_root, _ist}, true};
            }
            else{
                if(!_M_comp(_key, _ptr_key))
                    if _YXXX_CONSTEXPR(_UniqueKey)
                        return {iterator{_M_root, _ptr}, false};
                if(_ptr->left())
                    return _M_insert(_key, _ist, _ptr->left());
                _ptr->left(_ist);
                ++_M_size;
                _M_after_insert(_ist);
                return {iterator{_M_root, _ist}, true};
            }
        }
        template<typename ...Args>
        insert_type _M_emplace(Args&& ..._args){
            if(empty()){
                _M_init_root(std::forward<Args>(_args)...);
                return {iterator{_M_root, _M_root}, true};
            }
            _node_pointer _ist = this->_M_construct_node(
                std::forward<Args>(_args)...);
            key_type _key = _ExtractKey{}(_ist->value());
            return _M_insert(_key, _ist, _M_root);
        }
        
        // 移除一个节点
        //   1.(若存在)交换当前节点与子树内前驱节点
        //   2.删除节点
        void _M_remove(_node_pointer _ptr) _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return;
            if(_node_pointer _prev = _ptr->left()->rightmost()){
                _ptr->_M_set_value(std::move(_prev->value()));
                _ptr = _prev;
            }

            // 根节点
            if(_M_root == _ptr){
                _M_root = _ptr->right();
                _M_root->_M_set_prev(nullptr);
                this->_M_destroy_node(_ptr);
                return;
            }

            // 移除节点，唯一子树向上补充
            _node_pointer _pnt = _ptr->prev(), _chd = _ptr->left();
            color_type _color = _ptr->color();
            if(nullptr == _chd) _chd = _ptr->right();
            if(_pnt->left() == _ptr)
                _pnt->left(_chd);
            else
                _pnt->right(_chd);
            this->_M_destroy_node(_ptr);
            --_M_size;
            if(!_color) return;

            _RotateMethod _pnt_rotate{_pnt, _pnt->left() == _chd};
            _node_pointer _bro = nullptr;
            while(1){
                _bro = _pnt_rotate.outer_node();
                if(!_bro->color()){
                    // 红色兄弟节点
                    // 旋转得到黑色兄弟节点
                    // 此处将红色转移到父节点，确保了即使兄弟节点没有红色子树
                    //   父节点是红节点，能够迅速回溯搜索到红节点
                    swap_color(_pnt_rotate.pivot(), _bro);
                    _base_pointer _tmp = _pnt_rotate.rotate();
                    if(_pnt_rotate.pivot() == _M_root)
                        _M_root = _node_pointer(_tmp);
                    _bro = _pnt_rotate.outer_node();
                }
                if(_bro->color() && _bro->color()){
                    // 兄弟节点无红色子节点
                    // 则只能减少一个黑色节点
                    // 向上回溯，维护树
                    // 遇到黑色节点，视为一侧子树删除了一个节点，并补充了一个黑节点子树
                    //   从另一子树抽取一个红色节点，或继续向上回溯
                    // 遇到红色节点，视为一侧子树删除了一个节点，并补充了一个红节点子树
                    //   直接退出回溯，将红节点涂黑
                    _bro->color(color_type::_Red);
                    if(_pnt_rotate.pivot() == _M_root) return;
                    _chd = _pnt_rotate.pivot();
                    if(!_chd->color()) break;
                    _pnt_rotate.pivot(_pnt_rotate.prev());
                    _pnt_rotate.left_rotate(_pnt_rotate.left() == _chd);
                }
                else{
                    if(_pnt_rotate.like_outer(_bro)->color()){
                        _RotateMethod{_bro, !_pnt_rotate.left_rotate()}.rotate();
                        swap_color(_bro, _bro->prev());
                        _bro = _bro->prev();
                    }
                    swap_color(_bro, _pnt_rotate.prev());
                    _base_pointer _tmp = _pnt_rotate.rotate();
                    if(_pnt_rotate.pivot() == _M_root)
                        _M_root = _node_pointer(_tmp);
                    break;
                }
            }
            _chd->color(color_type::_Black);
        }
        void _M_erase(const key_type& _key, _node_pointer _ptr) _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return;
            key_type _ptr_key = _ExtractKey{}(_ptr->value());

            if(_M_comp(_key, _ptr_key)){
                _M_erase(_key, _ptr->left());
            }
            else if(_M_comp(_ptr_key, _key)){
                _M_erase(_key, _ptr->right());
            }
            else{
                _M_erase(_key, _ptr->left());
                _M_erase(_key, _ptr->right());
                // 删除当前节点
                _M_remove(_ptr);
            }
        }
        void _M_erase(iterator _iter, iterator _end) _YXXX_NOEXCEPT{
            while(_iter != _end){
                _M_remove(_iter._M_ptr);
            }
        }

        iterator _M_find(const key_type& _key,
            _node_pointer _ptr) _YXXX_NOEXCEPT{
            while(_ptr){
                key_type _ptr_key = _ExtractKey{}(_ptr->value());
                if(_M_comp(_key, _ptr_key)){
                    _ptr = _ptr->left();
                }
                else if(_M_comp(_ptr_key, _key)){
                    _ptr = _ptr->right();
                }
                else break;
            }
            return {_M_root, _ptr};
        }
        const_iterator _M_find(const key_type& _key,
            _node_pointer _ptr) const _YXXX_NOEXCEPT{
            while(_ptr){
                key_type _ptr_key = _ExtractKey{}(_ptr->value());
                if(_M_comp(_key, _ptr_key)){
                    _ptr = _ptr->left();
                }
                else if(_M_comp(_ptr_key, _key)){
                    _ptr = _ptr->right();
                }
                else break;
            }
            return {_M_root, _ptr};
        }
        size_type _M_node_count(_node_pointer _ptr) const _YXXX_NOEXCEPT{
            size_type _count = 0;
            _M_preorder(_ptr, [&_count](const value_type&){_count += 1;});
            return _count;
        }

    public:
        allocator_type _M_allocator;
        _node_pointer _M_root;
        size_type _M_size;
        key_compare _M_comp;
    };
};

#endif