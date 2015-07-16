
#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

namespace iter
{

///Forward declaration for friendship
template< typename Container, typename KeyFunc >
class Collapse;
template< typename Container, typename KeyFunc >
Collapse< Container, KeyFunc > collapse( Container&&, KeyFunc );

///
template< typename Container, typename KeyFunc >
class Collapse
{
public:
    ///
    using iterator_type =
        typename decltype( std::begin( std::declval< Container >() ) );
    using iterator_ref =
        typename std::iterator_traits< iterator_type >::reference;
    using keyfunc_return_type = typename decltype(
        std::declval< KeyFunc >()( std::declval< iterator_ref >() ) );

    ///
    Collapse() = delete;
    Collapse( Collapse const& ) = delete;
    Collapse& operator=( Collapse const& ) = delete;
    Collapse& operator=( Collapse&& ) = delete;
    Collapse( Collapse&& o ) = delete;

private:
    ///
    Collapse(
        Container&& container,
        KeyFunc keyfunc )
        :
        m_container( std::forward< Container >( container ) ),
        m_keyfunc( std::move( keyfunc ) )
    {
        ;
    }
    friend Collapse collapse< Container, KeyFunc >( Container&&, KeyFunc );

    ///Container is lvalue or rvalue
    Container m_container;
    KeyFunc m_keyfunc;

    ///
    class Range
    {
    public:
        Range(
            iterator_type beg,
            iterator_type end,
            iterator_type rngbeg,
            iterator_type rngend,
            bool wrap )
            :
            m_beg( std::move( beg ) ),
            m_end( std::move( end ) ),
            m_rngbeg( std::move( rngbeg ) ),
            m_rngend( std::move( rngend ) ),
            m_wrap( wrap )
        {
            ;
        }

    private:
        iterator_type const m_beg;
        iterator_type const m_end;
        iterator_type const m_rngbeg;
        iterator_type const m_rngend;
        bool const m_wrap;

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
            KeyFunc const& keyfunc,
            bool wrap = false )
            :
            m_beg( std::move( beg ) ),
            m_end( std::move( end ) ),
            m_rngbeg( m_beg ),
            m_rngend( m_beg ),
            m_rotend( m_end ),
            m_keyfunc( keyfunc ),
            m_wrap( wrap )
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
                Range( m_beg, m_end, m_rngbeg, m_rngend, m_wrap )
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
            typename keyfunc_return_type value = m_keyfunc( *ritr++ );
            while( ritr != rend && m_keyfunc( *ritr ) == value )
            {
                ++ritr;
            }
            m_rngbeg = ritr.base();
            //If we iterated all the way through the container,
            //all the items are the same and no wrapping is needed
            if( ritr == rend ){ m_wrap = false; m_rngend = m_end; }
            else m_rotend = m_rngbeg;
        }

        void advance_range_end()
        {
            if( m_rngend == m_rotend ) return;
            typename keyfunc_return_type value = m_keyfunc( *m_rngend++ );
            while( m_rngend != m_rotend && m_keyfunc( *m_rngend ) == value )
            {
                ++m_rngend;
            }
        }

        iterator_type const m_beg;
        iterator_type const m_end;
        iterator_type m_rngbeg;
        iterator_type m_rngend;
        iterator_type m_rotend;
        KeyFunc const& m_keyfunc;
        bool m_wrap;
    };

public:
    ///
    GroupIterator begin()
    {
        iterator_type beg = std::begin( m_container );
        bool wrap =
            ( m_keyfunc( *beg ) == m_keyfunc( *std::rbegin( m_container ) ) );
        return { std::move( beg ), std::end( m_container ), m_keyfunc, wrap };
    }

    ///
    GroupIterator end()
    {
        return { std::end( m_container ), std::end( m_container ), m_keyfunc };
    }
};

///
template< typename Container >
class DefKeyFunc
{
public:
    using iterator_type =
        typename decltype( std::begin( std::declval< Container >() ) );
    using iterator_ref =
        typename std::iterator_traits< iterator_type >::reference;

    iterator_ref operator ()(
        iterator_ref val ) const
    {
        return val;
    }
};

///
template< typename Container, typename KeyFunc >
Collapse< Container, KeyFunc > collapse(
    Container&& container,
    KeyFunc keyfunc )
{
    return { std::forward< Container >( container ), keyfunc };
}

///
template< typename Container >
auto collapse(
    Container&& container ) -> decltype( collapse(
        std::forward< Container >( container ),
        DefKeyFunc< Container >() ) )
{
    return collapse(
        std::forward< Container >( container ),
        DefKeyFunc< Container >() );
}

} //iter
