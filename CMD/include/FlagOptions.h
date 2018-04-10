//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : FlagOptions.h                                                 //
//  Project   : CMD                                                           //
//  Date      : Mar 06, 2018                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2018                                             //
//                                                                            //
//  Description :                                                             //
//                                                                            //
//---------------------------------------------------------------------------~//

#pragma once

// std
#include <cstdint>
// CMD
#include "CMD_Utils.h"


NS_CMD_BEGIN

//----------------------------------------------------------------------------//
// Enums / Constants / Typedefs                                               //
//----------------------------------------------------------------------------//
enum {
    StopOnView = 1 << 0,       // 0000 0000 0001

    NoArgument       = 1 << 1, // 0000 0000 0010
    OptionalArgument = 1 << 2, // 0000 0000 0100
    RequiredArgument = 1 << 3, // 0000 0000 1000

    NoDuplicates    = 1 << 4,  // 0000 0001 0000
    AllowDuplicates = 1 << 5,  // 0000 0010 0000

    MergeArgument           = 1 << 6, // 0000 0100 0000
    DiscardPreviousArgument = 1 << 7, // 0000 1000 0000
    DiscardCurrentArgument  = 1 << 8, // 0001 0000 0000
};

typedef uint32_t FlagOptions;

//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//
void CheckFlagOptionsValidity(FlagOptions options) noexcept;
void AddFlagOptionsMissingDefaults(FlagOptions *pOptions) noexcept;


NS_CMD_END