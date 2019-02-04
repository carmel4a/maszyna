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

#ifndef TERRAIN_THREADS_06_02_19
#define TERRAIN_THREADS_06_02_19
#pragma once

#include "../Types.h"

#include "../../stdafx.h"

namespace Terrain
{
    class TerrainTask; class BatchingTask; class Manager;

    class TerrainThreads
    {
        friend Manager;
        public:
        TerrainThreads( unsigned workers_count );

        template< class T, typename R=void, typename ...Args >
        void pushTask( R r, Args... args );

        void pushTask( TaskPtr );
        auto popTask() -> TaskPtr;

        private:
        static void run( TerrainThreads& );
        std::vector< std::thread > workers;
        std::deque< std::unique_ptr< TerrainTask > > tasks_queue;
        std::mutex tasks_queue_lock;
        bool kill;
    };

    template< class T, typename R=void, typename ...Args >
    void TerrainThreads::pushTask( R r, Args... args )
    {
        pushTask( std::make_unique< T >( r, args... ) );
    }
}

#endif // !TERRAIN_THREADS_06_02_19
