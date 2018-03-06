// Header
#include "../include/Flag.h"
// AmazingCow Libs
#include "acow/cpp_goodies.h"
#include "CoreAssert/CoreAssert.h"
// CMD
#include "include/private/Logger.h"

// Usings
USING_NS_CMD


//----------------------------------------------------------------------------//
// CTOR / DTOR                                                                //
//----------------------------------------------------------------------------//
Flag::Flag(
    const std::string &shortName,
    const std::string &longName,
    const std::string &description,
    FlagOptions        options)
    // Members...
    : m_shortName  (  shortName)
    , m_longName   (   longName)
    , m_description(description)
    , m_options    (    options)
    , m_found      (      false)
    , m_foundCount (          0)
{
    //--------------------------------------------------------------------------
    // Sanity Checks.
    COREASSERT_ASSERT(
        !shortName.empty() || !longName.empty(),
        "Flag must have a short or long name."
    );

    COREASSERT_ONLY_IN_DEBUG({
        CheckFlagOptionsValidity(m_options);
    });

    //--------------------------------------------------------------------------
    // Add needed stuff...
    AddFlagOptionsMissingDefaults(&m_options);
}

//----------------------------------------------------------------------------//
// Public Methods                                                             //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
// Value.
std::string
Flag::GetValue() const noexcept
{
    COREASSERT_ASSERT(
        ACOW_FLAG_HAS(AllowDuplicates, m_options),
        "Flag (%s - %s) allows duplicates - Use Flag::GetValues instead"
    );

    if(m_values.empty())
        return "";

    return m_values[0];
}

const std::vector<std::string>&
Flag::GetValues() const noexcept
{
    return m_values;
}

void
Flag::AddValue(const std::string &value) noexcept
{
    //--------------------------------------------------------------------------
    // No duplicates.
    if(ACOW_FLAG_HAS(NoDuplicates, m_options) && m_values.size() >= 1)
    {
        _private::get_logger_instance()->Fatal(
            "Flag (%s - %s) doesn't allow duplicates.",
            m_shortName.c_str(),
            m_longName .c_str()
        );
    }

    //--------------------------------------------------------------------------
    // Merge Arguments.
    else if(ACOW_FLAG_HAS(MergeArgument, m_options))
    {
        m_values.push_back(value);
    }

    //--------------------------------------------------------------------------
    // Discard Previous
    else if(ACOW_FLAG_HAS(DiscardPreviousArgument, m_options))
    {
        m_values[0] = value;
    }

    //--------------------------------------------------------------------------
    // Discard Current...
    //   There's nothing to do...
}


// //----------------------------------------------------------------------------//
// //               █      █                                                     //
// //               ████████                                                     //
// //             ██        ██                                                   //
// //            ███  █  █  ███        Flag.cpp                                  //
// //            █ █        █ █        CMD                                       //
// //             ████████████                                                   //
// //           █              █       Copyright (c) 2016, 2017                  //
// //          █     █    █     █      AmazingCow - www.AmazingCow.com           //
// //          █     █    █     █                                                //
// //           █              █       N2OMatt - n2omatt@amazingcow.com          //
// //             ████████████         www.amazingcow.com/n2omatt                //
// //                                                                            //
// //                  This software is licensed as GPLv3                        //
// //                 CHECK THE COPYING FILE TO MORE DETAILS                     //
// //                                                                            //
// //    Permission is granted to anyone to use this software for any purpose,   //
// //   including commercial applications, and to alter it and redistribute it   //
// //               freely, subject to the following restrictions:               //
// //                                                                            //
// //     0. You **CANNOT** change the type of the license.                      //
// //     1. The origin of this software must not be misrepresented;             //
// //        you must not claim that you wrote the original software.            //
// //     2. If you use this software in a product, an acknowledgment in the     //
// //        product IS HIGHLY APPRECIATED, both in source and binary forms.     //
// //        (See opensource.AmazingCow.com/acknowledgment.html for details).    //
// //        If you will not acknowledge, just send us a email. We'll be         //
// //        *VERY* happy to see our work being used by other people. :)         //
// //        The email is: acknowledgment_opensource@AmazingCow.com              //
// //     3. Altered source versions must be plainly marked as such,             //
// //        and must not be misrepresented as being the original software.      //
// //     4. This notice may not be removed or altered from any source           //
// //        distribution.                                                       //
// //     5. Most important, you must have fun. ;)                               //
// //                                                                            //
// //      Visit opensource.amazingcow.com for more open-source projects.        //
// //                                                                            //
// //                                  Enjoy :)                                  //
// //----------------------------------------------------------------------------//

// //Header
// #include "../include/Flag.h"
// //std
// #include <limits>
// #include <stdexcept>
// //CMD
// #include "../include/CMD_Macros.h"


// //Usings
// USING_NS_CMD;


// ////////////////////////////////////////////////////////////////////////////////
// // Constants                                                                  //
// ////////////////////////////////////////////////////////////////////////////////
// const int Flag::kAnyNumber = std::numeric_limits<int>::max();


// ////////////////////////////////////////////////////////////////////////////////
// // CTOR                                                                        //
// ////////////////////////////////////////////////////////////////////////////////
// Flag::Flag():
//     m_requiredArgs     (false),
//     m_requiredArgsCount(0),

//     m_optionalArgs     (false),
//     m_optionalArgsCount(0),

//     m_stopOnView(false),

//     m_allowDuplicates(false),
//     m_duplicateMode  (Flag::DuplicateMode::Count),

//     m_foundCount(0)
// {
//     //Empty...
// }


// ////////////////////////////////////////////////////////////////////////////////
// // Options "Setters"                                                          //
// ////////////////////////////////////////////////////////////////////////////////
// //Args
// Flag& Flag::RequiresNoArgs()
// {
//     m_requiredArgs = false;
//     m_optionalArgs = false;

//     m_requiredArgsCount = 0;
//     m_optionalArgsCount = 0;

//     return *this;
// }

// Flag& Flag::RequiresArgs(int count /* = 1 */)
// {
//     m_requiredArgs      = true;
//     m_requiredArgsCount = count;

//     THROW_IF_(
//         m_requiredArgsCount <= 0,
//         std::out_of_range,
//         "CMD::Flag::RequiredArgs must be called with > 0 value - Current is: %d",
//         m_requiredArgsCount
//     );

//     return *this;
// }

// Flag& Flag::OptionalArgs(int count /* = 1 */ )
// {
//     m_optionalArgs      = true;
//     m_optionalArgsCount = count;

//     THROW_IF_(
//         m_optionalArgsCount <= 0,
//         std::out_of_range,
//         "CMD::Flag::OptionalArgs must be called with > 0 value - Current is: %d",
//         m_optionalArgsCount
//     );

//     return *this;
// }


// //
// Flag& Flag::StopOnView()
// {
//     m_stopOnView = true;
//     return *this;
// }


// //Defaults
// ///
// Flag& Flag::Default(const std::string &value)
// {
//     m_defaultValue = value;
//     return *this;
// }


// //Duplicates
// Flag& Flag::NotAllowDuplicates()
// {
//     m_allowDuplicates = false;
//     return *this;
// }

// Flag& Flag::AllowDuplicates(DuplicateMode duplicateMode)
// {
//     m_allowDuplicates = true;
//     m_duplicateMode  = duplicateMode;

//     return *this;
// }


// //Strings
// Flag& Flag::Short(const std::string &shortStr)
// {
//     m_shortStr = shortStr;
//     return *this;
// }

// Flag& Flag::Long(const std::string &longStr)
// {
//     m_longStr = longStr;
//     return *this;
// }



// ////////////////////////////////////////////////////////////////////////////////
// // Options "Gettters"                                                         //
// ////////////////////////////////////////////////////////////////////////////////
// //Args
// bool Flag::getNoArgsRequired() const
// {
//     return m_requiredArgs == false && m_optionalArgsCount == false;
// }


// bool Flag::getRequiredArgs() const
// {
//     return m_requiredArgs;
// }

// int Flag::getRequiredArgsCount() const
// {
//     return m_requiredArgsCount;
// }

// int Flag::getRequiredArgsFoundCount() const
// {
//     return m_requiredValuesVec.size();
// }

// const std::vector<std::string>& Flag::getRequiredValues() const
// {
//     return m_requiredValuesVec;
// }


// bool Flag::getOptionalArgs() const
// {
//     return m_optionalArgs;
// }

// int Flag::getOptionalArgsCount() const
// {
//     return m_optionalArgsCount;
// }

// int Flag::getOptionalArgsFoundCount() const
// {
//     return m_optionalValuesVec.size();
// }

// const std::vector<std::string>& Flag::getOptionalValues() const
// {
//     return m_optionalValuesVec;
// }


// //
// bool Flag::getStopOnView() const
// {
//     return m_stopOnView;
// }


// //Defaults
// const std::string& Flag::getDefaultValue() const
// {
//     return m_defaultValue;
// }


// //Duplicates
// bool Flag::getAllowDuplicates() const
// {
//     return m_allowDuplicates;
// }

// Flag::DuplicateMode Flag::getAllowDuplicatesMode() const
// {
//    ASSERT_(getAllowDuplicates() == true,
//            "BAD LOGIC - We should not call this when getAllowDuplicates() is false. %s",
//            getLongStr().c_str());

//    return m_duplicateMode;
// }


// //Strings
// bool Flag::hasShortStr() const
// {
//     return !m_shortStr.empty();
// }

// const std::string& Flag::getShortStr() const
// {
//    ASSERT_(hasShortStr() == true,
//            "BAD LOGIC - We should not call this when hasShortStr() is false.");

//    return m_shortStr;
// }

// const std::string Flag::getShortDisplayStr() const
// {
//     return (hasShortStr())
//         ? std::string("-") + getShortStr()
//         : "";
// }

// bool Flag::hasLongStr() const
// {
//     return !m_longStr.empty();
// }

// const std::string& Flag::getLongStr() const
// {
//     ASSERT_(hasLongStr() == true,
//            "BAD LOGIC - We should not call this when hasLongStr() is false.");

//     return m_longStr;
// }

// const std::string Flag::getLongDisplayStr() const
// {
//     return (hasLongStr())
//         ? std::string("--") + getLongStr()
//         : "";
// }

// const std::string Flag::getShortLongDisplayStr() const
// {
//     auto format = (hasShortStr() && hasLongStr())
//         ? "%s %s"
//         : "%s";

//     auto shortStr = getShortDisplayStr();
//     auto longStr  = getLongDisplayStr ();

//     char buffer[shortStr.size() + longStr.size() + 1];
//     sprintf(
//         buffer,
//         format,
//         shortStr.c_str(),
//         longStr.c_str ()
//     );

//     return buffer;
// }


// //Found
// bool Flag::wasFound() const
// {
//     return m_foundCount != 0;
// }

// int Flag::getFoundCount() const
// {
//     return m_foundCount;
// }


// ////////////////////////////////////////////////////////////////////////////////
// // Private Methods                                                            //
// ////////////////////////////////////////////////////////////////////////////////
// void Flag::addFoundCount()
// {
//     //COWTODO: Throw if do not allow duplicates and this value is > 1
//     m_foundCount++;
// }

// void Flag::addRequiredValue(const std::string &value)
// {
//     //COWTODO: Throw if already reached the ammount needed.
//     m_requiredValuesVec.push_back(value);
// }

// void Flag::addOptionalValue(const std::string &value)
// {
//     //COWTODO: Throw if already reached the ammount needed.
//     m_optionalValuesVec.push_back(value);
// }


// void Flag::validateStrings() const
// {
//     //Validate the strings...
//     THROW_IF_(
//         (!hasShortStr() && !hasLongStr()),
//         std::logic_error,
//         "CMD::Flag must contain either a short or long string"
//     );

//     //Short string must has only one char.
//     THROW_IF_(
//         (hasShortStr() && m_shortStr.size() != 1),
//         std::domain_error,
//         "CMD::Flag shortStr must be only char long - shortStr is: %s",
//         m_shortStr.c_str()
//     );
// }

// void Flag::clearValues()
// {
//     m_requiredValuesVec.clear();
//     m_optionalValuesVec.clear();
// }
