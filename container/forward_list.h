#ifndef _YXXX_FORWARD_LIST_H_
#define _YXXX_FORWARD_LIST_H_

#include <stdexcept>
#include "./_container_base.h"

namespace stl{
    template <typename _Type, typename _Alloc>
    class forward_list;
    namespace _forward_list{
        class node_connection_error: public std::runtime_error{
        public:
            explicit node_connection_error(): std::runtime_error(
                "A dengerous node connection was detected."){}
        };
        
        template <typename _Type>
        class _forward_list_node{
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

            typedef _forward_list_node         self_type;
            typedef _forward_list_node*        _node_pointer;

            _forward_list_node(_forward_list_node* _prev = nullptr,
                _forward_list_node* _next = nullptr){
                if(_prev == nullptr){
                    if(_next == nullptr){
                        _init(nullptr, nullptr);
                    }
                    else{
                        _init( nullptr, _next);
                    }
                }
                else{
                    if(_next == nullptr){
                        _init(_prev, _prev->_M_next);
                    }
                    else if(_prev->_M_next == nullptr
                            || _prev->_M_next == _next){
                        _init(_prev, _next);
                    }
                    else{
                        throw node_connection_error();
                    }
                }}
            template <typename ...Args>
            _forward_list_node(
                _forward_list_node* _prev,
                _forward_list_node* _next,
                Args ...args){
                if(_prev == nullptr){
                    if(_next == nullptr){
                        _init(nullptr, nullptr, std::forward<Args>(args)...);
                    }
                    else{
                        _init( nullptr, _next, std::forward<Args>(args)...);
                    }
                }
                else{
                    if(_next == nullptr){
                        _init(_prev, _prev->_M_next, std::forward<Args>(args)...);
                    }
                    else if(_prev->_M_next == nullptr
                            || _prev->_M_next == _next){
                        _init(_prev, _next, std::forward<Args>(args)...);
                    }
                    else{
                        throw node_connection_error();
                    }
                }
            }
            _forward_list_node(const _forward_list_node&) = delete;
            _forward_list_node(_forward_list_node&&) = delete;
            
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
        private:
            void _init(
                _forward_list_node* _prev,
                _forward_list_node* _next){
                _M_value = value_type();
                if(_prev != nullptr) _prev->_M_next = this;
                _M_next = _next;
            }
            template <typename ...Args>
            void _init(
                _forward_list_node* _prev,
                _forward_list_node* _next,
                Args ...args){
                _M_value = value_type(std::forward<Args>(args)...);
                if(_prev != nullptr) _prev->_M_next = this;
                _M_next = _next;
            }
        protected:
            value_type _M_value;
            _node_pointer _M_next;
        };
    };

    template <typename _Type, typename _Alloc = _alloc::allocator<_Type>>
    class forward_list{
    public:
        typedef _Type          value_type;
        typedef _Alloc         allocator_type;
        typedef _forward_list::_forward_list_node<value_type>    _node_type;
        typedef _alloc::alloc_rebind<allocator_type, _node_type> _node_alloc;

        typedef typename _node_type::pointer          pointer;
        typedef typename _node_type::const_pointer    const_pointer;
        typedef typename _node_type::reference        reference;
        typedef typename _node_type::const_reference  const_reference;
        typedef typename _node_type::size_type        size_type;
        typedef typename _node_type::difference_type  difference_type;

        typedef typename _node_type::_node_pointer    _node_pointer;

        typedef forward_list     self_type;

        using iterator          = _iterator::forward_node_iterator<
            _node_pointer, false, self_type>;
        using const_iterator    = _iterator::forward_node_iterator<
            _node_pointer, true, self_type>;


        forward_list(const allocator_type& _alloc = allocator_type()) _YXXX_NOEXCEPT:
            _M_allocator(_alloc){
            _M_head = _M_construct_after(nullptr);
        }
        forward_list(const forward_list& _list) _YXXX_NOEXCEPT:
            forward_list(_list._M_allocator){
            _M_copy_or_assign(_list.begin(), _list.end());
        }
        forward_list(forward_list&& _list) _YXXX_NOEXCEPT{
            _M_move(_list);
        }
        forward_list(std::initializer_list<value_type>&& _il) _YXXX_NOEXCEPT:
            forward_list(){
            _M_move_or_assign(_il.begin(), _il.end());
        }
        template <typename _InputIterator>
        forward_list(_InputIterator _first, _InputIterator _last,
            const allocator_type& _alloc = allocator_type()) _YXXX_NOEXCEPT:
            forward_list(_alloc){
            _M_copy_or_assign(_first, _last);
        }
        ~forward_list() _YXXX_NOEXCEPT{
            clear();
            _M_allocator.deallocate(_M_head);
            _M_head = nullptr;
        }

        self_type& operator=(const forward_list& _fl) _YXXX_NOEXCEPT{
            _M_allocator = _fl._M_allocator;
            _M_copy_or_assign(_fl.begin(), _fl.end());
            return *this;
        }
        self_type& operator=(forward_list&& _fl) _YXXX_NOEXCEPT{
            _M_move(_fl);
            return *this;
        }
        void push_front(const value_type& _value){
            _M_construct_after(_M_head, _value);
        }
        void push_front(value_type&& _value){
            _M_construct_after(_M_head, std::move(_value));
        }
        template <typename ...Args>
        void emplace_front(Args ...args){
            _M_construct_after(_M_head, std::forward<Args>(args)...);
        }
        void insert_after(iterator _iter, const value_type& _value){
            if(_iter == end()) return;
            _M_construct_after(_iter._M_ptr, _value);
        }
        void insert_after(iterator _iter, value_type&& _value){
            if(_iter == end()) return;
            _M_construct_after(_iter._M_ptr, std::move(_value));
        }
        template <typename ...Args>
        void emplace_after(iterator _iter, Args ...args){
            if(_iter == end()) return;
            _M_construct_after(_iter._M_ptr, std::forward<Args>(args)...);
        }
        void pop_front() _YXXX_NOEXCEPT{
            if(!empty()){
                _M_destroy_next(_M_head);
            }
        }
        void erase_after(iterator _iter) _YXXX_NOEXCEPT{
            if(_iter == end()) return;
            iterator _tmp = _iter;
            while((++_tmp) != end()){
                _M_destroy_next(_iter._M_ptr);
                _tmp = _iter;
            }
        }
        void erase_after(iterator _prev, iterator _end) _YXXX_NOEXCEPT{
            _node_pointer _prev_ptr = _prev._M_ptr, _next_ptr = _prev_ptr->next(), _end_ptr = _end._M_ptr;
            while(_next_ptr != _end_ptr && _next_ptr != nullptr){
                _M_destroy_next(_prev_ptr);
            }
        }
        void reverse() _YXXX_NOEXCEPT{
            if(empty()|| _M_head->next()->next() == nullptr) return;
            _node_pointer _ptr = _M_head->next(), _prev_ptr = nullptr, _next_ptr = _ptr->next();
            while(_ptr != nullptr){
                _next_ptr = _ptr->next();
                _ptr->next() = _prev_ptr;
                _prev_ptr = _ptr;
                _ptr = _next_ptr;
            }
            _M_head->next() = _prev_ptr;
        }
        friend void swap(forward_list& _l_list, forward_list& _r_list) _YXXX_NOEXCEPT{
            std::swap(_l_list._M_head, _r_list._M_head);
        }

        bool empty() const _YXXX_NOEXCEPT{
            return nullptr == _M_head || _M_head->next() == nullptr;}
        void clear() _YXXX_NOEXCEPT{
            while(!empty()){
                _M_destroy_next(_M_head);
            }
        }
        
        iterator begin() _YXXX_NOEXCEPT{return iterator(_M_head->next());}
        const_iterator begin() const _YXXX_NOEXCEPT{return const_iterator(_M_head->next());}
        iterator end() _YXXX_NOEXCEPT{return iterator(nullptr);}
        const_iterator end() const _YXXX_NOEXCEPT{return const_iterator(nullptr);}
        const_iterator cbegin() const _YXXX_NOEXCEPT{return const_iterator(_M_head->next());}
        const_iterator cend() const _YXXX_NOEXCEPT{return const_iterator(nullptr);}

        // void show() const _YXXX_NOEXCEPT{
        //     std::cout << "forward_list->";
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
            iterator _iter = begin(), _prev_iter = iterator(_M_head), _end = end();
            while(_iter != _end && _first != _last){
                *_iter = *(_first++);
                _prev_iter = _iter++;
            }
            if(_iter != _end) erase_after(_prev_iter);
            else{
                reverse();
                for(; _first != _last; ++_first){
                    push_front(*_first);
                }
                reverse();
            }
        }
        template <typename _InputIterator>
        void _M_move_or_assign(_InputIterator _first, _InputIterator _last) _YXXX_NOEXCEPT{
            iterator _iter = begin(), _prev_iter = iterator(_M_head), _end = end();
            while(_iter != _end && _first != _last){
                *_iter = std::move(*(_first++));
                _prev_iter = _iter++;
            }
            if(_iter != _end) erase_after(_prev_iter);
            else{
                reverse();
                for(; _first != _last; ++_first){
                    push_front(std::move(*_first));
                }
                reverse();
            }
        }

        template <typename ...Args>
        _node_pointer _M_construct_after(
            _node_pointer _prev = nullptr,
            Args ...args) _YXXX_NOEXCEPT{
            _node_pointer _ptr = _M_allocator.allocate(1);
            _M_allocator.construct(
                _ptr, _prev, nullptr,
                std::forward<Args>(args)...
            );
            return _ptr;
        }

        void _M_destroy_next(_node_pointer _node) _YXXX_NOEXCEPT{
            _node_pointer _tmp = _node->next();
            _node->next() = _tmp->next();
            _M_allocator.destroy(_tmp);
            _M_allocator.deallocate(_tmp);
        }

    protected:
        _node_alloc _M_allocator;
        _node_pointer _M_head;
    };
};

#endif