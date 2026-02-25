#ifndef _YXXX_POOL_ALLOCATOR_H_
#define _YXXX_POOL_ALLOCATOR_H_

#include <bits/shared_ptr.h>
#include <stdexcept>
#include "../adaptive_c++config.h"

namespace _pool_alloc{
    #ifndef _YXXX_CHUNK_MAX_SIZE
    #define _YXXX_CHUNK_MAX_SIZE 10
    #endif
    #if _YXXX_CHUNK_MAX_SIZE < __SHRT_MAX__
    #define _YXXX_BLOCK_COUNT_TYPE short
    #elif _YXXX_CHUNK_MAX_SIZE < __INT_MAX__
    #define _YXXX_BLOCK_COUNT_TYPE int
    #else
    #define _YXXX_BLOCK_COUNT_TYPE stl::size_t
    #endif

    struct _chunk{
        typedef _YXXX_BLOCK_COUNT_TYPE count_type;

        class bad_chunk: public std::runtime_error{
            public:
                bad_chunk():
std::runtime_error("Bytes of chunk is insufficient to accommodate at least one 4-byte block."){}
        };
    protected:
        void* _M_begin;
        void* _M_end;
        stl::size_t _M_block_size;
        count_type _M_first;
        count_type _M_free_count;

    public:
        _chunk(void* _begin, void* _end, stl::size_t _block_size){
            count_type _block_num = (
                static_cast<char*>(_end) - static_cast<char*>(_begin)) 
                / _block_size;
            if(_block_num < 1)
                throw bad_chunk();

            _M_begin = _begin;
            _M_end = _end;
            _M_block_size = _block_size;
            _M_first = 0;
            _M_free_count = _block_num;
            
            char* _block_ptr = static_cast<char*>(_begin);
            for(count_type _i = 0; _i < _M_free_count-1; ++_i){
                *(reinterpret_cast<count_type*>(_block_ptr)) = _i + 1;
                _block_ptr += _block_size;
            }
            *(reinterpret_cast<count_type*>(_block_ptr)) = -1;
        }
        // _chunk(_chunk&& _c) _YXXX_NOEXCEPT{
        //     _move(std::move(_c));
        // }

        // _chunk& operator=(_chunk&& _c) _YXXX_NOEXCEPT{
        //     _move(std::move(_c));
        //     return *this;
        // }

        void* allocate() _YXXX_NOEXCEPT{
            // 此chunk所有block已全部分配
            if(refuse()) return nullptr;
            // 分配一个block
            void* _ptr = static_cast<char*>(_M_begin) + _M_block_size * _M_first;
            _M_first = *(static_cast<count_type*>(_ptr));
            -- _M_free_count;
            return _ptr;
        }
        bool deallocate(void* _ptr) _YXXX_NOEXCEPT{
            // 不属于此chunk，归还失败
            if(_ptr < _M_begin || _ptr >= _M_end) return false;
            stl::size_t _loc = static_cast<char*>(_ptr) - static_cast<char*>(_M_begin);
            // 未在正确block地址，归还失败
            if(_loc % _M_block_size) return false;
            // 归还
            *(static_cast<count_type*>(_ptr)) = _M_first;
            _M_first = _loc / _M_block_size;
            ++ _M_free_count;
            return true;
        }
        void release() _YXXX_NOEXCEPT{}

        bool refuse() const _YXXX_NOEXCEPT{
            return full() || (-1 == _M_first);
        }
        bool full() const _YXXX_NOEXCEPT{
            return 0 == _M_free_count;
        }
        bool empty() const _YXXX_NOEXCEPT{
            return (static_cast<char*>(_M_end) - static_cast<char*>(_M_begin))
                / _M_block_size == _M_free_count;
        }
    private:
        // void _move(_chunk&& _c) _YXXX_NOEXCEPT{
        //     _M_begin = _c._M_begin;
        //     _M_end = _c._M_end;
        //     _M_block_size = _c._M_block_size;
        //     _M_first = _c._M_first;
        //     _M_free_count = _c._M_free_count;
        //     _c._M_begin = _c._M_end = nullptr;
        // }
    };

    struct _retain{
        void *_M_begin, *_M_end;

        _retain(const _retain& _r) _YXXX_NOEXCEPT:
            _M_begin(_r._M_begin), _M_end(_r._M_end){}
        _retain& operator=(const _retain& _r) _YXXX_NOEXCEPT{
            _M_begin = _r._M_begin;
            _M_end = _r._M_end;
            return *this;
        }
        
        void* allocate(stl::size_t _block_size) _YXXX_NOEXCEPT{
            _chunk::count_type _block_num = (
                static_cast<char*>(_M_end) - static_cast<char*>(_M_begin))
                / _block_size;
            if(_block_num < 1) return nullptr;
            _block_num = std::min(_block_num,
                static_cast<_chunk::count_type>(_YXXX_CHUNK_MAX_SIZE));
            void* _ptr = _M_begin;
            _M_begin = static_cast<char*>(_M_begin)
                + _block_num * _block_size;
            return _ptr;
        }
        void refill(stl::size_t _size) _YXXX_NOEXCEPT{
            if(!empty()) return;
            void* _new = ::operator new(_size);
            _M_begin = _new;
            _M_end = static_cast<char*>(_M_begin) + _size;
        }
        bool empty() const _YXXX_NOEXCEPT{
            return _M_begin == _M_end;
        }
    };

    class _chunks_array{
    public:
        _chunks_array(stl::size_t _block_size) _YXXX_NOEXCEPT{
            _M_block_size = _block_size;
            _M_begin = _M_end = _M_over = nullptr;
        }
        _chunks_array(_chunks_array&& _ca) _YXXX_NOEXCEPT{
            _M_block_size = _ca._M_block_size;
            _M_begin = _ca._M_begin;
            _M_end = _ca._M_end;
            _M_over = _ca._M_over;
            _ca._M_begin = _ca._M_end = _ca._M_over = nullptr;
        }
        ~_chunks_array() _YXXX_NOEXCEPT{
            _chunk* _current = _M_begin;
            while(_current != _M_end){
                delete _current;
                _current = nullptr;
                ++_current;
            }
        }

        // 优先在管理的chunks中分配内存，失败后尝试从_retain中获取并分配
        void* allocate(_retain& _retain) _YXXX_NOEXCEPT{
            void* _ptr = nullptr;
            // 从chunk分配
            if(_M_over != _M_begin){
                for(_chunk* _chunk_ptr = _M_end-1;
                    _chunk_ptr >= _M_begin; --_chunk_ptr){
                    _ptr = _chunk_ptr->allocate();
                    if(_ptr != nullptr) return _ptr;
                }
            }
            // retain分配chunk
            _ptr = _retain.allocate(_M_block_size);
            // 分配失败
            if(nullptr == _ptr) return _ptr;
            // 分配成功，构建新chunk、加入array、从新chunk分配block
            _maybe_expand();
            new (_M_end) _chunk(_ptr, _retain._M_begin, _M_block_size);
            ++_M_end;
            return allocate(_retain);
        }
        // 在管理的chunks中分配内存
        void extract(stl::size_t _size, _retain& _retain)
        _YXXX_NOEXCEPT{}
        bool deallocate(void* _ptr) _YXXX_NOEXCEPT{
            if(nullptr == _ptr) return false;
            _chunk* _chunk_ptr = _M_begin;
            while(_chunk_ptr < _M_end){
                if(_chunk_ptr->deallocate(_ptr)){
                    _after_chunk_deallocate(_chunk_ptr);
                    return true;
                }
                ++_chunk_ptr;
            }
            return false;
        }

    private:
        void _expand() _YXXX_NOEXCEPT{
            stl::size_t _old_bytes = (_M_over - _M_begin)
                * sizeof(_chunk);
            stl::size_t _new_bytes = (
                (_M_over - _M_begin) * 1.5 + 1)
                * sizeof(_chunk);
            char* _new_chunk = static_cast<char*>(::operator new(_new_bytes));
            
            memcpy(_new_chunk, _M_begin, _old_bytes);
            _M_over = reinterpret_cast<_chunk*>(
                _new_chunk + _new_bytes);
            _M_end = reinterpret_cast<_chunk*>(
                _new_chunk + (_M_end - _M_begin)
                * sizeof(_chunk));
            ::operator delete(_M_begin, _old_bytes);
            _M_begin = reinterpret_cast<_chunk*>(_new_chunk);
        }
        void _maybe_expand() _YXXX_NOEXCEPT{
            if(_M_end == _M_over) _expand();
        }
        void _after_chunk_deallocate(_chunk* _chunk_ptr)
        _YXXX_NOEXCEPT{
            if(_chunk_ptr->empty()){
                _chunk_ptr->release();
                if(_chunk_ptr + 1 != _M_end){
                    *_chunk_ptr = std::move(*(_M_end - 1));
                }
                --_M_end;
            }
        }

        stl::size_t _M_block_size;
        _chunk* _M_begin;
        _chunk* _M_end;
        _chunk* _M_over;
    };

    enum{
        _min_block_bytes = 8,
        _max_block_bytes = 128,
        _array_num = _max_block_bytes / _min_block_bytes
    };

    static inline _YXXX_CONSTEXPR
    stl::size_t align_up(stl::size_t _size, stl::size_t _align){
        return (_size + _align - 1) & ~(_align - 1);
    }

    // 问题：释放内存
    template <typename _Tp>
    class _pool_allocator_base{
    public:
        typedef _Tp             value_type;
        typedef stl::size_t     size_type;
        typedef std::ptrdiff_t  difference_type;

        typedef _Tp*            pointer;
        typedef const _Tp*      const_pointer;
        typedef _Tp&            reference;
        typedef const _Tp&      const_reference;

        static _YXXX_CONSTEXPR stl::size_t block_size =
            sizeof(value_type) < sizeof(_chunk::count_type) ? 
            sizeof(_chunk::count_type) : sizeof(value_type);

        _pool_allocator_base() _YXXX_NOEXCEPT:
            _M_retain({nullptr, nullptr}),
            _M_pool(static_cast<_chunks_array*>(
                ::operator new(sizeof(_chunks_array) * _array_num))){
            for(int _i = 0; _i < _array_num; ++_i){
                new (_M_pool + _i) _chunks_array{
                    stl::size_t((_i + 1) * _min_block_bytes)
                };
            }
        }
        ~_pool_allocator_base() _YXXX_NOEXCEPT{
            ::operator delete (_M_pool, sizeof(_chunks_array) * _array_num);
        }

        pointer allocate(size_type _size, const void* = 0){
            // 字节处理
            size_type _bytes_size = _aligned_bytes_size(_size);

            // 大块分配
            size_type _index = _bytes_index(_bytes_size);
            if(-1 == _index)
                return static_cast<pointer>(::operator new(_bytes_size));

            // 从对应chunks_array分配；
            // 当分配失败，从retain抽取内存构建chunk并分配；
            // 当抽取失败，则分配失败
            void* _res = _M_pool[_index].allocate(_M_retain);
            if(_res != nullptr) return static_cast<pointer>(_res);
            // 碎片管理，使用retain剩余的内存构建适当的chunk
            _fragment_manager();

            // 从更大chunks_array抽取内存到retain(此时为空)
            size_type _idx = _array_num - 1;
            while(_M_retain.empty() && _idx > _index){
                _M_pool[_idx].extract(_bytes_size, _M_retain);
                --_idx;
            }
            // 抽取成功
            // 再次从对应chunks_array分配
            if(!_M_retain.empty())
                return static_cast<pointer>(
                    _M_pool[_index].allocate(_M_retain));

            // 从更小chunks_array整理内存
            // ...

            // 抽取失败
            // 申请新内存到retain，再次从对应chunks_array分配
            _refill(_bytes_size);
            // 申请失败，无法分配
            if(_M_retain.empty()) throw std::bad_alloc();
            return static_cast<pointer>(_M_pool[_index].allocate(_M_retain));
        }
        bool deallocate(void* _ptr){
            if(nullptr == _ptr) return false;
            for(size_type _index = 0; _index < _array_num; ++_index){
                if(_M_pool[_index].deallocate(_ptr))
                    return true;
            }
            return false;
        }
        
    private:
        size_type _aligned_bytes_size(size_type _size)
        const _YXXX_NOEXCEPT{
            size_type _aligned_size = _size * block_size;
            return align_up(_aligned_size, sizeof(pointer));
        }
        size_type _bytes_index(size_type _size)
        const _YXXX_NOEXCEPT{
            size_type _index = _size/_min_block_bytes - 1;
            return _valid_index(_index) ? _index : -1;
        }
        bool _valid_index(size_type _index)
        const _YXXX_NOEXCEPT{
            return _index < _array_num;
        }
        void _fragment_manager() _YXXX_NOEXCEPT{
            if(_M_retain.empty()) return;
            stl::size_t _bytes =
                static_cast<char*>(_M_retain._M_end)
                - static_cast<char*>(_M_retain._M_begin);
            size_type _index = _bytes/_min_block_bytes;
        }
        void _refill(size_type _size) _YXXX_NOEXCEPT{
            _size *= 2;
            _M_retain.refill(_size);
        }
        
    protected:
        _retain _M_retain;
        _chunks_array* _M_pool;
    };
};

namespace _alloc{
    template <typename _Tp>
    class pool_allocator{
    protected:
        typedef _pool_alloc::_pool_allocator_base<_Tp>  _allocator;
    public:
        typedef typename _allocator::value_type         value_type;
        typedef typename _allocator::size_type          size_type;
        typedef typename _allocator::difference_type    difference_type;
        typedef typename _allocator::pointer            pointer;
        typedef typename _allocator::const_pointer      const_pointer;
        typedef typename _allocator::reference          reference;
        typedef typename _allocator::const_reference    const_reference;
        
    #if __cplusplus >= 201103L
    protected:
        std::shared_ptr<_allocator> _M_allocator;
    public:
        pool_allocator() _YXXX_NOEXCEPT:
            _M_allocator(std::make_shared<_allocator>()){}
        template <typename _Up>
        pool_allocator(const pool_allocator<_Up>& other)
        _YXXX_NOEXCEPT: _M_allocator(other._M_allocator){}
        ~pool_allocator() _YXXX_NOEXCEPT = default;
    #else
    protected:
        _allocator* _M_allocator;
        static stl::size_t _M_pool_alloc_countor;
    public:
        pool_allocator() _YXXX_NOEXCEPT:
            _M_allocator(new _allocator{}){
            ++_M_pool_alloc_countor;
        }
        template <typename _Up>
        pool_allocator(const pool_allocator<_Up>& other)
        _YXXX_NOEXCEPT: _M_allocator(other._M_allocator){
            ++_M_pool_alloc_countor;
        }
        ~pool_allcator() _YXXX_NOEXCEPT{
            --_M_pool_alloc_countor;
            if(0 == _M_pool_alloc_countor)
                delete _M_allocator;
            _M_allocator = nullptr;
        }
    #endif

        pointer allocate(size_type _size) _YXXX_NOEXCEPT{
            return _M_allocator->allocate(_size);
        }
        bool deallocate(pointer _ptr) _YXXX_NOEXCEPT{
            _M_allocator->deallocate(_ptr);
        }

        void construct(pointer _ptr){
            new ((void*)_ptr) value_type();
        }
        template <typename ...Args>
        void construct(pointer _ptr, Args&&... _args){
            new ((void*)_ptr) value_type(
                std::forward<Args>(_args)...
            );
        }
        void destroy(pointer _ptr){
            if(nullptr == _ptr) return;
            _ptr->~value_type();
        }

        pool_allocator&
        operator=(const pool_allocator& other)
        _YXXX_NOEXCEPT{
            _M_allocator = other._M_allocator;
            return *this;
        }
        friend bool
        operator==(const pool_allocator& lhs,
            const pool_allocator& rhs) _YXXX_NOEXCEPT{
            return lhs._M_allocator == rhs._M_allocator;
        }
        friend bool
        operator!=(const pool_allocator& lhs,
            const pool_allocator& rhs) _YXXX_NOEXCEPT{
            return !(lhs == rhs);
        }
    };

    // 无效分配器
    template <typename _Tp>
    class pool_allocator<const _Tp>{
    public:
        typedef _Tp             value_type;
        pool_allocator()=delete;
    };
    template <typename _Tp>
    class pool_allocator<volatile _Tp>{
    public:
        typedef _Tp             value_type;
        pool_allocator()=delete;
    };
    template <typename _Tp>
    class pool_allocator<const volatile _Tp>{
    public:
        typedef _Tp             value_type;
        pool_allocator()=delete;
    };
    template <>
    class pool_allocator<void>{
    public:
        typedef void     value_type;
        pool_allocator()=delete;
    };
};


#endif