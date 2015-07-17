
#pragma once

#include <iterator>
#include <functional>
#include <type_traits>
#include <utility>

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
    ///
    using iterator_type =
        decltype( std::begin( std::declval< Container >() ) );
    using iterator_diff =
        typename std::iterator_traits< iterator_type >::difference_type;
    using iterator_ref =
        typename std::iterator_traits< iterator_type >::reference;
    using keyfunc_return_type =
        decltype( std::declval< KeyFunc >()( std::declval< iterator_ref >() ) );

    ///
    Collapse() = delete;
    Collapse( Collapse const& ) = delete;
    Collapse& operator=( Collapse const& ) = delete;
    Collapse& operator=( Collapse&& ) = delete;

    //Not supported in VS 2013
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
    class Range
    {
    public:
        Range(
            iterator_type beg,
            iterator_type end,
            iterator_type rngbeg,
            iterator_type rngend,
            bool wrap,
            iterator_diff size )
            :
            m_beg( std::move( beg ) ),
            m_end( std::move( end ) ),
            m_rngbeg( std::move( rngbeg ) ),
            m_rngend( std::move( rngend ) ),
            m_wrap( wrap ),
            m_size( size )
        {
            ;
        }

    private:
        iterator_type const m_beg;
        iterator_type const m_end;
        iterator_type const m_rngbeg;
        iterator_type const m_rngend;
        bool const m_wrap;
        iterator_diff const m_size;

        ///
        class RangeIterator : public std::iterator< std::input_iterator_tag,
            std::remove_reference< iterator_ref > >
        {
        public:
            RangeIterator(
                iterator_type beg,
                iterator_type end,
                iterator_type rngbeg,
                iterator_type rngend,
                bool wrap )
                :
                m_beg( std::move( beg ) ),
                m_end( std::move( end ) ),
                m_itr( std::move( rngbeg ) ),
                m_rngend( std::move( rngend ) ),
                m_wrap( wrap )
            {
                ;
            }

            bool operator ==(
                RangeIterator const& rhs ) const
            {
                return ( m_itr == rhs.m_itr );
            }

            bool operator !=(
                RangeIterator const& rhs ) const
            {
                return !( *this == rhs );
            }

            ///prefix
            RangeIterator& operator ++()
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
                if( m_wrap && m_itr == m_end ) m_itr = m_beg;
            }

            iterator_type const m_beg;
            iterator_type const m_end;
            iterator_type m_itr;
            iterator_type const m_rngend;
            bool const m_wrap;
        };

    public:
        RangeIterator begin() const
        {
            return { m_beg, m_end, m_rngbeg, m_rngend, m_wrap };
        }

        RangeIterator end() const
        {
            return { m_beg, m_end, m_rngend, m_rngend, m_wrap };
        }

        iterator_diff size() const
        {
            return m_size;
        }
    };

    ///
    class Group
    {
    public:
        Group(
            keyfunc_return_type&& value,
            Range&& range )
            :
            m_value( std::forward< keyfunc_return_type >( value ) ),
            m_range( std::move( range ) )
        {
            ;//std::cout << &m_value << std::endl;
        }

        keyfunc_return_type value() const
        {
            return m_value;
        }

        Range const& range() const
        {
            return m_range;
        }

    private:
        keyfunc_return_type m_value;
        Range m_range;
    };

    ///
    class GroupIterator : public std::iterator< std::input_iterator_tag, Group >
    {
    public:
        GroupIterator(
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
            GroupIterator const& rhs ) const
        {
            return ( m_rngbeg == rhs.m_rngbeg );
        }

        bool operator !=(
            GroupIterator const& rhs ) const
        {
            return !( *this == rhs );
        }

        ///prefix
        GroupIterator& operator ++()
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
            return
            {
                m_keyfunc( *m_rngbeg ),
                Range( m_beg, m_end, m_rngbeg, m_rngend, m_wrap, m_size )
            };
        }

    private:
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

public:
    ///
    GroupIterator begin()
    {
        iterator_type beg = std::begin( m_container );
        iterator_type end = std::end( m_container );
        bool wrap = m_binpred(
            m_keyfunc( *beg ), m_keyfunc( *std::prev( end ) ) );
            //No std::rbegin for gcc 4.9.2
            //( m_keyfunc( *beg ) == m_keyfunc( *std::rbegin( m_container ) ) );
        return
        {
            std::move( beg ), std::move( end ),
            m_keyfunc, m_binpred, wrap
        };
    }

    ///
    GroupIterator end()
    {
        return
        {
            std::end( m_container ), std::end( m_container ),
            m_keyfunc, m_binpred
        };
    }
};

///
template< typename Container >
class DefKeyFunc
{
public:
    using iterator_ref = decltype( *std::declval<
        decltype( std::begin( std::declval< Container >() ) ) >() );
        //Doesn't work with gcc 4.9.2
        //typename std::iterator_traits< iterator_type >::reference;

    iterator_ref operator ()(
        iterator_ref val ) const
    {
        return val;
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
        collapse( std::forward< Container >( container ),
            keyfunc, std::equal_to<>() ) )
{
    return collapse(
        std::forward< Container >( container ),
        keyfunc,
        std::equal_to<>() );
}

///
template< typename Container >
auto collapse(
    Container&& container ) -> decltype(
        collapse( std::forward< Container >( container ),
            DefKeyFunc< Container >(), std::equal_to<>() ) )
{
    return collapse(
        std::forward< Container >( container ),
        DefKeyFunc< Container >(),
        std::equal_to<>() );
}

} //iter
