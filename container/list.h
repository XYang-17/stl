#ifndef _YXXX_LIST_H_
#define _YXXX_LIST_H_

#include <stdexcept>
#include "_container_base.h"

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
        typedef _list::_list_node<value_type> _node_type;

        typedef _alloc::alloc_rebind<allocator_type, _node_type> _node_alloc;

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

        list(const allocator_type& _alloc = allocator_type()) _YXXX_NOEXCEPT:
            _M_allocator(_alloc){
            _M_head = _M_allocator.allocate(1);
            _M_allocator.construct(_M_head, nullptr, nullptr);
            _M_head->prev() = _M_head->next() = _M_head;
        }
        list(const list& _list) _YXXX_NOEXCEPT: list(_list._M_allocator){
            _M_copy_or_assign(_list.begin(), _list.end());
        }
        list(list&& _list) _YXXX_NOEXCEPT{
            _M_move(_list);
        }
        list(std::initializer_list<value_type>&& _list) _YXXX_NOEXCEPT:
            list(){
            _M_move_or_assign(_list.begin(), _list.end());
        }
        template <typename _InputIterator>
        list(_InputIterator _first, _InputIterator _last,
            const allocator_type& _alloc = allocator_type()) _YXXX_NOEXCEPT:
            list(_alloc){
            _M_copy_or_assign(_first, _last);
        }
        ~list(){
            clear();
            _M_allocator.deallocate(_M_head);
            _M_head = nullptr;
        }

        self_type& operator=(const list& _list) _YXXX_NOEXCEPT{
            _M_copy_or_assign(_list.begin(), _list.end());
            return *this;
        }
        self_type& operator=(list&& _list) _YXXX_NOEXCEPT{
            _M_move(_list);
            return *this;
        }
        void push_back(const value_type& _value){
            _M_construct_node(end(), _value);
        }
        void push_back(value_type&& _value){
            _M_construct_node(end(), std::move(_value));
        }
        template <typename ...Args>
        void emplace_back(Args ...args){
            _M_construct_node(end(), std::forward<Args>(args)...);
        }
        void push_front(const value_type& _value){
            _M_construct_node(begin(), _value);
        }
        void push_front(value_type&& _value){
            _M_construct_node(begin(), std::move(_value));
        }
        template <typename ...Args>
        void emplace_front(Args ...args){
            _M_construct_node(begin(), std::forward<Args>(args)...);
        }
        void insert(iterator _pos_iter, const value_type& _value){
            _M_construct_node(_pos_iter, _value);
        }
        void insert(iterator _pos_iter, value_type&& _value){
            _M_construct_node(_pos_iter, std::move(_value));
        }
        template <typename ...Args>
        void emplace(iterator _pos_iter, Args ...args){
            _M_construct_node(_pos_iter, std::forward<Args>(args)...);
        }
        void pop_back() _YXXX_NOEXCEPT{
            _M_destroy_node(end()-1);
        }
        void pop_front() _YXXX_NOEXCEPT{
            _M_destroy_node(begin());
        }
        void erase(iterator _pos_iter) _YXXX_NOEXCEPT{
            if(end() == _pos_iter) return;
            _M_destroy_node(_pos_iter);
        }
        void erase(iterator _begin_pos, iterator _end_pos) _YXXX_NOEXCEPT{
            iterator _end = end();
            while(_end != _begin_pos && _end_pos != _begin_pos){
                _M_destroy_node(_begin_pos++);
            }
        }

        reference front() _YXXX_NOEXCEPT{return _M_head->next()->value();}
        const_reference front() const _YXXX_NOEXCEPT{return _M_head->next()->value();}
        reference back() _YXXX_NOEXCEPT{return _M_head->prev()->value();}
        const_reference back() const _YXXX_NOEXCEPT{return _M_head->prev()->value();}

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

        void reverse() _YXXX_NOEXCEPT{
            if(empty() || _M_head->next() = _M_head->prev()) return;
            _node_pointer _ptr = _M_head->next();
            while(_ptr != _M_head) std::swap(_ptr->prev(), _ptr->next());
            std::swap(_M_head->prev(), _M_head->next());            
        }
        friend void swap(list& _list1, list& _list2) _YXXX_NOEXCEPT{
            std::swap(_list1._M_head, _list2._M_head);
        }
        bool empty() const _YXXX_NOEXCEPT{return nullptr == _M_head || _M_head->next() == _M_head;}
        void clear() _YXXX_NOEXCEPT{if(!empty()) erase(begin(), end());}

        // void show() const _YXXX_NOEXCEPT{
        //     std::cout << "list->";
        //     for(const_iterator _iter = begin(); _iter != end(); ++_iter){
        //         std::cout << *_iter << "->";
        //     }
        // }
    
    private:
        void _M_move(self_type& _l) _YXXX_NOEXCEPT{
            clear();
            _M_allocator.deallocate(_M_head);
            _M_head = _l._M_head;
            _l._M_head = nullptr;
            _M_allocator = std::move(_l._M_allocator);
        }
        template <typename _InputIterator>
        void _M_copy_or_assign(_InputIterator _first, _InputIterator _last) _YXXX_NOEXCEPT{
            iterator _iter = begin(), _end = end();
            while(_iter != _end && _first != _last) *(_iter++) = *(_first++);
            if(_iter != _end) erase(_iter, _end);
            else{
                for(; _first != _last; ++_first){
                    push_back(*_first);
                }
            }
        }
        template <typename _InputIterator>
        void _M_move_or_assign(_InputIterator _first, _InputIterator _last) _YXXX_NOEXCEPT{
            iterator _iter = begin(), _end = end();
            while(_iter != _end && _first != _last) *(_iter++) = std::move(*(_first++));
            if(_iter != _end) erase(_iter, _end);
            else{
                for(; _first != _last; ++_first){
                    push_back(std::move(*_first));
                }
            }
        }

        template <typename ...Args>
        void _M_construct_node(iterator _pos, Args ...args) _YXXX_NOEXCEPT{
            _node_pointer _node = _pos._M_ptr;
            _M_allocator.construct(
                _M_allocator.allocate(1),
                _node->prev(),
                _node,
                value_type(std::forward<Args>(args)...)
            );
        }
        void _M_destroy_node(iterator _pos) _YXXX_NOEXCEPT{
            _node_pointer _node = _pos._M_ptr;
            _node->prev()->next() = _node->next();
            _node->next()->prev() = _node->prev();
            _M_allocator.destroy(_node);
            _M_allocator.deallocate(_node);
        }

    protected:
        _node_alloc _M_allocator;
        _node_pointer _M_head;
    };
};

#endif