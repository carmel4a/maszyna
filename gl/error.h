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

#ifndef GL_ERROR_01_02_19
#define GL_ERROR_01_02_19

#define GlClearErrors()                    \
        while( glError() != GL_NO_ERROR ); \

#ifndef NDEBUG
    #define GlCall(x)                  \
        GlClearErrors()                \
        (x);                           \
        if( glError() != GL_NO_ERROR )
#else
    #define GlCall(x) \
        (x);
#endif

#endif // !GL_ERROR_01_02_19
