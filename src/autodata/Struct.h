#ifndef AUTOSTRUCTNOTH_H
#define AUTOSTRUCTNOTH_H

// --- POCO Includes --- //
#include <Poco/Dynamic/Struct.h>
#include <Poco/Data/Session.h>


/// AutoStructNoTH holds a Poco::Dynamic::Struct< std::string > in a Dynamic::Var,
/// and also contains typename and id attributes that are needed for interacting
/// with datastores.
using Poco::Dynamic::Var;
class AutoStructNoTH
{
public:
    ///
    AutoStructNoTH()
        :
        m_struct( Poco::Dynamic::Struct< std::string >() ),
        m_check_table( true )
    {

    }

    ///
    Poco::Dynamic::Var& GetStruct()
    {
        return m_struct;
    }

    ///
    const Poco::Dynamic::Var& GetStruct() const
    {
        return m_struct;
    }

    ///
    std::string GetTypename()
    {
        return m_typename;
    }

    ///
    void SetTypename(std::string const& name)
    {
        m_typename = name;
    }

    ///
    std::string GetID()
    {
        return m_id;
    }

    ///
    void SetID(std::string const& id)
    {
        m_id = id;
        m_struct["id"] = m_id;
    }

    ///
    Poco::Dynamic::Var& operator [](
        std::string const& name )
    {
        return m_struct[ name ];
    }

    ///
    Poco::Dynamic::Var const& operator [](
        std::string const& name ) const
    {
        return m_struct[ name ];
    }

    ///
    virtual void CreateID();

    ///
    virtual std::string ToJSON();

    ///
    virtual void FromJSON( std::string const& json );

    ///
    virtual void Load( Poco::Data::Session& session );

    ///
    virtual void Save( Poco::Data::Session& session );

    ///
    virtual Poco::Dynamic::Struct< std::string >::ConstIterator begin() const;

    ///
    virtual Poco::Dynamic::Struct< std::string >::ConstIterator end() const;

private:
    ///
    void CreateTable( Poco::Data::Session& session );
    ///
    bool TableExists( Poco::Data::Session& session );
    ///
    Poco::Dynamic::Var m_struct;
    ///
    std::string m_typename;

    /// Mutable because we use lazy id creation and might need to create an
    /// id for an instance that is currently const
    mutable std::string m_id;

    /// Mutable so table can be created even when instance is const
    mutable bool m_check_table;
};


#endif // AUTOSTRUCTNOTH_H
