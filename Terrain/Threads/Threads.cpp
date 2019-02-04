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

#include "./Threads.hpp"
#include "./Tasks/TerrainTask.hpp"

using Terrain::TerrainThreads;

TerrainThreads::TerrainThreads( unsigned workers_count )
        : workers {  }
        , tasks_queue {  }
        , tasks_queue_lock {  }
        , kill { false }
{
    while( workers_count-- )
        workers.push_back( std::thread( TerrainThreads::run, std::ref( *this ) ) );
}

void TerrainThreads::run( TerrainThreads& threads )
{
    while( !threads.kill )
        if( auto task = threads.popTask(); task ) task->run();
}

void TerrainThreads::pushTask( TaskPtr t )
{
    std::scoped_lock< std::mutex > lock( tasks_queue_lock );
    tasks_queue.push_back( std::move( t ) );
}

auto TerrainThreads::popTask() -> TaskPtr
{
    std::scoped_lock< std::mutex > lock( tasks_queue_lock );

    if( tasks_queue.empty() ) return nullptr;
    TaskPtr t { std::move( tasks_queue.front() ) };
    tasks_queue.pop_front();
    if( t->isInfinite() )
        tasks_queue.push_back( t->copy() );
    return t;
}

