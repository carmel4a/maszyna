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

#ifndef BATCHING_TERRAIN_TASK_05_02_19
#define BATCHING_TERRAIN_TASK_05_02_19

#include "./TerrainTask.hpp"

#include "../../Config.hpp"
#include "../../../Scene/SceneConfig.h"

#include "stdafx.h"

/** @addtogroup Terrain
 * 
 * Update square side.
 *
 *      ...........
 *      .."""|""".. ^ ^
 *      .."""|""".. | |  Manager::update_range = 3
 *      .."""|""".. | V
 *      ..---X---.. |    BatchingTask::range = 7
 *      .."""|""".. |
 *      .."""|""".. |    X - Starting section (camera position)
 *      .."""|""".. V
 *      ...........
 * 
 *  `update range` - number of sections EXCLUDING section with camera, which
 *  should be rendered.
 * 
 *  `range` - lenght of side of square with a active sections.
 *
 *  `active section` - section which is loaded, backed and ready to be drawn.
 *  It is in `update range` from camera. Else it will be unloaded. */
namespace Terrain
{
    class Manager;

    /// Section management task.
    /** It is responsible for loading and unloading (to RAM) Sections. It also provides
     *  active Section list to Manager.
     * 
     *  @note Manager friend.
     *  @note no actual operation on GPU is performed. Manager on mainthread
     *  should do that.
     *  
     *  @todo implement operations on GPU here. It should be done after
     *  task manager refactor (throw away task management from Python
     *  interpreter, then move terrain tasks to new place). */
    class BatchingTask : public TerrainTask
    {
      public:
        BatchingTask( Manager* const );
        ~BatchingTask() = default;

        /// Implementation of task
        /** Loads and unloads Sections. Swaps section list with Manager. */
        void run() override;

        /// It is.
        /** It should execute as long as Terrain exists. */
        constexpr bool isInfinite() const override;

        /// Providdes copy of this task.
        /** Shallow copy of @ref manager pointer. */
        virtual auto copy() const -> TaskPtr override;

      private:
        struct Sections
        {
            SectionsContainer active;
            std::set< unsigned > to_load;
            std::set< unsigned > to_unload;
        };

        /// Load sections around provided origin Section.
        /***/
        void loadSections( glm::ivec2 origin );

        /// Unload sections around provided origin Section.
        void unloadSections( glm::ivec2 origin );

        /// Swap data with @ref manager.
        void swapSectionsWithManager();

        Manager* const manager;

        Sections sections;

        /// Section side size (in meters).
        static constexpr unsigned section_side = scene::SECTION_SIZE;

        /// Region sections side number.
        static constexpr unsigned section_s_num =
                scene::REGION_SIDE_SECTION_COUNT;

        static constexpr short range
                { (short)( 2 * Terrain::update_range + 1 ) };
    };
} // Terrain

#endif // !BATCHING_TERRAIN_TASK_05_02_19
