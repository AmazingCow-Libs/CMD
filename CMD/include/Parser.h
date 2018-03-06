#pragma once

// std
#include <string>
#include <vector>
#include <initializer_list>
// CMD
#include "CMD_Utils.h"
#include "Flag.h"

NS_CMD_BEGIN

enum ParserOptions {
    DieOnNonValidFlags  = 1 << 0,
    IgnoreNonValidFlags = 1 << 1,
    WarnNonValidFlags   = 1 << 2
};

void CheckParserOptionsValidity(ParserOptions options) noexcept;

class Parser
{
    //------------------------------------------------------------------------//
    // CTOR / DTOR                                                            //
    //------------------------------------------------------------------------//
public:
    Parser(
        ParserOptions options,
        const std::initializer_list<Flag::SPtr> &flags = {}) noexcept;

    //------------------------------------------------------------------------//
    // Public Methods                                                         //
    //------------------------------------------------------------------------//
public:
    //--------------------------------------------------------------------------
    // Flags.
    Parser& AddFlag(Flag::SPtr pFlag) noexcept;

    Parser& AddFlag(
        const std::string &shortName,
        const std::string &longName,
        const std::string &description,
        CMD::FlagOptions   flagOptions) noexcept;

    inline const std::vector<Flag::SPtr> GetFlags() const noexcept
    {
        return m_flags;
    }

    inline const std::vector<std::string> GetNonFlagValues() const noexcept
    {
        return m_nonFlagValues;
    }


    //--------------------------------------------------------------------------
    // Parser.
    Parser& Parse(int argc, char **argv) noexcept;

    //--------------------------------------------------------------------------
    // Utils.
    std::string GenerateHelpString(int maxColumns = -1) const noexcept;


    //------------------------------------------------------------------------//
    // Private Methods                                                        //
    //------------------------------------------------------------------------//
private:
    bool ShouldContinueOrDieOnNonValidFlags(
        const std::string &flag) const noexcept;


    //--------------------------------------------------------------------------
    // Find Flag.
    Flag* FindFlagByShortName(const std::string &shortName) const noexcept;
    Flag* FindFlagByLongName (const std::string &shortName) const noexcept;

    Flag* FindFlagByName(
        const std::string &shortName,
        const std::string &longName) const noexcept;


    //------------------------------------------------------------------------//
    // iVars                                                                  //
    //------------------------------------------------------------------------//
private:
    std::vector<Flag::SPtr>  m_flags;
    std::vector<std::string> m_nonFlagValues;

    ParserOptions m_options;
};

NS_CMD_END
