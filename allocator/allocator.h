#ifndef _YXXX_ALLOCATOR_H_
#define _YXXX_ALLOCATOR_H_

#include "../adaptive_c++config.h"

namespace _alloc{
    template <typename _Alloc>
    class _alloc_traits: public std::allocator_traits<_Alloc>{
    public:
        typedef std::allocator_traits<_Alloc>      _allocator_traits;
        typedef typename _allocator_traits::value_type       value_type;
        typedef typename _allocator_traits::pointer          pointer;
        typedef typename _allocator_traits::const_pointer    const_pointer;
        typedef typename _allocator_traits::size_type        size_type;
        typedef typename _allocator_traits::difference_type  difference_type;
        typedef value_type&        reference;
        typedef const value_type&  const_reference;

        template <typename _Up>
        using rebind_alloc = typename std::__replace_first_arg<_Alloc, _Up>::type;
    };

    template <typename _Alloc, typename _Up>
    using alloc_rebind = typename _alloc_traits<_Alloc>::template rebind_alloc<_Up>;

    template <typename _Tp>
    class allocator{
    public:
        typedef _Tp             value_type;
        typedef std::size_t     size_type;
        typedef std::ptrdiff_t  difference_type;

        typedef _Tp*            pointer;
        typedef const _Tp*      const_pointer;
        typedef _Tp&            reference;
        typedef const _Tp&      const_reference;

        static const pointer zero_bytes;

        allocator() _YXXX_NOEXCEPT = default;
        template <typename _Up>
        allocator(const allocator<_Up>&) _YXXX_NOEXCEPT{}

        allocator& operator=(const allocator&)
        _YXXX_NOEXCEPT{return *this;}

        /* 分配_size个连续的value_type的内存空间 */
        pointer allocate(size_type _size){
            if(0 == _size)  return zero_bytes;
            // ...bad_alloc...
            // ...align...
            return static_cast<pointer>(
                ::operator new(sizeof(value_type) * _size)
            );
        }
        /* 收回_ptr指向的内存块，_size指示该内存块的大小 */
        void deallocate(value_type* _ptr, size_type = 0){
            if(zero_bytes == _ptr) return;
            ::operator delete(_ptr);
        }

        /* 扩充_ptr指向的内存块，_size指示扩充前内存块的大小,_need指示扩充后内存块的最小容量 */
        _YXXX_CONSTEXPR bool
        expandable(pointer _ptr, size_type _size = 0,
            size_type _need = 0) const{
            return false;
        }
        std::pair<pointer, size_type>
        expand(pointer _ptr, size_type _size, size_type _need){
            if(_need <= _size) return {_ptr, _size};
            pointer _rtn = ::operator new(_need);
            if(zero_bytes != _ptr){
                memmove(_rtn, _ptr, sizeof(value_type) * _size);
                ::operator delete(_ptr);
            }
            return {_rtn, _need};
        }
        
        /* 在_ptr指向的地址构造value_type对象 */
        void construct(pointer _ptr){
            new ((void*)_ptr) value_type();
        }
        template <typename ...Args>
        void construct(pointer _ptr, Args&&... _args){
            new ((void*)_ptr) value_type(
                std::forward<Args>(_args)...
            );
        }
        /* 销毁_ptr指向的地址的value_type对象 */
        void destroy(pointer _ptr){
            if(zero_bytes == _ptr) return;
            _ptr->~value_type();
        }
    };
    template <typename _Tp> bool
    operator==(const allocator<_Tp>&, const allocator<_Tp>&){
        return true;
    }
    template <typename _Tp> bool
    operator!=(const allocator<_Tp>&, const allocator<_Tp>&){
        return false;
    }

    // 无效分配器
    template <typename _Tp>
    class allocator<const _Tp>{
    public:
        typedef _Tp             value_type;
        allocator()=delete;
    };
    template <typename _Tp>
    class allocator<volatile _Tp>{
    public:
        typedef _Tp             value_type;
        allocator()=delete;
    };
    template <typename _Tp>
    class allocator<const volatile _Tp>{
    public:
        typedef _Tp             value_type;
        allocator()=delete;
    };
    template <>
    class allocator<void>{
    public:
        typedef void     value_type;
        allocator()=delete;
    };

    template <typename _Tp>
    const typename allocator<_Tp>::pointer 
    allocator<_Tp>::zero_bytes
        = static_cast<pointer>(::operator new(0));
};

#endif