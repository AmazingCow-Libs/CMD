#pragma once

// std
#include <vector>
#include <string>
// AmazingCow Libs
#include "acow/cpp_goodies.h"
// CMD
#include "CMD_Utils.h"
#include "FlagOptions.h"

NS_CMD_BEGIN


class Flag
{
    //------------------------------------------------------------------------//
    // Enums / Constants / Typedefs                                           //
    //------------------------------------------------------------------------//
public:
    ACOW_SHARED_PTR_OF(Flag);


    //------------------------------------------------------------------------//
    // Static Methods                                                         //
    //------------------------------------------------------------------------//
public:
    inline static SPtr Create(
        const std::string &shortName,
        const std::string &longName,
        const std::string &description,
        FlagOptions        options,
        const std::string &group = "") noexcept
    {
        return std::make_shared<Flag>(
            shortName,
            longName,
            description,
            options,
            group
        );
    }

    //------------------------------------------------------------------------//
    // CTOR / DTOR                                                            //
    //------------------------------------------------------------------------//
public:
    Flag(
        const std::string &shortName,
        const std::string &longName,
        const std::string &description,
        FlagOptions        options,
        const std::string &group = "");


    //------------------------------------------------------------------------//
    // Public Methods                                                         //
    //------------------------------------------------------------------------//
public:
    //--------------------------------------------------------------------------
    // Info.
    inline        FlagOptions GetOptions    () const noexcept { return m_options;     }
    inline const std::string& GetShortName  () const noexcept { return m_shortName;   }
    inline const std::string& GetLongName   () const noexcept { return m_longName;    }
    inline const std::string& GetDescription() const noexcept { return m_description; }

    //--------------------------------------------------------------------------
    // Found.
    inline bool Found() const noexcept { return m_foundCount != 0; }
    inline void IncrementFoundCount() noexcept { ++m_foundCount; }

    //--------------------------------------------------------------------------
    // Value.
    std::string GetValue () const noexcept;
    const std::vector<std::string>& GetValues() const noexcept;

    void AddValue(const std::string &value) noexcept;


    //------------------------------------------------------------------------//
    // iVars                                                                  //
    //------------------------------------------------------------------------//
private:
    //--------------------------------------------------------------------------
    // Info.
    std::string m_shortName;
    std::string m_longName;
    std::string m_description;
    FlagOptions m_options;

    std::string m_group;

    //--------------------------------------------------------------------------
    // Values
    std::vector<std::string> m_values;

    bool m_found;
    int  m_foundCount;

}; // class Flag

NS_CMD_END
