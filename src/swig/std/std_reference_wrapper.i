%include <std_common.i>

%{
    #include <functional>
%}

namespace std
{
template< typename T >
class reference_wrapper
{
public:
    typedef T type;

    reference_wrapper( T& ref ) noexcept : _ptr( std::addressof( ref ) ){}
    reference_wrapper( T&& ) = delete;
    reference_wrapper( reference_wrapper const& ) noexcept = default;
    reference_wrapper& operator =( reference_wrapper const& x ) noexcept = default;

    //operator T&() const noexcept{ return *_ptr; }
    T& get() const noexcept{ return *_ptr; }

    /*template< typename... ArgTypes >
    typename std::result_of< T&( ArgTypes&&... ) >::type operator ()(
        ArgTypes&&... args ) const
    {
        return std::invoke( get(), std::forward< ArgTypes >( args )... );
    }*/

private:
    T* _ptr;
};
}
