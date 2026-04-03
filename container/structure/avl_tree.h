#ifndef _YXXX_AVL_TREE_H_
#define _YXXX_AVL_TREE_H_

#include "./tree_base.h"
#include "../../iterator.h"

namespace _tree{
    template <typename _Type>
    struct _Avl_tree_node;
    template <typename _Type>
    struct _node_height_base{
    protected:
        using _node_type    = _Avl_tree_node<_Type>;
        using _node_pointer = _node_type*;
    public:
        _node_height_base() _YXXX_NOEXCEPT: _M_height(0){}

        stl::size_t height() const _YXXX_NOEXCEPT{
            if(nullptr == _node_pointer(this)) return 0;
            return _M_height;
        }
        void update_height() _YXXX_NOEXCEPT{
            if(nullptr == _node_pointer(this)) return;
            _node_pointer _this = static_cast<_node_pointer>(this);
            stl::size_t _h = 1 + std::max(
                _node_pointer(_this->_M_left)->height(),
                _node_pointer(_this->_M_right)->height());
            if(_h == _M_height) return;
            _M_height = _h;
            _node_pointer(_this->_M_prev)->update_height();
        }
    protected:
        stl::size_t _M_height;
    };
    
    template <typename _Type>
    struct _Avl_tree_node:
        protected _tree_node_base<_Type>,
        public _node_height_base<_Type>{
    public:
        typedef _tree_node_base<_Type>   base_type;
        typedef typename base_type::value_type    value_type;
        typedef typename base_type::pointer       pointer;
        typedef typename base_type::const_pointer const_pointer;
        typedef typename base_type::reference     reference;
        typedef typename base_type::const_reference  const_reference;
        typedef typename base_type::size_type     size_type;
        typedef typename base_type::difference_type  difference_type;

        typedef _Avl_tree_node             self_type;
        typedef self_type*               self_pointer;

        friend class _node_height_base<_Type>;

        _Avl_tree_node(const value_type& _data = value_type(),
            self_pointer _prev = nullptr,
            self_pointer _left = nullptr,
            self_pointer _right = nullptr):
            base_type(_data, _prev, _left, _right){
            this->update_height();
        }

        const_reference value() const _YXXX_NOEXCEPT{return this->_M_value;}
        self_pointer prev() const _YXXX_NOEXCEPT{return self_pointer(this->_M_prev);}
        self_pointer left() const _YXXX_NOEXCEPT{return self_pointer(this->_M_left);}
        self_pointer right() const _YXXX_NOEXCEPT{return self_pointer(this->_M_right);}
        void left(self_pointer _p) _YXXX_NOEXCEPT{
            if(this->_M_left) this->_M_left->_M_prev = nullptr;
            this->_M_left = _p;
            self_pointer(this->_M_left)->prev(this);
        }
        void right(self_pointer _p) _YXXX_NOEXCEPT{
            if(this->_M_right) this->_M_right->_M_prev = nullptr;
            this->_M_right = _p;
            self_pointer(this->_M_right)->prev(this);
        }

        bool right_overload(stl::size_t _th) const _YXXX_NOEXCEPT{
            return this ?
                self_pointer(this->_M_right)->height()
                    > (_th + self_pointer(this->_M_left)->height()) :
                false;
        }
        bool left_overload(stl::size_t _th) const _YXXX_NOEXCEPT{
            return this ?
                self_pointer(this->_M_left)->height()
                    > (_th + self_pointer(this->_M_right)->height()) :
                false;
        }
        self_pointer extract_left() _YXXX_NOEXCEPT{
            self_pointer _rtn = self_pointer(this->_M_left);
            if(_rtn) this->left(nullptr);
            return _rtn;
        }
        self_pointer extract_right() _YXXX_NOEXCEPT{
            self_pointer _rtn = self_pointer(this->_M_right);
            if(_rtn) this->right(nullptr);
            return _rtn;
        }

        self_pointer leftmost() _YXXX_NOEXCEPT{
            if(nullptr == this) return nullptr;
            self_pointer _ptr = this;
            while(_ptr->left()) _ptr = _ptr->left();
            return _ptr;
        }
        self_pointer rightmost() _YXXX_NOEXCEPT{
            if(nullptr == this) return nullptr;
            self_pointer _ptr = this;
            while(_ptr->right()) _ptr = _ptr->right();
            return _ptr;
        }
        
        self_pointer successor() _YXXX_NOEXCEPT{
            if(nullptr == this) return nullptr;
            if(this->_M_right)
                return self_pointer(this->_M_right)->leftmost();
            if(this->_M_prev && this->_M_prev->_M_left == this)
                return self_pointer(this->_M_prev);
            return nullptr;
        }
        self_pointer predecessor() _YXXX_NOEXCEPT{
            if(nullptr == this) return nullptr;
            if(this->_M_left)
                return self_pointer(this->_M_left)->rightmost();
            if(this->_M_prev
                && this->_M_prev->_M_right == this)
                return self_pointer(this->_M_prev);
            return nullptr;
        }
    protected:
        void prev(self_pointer _p) _YXXX_NOEXCEPT{
            if(this) this->_M_prev= _p;
            if(_p) _p->update_height();
        }
    };

    template <typename _ValueType, typename _Comp,
        typename _Alloc>
    class _Avl_tree{
    public:
        typedef _ValueType       value_type;
        typedef _Comp            key_compare;
        typedef _Alloc           allocator_type;

        using size_type      = stl::size_t;
    protected:
        using _node_type     = _Avl_tree_node<value_type>;
        using _node_pointer  = _node_type*;
        using _node_alloc    = _alloc::alloc_rebind<
            allocator_type, _node_type>;

        // template <bool _Const>
        // class _tree_iterator{
        // public:
        //     using value_type           = _Type;
        //     using pointer              = std::conditional_t<
        //         _Const, const value_type*, value_type*>;
        //     using reference           = std::conditional_t<
        //         _Const, const value_type&, value_type&>;
        //     using iterator_category    = std::bidirectional_iterator_tag;
        //     using difference_type      = std::ptrdiff_t;
        //     using self_type            = _tree_iterator;
        // 
        //     _tree_iterator(_node_pointer _ptr, const AVLTree& _tree)
        //         _YXXX_NOEXCEPT: _M_ptr(_ptr), _M_tree(_tree){}
        //     _tree_iterator(const _tree_iterator& _it)
        //         _YXXX_NOEXCEPT: _M_ptr(_it._M_ptr), _M_tree(_it._M_tree){}
        // 
        //     reference operator*() const _YXXX_NOEXCEPT{return _M_ptr->value();}
        //     pointer operator->() const _YXXX_NOEXCEPT{return &operator*();}
        //     self_type& operator++() _YXXX_NOEXCEPT{
        //         if(_M_ptr)
        //             _M_ptr = _M_ptr->successor();
        //         else
        //             _M_ptr = _M_tree->leftmost();
        //         return *this;
        //     }
        //     self_type operator++(int) _YXXX_NOEXCEPT{
        //         self_type _rtn(*this);
        //         ++(*this);
        //         return _rtn;
        //     }
        //     self_type& operator--() _YXXX_NOEXCEPT{
        //         if(_M_ptr)
        //             _M_ptr = _M_ptr->predecessor();
        //         else
        //             _M_ptr = _M_tree->rightmost();
        //         return *this;
        //     }
        //     self_type operator--(int) _YXXX_NOEXCEPT{
        //         self_type _rtn(*this);
        //         --(*this);
        //         return _rtn;
        //     }
        //     friend bool operator==(const self_type& _lhs,
        //         const self_type& _rhs) _YXXX_NOEXCEPT{
        //         return _lhs._M_ptr == _rhs._M_ptr;
        //     }
        //     friend bool operator!=(const self_type& _lhs,
        //         const self_type& _rhs) _YXXX_NOEXCEPT{
        //         return !(_lhs == _rhs);
        //     }
        //     friend bool operator<(const self_type& _lhs,
        //         const self_type& _rhs) _YXXX_NOEXCEPT;
        // protected:
        //     _node_pointer _M_ptr;
        //     AVLTree& _M_tree;
        // };

    public:
        _Avl_tree(
            const key_compare& _comp = key_compare(),
            const allocator_type& _alloc = allocator_type()) _YXXX_NOEXCEPT:
            _M_alloc(_alloc), _M_root(nullptr),
            _M_size(0), _M_comp(_comp){}
        ~_Avl_tree() _YXXX_NOEXCEPT{
            _M_release(_M_root);
            _M_root = nullptr;
        }

        void insert(const value_type& _val) _YXXX_NOEXCEPT{
            _M_emplace(_val);
        }
        void insert(value_type&& _val) _YXXX_NOEXCEPT{
            _M_emplace(std::move(_val));
        }
        template <typename ...Args>
        void emplace(Args&& ..._args){
            _M_emplace(std::forward<Args>(_args)...);
        }
        void erase(const value_type& _val) _YXXX_NOEXCEPT{
            _M_erase(_val, _M_root);
        }

        bool find(const value_type& _val) const _YXXX_NOEXCEPT{
            return _M_find(_val, _M_root);
        }
        template <typename _Func>
        void preorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_preorder(_M_root, std::forward<_Func>(_f));
        }
        template <typename _Func>
        void postorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_postorder(_M_root, _f);
        }
        template <typename _Func>
        void inorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_inorder(_M_root, _f);
        }
        template <typename _Func>
        void levelorder(_Func&& _f) const _YXXX_NOEXCEPT{
            _M_levelorder(_M_root, _f);
        }

        size_type empty() const _YXXX_NOEXCEPT{return 0 == _M_size;}
        void clear() _YXXX_NOEXCEPT{
            _M_release(_M_root);
            _M_root = nullptr;
        }

    private:
        template <typename ...Args>
        _node_pointer _M_construct_node(Args&& ...args){
            _node_pointer _ptr = _M_alloc.allocate(1);
            _M_alloc.construct(_ptr, std::forward<Args>(args)...);
            return _ptr;
        }
        template <typename ...Args>
        void _M_init_root(Args&& ..._args){
            _M_root = _M_construct_node(std::forward<Args>(_args)...);
            ++_M_size;
        }
        void _M_release(_node_pointer _ptr) _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return;
            _M_release(_ptr->left());
            _M_release(_ptr->right());
            _M_alloc.destroy(_ptr);
            _M_alloc.deallocate(_ptr);
            --_M_size;
        }
        
        _node_pointer _M_find(const value_type& _val,
            _node_pointer _ptr) const _YXXX_NOEXCEPT{
            while(_ptr){
                if(_M_comp(_val, _ptr->value()))
                    _ptr = _ptr->left();
                else if(_M_comp(_ptr->value(), _val))
                    _ptr = _ptr->right();
                else
                    return _ptr;
            }
            return _ptr;
        }
        void _M_replace(_node_pointer _ptr, _node_pointer _sub) _YXXX_NOEXCEPT{
            if(_node_pointer _pnt = _ptr->prev()){
                if(_pnt->left() == _ptr)_pnt->left(_sub);
                else _pnt->right(_sub);
            }
            else{
                _M_root = _sub;
            }
        }
        
        void _M_left_rotate(_node_pointer _ptr) _YXXX_NOEXCEPT{
            if(nullptr == _ptr || nullptr == _ptr->right())
                return;
            _node_pointer _npt = _ptr->right();
            _ptr->right(_npt->extract_left());
            _M_replace(_ptr, _npt);
            _npt->left(_ptr);
        }
        void _M_right_rotate(_node_pointer _ptr) _YXXX_NOEXCEPT{
            if(nullptr == _ptr || nullptr == _ptr->left())
                return;
            _node_pointer _npt = _ptr->left();
            _ptr->left(_npt->extract_right());
            _M_replace(_ptr, _npt);
            _npt->right(_ptr);
        }
        void _M_left_balance(_node_pointer _ptr) _YXXX_NOEXCEPT{
            if(!_ptr->left_overload(1)) return;
            if(_ptr->left()->right_overload(0))
                _M_left_rotate(_ptr->left());
            _M_right_rotate(_ptr);
        }
        void _M_right_balance(_node_pointer _ptr) _YXXX_NOEXCEPT{
            if(!_ptr->right_overload(1)) return;
            if(_ptr->right()->left_overload(0))
                // _ptr->right()->_M_right_rotate();
                _M_right_rotate(_ptr->right());
            _M_left_rotate(_ptr);
        }

        void _M_insert_node(_node_pointer _ist, _node_pointer _ptr) _YXXX_NOEXCEPT{
            if(_M_comp(_ist->value(), _ptr->value())){
                if(nullptr == _ptr->left()){
                    _ptr->left(_ist);
                    ++_M_size;
                }
                else{
                    _M_insert_node(_ist, _ptr->left());
                    _M_left_balance(_ptr);
                }
            }
            else{
                if(nullptr == _ptr->right()){
                    _ptr->right(_ist);
                    ++_M_size;
                }
                else{
                    _M_insert_node(_ist, _ptr->right());
                    _M_right_balance(_ptr);
                }
            }
        }
        template <typename ...Args>
        void _M_emplace(Args&& ..._args) _YXXX_NOEXCEPT{
            if(empty())
                _M_init_root(std::forward<Args>(_args)...);
            else
                _M_insert_node(
                    _M_construct_node(
                        std::forward<Args>(_args)...
                    ),
                    _M_root
                );
        }
        
        _node_pointer _M_extract_first(_node_pointer _ptr) _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return nullptr;
            while(_ptr->left()) _ptr = _ptr->left();
            _node_pointer _res = _ptr;
            _M_replace(_ptr, _ptr->extract_right());
            return _res;
        }
        _node_pointer _M_extract_last(_node_pointer _ptr) _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return nullptr;
            while(_ptr->right()) _ptr = _ptr->right();
            _node_pointer _res = _ptr;
            _M_replace(_ptr, _ptr->extract_left());
            return _res;
        }
        _node_pointer _M_extract_previous(_node_pointer _ptr) _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return nullptr;
            return _M_extract_last(_ptr->left());
        }
        _node_pointer _M_extract_next(_node_pointer _ptr) _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return nullptr;
            return _M_extract_first(_ptr->right());
        }
        void _M_erase(const value_type& _val, _node_pointer _ptr) _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return;
            if(_M_comp(_val, _ptr->value())){
                _M_erase(_val, _ptr->left());
                _M_right_balance(_ptr);
            }
            else if(_M_comp(_ptr->value(), _val)){
                _M_erase(_val, _ptr->right());
                _M_left_balance(_ptr);
            }
            // 移除节点
            else{
                _node_pointer _ext = nullptr;
                // 从更高子树提取数值最近的节点
                if(_ptr->left()->height() > _ptr->right()->height()){
                    _ext = _M_extract_previous(_ptr);
                }
                else if(_ptr->right()){
                    _ext = _M_extract_next(_ptr);
                }

                // 提取成功，移动目标节点左右子树
                if(_ext){
                    _node_pointer _tmp = nullptr;
                    if(_tmp = _ptr->left()){
                        _ptr->left(nullptr);
                        _ext->left(_tmp);
                    }
                    if(_tmp = _ptr->right()){
                        _ptr->right(nullptr);
                        _ext->right(_tmp);
                    }
                }
                // 提取的节点挂载到父节点
                _M_replace(_ptr, _ext);
                _M_release(_ptr);
            }
        }

        template <typename _Func>
        void _M_preorder(_node_pointer _ptr, _Func&& _f) const _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return;
            _f(_ptr->value());
            _M_preorder(_ptr->left(), _f);
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
            _deq{_M_alloc};
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

    protected:
        _node_alloc _M_alloc;
        _node_pointer _M_root;
        size_type _M_size;
        _Comp _M_comp;
    };
};

#endif