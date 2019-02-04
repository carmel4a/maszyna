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

#ifndef SCENE_TERRAIN_TYPES_24_12_18
#define SCENE_TERRAIN_TYPES_24_12_18
#pragma once

#include "../stdafx.h"

namespace Terrain
{
    class Section; class TerrainTask;

    /// Stores ID of section, and pointer to it.
    using SectionsContainer =
            std::unordered_map< unsigned, Terrain::Section* >;

    using TaskPtr = std::unique_ptr< TerrainTask >;

}

#endif // !SCENE_TERRAIN_TYPES_24_12_18
