//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : FlagOptions.cpp                                               //
//  Project   : CMD                                                           //
//  Date      : Mar 06, 2018                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2018                                             //
//                                                                            //
//  Description :                                                             //
//                                                                            //
//---------------------------------------------------------------------------~//

#include "../include/FlagOptions.h"
// AmazingCow Libs
#include "acow/cpp_goodies.h"
#include "CoreAssert/CoreAssert.h"


//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//
void CMD::CheckFlagOptionsValidity(FlagOptions options) noexcept
{
    //--------------------------------------------------------------------------
    // Check arguments options.
    COREASSERT_ONLY_IN_DEBUG({
        auto has_argument_flag = ACOW_FLAG_HAS_ANY(
            options,
            NoArgument,
            OptionalArgument,
            RequiredArgument
        );


        if(has_argument_flag)
        {

            auto argument_flag_exclusive = ACOW_FLAG_ARE_EXCLUSIVE(
                options,
                NoArgument,
                OptionalArgument,
                RequiredArgument
            );

            COREASSERT_ASSERT(
                argument_flag_exclusive,
                "Argument flags are Exclusive flags and can't match - Flags: %u",
                options
            );
        }
    });

    //--------------------------------------------------------------------------
    // Duplicate options
    COREASSERT_ONLY_IN_DEBUG({
        auto has_duplicate_flag = ACOW_FLAG_HAS_ANY(
            options,
            NoDuplicates,
            AllowDuplicates
        );

        if(has_duplicate_flag)
        {
            auto duplicate_flag_exclusive = ACOW_FLAG_ARE_EXCLUSIVE(
                options,
                NoDuplicates,
                AllowDuplicates
            );

            COREASSERT_ASSERT(
                duplicate_flag_exclusive,
                "Duplicate flags are Exclusive flags and can't match - Flags: %u",
                options
            );
        }
    });

    //--------------------------------------------------------------------------
    // Replacement flags.
    COREASSERT_ONLY_IN_DEBUG({
        auto has_replacement_flag = ACOW_FLAG_HAS_ANY(
            options,
            MergeArgument,
            DiscardPreviousArgument,
            DiscardCurrentArgument
        );

        if(has_replacement_flag)
        {
            auto replacement_flag_exclusive = ACOW_FLAG_HAS_ANY(
                options,
                MergeArgument,
                DiscardPreviousArgument,
                DiscardCurrentArgument
            );

            COREASSERT_ASSERT(
                replacement_flag_exclusive,
                "Replacement flags are Exclusive flags and can't match - Flags: %u",
                options
            );
        }
    });
}

void CMD::AddFlagOptionsMissingDefaults(FlagOptions *pOptions) noexcept
{
    COREASSERT_ASSERT(pOptions, "pOptions can't be null");

    //--------------------------------------------------------------------------
    // Argument Allowance.
    if(!ACOW_FLAG_HAS_ANY(
        *pOptions,
        NoArgument,
        OptionalArgument,
        RequiredArgument))
    {
        ACOW_FLAG_ADD(NoArgument, *pOptions);
    }

    //--------------------------------------------------------------------------
    // Argument Duplication.
    if(!ACOW_FLAG_HAS_ANY(
        *pOptions,
        NoDuplicates,
        AllowDuplicates))
    {
        ACOW_FLAG_ADD(NoDuplicates, *pOptions);
    }

    //--------------------------------------------------------------------------
    // Argument Replacement.
    if(!ACOW_FLAG_HAS_ANY(
        *pOptions,
        MergeArgument,
        DiscardPreviousArgument,
        DiscardCurrentArgument))
    {
        ACOW_FLAG_ADD(DiscardPreviousArgument, *pOptions);
    }
}
