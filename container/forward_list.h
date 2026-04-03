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
                "A dengerous node connection is detected."){}
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
    protected:
        typedef _forward_list::_forward_list_node<value_type>    _node_type;
        typedef _alloc::alloc_rebind<allocator_type, _node_type> _node_alloc;
    public:
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

        using return_type = iterator;

    protected:
        struct _data{
            _node_pointer _begin;
            _node_pointer _over;
            size_type _size;
            
            template <typename... _Args>
            _data(_node_alloc& _alloc,
                size_type _n, _Args&& ..._args){
                // _n != 0
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
            _data(_node_alloc& _alloc,
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
        forward_list() _YXXX_NOEXCEPT:
            _M_allocator(_node_alloc{}){
            _M_construct();
        }
        forward_list(const allocator_type& _alloc):
            _M_allocator(_alloc){
            _M_construct();
        }
        forward_list(allocator_type&& _alloc):
            _M_allocator(std::move(_alloc)){
            _M_construct();
        }

        forward_list(const forward_list& _fl):
            forward_list(_fl.begin(), _fl.end(), _fl._M_allocator){}
        forward_list(const forward_list& _fl,
            const allocator_type& _alloc):
            forward_list(_fl.begin(), _fl.end(), _alloc){}
        forward_list(const forward_list& _fl,
            allocator_type&& _alloc):
            forward_list(_fl.begin(), _fl.end(), std::move(_alloc)){}

        forward_list(forward_list&& _fl):
            forward_list(std::move(_fl._M_allocator)){
            _M_move_data(_fl);
        }
        forward_list(forward_list&& _fl,
            const allocator_type& _alloc):
            forward_list(_alloc){
            _M_move_data(_fl);
        }
        forward_list(forward_list&& _fl,
            allocator_type&& _alloc):
            forward_list(std::move(_alloc)){
            _M_move_data(_fl);
        }

        template <typename _Iterator,
            typename = std::enable_if_t<stl::is_iterator_v<_Iterator>>>
        forward_list(_Iterator _beg, _Iterator _end):
            forward_list(){
            _M_merge_after(_M_head, _data{_M_allocator, _beg, _end});
        }
        template <typename _Iterator,
            typename = std::enable_if_t<stl::is_iterator_v<_Iterator>>>
        forward_list(_Iterator _beg, _Iterator _end,
            const allocator_type& _alloc):
            forward_list(_alloc){
            _M_merge_after(_M_head, _data(_M_allocator, _beg, _end));
        }
        template <typename _Iterator,
            typename = std::enable_if_t<stl::is_iterator_v<_Iterator>>>
        forward_list(_Iterator _beg, _Iterator _end,
            allocator_type&& _alloc):
            forward_list(std::move(_alloc)){
            _M_merge_after(_M_head, _data(_M_allocator, _beg, _end));
        }

        forward_list(std::initializer_list<value_type>&& _il):
            forward_list(_il.begin(), _il.end()){}
        forward_list(std::initializer_list<value_type>&& _il,
            const allocator_type& _alloc):
            forward_list(_il.begin(), _il.end(), _alloc){}
        forward_list(std::initializer_list<value_type>&& _il,
            allocator_type&& _alloc):
            forward_list(_il.begin(), _il.end(), std::move(_alloc)){}

        ~forward_list() _YXXX_NOEXCEPT{
            clear();
            _M_allocator.deallocate(_M_head, sizeof(value_type));
            _M_head = _M_tail = nullptr;
        }
        
        reference front(){
            return *begin();
        }
        const_reference front() const{
            return *begin();
        }
        reference back(){
            return _M_tail->value();
        }
        const_reference back() const{
            return _M_tail->value();
        }

        forward_list& operator=(const forward_list& _fl){
            if(&_fl == this) return *this;
            _M_allocator = _fl._M_allocator;
            _M_copy_or_assign(_fl.begin, _fl.end());
            return *this;
        }
        forward_list& operator=(forward_list&& _fl)
        _YXXX_NOEXCEPT{
            if(&_fl == this) return *this;
            _M_move(std::move(_fl));
            return *this;
        }

        // assign
        void assign(size_type _n, const value_type& _val){
            _node_pointer _ptr = _M_head;
            size_type _r = _n;
            while(nullptr != _ptr && _r--){
                _ptr = _ptr->next();
                _ptr->value() = _val;
            }
            if(_r) _M_merge_after(_M_tail, _data{_M_allocator, _n, _val});
            else _M_erase_after(_ptr);
        }
        template <typename _Iterator>
        std::enable_if_t<
            stl::is_iterator_v<_Iterator>,
            void>
        assign(_Iterator _beg, _Iterator _end){
            _M_copy_or_assign(_beg, _end);
        }
        void assign(std::initializer_list<value_type>&& _il){
            _M_copy_or_assign(_il.begin(), _il.end());
        }

        // push_back/push_front
        return_type push_front(const value_type& _value){
            return _M_emplace_after(_M_head, _value);
        }
        return_type push_back(const value_type& _value){
            return _M_emplace_after(_M_tail, _value);
        }

        return_type push_front(value_type&& _value){
            return _M_emplace_after(_M_head, std::move(_value));
        }
        return_type push_back(value_type&& _value){
            return _M_emplace_after(_M_tail, std::move(_value));
        }

        return_type
        push_front(size_type _n, const value_type& _value){
            return _M_merge_after(_M_head, _data(_M_allocator, _n, _value));
        }
        return_type
        push_back(size_type _n, const value_type& _value){
            return _M_merge_after(_M_tail, _data(_M_allocator, _n, _value));
        }

        template <typename _Iterator>
        std::enable_if_t<
            stl::is_iterator_v<_Iterator>,
            return_type>
        push_front(_Iterator _beg, _Iterator _end){
            return _M_merge_after(_M_head, _data(_M_allocator, _beg, _end));
        }
        template <typename _Iterator>
        std::enable_if_t<
            stl::is_iterator_v<_Iterator>,
            return_type>
        push_back(_Iterator _beg, _Iterator _end){
            return _M_merge_after(_M_tail, _data(_M_allocator, _beg, _end));
        }

        // emplace_back/emplace_front
        template <typename... _Args>
        return_type emplace_front(_Args&& ..._args){
            return _M_insert_after(
                _M_head,
                std::forward<_Args>(_args)...
            );
        }
        template <typename... _Args>
        return_type emplace_back(_Args&& ..._args){
            return _M_insert_after(
                _M_tail,
                std::forward<_Args>(_args)...
            );
        }

        // insert
        return_type
        insert(iterator _pos, const value_type& _value){
            if(!_M_valid_insert(_pos)) return end(); 
            return _M_emplace(_pos._M_ptr, _value);
        }
        return_type
        insert(difference_type _idx, const value_type& _value){
            if(!_M_valid_insert(_idx)) return end(); 
            return _M_emplace(_M_ptr(_idx), _value);
        }

        return_type
        insert(iterator _pos, value_type&& _value){
            if(!_M_valid_insert(_pos)) return end(); 
            return _M_emplace(_pos._M_ptr, std::move(_value));
        }
        return_type
        insert(difference_type _idx, value_type&& _value){
            if(!_M_valid_insert(_idx)) return end(); 
            return _M_emplace(_M_ptr(_idx), std::move(_value));
        }

        // insert_after
        return_type
        insert_after(iterator _pos, const value_type& _value){
            if(!_M_valid_prev(_pos)) return end();
            return _M_emplace_after(_pos._M_ptr, _value);
        }
        return_type
        insert_after(difference_type _idx, const value_type& _value){
            if(!_M_valid_prev(_idx)) return end();
            return _M_emplace_after(_M_ptr(_idx), _value);
        }
        
        return_type
        insert_after(iterator _pos, value_type&& _value){
            if(!_M_valid_prev(_pos)) return end();
            return _M_emplace_after(_pos._M_ptr, std::move(_value));
        }
        return_type
        insert_after(difference_type _idx, value_type&& _value){
            if(!_M_valid_prev(_idx)) return end();
            return _M_emplace_after(_M_ptr(_idx), std::move(_value));
        }

        return_type
        insert_after(iterator _pos,
            size_type _n, const value_type& _value){
            if(0 == _n || !_M_valid_prev(_pos)) return end();
            return _M_merge_after(_pos._M_ptr, _data(_M_allocator, _n, _value));
        }
        return_type
        insert_after(difference_type _idx,
            size_type _n, const value_type& _value){
            if(0 == _n || !_M_valid_prev(_idx)) return end();
            return _M_merge_after(_M_ptr(_idx), _data(_M_allocator, _n, _value));
        }

        template <typename _Iterator>
        std::enable_if_t<
            stl::is_iterator_v<_Iterator>,
            return_type>
        insert_after(iterator _pos, _Iterator _beg, _Iterator _end){
            if(_beg == _end || !_M_valid_prev(_pos)) return end();
            return _M_merge_after(_pos._M_ptr, _data(_M_allocator, _beg, _end));
        }
        template <typename _Iterator>
        std::enable_if_t<
            stl::is_iterator_v<_Iterator>,
            return_type>
        insert_after(difference_type _idx, _Iterator _beg, _Iterator _end){
            if(_beg == _end || !_M_valid_prev(_idx)) return end();
            return _M_merge_after(_M_ptr(_idx), _data(_M_allocator, _beg, _end));
        }

        // emplace
        template <typename ..._Args>
        return_type
        emplace(iterator _pos, _Args&& ..._args){
            if(!_M_valid_insert(_pos)) return end(); 
            return _M_emplace(
                _pos._M_ptr,
                std::forward<_Args>(_args)...
            );
        }
        template <typename ..._Args>
        return_type
        emplace(difference_type _idx, _Args&& ..._args){
            if(!_M_valid_insert(_idx)) return end(); 
            return _M_emplace(
                _M_ptr(_idx),
                std::forward<_Args>(_args)...
            );
        }

        // emplace_after
        template <typename ..._Args>
        return_type
        emplace_after(iterator _pos, _Args&& ..._args)
        _YXXX_NOEXCEPT{
            if(!_M_valid_prev(_pos)) return end();
            return _M_emplace_after(
                _pos._M_ptr,
                std::forward<_Args>(_args)...
            );
        }
        template <typename ..._Args>
        return_type
        emplace_after(difference_type _idx, _Args&& ..._args)
        _YXXX_NOEXCEPT{
            if(!_M_valid_prev(_idx)) return end();
            return _M_emplace_after(
                _M_ptr(_idx),
                std::forward<_Args>(_args)...
            );
        }

        // erase_after
        return_type erase_after(iterator _pos)
        _YXXX_NOEXCEPT{
            if(_pos._M_ptr == _M_tail || !_M_valid_prev(_pos))
                return end();
            return _M_erase_after(_pos._M_ptr);
        }
        return_type erase_after(difference_type _idx)
        _YXXX_NOEXCEPT{
            if(_idx == _M_size - 1 || !_M_valid_prev(_idx))
                return end();
            return _M_erase_after(_M_ptr(_idx));
        }

        return_type 
        erase_after(iterator _prev, iterator _end)
        _YXXX_NOEXCEPT{
            if(!_M_valid_range_after(_prev, _end)) return end();
            return _M_erase_after(_prev._M_ptr, _end._M_ptr);
        }
        return_type
        erase_after(difference_type _idx, size_type _n)
        _YXXX_NOEXCEPT{
            if(_idx == _M_size - 1 || !_M_valid_prev(_idx))
                return end();
            if(0 == _n) return _M_ptr(_idx)->next();
            return _M_erase_after(_M_ptr(_idx), _n);
        }

        iterator begin()
        _YXXX_NOEXCEPT{
            return iterator(_M_head->next());
        }
        const_iterator begin()
        const _YXXX_NOEXCEPT{
            return const_iterator(_M_head->next());
        }
        iterator end()
        _YXXX_NOEXCEPT{
            return iterator(nullptr);
        }
        const_iterator end()
        const _YXXX_NOEXCEPT{
            return const_iterator(nullptr);
        }
        const_iterator cbegin()
        const _YXXX_NOEXCEPT{
            return const_iterator(_M_head->next());
        }
        const_iterator cend()
        const _YXXX_NOEXCEPT{
            return const_iterator(nullptr);
        }
        
        void unique(){
            if(_M_tail == _M_head) return;
            _node_pointer _ptr = _M_head->next();
            while(nullptr != _ptr->next()){
                if(_ptr->next()->value() == _ptr->value())
                    _M_erase_after(_ptr);
                else
                    _ptr = _ptr->next();
            }
        }
        void unique(const value_type& _val){
            if(_M_tail == _M_head) return;
            _node_pointer _ptr = _M_head->next();
            while(nullptr != _ptr->next()){
                if(_val == _ptr->value() &&
                    _ptr->next()->value() == _ptr->value())
                    _M_erase_after(_ptr);
                else
                    _ptr = _ptr->next();
            }
        }
        
        void remove(const value_type& _val){
            _node_pointer _prev = _M_head;
            while(nullptr != _prev->next()){
                if(_val == _prev->next()->value())
                    _M_erase_after(_prev);
                else
                    _prev = _prev->next();
            }
        }
        template <typename _Predicate>
        void remove_if(const _Predicate& _pred){
            _node_pointer _prev = _M_head;
            while(nullptr != _prev->next()){
                if(_pred(_prev->next()->value()))
                    _M_erase_after(_prev);
                else
                    _prev = _prev->next();
            }
        }

        void reverse() _YXXX_NOEXCEPT{
            if(_M_tail == _M_head ||
                _M_tail == _M_head->next())
                return;
            _M_tail = _M_head->next();
            _node_pointer _prev = nullptr, _cur = _M_head->next(), _next = _cur->next();
            while(nullptr != _next){
                _cur->next() = _prev;
                _prev = _cur;
                _cur = _next;
                _next = _next->next();
            }
            _cur->next() = _prev;
            _M_head->next() = _cur;
        }
            
        friend void swap(forward_list& _ll, forward_list& _rl)
        _YXXX_NOEXCEPT{
            std::swap(_ll._M_allocator, _rl._M_allocator);
            std::swap(_ll._M_head, _rl._M_head);
            std::swap(_ll._M_tail, _rl._M_tail);
            std::swap(_ll._M_size, _rl._M_size);
        }

        bool empty() const _YXXX_NOEXCEPT{
            return 0 == _M_size;
        }
        size_type size() const _YXXX_NOEXCEPT{
            return _M_size;
        }
        void clear() _YXXX_NOEXCEPT{
            _M_erase_after(_M_head, nullptr);
        }

    private:
        bool _M_valid(difference_type _idx)
        const _YXXX_NOEXCEPT{
            return size_type(_idx) < _M_size;
        }
        bool _M_valid(iterator _pos)
        const _YXXX_NOEXCEPT{
            if(nullptr == _pos._M_ptr) return false;
            if(_pos._M_ptr == _M_tail) return true;
            _node_pointer _ptr = _M_head->next();
            while(nullptr != _ptr){
                if(_pos._M_ptr == _ptr) return true;
                _ptr = _ptr->next();
            }
            return false;
        }
        bool _M_valid_insert(difference_type _idx)
        const _YXXX_NOEXCEPT{
            return size_type(_idx) <= _M_size;
        }
        bool _M_valid_insert(iterator _pos)
        const _YXXX_NOEXCEPT{
            if(nullptr == _pos._M_ptr || _pos._M_ptr == _M_tail)
                return true;
            _node_pointer _ptr = _M_head->next();
            while(nullptr != _ptr){
                if(_pos._M_ptr == _ptr) return true;
                _ptr = _ptr->next();
            }   
            return false;
        }
        bool _M_valid_prev(iterator _idx)
        const _YXXX_NOEXCEPT{
            return _M_valid(_idx);
        }
        bool _M_valid_prev(difference_type _pos)
        const _YXXX_NOEXCEPT{
            return _M_valid(_pos);
        }
        bool _M_valid_range_after(iterator _prev, iterator _end)
        const _YXXX_NOEXCEPT{
            if(_end == _prev  // 相等迭代器
                || !_M_valid_prev(_prev) // 无效的前置迭代器
                || _prev._M_ptr == nullptr // 前置迭代器为容器超尾迭代器
                || _end._M_ptr == _prev._M_ptr->next()) // 超尾迭代器紧跟前置迭代器
                return false;
            if(nullptr == _end._M_ptr) return true;
            _node_pointer _cur = _prev._M_ptr->next();
            while(_cur != _M_head){
                if(_cur == _end._M_ptr) return true;
                _cur = _cur->next();
            }
            return false;
        }

        _node_pointer _M_ptr(difference_type _idx)
        const _YXXX_NOEXCEPT{
            if(_idx == _M_size - 1) return _M_tail;
            if(_idx == _M_size) return nullptr;
            _node_pointer _ptr = _M_head->next();
            while(_idx--) _ptr = _ptr->next();
            return _ptr;
        }
        difference_type _M_idx(iterator _it)
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
            _M_tail = _M_head = _M_allocator.allocate(1);
            _M_allocator.construct(_M_head, nullptr, nullptr, value_type{});
            _M_size = 0;
        }

        void _M_move(forward_list&& _fl){
            _M_before_move();
            _M_allocator = std::move(_fl._M_allocator);
            _M_move_data(_fl);
        }
        void _M_move_data(forward_list& _fl){
            _M_head->next() = _fl._M_head->next();
            _M_tail = _fl._M_tail;
            _M_size = _fl._M_size;
            _fl._M_after_move();
        }
        void _M_before_move(){
            _M_erase_after(_M_head, nullptr);
        }
        void _M_after_move(){
            _M_tail = _M_head;
            _M_head->next() = nullptr;
            _M_size = 0;
        }

        template <typename _Iterator>
        std::enable_if_t<
            stl::is_iterator_v<_Iterator>,
            void>
        _M_copy_or_assign(_Iterator _beg, _Iterator _end){
            _node_pointer _ptr = _M_head;
            if(_M_tail != _M_head){
                _node_pointer _end = _M_tail;
                _ptr = _ptr->next();
                while(_beg != _end){
                    _ptr->value() = *(_beg++);
                    if(_ptr == _M_tail) break;
                    _ptr = _ptr->next();
                }
            }
            if(_beg != _end)
                _M_merge_after(_ptr, _data{_M_allocator, _beg, _end});
            else _M_erase_after(_ptr);
        }

        template <typename ..._Args>
        return_type
        _M_emplace(_node_pointer _ptr,
            _Args&& ..._args) _YXXX_NOEXCEPT{
            _node_pointer _ist = _M_allocator.allocate(1);
            if(nullptr == _ptr)
                return _M_emplace_after(_M_tail, std::forward<_Args>(_args)...);
            _M_allocator.construct(_ist, _ptr, nullptr, value_type{});
            _ist->value() = std::move(_ptr->value());
            _ptr->value() = value_type{std::forward<_Args>(_args)...};
            ++_M_size;
            return _ist;
        }
        template <typename ..._Args>
        return_type
        _M_emplace_after(_node_pointer _prev,
            _Args&& ..._args) _YXXX_NOEXCEPT{
            _node_pointer _ptr = _M_allocator.allocate(1);
            _M_allocator.construct(
                _ptr, _prev, nullptr,
                std::forward<_Args>(_args)...
            );
            if(_M_tail == _prev) _M_tail = _ptr;
            ++_M_size;
            return _ptr;
        }

        return_type
        _M_erase_after(_node_pointer _prev)
        _YXXX_NOEXCEPT{
            _node_pointer _node = _prev->next();
            _prev->next() = _node->next();
            _M_allocator.destroy(_node);
            if(_node == _M_tail) _M_tail = _prev;
            --_M_size;
            _M_allocator.deallocate(_node, sizeof(_node_type));
            return _prev->next();
        }
        return_type
        _M_erase_after(_node_pointer _prev, _node_pointer _end)
        _YXXX_NOEXCEPT{
            while(_end != _prev->next())
                _M_erase_after(_prev);
            return _prev->next();
        }
        return_type
        _M_erase_after(_node_pointer _prev, size_type _n)
        _YXXX_NOEXCEPT{
            while(nullptr != _prev->next() && _n--){
                _M_erase_after(_prev);
            }
            return _prev->next();
        }

        return_type
        _M_merge_after(_node_pointer _prev, _data _dat)
        _YXXX_NOEXCEPT{
            _dat._over->next() = _prev->next();
            _prev->next() = _dat._begin;
            _M_size += _dat._size;
            if(_prev == _M_tail) _M_tail = _dat._over;
            return _prev->next();
        }

    protected:
        _node_alloc _M_allocator;
        _node_pointer _M_head;
        _node_pointer _M_tail;
        size_type _M_size;
    };
};

#endif