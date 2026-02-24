#ifndef _YXXX_ITERATOR_H_
#define _YXXX_ITERATOR_H_

#include "adaptive_c++config.h"

namespace stl{
    class input_iterator_tag{};
    class output_iterator_tag{};
    class forward_iterator_tag: public input_iterator_tag{};
    class bidirectional_iterator_tag: public forward_iterator_tag{};
    class random_access_iterator_tag: public bidirectional_iterator_tag{};

    template <typename _Iterator>
    class iterator_traits{
    public:
        typedef typename _Iterator::value_type         value_type;
        typedef typename _Iterator::pointer            pointer;
        typedef typename _Iterator::reference          reference;
        typedef typename _Iterator::difference_type    difference_type;
        typedef typename _Iterator::iterator_category  iterator_category;
    };
    template <typename T>
    struct iterator_traits<T*>{
        typedef T                                value_type;
        typedef T*                               pointer;
        typedef T&                               reference;
        typedef std::ptrdiff_t                   difference_type;
        typedef stl::random_access_iterator_tag       iterator_category;
    };
    template <typename T>
    struct iterator_traits<const T*>{
        typedef T                                value_type;
        typedef const T*                         pointer;
        typedef const T&                         reference;
        typedef std::ptrdiff_t                   difference_type;
        typedef stl::random_access_iterator_tag       iterator_category;
    };

};

namespace _iterator{
    template <typename _Iterator>
    class normal_iterator{
    public:
        typedef _Iterator       base_iterator;
        typedef stl::iterator_traits<base_iterator>    _base_traits;

        typedef typename _base_traits::iterator_category  iterator_category;
        typedef typename _base_traits::value_type         value_type;
        typedef typename _base_traits::difference_type    difference_type;
        typedef typename _base_traits::pointer            pointer;
        typedef typename _base_traits::reference          reference;

        typedef normal_iterator       self_type;

        normal_iterator() = default;
        normal_iterator(const base_iterator& _iter)
            _YXXX_NOEXCEPT: _M_iter(_iter){}
        normal_iterator(const self_type& _iterator)
            _YXXX_NOEXCEPT: _M_iter(_iterator._M_iter){}

        _YXXX_CONSTEXPR reference
        operator*() const _YXXX_NOEXCEPT{
            return *_M_iter;
        }
        _YXXX_CONSTEXPR pointer
        operator->() const _YXXX_NOEXCEPT{
            return _M_iter;
        }
        _YXXX_CONSTEXPR reference
        operator[](difference_type _offset)
        const _YXXX_NOEXCEPT{
            return _M_iter[_offset];
        }

        _YXXX_CONSTEXPR self_type
        operator++(int) _YXXX_NOEXCEPT{
            return self_type{_M_iter++};
        }
        _YXXX_CONSTEXPR self_type&
        operator++() _YXXX_NOEXCEPT{
            ++_M_iter;
            return *this;
        }
        _YXXX_CONSTEXPR self_type
        operator--(int) _YXXX_NOEXCEPT{
            return self_type{_M_iter--};
        }
        _YXXX_CONSTEXPR self_type&
        operator--() _YXXX_NOEXCEPT{
            --_M_iter;
            return *this;
        }

        _YXXX_CONSTEXPR self_type 
        operator+(difference_type _offset)
        const _YXXX_NOEXCEPT{
            return self_type{_M_iter + _offset};
        }
        friend _YXXX_CONSTEXPR self_type
        operator+(difference_type _offset, const self_type& _iter)
        _YXXX_NOEXCEPT{
            return _iter + _offset;
        }
        _YXXX_CONSTEXPR self_type
        operator-(difference_type _offset)
        const _YXXX_NOEXCEPT{
            return self_type{_M_iter - _offset};
        }
        _YXXX_CONSTEXPR self_type&
        operator+=(difference_type _offset)
        _YXXX_NOEXCEPT{
            _M_iter += _offset;
            return *this;
        }
        _YXXX_CONSTEXPR self_type&
        operator-=(difference_type _offset)
        _YXXX_NOEXCEPT{
            _M_iter -= _offset;
            return *this;
        }

        _YXXX_CONSTEXPR const base_iterator&
        base() const _YXXX_NOEXCEPT{
            return _M_iter;
        }
    protected:
        base_iterator _M_iter;
    };
    template <typename _Type>
    inline _YXXX_CONSTEXPR bool
    operator==(const normal_iterator<_Type>& _l_iter,
        const normal_iterator<_Type>& _r_iter)
    _YXXX_NOEXCEPT{
        return _l_iter.base() == _r_iter.base();
    }
    template <typename _Type>
    inline _YXXX_CONSTEXPR bool
    operator!=(const normal_iterator<_Type>& _l_iter,
        const normal_iterator<_Type>& _r_iter)
    _YXXX_NOEXCEPT{
        return _l_iter.base() != _r_iter.base();
    }
    template <typename _Type>
    inline _YXXX_CONSTEXPR bool
    operator<(const normal_iterator<_Type>& _l_iter,
        const normal_iterator<_Type>& _r_iter)
    _YXXX_NOEXCEPT{
        return _l_iter.base() < _r_iter.base();
    }
    template <typename _Type>
    inline _YXXX_CONSTEXPR bool
    operator>(const normal_iterator<_Type>& _l_iter,
        const normal_iterator<_Type>& _r_iter)
    _YXXX_NOEXCEPT{
        return _l_iter.base() > _r_iter.base();
    }
    template <typename _Type>
    inline _YXXX_CONSTEXPR bool
    operator<=(const normal_iterator<_Type>& _l_iter,
        const normal_iterator<_Type>& _r_iter)
    _YXXX_NOEXCEPT{
        return _l_iter.base() <= _r_iter.base();
    }
    template <typename _Type>
    inline _YXXX_CONSTEXPR bool
    operator>=(const normal_iterator<_Type>& _l_iter,
        const normal_iterator<_Type>& _r_iter)
    _YXXX_NOEXCEPT{
        return _l_iter.base() >= _r_iter.base();
    }
    template <typename _Type>
    inline _YXXX_CONSTEXPR
    typename normal_iterator<_Type>::difference_type 
    operator-(const normal_iterator<_Type>& _l_iter,
        const normal_iterator<_Type>& _r_iter)
    _YXXX_NOEXCEPT{
        return _l_iter.base() - _r_iter.base();
    }

    template <typename _Iterator>
    class reverse_iterator{
    public:
        typedef _Iterator       base_iterator;
        typedef stl::iterator_traits<base_iterator>    _base_traits;

        typedef typename _base_traits::value_type         value_type;
        typedef typename _base_traits::pointer            pointer;
        typedef typename _base_traits::reference          reference;
        typedef typename _base_traits::difference_type    difference_type;
        typedef typename _base_traits::iterator_category  iterator_category;

        typedef reverse_iterator       self_type;

        reverse_iterator(const base_iterator& _iter)
            _YXXX_NOEXCEPT: _M_iter(_iter){}
        reverse_iterator(const self_type& _iterator)
            _YXXX_NOEXCEPT: _M_iter(_iterator._M_iter){}

        _YXXX_CONSTEXPR reference
        operator*() const _YXXX_NOEXCEPT{
            base_iterator _tmp = _M_iter;
            return *--_tmp;
        }
        _YXXX_CONSTEXPR pointer
        operator->() const _YXXX_NOEXCEPT{
            base_iterator _tmp = _M_iter;
            --_tmp;
            return _M_pointer(_tmp);
        }
        _YXXX_CONSTEXPR reference
        operator[](difference_type _offset)
        const _YXXX_NOEXCEPT{
            return *(*this + _offset);
        }

        _YXXX_CONSTEXPR self_type
        operator++(int) _YXXX_NOEXCEPT{
            return self_type{_M_iter--};
        }
        _YXXX_CONSTEXPR self_type&
        operator++() _YXXX_NOEXCEPT{
            --_M_iter; return *this;
        }
        _YXXX_CONSTEXPR self_type
        operator--(int) _YXXX_NOEXCEPT{
            return self_type{_M_iter++};
        }
        _YXXX_CONSTEXPR self_type&
        operator--() _YXXX_NOEXCEPT{
            ++_M_iter; return *this;
        }

        _YXXX_CONSTEXPR self_type
        operator+(difference_type _offset)
        const _YXXX_NOEXCEPT{
            return self_type{_M_iter - _offset};
        }
        friend _YXXX_CONSTEXPR self_type
        operator+(difference_type _offset,
            const self_type& _iter)
        _YXXX_NOEXCEPT{
            return _iter + _offset;
        }
        _YXXX_CONSTEXPR self_type
        operator-(difference_type _offset)
        const _YXXX_NOEXCEPT{
            return self_type{_M_iter + _offset};
        }
        _YXXX_CONSTEXPR self_type&
        operator+=(difference_type _offset)
        _YXXX_NOEXCEPT{
            _M_iter -= _offset;
            return *this;
        }
        _YXXX_CONSTEXPR self_type&
        operator-=(difference_type _offset)
        _YXXX_NOEXCEPT{
            _M_iter += _offset;
            return *this;
        }

        _YXXX_CONSTEXPR base_iterator
        base() const
        _YXXX_NOEXCEPT_IF(_YXXX_NOEXCEPT(
            base_iterator(_M_iter))){
            return _M_iter;
        }
        
    protected:
        template <typename _Tp>
        static _YXXX_CONSTEXPR _Tp*
        _M_pointer(_Tp* _it) _YXXX_NOEXCEPT{
            return _it;
        }
        template <typename _Tp>
        static _YXXX_CONSTEXPR pointer
        _M_pointer(_Tp _it) _YXXX_NOEXCEPT{
            return _it.operator->();
        }

        base_iterator _M_iter;
    };
    template <typename _Type>
    inline _YXXX_CONSTEXPR bool
    operator==(const reverse_iterator<_Type>& _l_iter,
        const reverse_iterator<_Type>& _r_iter)
    _YXXX_NOEXCEPT{
        return _l_iter.base() == _r_iter.base();
    }
    template <typename _Type>
    inline _YXXX_CONSTEXPR bool
    operator!=(const reverse_iterator<_Type>& _l_iter,
        const reverse_iterator<_Type>& _r_iter)
    _YXXX_NOEXCEPT{
        return _l_iter.base() != _r_iter.base();
    }
    template <typename _Type>
    inline _YXXX_CONSTEXPR bool
    operator<(const reverse_iterator<_Type>& _l_iter,
        const reverse_iterator<_Type>& _r_iter)
    _YXXX_NOEXCEPT{
        return _l_iter.base() < _r_iter.base();
    }
    template <typename _Type>
    inline _YXXX_CONSTEXPR bool
    operator>(const reverse_iterator<_Type>& _l_iter,
        const reverse_iterator<_Type>& _r_iter)
    _YXXX_NOEXCEPT{
        return _l_iter.base() > _r_iter.base();
    }
    template <typename _Type>
    inline _YXXX_CONSTEXPR bool
    operator<=(const reverse_iterator<_Type>& _l_iter,
        const reverse_iterator<_Type>& _r_iter)
    _YXXX_NOEXCEPT{
        return _l_iter.base() <= _r_iter.base();
    }
    template <typename _Type>
    inline _YXXX_CONSTEXPR bool
    operator>=(const reverse_iterator<_Type>& _l_iter,
        const reverse_iterator<_Type>& _r_iter)
    _YXXX_NOEXCEPT{
        return _l_iter.base() >= _r_iter.base();
    }
    template <typename _Type>
    inline _YXXX_CONSTEXPR
    typename reverse_iterator<_Type>::difference_type 
    operator-(const reverse_iterator<_Type>& _l_iter,
        const reverse_iterator<_Type>& _r_iter)
    _YXXX_NOEXCEPT{
        return _l_iter.base() - _r_iter.base();
    }

    template <typename _NodePtr, bool _Const, typename _Container>
    class forward_node_iterator{
    public:
        using _node_ptr           = _NodePtr;
        using _node_type          = typename std::pointer_traits<_node_ptr>::element_type;
        using value_type          = typename _node_type::value_type;
        using pointer             = std::conditional_t<_Const, const value_type*, value_type*>;
        using reference           = std::conditional_t<_Const, const value_type&, value_type&>;
        using difference_type     = std::ptrdiff_t;
        using iterator_category   = stl::forward_iterator_tag;

        using self_type           = forward_node_iterator;

        friend _Container;

        forward_node_iterator(_NodePtr _ptr = nullptr)
            _YXXX_NOEXCEPT: _M_ptr(_ptr){}
        forward_node_iterator(const forward_node_iterator& _other)
            _YXXX_NOEXCEPT: _M_ptr(_other._M_ptr){}

        _YXXX_CONSTEXPR self_type&
        operator=(const forward_node_iterator& _other)
        _YXXX_NOEXCEPT{
            _M_ptr = _other._M_ptr;
            return *this;
        }
        _YXXX_CONSTEXPR reference
        operator*() const _YXXX_NOEXCEPT{
            return _M_ptr->value();
        }
        _YXXX_CONSTEXPR pointer
        operator->() const _YXXX_NOEXCEPT{
            return std::addressof(operator*());
        }
        _YXXX_CONSTEXPR self_type&
        operator++() _YXXX_NOEXCEPT{
            _M_ptr = _M_ptr->next();
            return *this;
        }
        _YXXX_CONSTEXPR self_type
        operator++(int) _YXXX_NOEXCEPT{
            self_type _tmp(*this);
            ++*this;
            return _tmp;
        }
        friend _YXXX_CONSTEXPR bool
        operator==(const forward_node_iterator& _lhs,
            const forward_node_iterator& _rhs)
        _YXXX_NOEXCEPT{
            return _lhs._M_ptr == _rhs._M_ptr;
        }
        friend _YXXX_CONSTEXPR bool
        operator!=(const forward_node_iterator& _lhs,
            const forward_node_iterator& _rhs)
        _YXXX_NOEXCEPT{
            return _lhs._M_ptr != _rhs._M_ptr;
        }
    protected:
        _node_ptr _M_ptr;
    };
    template <typename _NodePtr, bool _Const, typename _Container>
    class bidirectional_node_iterator{
    public:
        using _node_ptr           = _NodePtr;
        using _node_type          = typename std::pointer_traits<_node_ptr>::element_type;
        using value_type          = typename _node_type::value_type;
        using pointer             = std::conditional_t<_Const, const value_type*, value_type*>;
        using reference           = std::conditional_t<_Const, const value_type&, value_type&>;
        using difference_type     = std::ptrdiff_t;
        using iterator_category   = stl::bidirectional_iterator_tag;

        using self_type           = bidirectional_node_iterator;

        friend _Container;

        bidirectional_node_iterator(_NodePtr _ptr = nullptr)
            _YXXX_NOEXCEPT: _M_ptr(_ptr){}
        bidirectional_node_iterator(
            const bidirectional_node_iterator& _other)
            _YXXX_NOEXCEPT: _M_ptr(_other._M_ptr){}

        _YXXX_CONSTEXPR self_type&
        operator=(const bidirectional_node_iterator& _other)
        _YXXX_NOEXCEPT{
            _M_ptr = _other._M_ptr;
            return *this;
        }
        _YXXX_CONSTEXPR reference
        operator*() const _YXXX_NOEXCEPT{
            return _M_ptr->value();
        }
        _YXXX_CONSTEXPR pointer
        operator->() const _YXXX_NOEXCEPT{
            return std::addressof(operator*());
        }
        _YXXX_CONSTEXPR self_type&
        operator++() _YXXX_NOEXCEPT{
            _M_ptr = _M_ptr->next();
            return *this;
        }
        _YXXX_CONSTEXPR self_type
        operator++(int) _YXXX_NOEXCEPT{
            self_type _tmp(*this);
            ++*this;
            return _tmp;
        }
        _YXXX_CONSTEXPR self_type&
        operator--() _YXXX_NOEXCEPT{
            _M_ptr = _M_ptr->prev();
            return *this;
        }
        _YXXX_CONSTEXPR self_type
        operator--(int) _YXXX_NOEXCEPT{
            self_type _tmp(*this);
            --*this;
            return _tmp;
        }
        friend _YXXX_CONSTEXPR bool
        operator==(const bidirectional_node_iterator& _lhs,
            const bidirectional_node_iterator& _rhs)
        _YXXX_NOEXCEPT{
            return _lhs._M_ptr == _rhs._M_ptr;
        }
        friend _YXXX_CONSTEXPR bool
        operator!=(const bidirectional_node_iterator& _lhs,
            const bidirectional_node_iterator& _rhs)
        _YXXX_NOEXCEPT{
            return _lhs._M_ptr != _rhs._M_ptr;
        }
    protected:
        _node_ptr _M_ptr;
    };
};

#endif