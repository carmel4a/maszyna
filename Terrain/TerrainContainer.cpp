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

#include "TerrainContainer.h"

#include "Section.h"

namespace Terrain
{
    TerrainContainer::TerrainContainer( const unsigned max_side_density )
    {
        // \note IDs of sections start at 0
        for( int i = 0; i < content.size(); ++i )
            content[ i ].reset( new Terrain::Section( max_side_density, i ) );
    }
}
