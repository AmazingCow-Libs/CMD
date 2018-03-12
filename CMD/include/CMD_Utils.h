//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : CMD_Utils.h                                                   //
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

////////////////////////////////////////////////////////////////////////////////
// Versioning                                                                 //
////////////////////////////////////////////////////////////////////////////////
//The core version number.
#define COW_CMD_VERSION_MAJOR    "0"
#define COW_CMD_VERSION_MINOR    "0"
#define COW_CMD_VERSION_REVISION "2"

#define COW_CMD_VERSION       \
    COW_CMD_VERSION_MAJOR "." \
    COW_CMD_VERSION_MINOR "." \
    COW_CMD_VERSION_REVISION


////////////////////////////////////////////////////////////////////////////////
// Namespaces                                                                 //
////////////////////////////////////////////////////////////////////////////////
#define NS_CMD_BEGIN namespace CMD {
#define NS_CMD_END   }
#define USING_NS_CMD using namespace CMD;
