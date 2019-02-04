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

#ifndef TERRAIN_TASK_05_02_19
#define TERRAIN_TASK_05_02_19
#pragma once

#include "../../Types.h"

namespace Terrain
{
    /// Base class for thread designed tasks.
    /** @addtogroup Terrain
     *  Used by Manager. */
    class TerrainTask
    {
      public:
        /// Execute task.
        virtual void run() =0;

        /// Is this task infinite?
        /** Infinity tasks are recreated after execution. Should be pushed back
         * to task queue. */
        virtual bool isInfinite() const =0;

        /// Copy this task.
        /** Should be implemented to create equal task. May be used by
         *  taskqueue. */
        virtual auto copy() const -> TaskPtr =0;
    };
} // Terrain

#endif // !TERRAIN_TASK_05_02_19
