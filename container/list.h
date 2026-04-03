#ifndef _YXXX_LIST_H_
#define _YXXX_LIST_H_

#include <stdexcept>
#include "./_container_base.h"

namespace stl{
    template <typename _Type, typename _Alloc>
    class list;
    namespace _list{
        class node_connection_error: public std::runtime_error{
        public:
            explicit node_connection_error(): std::runtime_error(
                "A dengerous node connection was detected."){}
        };

        template <typename _Type>
        class _list_node{
        public:
            typedef _Type          value_type;
            typedef std::allocator<value_type>              allocator_type;
            typedef _alloc::_alloc_traits<allocator_type>  _alloc_traits;

            typedef typename _alloc_traits::pointer          pointer;
            typedef typename _alloc_traits::const_pointer    const_pointer;
            typedef typename _alloc_traits::reference        reference;
            typedef typename _alloc_traits::const_reference  const_reference;
            typedef typename _alloc_traits::size_type        size_type;
            typedef typename _alloc_traits::difference_type  difference_type;

            typedef _list_node         self_type;
            typedef _list_node*        _node_pointer;

            _list_node(_list_node* _prev = nullptr,
                _list_node* _next = nullptr){
                if(_prev == nullptr){
                    if(_next == nullptr)
                        _init(nullptr, nullptr);
                    else
                        _init(_next->_M_prev, _next);
                }
                else{
                    if(_next == nullptr)
                        _init(_prev, _prev->_M_next);
                    else if((_prev->_M_next == nullptr
                                && _next->_M_prev == nullptr)
                            || (_prev->_M_next == _next
                                && _prev->_M_next->_M_prev == _prev)){
                        _init(_prev, _next);
                    }
                    else{
                        throw _list::node_connection_error();
                    }
                }
            }
            template <typename ...Args>
            _list_node(_list_node* _prev, _list_node* _next, Args ...args){
                if(_prev == nullptr){
                    if(_next == nullptr)
                        _init(nullptr, nullptr, std::forward<Args>(args)...);
                    else
                        _init(_next->_M_prev, _next, std::forward<Args>(args)...);
                }
                else{
                    if(_next == nullptr)
                        _init(_prev, _prev->_M_next, std::forward<Args>(args)...);
                    else if((_prev->_M_next == nullptr
                                && _next->_M_prev == nullptr)
                            || (_prev->_M_next == _next
                                && _prev->_M_next->_M_prev == _prev)){
                        _init(_prev, _next, std::forward<Args>(args)...);
                    }
                    else{
                        throw _list::node_connection_error();
                    }
                }
            }
            _list_node(const _list_node&) = delete;
            _list_node(_list_node&&) = delete;

            value_type& value() _YXXX_NOEXCEPT{
                return _M_value;
            }
            const value_type& value() const _YXXX_NOEXCEPT{
                return _M_value;
            }
            _node_pointer& next() _YXXX_NOEXCEPT{
                return _M_next;
            }
            const _node_pointer& next() const _YXXX_NOEXCEPT{
                return _M_next;
            }
            _node_pointer& prev() _YXXX_NOEXCEPT{
                return _M_prev;
            }
            const _node_pointer& prev() const _YXXX_NOEXCEPT{
                return _M_prev;
            }
        private:
            void _init(_list_node* _prev, _list_node* _next){
                _M_value = value_type();
                _M_prev = _prev;
                if(_M_prev != nullptr) _M_prev->_M_next = this;
                _M_next = _next;
                if(_M_next != nullptr) _M_next->_M_prev = this;
            }
            template <typename ...Args>
            void _init(_list_node* _prev, _list_node* _next,
                Args ...args){
                _M_value = value_type(std::forward<Args>(args)...);
                _M_prev = _prev;
                if(_M_prev != nullptr) _M_prev->_M_next = this;
                _M_next = _next;
                if(_M_next != nullptr) _M_next->_M_prev = this;
            }
        protected:
            value_type _M_value;
            _node_pointer _M_prev;
            _node_pointer _M_next;
        };
    };

    template <typename _Type, typename _Alloc = _alloc::allocator<_Type>>
    class list{
    public:
        typedef _Type          value_type;
        typedef _Alloc         allocator_type;
    protected:
        typedef _list::_list_node<value_type> _node_type;
        typedef _alloc::alloc_rebind<allocator_type, _node_type> _node_alloc;
    public:
        typedef typename _node_type::pointer          pointer;
        typedef typename _node_type::const_pointer    const_pointer;
        typedef typename _node_type::reference        reference;
        typedef typename _node_type::const_reference  const_reference;
        typedef typename _node_type::size_type        size_type;
        typedef typename _node_type::difference_type  difference_type;

        typedef typename _node_type::_node_pointer    _node_pointer;

        typedef list     self_type;
        using iterator               = _iterator::bidirectional_node_iterator<
            _node_pointer, false, self_type>;
        using const_iterator         = _iterator::bidirectional_node_iterator<
            _node_pointer, true, self_type>;
        using reverse_iterator       = _iterator::reverse_iterator<iterator>;
        using const_reverse_iterator = _iterator::reverse_iterator<const_iterator>;

        using return_type       = iterator;

    protected:
        struct _data{
            _node_pointer _begin;
            _node_pointer _over;
            size_type _size;

            template <typename ..._Args>
            _data(const _node_alloc& _alloc,
                size_type _n, _Args&& ..._args){
                // 0 != _n
                _over = _begin = _alloc.allocate(1);
                _alloc.construct(
                    _begin, nullptr, nullptr,
                    std::forward<_Args>(_args)...
                );
                while(--_n){
                    _alloc.construct(
                        _alloc.allocate(1),
                        _over, nullptr,
                        std::forward<_Args>(_args)...
                    );
                    _over = _over->next();
                }
                _size = _n;
            }
            template <typename _Iterator,
                typename = std::enable_if_t<stl::is_iterator_v<_Iterator>>>
            _data(const _node_alloc& _alloc,
                _Iterator _beg, _Iterator _end){
                // _beg != _end
                _over = _begin = _alloc.allocate(1);
                _alloc.construct(_begin, nullptr, nullptr, *_beg);
                _size = 1;
                while(++_beg != _end){
                    _alloc.construct(
                        _alloc.allocate(1),
                        _over, nullptr,
                        *_beg
                    );
                    ++_size;
                    _over = _over->next();
                }
            }
        };
    public:
        list() _YXXX_NOEXCEPT:
            _M_allocator(allocator_type{}){
            _M_construct();
        }
        list(const allocator_type& _alloc):
            _M_allocator(_alloc){
            _M_construct();
        }
        list(allocator_type&& _alloc):
            _M_allocator(std::move(_alloc)){
            _M_construct();
        }

        list(const list& _l):
            list(_l.begin(), _l.end(), _l._M_allocator){}
        list(const list& _l, const allocator_type& _alloc):
            list(_l.begin(), _l.end(), _alloc){}
        list(const list& _l, allocator_type&& _alloc):
            list(_l.begin(), _l.end(), std::move(_alloc)){}

        list(list&& _l):
            list(std::move(_l._M_allocator)){
            _M_move_data(std::move(_l));
        }
        list(list&& _l, const allocator_type& _alloc):
            list(_alloc){
            _M_move_data(std::move(_l));
        }
        list(list&& _l):
            list(std::move(_alloc)){
            _M_move_data(std::move(_l));
        }

        template <typename _Iterator,
            typename = std::enable_if_t<stl::is_iterator_v<_Iterator>>>
        list(_Iterator _beg, _Iterator _end):
            list(){
            _M_merge(_M_head, _data{_M_allocator, _beg, _end});
        }
        template <typename _Iterator,
            typename = std::enable_if_t<stl::is_iterator_v<_Iterator>>>
        list(_Iterator _beg, _Iterator _end,
            const allocator_type& _alloc):
            list(_alloc){
            _M_merge(_M_head, _data{_M_allocator, _beg, _end});
        }
        template <typename _Iterator,
            typename = std::enable_if_t<stl::is_iterator_v<_Iterator>>>
        list(_Iterator _beg, _Iterator _end,
            allocator_type&& _alloc):
            list(std::move(_alloc)){
            _M_merge(_M_head, _data{_M_allocator, _beg, _end});
        }

        list(std::initializer_list<value_type>&& _il):
            list(_il.begin(), _il.end()){}
        list(std::initializer_list<value_type>&& _il,
            const allocator_type& _alloc):
            list(_il.begin(), _il.end(), _alloc){}
        list(std::initializer_list<value_type>&& _il,
            allocator_type&& _alloc):
            list(_il.begin(), _il.end(), std::move(_alloc)){}

        // push_back/push_front
        return_type
        push_back(const value_type& _val){
            return _M_emplace(_M_head, _val);
        }
        return_type
        push_front(const value_type& _val){
            return _M_emplace(_M_head->next(), _val);
        }

        return_type
        push_back(value_type&& _val){
            return _M_emplace(_M_head, std::move(_val));
        }
        return_type
        push_front(value_type&& _val){
            return _M_emplace(_M_head->next(), std::move(_val));
        }

        return_type
        push_back(size_type _n, const value_type& _val){
            return _M_merge(_M_head,
                _data{_M_allocator, _n, _val});
        }
        return_type
        push_front(size_type _n, const value_type& _val){
            return _M_merge(_M_head->next(),
                _data{_M_allocator, _n, _val});
        }

        template <typename _Iterator>
        std::enable_if_t<
            stl::is_iterator_v<_Iterator>,
            return_type>
        push_back(_Iterator _beg, _Iterator _end){
            return _M_merge(_M_head,
                _data{_M_allocator, _beg, _end});
        }
        template <typename _Iterator>
        std::enable_if_t<
            stl::is_iterator_v<_Iterator>,
            return_type>
        push_front(_Iterator _beg, _Iterator _end){
            return _M_merge(_M_head->next(),
                _data{_M_allocator, _beg, _end});
        }
        
        // emplace_back/emplace_front
        template <typename ..._Args>
        return_type emplace_back(_Args&& ..._args){
            return _M_emplace(_M_head,
                std::forward<_Args>(_args)...);
        }
        template <typename ..._Args>
        return_type emplace_front(_Args&& ..._args){
            return _M_emplace(_M_head->next(),
                std::forward<_Args>(_args)...);
        }

        // insert
        return_type
        insert(iterator _pos, const value_type& _val){
            if(!_M_valid_insert(_pos)) return end();
            return _M_emplace(_pos._M_ptr, _val);
        }
        return_type
        insert(difference_type _idx, const value_type& _val){
            if(!_M_valid_insert(_idx)) return end();
            return _M_emplace(_M_ptr(_idx), _val);
        }

        return_type
        insert(iterator _pos, value_type&& _val){
            if(!_M_valid_insert(_pos)) return end();
            return _M_emplace(_pos._M_ptr, std::move(_val));
        }
        return_type
        insert(difference_type _idx, value_type&& _val){
            if(!_M_val_M_valid_insertid(_idx)) return end();
            return _M_emplace(_M_ptr(_idx), std::move(_val));
        }

        return_type
        insert(iterator _pos, size_type _n,
            const value_type& _val){
            if(!_M_valid_insert(_pos)) return end();
            return _M_merge(_pos._M_ptr,
                _data{_M_allocator, _n, _val});
        }
        return_type
        insert(difference_type _idx, size_type _n,
            const value_type& _val){
            if(!_M_valid_insert(_idx)) return end();
            return _M_merge(_M_ptr(_idx),
                _data{_M_allocator, _n, _val});
        }

        template <typename _Iterator>
        std::enable_if_t<
            stl::is_iterator_v<_Iterator>,
            return_type>
        insert(iterator _pos,
            _Iterator _beg, _Iterator _end){
            if(!_M_valid_insert(_pos)) return end();
            return _M_merge(_pos._M_ptr,
                _data{_M_allocator, _beg, _end});
        }
        template <typename _Iterator>
        std::enable_if_t<
            stl::is_iterator_v<_Iterator>,
            return_type>
        insert(difference_type _idx,
            _Iterator _beg, _Iterator _end){
            if(!_M_valid_insert(_idx)) return end();
            return _M_merge(_M_ptr(_idx),
                _data{_M_allocator, _beg, _end});
        }

        // emplace
        template <typename ..._Args>
        return_type
        emplace(iterator _pos, _Args&& ..._args){
            if(!_M_valid_insert(_pos)) return end();
            return _M_emplace(_pos._M_ptr,
                std::forward<_Args>(_args)...);
        }
        template <typename ..._Args>
        return_type
        emplace(difference_type _idx, _Args&& ..._args){
            if(!_M_valid_insert(_idx)) return end();
            return _M_emplace(_M_ptr(_idx),
                std::forward<_Args>(_args)...);
        }

        // erase
        return_type
        erase(iterator _pos){
            if(!_M_valid(_pos)) return end();
            return _M_erase(_pos._M_ptr);
        }
        return_type
        erase(difference_type _idx){
            if(!_M_valid(_idx)) return end();
            return _M_erase(_M_ptr(_idx));
        }

        return_type
        erase(iterator _beg, iterator _end){
            if(!_M_valid_range(_beg, _end)) return end();
            return _M_erase(_beg, _end);
        }
        return_type
        erase(difference_type _idx, size_type _n){
            if(!_M_valid(_idx)) return end();
            return _M_erase(_idx, _n);
        }

        reference front(){return _M_head->next()->value();}
        const_reference front() const{return _M_head->next()->value();}
        reference back(){return _M_head->prev()->value();}
        const_reference back() const{return _M_head->prev()->value();}

        iterator begin() _YXXX_NOEXCEPT{return iterator(_M_head->next());}
        const_iterator begin() const _YXXX_NOEXCEPT{return const_iterator(_M_head->next());}
        iterator end() _YXXX_NOEXCEPT{return iterator(_M_head);}
        const_iterator end() const _YXXX_NOEXCEPT{return const_iterator(_M_head);}
        const_iterator cbegin() const _YXXX_NOEXCEPT{return const_iterator(_M_head->next());}
        const_iterator cend() const _YXXX_NOEXCEPT{return const_iterator(_M_head);}
        reverse_iterator rbegin() _YXXX_NOEXCEPT{return reverse_iterator(end());}
        const_reverse_iterator rbegin() const _YXXX_NOEXCEPT{return const_reverse_iterator(end());}
        reverse_iterator rend() _YXXX_NOEXCEPT{return reverse_iterator(begin());}
        const_reverse_iterator rend() const _YXXX_NOEXCEPT{return const_reverse_iterator(begin());}
        const_reverse_iterator crbegin() const _YXXX_NOEXCEPT{return const_reverse_iterator(end());}
        const_reverse_iterator crend() const _YXXX_NOEXCEPT{return const_reverse_iterator(begin());}

        void unique(){
            _node_pointer _ptr = _M_head->next();
            while(_ptr != _M_head){
                if(_ptr->value == _ptr->next()->value())
                    _M_erase(_ptr->next());
                else
                    _ptr = _ptr->next();
            }
        }
        void unique(const value_type& _val){
            _node_pointer _ptr = _M_head->next();
            while(_ptr != _M_head){
                if(_ptr->value() == _val
                    && _ptr->value == _ptr->next()->value())
                    _M_erase(_ptr->next());
                else
                    _ptr = _ptr->next();
            }
        }

        void remove(const value_type& _val){
            _node_pointer _ptr = _M_head->next();
            while(_ptr != _M_head){
                _ptr = _ptr->next();
                if(_ptr->prev()->value() == _val)
                    _M_erase(_ptr->prev());
            }
        }
        template <typename _Predicate>
        void remove_if(const _Predicate& _pred){
            _node_pointer _ptr = _M_head->next();
            while(_ptr != _M_head){
                _ptr = _ptr->next();
                if(_pred(_ptr->prev()->value()))
                    _M_erase(_ptr->prev());
            }
        }

        void reverse() _YXXX_NOEXCEPT{
            if(_M_head->next() = _M_head->prev()) return;
            _node_pointer _ptr = _M_head->next();
            while(_ptr != _M_head) std::swap(_ptr->prev(), _ptr->next());
            std::swap(_M_head->prev(), _M_head->next());            
        }
        friend void swap(list& _list1, list& _list2) _YXXX_NOEXCEPT{
            std::swap(_list1._M_allocator, _list2._M_allocator);
            std::swap(_list1._M_head, _list2._M_head);
            std::swap(_list1._M_size, _list2._M_size);
        }

        bool empty() const _YXXX_NOEXCEPT{
            return _M_head->next() == _M_head;
        }
        size_type size() const _YXXX_NOEXCEPT{
            return _M_size;
        }
        void clear() _YXXX_NOEXCEPT{
            if(!empty()) erase(begin(), end());
        }
    
    protected:
        bool _M_valid(difference_type _idx)
        const _YXXX_NOEXCEPT{
            return size_type(_idx) < _M_size;
        }
        bool _M_valid(iterator _pos)
        const _YXXX_NOEXCEPT{
            if(_pos == _M_head) return false;
            _node_pointer _cur = _M_head->next();
            while(_cur != _M_head){
                if(_cur == _pos._M_ptr) return true;
                _cur = _cur->next();
            }
            return false;
        }
        bool _M_valid_insert(difference_type _idx)
        const _YXXX_NOEXCEPT{
            return size_type(_idx) <= _M_size;
        }
        bool _M_valid_insert(iterator _pos)
        const _YXXX_NOEXCEPT{
            if(_pos == _M_head) return true;
            _node_pointer _cur = _M_head->next();
            while(_cur != _M_head){
                if(_cur == _pos._M_ptr) return true;
                _cur = _cur->next();
            }
            return false;
        }
        bool _M_valid_range(iterator _beg, iterator _end)
        const _YXXX_NOEXCEPT{
            if(_end == _beg || !_M_valid(_beg)
                || _end._M_ptr == _beg._M_ptr->next())
                return false;
            if(_end._M_ptr == _M_head) return true;
            _node_pointer _cur = _beg._M_ptr->next();
            while(_cur != _M_head){
                if(_cur == _end._M_ptr) return true;
                _cur = _cur->next();
            }
            return false;
        }

        _node_pointer _M_ptr(difference_type _idx)
        const _YXXX_NOEXCEPT{
            if(_idx == _M_size) return _M_head;
            _node_pointer _ptr = _M_head->next();
            while(_idx--) _ptr = _ptr->next();
            return _ptr;
        }
        difference_type _M_idx(iterator _pos)
        const _YXXX_NOEXCEPT{
            difference_type _idx = 0;
            _node_pointer _ptr = _M_head->next();
            while(_ptr != _it._M_ptr){
                _ptr = _ptr->next();
                ++_idx;
            }
            return _idx;
        }

        void _M_construct(){
            _M_head = _M_allocator.allocate(1);
            _M_allocator.construct(
                _M_head, _M_head, _M_head, value_type{}
            );
            _M_size = 0;
        }

        void _M_move(list&& _l){
            _M_before_move();
            _M_allocator = std::move(_l._M_allocator);
            _M_move_data(std::move(_l));
        }
        void _M_move_data(list&& _l){
            _M_head->next() = _l._M_head->next();
            _M_head->prev() = _l._M_head->prev();
            _M_size = _l._M_size;
            _l._M_after_move();
        }
        void _M_before_move(){
            _M_erase(_M_head->next(), _M_head);
        }
        void _M_after_move(){
            _M_head->prev() = _M_head->next() = _M_head;
            _M_size = 0;
        }

        template <typename ...Args>
        return_type
        _M_emplace(_node_pointer _ptr, Args&& ...args){
            _node_pointer _ist = _M_allocator.allocate(1);
            _M_allocator.construct(
                _ist, nullptr, _ptr,
                value_type(std::forward<Args>(args)...)
            );
            ++_M_size;
            return _ist;
        }

        return_type
        _M_erase(_node_pointer _ptr)
        _YXXX_NOEXCEPT{
            _node_pointer _nxt = _ptr->next();
            _ptr->prev()->next() = _nxt;
            _nxt->prev() = _ptr->prev();
            _M_allocator.destroy(_ptr);
            _M_allocator.deallocate(_ptr, sizeof(_node_type));
            --_M_size;
            return _nxt;
        }
        return_type
        _M_erase(_node_pointer _beg, _node_pointer _end)
        _YXXX_NOEXCEPT{
            while(_beg != _end) _M_erase(_beg++);
            return _beg;
        }
        return_type
        _M_erase(_node_pointer _beg, size_type _n)
        _YXXX_NOEXCEPT{
            while(_n-- && _beg._M_ptr != _M_head)
                _M_erase(_beg++);
            return _beg;
        }

        return_type
        _M_merge(_node_pointer _ptr, _data _dat){
            _dat._begin->prev() = _ptr->prev();
            _ptr->prev()->next() = _dat._begin;
            _dat._end->next() = _ptr;
            _ptr->prev() = _dat._end;
            _M_size += _dat._size;
            return _dat._begin;
        }
    protected:
        _node_alloc _M_allocator;
        _node_pointer _M_head;
        size_type _M_size;
    };
};

#endif