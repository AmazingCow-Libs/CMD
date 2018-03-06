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
    StopOnView = 1 << 0,

    NoArgument       = 1 << 1,
    OptionalArgument = 1 << 2,
    RequiredArgument = 1 << 3,

    NoDuplicates    = 1 << 4,
    AllowDuplicates = 1 << 5,

    MergeArgument           = 1 << 6,
    DiscardPreviousArgument = 1 << 7,
    DiscardCurrentArgument  = 1 << 8,
};

typedef uint32_t FlagOptions;

//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//
void CheckFlagOptionsValidity(FlagOptions options) noexcept;
void AddFlagOptionsMissingDefaults(FlagOptions *pOptions) noexcept;


NS_CMD_END