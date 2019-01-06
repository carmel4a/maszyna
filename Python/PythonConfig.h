/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/
/** @file
 * 
 */

#ifndef PYTHON_CONFIG_06_12_19
#define PYTHON_CONFIG_06_12_19
#pragma once

namespace python
{
    /// @todo Move that to other place.
    constexpr const bool is_64_bit = (bool)( sizeof( void* ) == 8 );
    constexpr const char* windows_lib_name[] { "python", "python64" };
    constexpr const char* linux_lib_name[]   { "linuxpython", "linuxpython64" };
    constexpr inline auto get_windows_lib_name() -> const char* const
    { return windows_lib_name[ is_64_bit ]; }
    constexpr inline auto get_linux_lib_name() -> const char* const
    { return linux_lib_name[ is_64_bit ]; }
}

#endif // !PYTHON_CONFIG_06_12_19