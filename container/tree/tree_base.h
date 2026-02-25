#ifndef _YXXX_TREE_BASE_H_
#define _YXXX_TREE_BASE_H_

#include "../../allocator/allocator.h"

namespace _tree{
    template<typename _Type>
    struct _tree_node_base{
        typedef _Type          value_type;
        typedef _Type*         pointer;
        typedef const _Type*   const_pointer;
        typedef _Type&         reference;
        typedef const _Type&   const_reference;
        typedef stl::size_t    size_type;
        typedef std::ptrdiff_t difference_type;

        typedef _tree_node_base*  self_pointer;

        struct _RotateMethod{
            // 旋转方向指向的一侧为内侧
            _RotateMethod() _YXXX_NOEXCEPT: _M_ptr(nullptr), _M_left_rotate(true){}
            _RotateMethod(self_pointer _ptr, bool _left_rotate)
                _YXXX_NOEXCEPT: _M_ptr(_ptr), _M_left_rotate(_left_rotate){}

            self_pointer pivot() const _YXXX_NOEXCEPT{return _M_ptr;}
            void pivot(self_pointer _ptr) _YXXX_NOEXCEPT{_M_ptr = _ptr;}
            bool left_rotate() const _YXXX_NOEXCEPT{return _M_left_rotate;}
            void left_rotate(bool _b) _YXXX_NOEXCEPT{_M_left_rotate = _b;}

            self_pointer prev() const _YXXX_NOEXCEPT{return _M_ptr->_M_prev;}
            self_pointer left() const _YXXX_NOEXCEPT{return _M_ptr->_M_left;}
            self_pointer right() const _YXXX_NOEXCEPT{return _M_ptr->_M_right;}
            void left(self_pointer _ptr) _YXXX_NOEXCEPT{_M_ptr->_M_left = _ptr;}
            void right(self_pointer _ptr) _YXXX_NOEXCEPT{_M_ptr->_M_right = _ptr;}
            
            self_pointer inner_node() const _YXXX_NOEXCEPT{
                if(_M_left_rotate)
                    return _M_ptr->_M_left;
                else
                    return _M_ptr->_M_right;
            }
            self_pointer outer_node() const _YXXX_NOEXCEPT{
                if(_M_left_rotate)
                    return _M_ptr->_M_right;
                else
                    return _M_ptr->_M_left;
            }
            self_pointer rotate() _YXXX_NOEXCEPT{
                if(_M_left_rotate)
                    return _M_ptr->_M_left_rotate();
                else
                    return _M_ptr->_M_right_rotate();
            }

            friend bool same_rotate(
                const _RotateMethod& _lr, const _RotateMethod& _rr)_YXXX_NOEXCEPT{
                return _lr._M_left_rotate == _rr._M_left_rotate;
            }
            self_pointer like_inner(self_pointer _ptr) const _YXXX_NOEXCEPT{
                if(nullptr == _ptr){
                    return nullptr;
                }
                if(_M_left_rotate)
                    return _ptr->_M_left;
                else
                    return _ptr->_M_right;
            }
            self_pointer like_outer(self_pointer _ptr) const _YXXX_NOEXCEPT{
                if(nullptr == _ptr){
                    return nullptr;
                }
                if(_M_left_rotate)
                    return _ptr->_M_right;
                else
                    return _ptr->_M_left;
            }

            self_pointer _M_ptr;
            bool _M_left_rotate;
        };

        _tree_node_base(const value_type& _data = value_type(),
            self_pointer _prev = nullptr,
            self_pointer _left = nullptr,
            self_pointer _right = nullptr):
            _M_value(_data), _M_prev(_prev),
            _M_left(_left), _M_right(_right){}
        
        self_pointer _M_extract_left() _YXXX_NOEXCEPT{
            self_pointer _rtn = _M_left;
            _M_left = nullptr;
            if(_rtn) _rtn->_M_prev = nullptr;
            return _rtn; 
        }
        self_pointer _M_extract_right() _YXXX_NOEXCEPT{
            self_pointer _rtn = _M_right;
            _M_right = nullptr;
            if(_rtn) _rtn->_M_prev = nullptr;
            return _rtn; 
        }

        void _M_set_prev(self_pointer _ptr) _YXXX_NOEXCEPT{
            if(nullptr == this) return;
            if(self_pointer _tmp = _M_prev){
                if(_tmp->_M_left == this) _tmp->_M_set_left(nullptr);
                else _tmp->_M_set_right(nullptr);
            }
            this->_M_prev = _ptr;
        }
        self_pointer _M_set_left(self_pointer _ptr) _YXXX_NOEXCEPT{
            self_pointer _rtn = _M_extract_left();
            _ptr->_M_set_prev(this);
            _M_left = _ptr;
            return _rtn;
        }
        self_pointer _M_set_right(self_pointer _ptr) _YXXX_NOEXCEPT{
            self_pointer _rtn = _M_extract_right();
            _ptr->_M_set_prev(this);
            _M_right = _ptr;
            return _rtn;
        }

        self_pointer _M_left_rotate() _YXXX_NOEXCEPT{
            self_pointer _tmp = _M_prev;
            if(_tmp){
                if(_tmp->_M_left == this){
                    _tmp->_M_set_left(_M_right);
                    _tmp = _tmp->_M_left;
                }
                else{
                    _tmp->_M_set_right(_M_right);
                    _tmp = _tmp->_M_right;
                }
            }
            else{
                _tmp = _M_right;
            }

            _M_set_right(_tmp->_M_left);
            _tmp->_M_set_left(this);
            return _tmp;
        }
        self_pointer _M_right_rotate() _YXXX_NOEXCEPT{
            self_pointer _tmp = _M_prev;
            if(_tmp){
                if(_tmp->_M_left == this){
                    _tmp->_M_set_left(_M_left);
                    _tmp = _tmp->_M_left;
                }
                else{
                    _tmp->_M_set_right(_M_left);
                    _tmp = _tmp->_M_right;
                }
            }
            else{
                _tmp = _M_left;
            }

            _M_set_left(_tmp->_M_right);
            _tmp->_M_set_right(this);
            return _tmp;
        }
        
        self_pointer _M_leftmost() _YXXX_NOEXCEPT{
            if(nullptr == this) return nullptr;
            self_pointer _rtn = this;
            while(_rtn->_M_left) _rtn = _rtn->_M_left;
            return _rtn;
        }
        self_pointer _M_rightmost() _YXXX_NOEXCEPT{
            if(nullptr == this) return nullptr;
            self_pointer _rtn = this;
            while(_rtn->_M_right) _rtn = _rtn->_M_right;
            return _rtn;
        }

        self_pointer _M_successor() _YXXX_NOEXCEPT{
            if(nullptr == this) return nullptr;
            self_pointer _suc = this;
            if(_suc->_M_right){
                _suc = _suc->_M_right;
                while(_suc->_M_left) _suc = _suc->_M_left;
                return _suc;
            }
            while(_suc->_M_prev &&
                    _suc->_M_prev->_M_right == _suc)
                _suc = _suc->_M_prev;
            return _suc->_M_prev;
        }
        self_pointer _M_precursor() _YXXX_NOEXCEPT{
            if(nullptr == this) return nullptr;
            self_pointer _pre = this;
            if(_pre->_M_left){
                _pre = _pre->_M_left;
                while(_pre->_M_right) _pre = _pre->_M_right;
                return _pre;
            }
            while(_pre->_M_prev &&
                    _pre->_M_prev->_M_left == _pre)
                _pre = _pre->_M_prev;
            return _pre->_M_prev;
        }
        
        template <typename ..._Args>
        void _M_set_value(_Args&& ..._args) _YXXX_NOEXCEPT{
            _M_value = {std::forward<_Args>(_args)...};
        }

        value_type _M_value;
        self_pointer _M_prev, _M_left, _M_right;
    };

    template <typename _NodeType, typename _Alloc>
    class _tree_alloc_base{
    protected:
        using _node_alloc    = _alloc::alloc_rebind<
            _Alloc, _NodeType>;
        using _node_pointer  = typename _alloc::_alloc_traits<
            _node_alloc>::pointer;

        using _node_base     = typename _NodeType::base_type;
        using _base_pointer  = _node_base*;
    public:
        using allocator_type = _Alloc;

        template <typename ...Args>
        _node_pointer _M_construct_node(Args&& ..._args){
            _node_alloc _alloc = _node_alloc(_M_allocator);
            _node_pointer _ptr = _alloc.allocate(1);
            _alloc.construct(_ptr, std::forward<Args>(_args)...);
            return _ptr;
        }
        void _M_destroy_node(_node_pointer _ptr) _YXXX_NOEXCEPT{
            _node_alloc _alloc = _node_alloc(_M_allocator);
            _alloc.destroy(_ptr);
            _alloc.deallocate(_ptr);
        }
    protected:
        allocator_type _M_allocator;
    };
};

#endif