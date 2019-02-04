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

#define GlClearErrors()                       \
        while( glGetError() != GL_NO_ERROR ); \

#ifndef NDEBUG
    #define glCall(x)                            \
        GlClearErrors()                          \
        x;                                       \
        if( glGetError() != GL_NO_ERROR ) throw;
#else
    #define glCall(x) \
        (x);
#endif

#endif // !GL_ERROR_01_02_19
