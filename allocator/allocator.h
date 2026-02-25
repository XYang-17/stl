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

        allocator() _YXXX_NOEXCEPT = default;
        template <typename _Up>
        allocator(const allocator<_Up>&) _YXXX_NOEXCEPT{}

        allocator& operator=(const allocator&)
        _YXXX_NOEXCEPT{return *this;}

        pointer allocate(size_type _size){
            // ...bad_alloc...
            // ...align...
            return static_cast<pointer>(
                ::operator new(sizeof(value_type) * _size)
            );
        }
        void deallocate(value_type* _ptr){
            ::operator delete(_ptr);
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
};

#endif