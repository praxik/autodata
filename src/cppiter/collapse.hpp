
#pragma once

#include <iterator>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>

namespace iter
{

///Forward declaration for friendship
template< typename Container, typename KeyFunc, typename BinPred >
class Collapse;
template< typename Container, typename KeyFunc, typename BinPred >
Collapse< Container, KeyFunc, BinPred > collapse(
    Container&&, KeyFunc, BinPred );

///
template< typename Container, typename KeyFunc, typename BinPred >
class Collapse
{
public:
    ///Forward declarations
    class iterator;
    class Group;

    ///
    using iterator_type =
        decltype( std::begin( std::declval< Container >() ) );
    using iterator_diff =
        typename std::iterator_traits< iterator_type >::difference_type;
    using iterator_ref =
        typename std::iterator_traits< iterator_type >::reference;
    using keyfunc_return_type =
        decltype( std::declval< KeyFunc >()( std::declval< iterator_ref >() ) );

    ///Not supported in VS 2013
    //Collapse( Collapse&& ) = default;
    Collapse(
        Collapse&& o )
        :
        m_container( std::forward< Container >( o.m_container ) ),
        m_keyfunc( o.m_keyfunc ),
        m_binpred( o.m_binpred )
    {
        ;
    }

    ///
    Collapse() = delete;
    Collapse( Collapse const& ) = delete;
    Collapse& operator =( Collapse const& ) = delete;
    Collapse& operator =( Collapse&& ) = delete;

    ///
    iterator begin()
    {
        iterator_type beg = std::begin( m_container );
        iterator_type end = std::end( m_container );
        bool wrap = m_binpred(
            m_keyfunc( *beg ), m_keyfunc( *std::prev( end ) ) );
        return
        {
            std::move( beg ), std::move( end ),
            m_keyfunc, m_binpred, wrap
        };
    }

    ///
    iterator end()
    {
        return
        {
            std::end( m_container ), std::end( m_container ),
            m_keyfunc, m_binpred
        };
    }

private:
    ///
    Collapse(
        Container&& container,
        KeyFunc keyfunc,
        BinPred binpred )
        :
        m_container( std::forward< Container >( container ) ),
        m_keyfunc( keyfunc ),
        m_binpred( binpred )
    {
        ;
    }
    friend Collapse collapse< Container, KeyFunc, BinPred >(
        Container&&, KeyFunc, BinPred );

    ///Container is lvalue or rvalue
    Container m_container;
    KeyFunc m_keyfunc;
    BinPred m_binpred;

    ///
    class iterator : public std::iterator< std::input_iterator_tag, Group >
    {
    public:
        iterator(
            iterator_type&& beg,
            iterator_type&& end,
            KeyFunc keyfunc,
            BinPred binpred,
            bool wrap = false )
            :
            m_beg( std::move( beg ) ),
            m_end( std::move( end ) ),
            m_rngbeg( m_beg ),
            m_rngend( m_beg ),
            m_rotend( m_end ),
            m_keyfunc( keyfunc ),
            m_binpred( binpred ),
            m_wrap( wrap ),
            m_size( 0 )
        {
            if( m_wrap ) advance_range_begin();
            advance_range_end();
        }

        bool operator ==(
            iterator const& rhs ) const
        {
            return ( m_rngbeg == rhs.m_rngbeg );
        }

        bool operator !=(
            iterator const& rhs ) const
        {
            return !( *this == rhs );
        }

        ///prefix
        iterator& operator ++()
        {
            //Reset the size
            m_size = 0;

            //If we have a rotated/wrapped grouping, and have hit the rotation end
            if( m_wrap && ( m_rngend == m_rotend ) ) m_rngbeg = m_end; //finished
            else m_rngbeg = m_rngend;

            advance_range_end();
            return *this;
        }

        Group operator *() const
        {
            return { *this, m_keyfunc( *m_rngbeg ) };
        }

    private:
        friend class Group;

        void advance_range_begin()
        {
            using reverse_iterator_type =
                typename std::reverse_iterator< iterator_type >;
            reverse_iterator_type ritr( m_end );
            reverse_iterator_type rend( m_beg );
            if( ritr == rend ) return;
            keyfunc_return_type value = m_keyfunc( *ritr );
            do{ ++m_size; }
            while( ( ++ritr != rend ) &&
                m_binpred( m_keyfunc( *ritr ), value ) );
            m_rngbeg = ritr.base();
            //If we iterated all the way through the container,
            //all the items are the same and no wrapping is needed
            if( ritr == rend ){ m_wrap = false; m_rngend = m_end; }
            else m_rotend = m_rngbeg;
        }

        void advance_range_end()
        {
            if( m_rngend == m_rotend ) return;
            keyfunc_return_type value = m_keyfunc( *m_rngend );
            do{ ++m_size; }
            while( ( ++m_rngend != m_rotend ) &&
                m_binpred( m_keyfunc( *m_rngend ), value ) );
        }

        iterator_type const m_beg;
        iterator_type const m_end;
        iterator_type m_rngbeg;
        iterator_type m_rngend;
        iterator_type m_rotend;
        KeyFunc m_keyfunc;
        BinPred m_binpred;
        bool m_wrap;
        iterator_diff m_size;
    };

    ///
    class Group
    {
    public:
        ///Forward declarations
        class iterator;

        ///
        Group(
            typename Collapse::iterator const& owner,
            keyfunc_return_type&& value )
            :
            m_owner( owner ),
            m_value( std::forward< keyfunc_return_type >( value ) )
        {
            ;//std::cout << &m_value << std::endl;
        }

        ///
        Group() = delete;
        Group( Group const& ) = delete;
        Group& operator =( Group const& ) = delete;
        Group& operator =( Group&& ) = delete;

        ///
        iterator begin() const
        {
            return { *this, m_owner.m_rngbeg };
        }

        ///
        iterator end() const
        {
            return { *this, m_owner.m_rngend };
        }

        ///
        iterator_diff size() const
        {
            return m_owner.m_size;
        }

        ///
        keyfunc_return_type value() const
        {
            return m_value;
        }

        ///Haven't figured out how to recursively group groups yet...
        std::vector< typename std::decay< iterator_ref >::type > to_vector() const
        {
            return std::vector< typename std::decay< iterator_ref >::type >( begin(), end() );
        }

    private:
        Group(
            Group&& obj )
            :
            m_owner( obj.m_owner ),
            m_value( std::forward< keyfunc_return_type >( obj.m_value ) )
        {
            ;
        }

        typename Collapse::iterator const& m_owner;
        keyfunc_return_type m_value;

        class iterator : public std::iterator< std::input_iterator_tag,
            typename std::decay< iterator_ref >::type >
        {
        public:
            iterator(
                Group const& group,
                iterator_type beg )
                :
                m_group( group ),
                m_itr( std::move( beg ) )
            {
                ;
            }

            bool operator ==(
                iterator const& rhs ) const
            {
                return ( m_itr == rhs.m_itr );
            }

            bool operator !=(
                iterator const& rhs ) const
            {
                return !( *this == rhs );
            }

            ///prefix
            iterator& operator ++()
            {
                advance();
                return *this;
            }

            iterator_ref operator *() const
            {
                return *m_itr;
            }

        private:
            void advance()
            {
                ++m_itr;
                if( m_group.m_owner.m_wrap && m_itr == m_group.m_owner.m_end )
                {
                    m_itr = m_group.m_owner.m_beg;
                }
            }

            Group const& m_group;
            iterator_type m_itr;
        };
    };
};

///
class DefKeyFunc
{
public:
    template< class T >
    auto operator ()(
        T&& val ) const -> decltype( std::forward< T >( val ) )
    {
        return std::forward< T >( val );
    }
};

///std::equal_to<> in c++14
class DefBinPred
{
public:
    template< class T >
    auto operator ()(
        T&& lhs,
        T&& rhs ) const -> decltype(
            std::forward< T >( lhs ) == std::forward< T >( rhs ) )
    {
        return ( std::forward< T >( lhs ) == std::forward< T >( rhs ) );
    }
};

///
template< typename Container, typename KeyFunc, typename BinPred >
Collapse< Container, KeyFunc, BinPred > collapse(
    Container&& container,
    KeyFunc keyfunc,
    BinPred binpred )
{
    return { std::forward< Container >( container ), keyfunc, binpred };
}

///
template< typename Container, typename KeyFunc >
auto collapse(
    Container&& container,
    KeyFunc keyfunc ) -> decltype(
        collapse(
            std::forward< Container >( container ),
            keyfunc, DefBinPred() ) )
{
    return collapse(
        std::forward< Container >( container ),
        keyfunc, DefBinPred() );
}

///
template< typename Container >
auto collapse(
    Container&& container ) -> decltype(
        collapse(
            std::forward< Container >( container ),
            DefKeyFunc(), DefBinPred() ) )
{
    return collapse(
        std::forward< Container >( container ),
        DefKeyFunc(), DefBinPred() );
}

} //iter
