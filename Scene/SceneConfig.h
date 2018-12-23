/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/
/** \file
 * 
 */

#ifndef SCENE_CONFIG_H_23_12_18
#define SCENE_CONFIG_H_23_12_18
#pragma once

namespace scene
{
    constexpr unsigned CELL_SIZE = 250;
    /// Signle section's side size in meters.
    constexpr unsigned SECTION_SIZE = 1000;
    /// Number of sections along a side of square region.
    constexpr unsigned REGION_SIDE_SECTION_COUNT = 500;
    /// Total number of sections.
    constexpr unsigned SECTIONS_COUNT =
            REGION_SIDE_SECTION_COUNT * REGION_SIDE_SECTION_COUNT;
}

#endif // !SCENE_CONFIG_H_23_12_18
