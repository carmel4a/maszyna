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
 *  @defgroup Terrain Terrain
 * 
 *  Terrain (ground level, water, buildings, static objects) is divided on
 *  Section s.
 *  
 *  Section consist of array of quads (2 triangles). Manager::max_side_density
 *  determines maximum number of quads. Terrain::Section should replace
 *  scene::Section.
 *
 *  `Active section` - Section which is close enough
 *  to [camera](@ref global_settings::pCamera), to be loaded into vram. */

#ifndef TERRAIN_H_19_10_18
#define TERRAIN_H_19_10_18
#pragma once

#include "./TerrainContainer.h"
#include "./GeometryBanksManager.h"
#include "./Types.h"
#include "../stdafx.h"
#include "../Scene/SceneConfig.h"
#include "./Threads/Threads.hpp"
#include "./Config.hpp"

/// Namespace for terrain entities.
/** @addtogroup Terrain */
namespace Terrain
{
    class BatchingTask;
    /// Entry point to Terrain management.
    /** @note no copyable.
     *  @todo move get/reset geometry buffer to private, like:
     *  ```C++
     *      friend Section::load;
     *      friend Section::unload;
     *  ```
     *  @todo Move thread creation to (no existing) global thread manager.
     *  @addtogroup Terrain */
    class Manager
    {
        friend BatchingTask;
      public:
      ///@{ @name Special member functions
        /// Default constructor.
        /** Fills @ref terrain array with newly created Secton s.
         *  
         *  Creates required [geometry banks](@ref geometry_banks). */
        Manager();

        /// Destructor
        /** Joins created threads. */
        ~Manager();

        /// Prevention of copying.
        Manager( Manager& ) = delete;

        /// Prevention of copying.
        Manager& operator=( Manager& ) = delete; ///@}

      ///@{ @name Types
        /// Struct to store mutexes.
        struct Mutexes
        {
            /// Guard swap @ref m_active_sections.
            /** Is locked on swap shared [sections list](@ref m_active_sections)
            with [renderer](@ref opengl_renderer). */
            std::mutex active_list_swap,
            /// Guard unloading sections, eg. in duration of rendering.
                       section_unload;
        }; ///@}

      ///@{ @name Serialization
        /// Deserialize terrain from provided parser.
        /** @param input - input from where Manager should read data.
         *  @return if deserialization was successfull.
         *  @todo To implement. */
        bool deserialize( cParser& input );

        /// Serialize terrain to provided parser.
        /** Possbile side efects in filesystem.
         *  @param output - where data will be stored. May be stdout and a file.
         *  @return if serialization was successfull.
         *  @todo To implement. */
        bool serialize( cParser& output ); ///@}

      ///@{ @name Getters
        /// Gets @ref m_active_sections.
        /** @return container of pairs witch id, and pointer, to `active
         *  sections`. */
        inline auto activeSections() const -> const SectionsContainer&; ///@}

        /// Returns section coordinate (x, y) from world-space point.
        /** @param T - Vector type. Must provide `x` and `z` field.
         *  @param t - point in world-space. */
        template< class T >
        static auto getSectionCoordFromPos( T t ) -> glm::ivec2;

        template< class X, class Y >
        static constexpr auto getSectionCoordFromPos( X x, Y y ) -> glm::ivec2;

        template< class X, class Y >
        static constexpr auto getSectionIDFromCoord( X x, Y y ) -> unsigned;
        template< class T >
        static auto getSectionIDFromCoord( T t ) -> unsigned;

        auto getCameraSectionId() const -> unsigned
        {
            unsigned id;
            camera_section_id.get( id );
            return id;
        }

        void setCameraSectionId( unsigned var )
        { camera_section_id.set( var ); }

      ///@{ @name Data
        /// Public mutexes.
        /** Currenlty used by renderer. */
        Mutexes mutexes; ///@}

      private:
      ///@{ @name Consts
        /// Half side of update sections square.
        constexpr static unsigned update_range { Terrain::update_range };

      ///@{ @name Data
        ///
        TerrainContainer terrain;
        SectionsContainer m_active_sections; ///@}

      /// @{ Threads
        TerrainThreads threads; ///@}

        class CameraSectionID
        {
          public:
            inline CameraSectionID();
            inline void get( unsigned& var ) const;
            inline void set( unsigned var );

          private:
            unsigned id;
            mutable std::mutex mutex;
        } camera_section_id;
    };

    auto Manager::activeSections() const -> const SectionsContainer&
    { return m_active_sections; }

    Manager::CameraSectionID::CameraSectionID()
            : id { std::numeric_limits< unsigned >::max() } {}

    void Manager::CameraSectionID::set( unsigned var )
    {
        std::scoped_lock< std::mutex > lock ( mutex );
        id = var;
    }

    void Manager::CameraSectionID::get( unsigned& var ) const
    {
        std::scoped_lock< std::mutex > lock ( mutex );
        var = id;
    }

    #include "./Terrain.tpp"
}

#endif // !TERRAIN_H_19_10_18
