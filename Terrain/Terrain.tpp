#include "./Terrain.h"

// getSectionCoordFromPos //

template< class T >
auto Manager::getSectionCoordFromPos( T t ) -> glm::ivec2
{ return getSectionCoordFromPos( t.x, t.y ); };

template<>
inline auto Manager::getSectionCoordFromPos( glm::vec3 t ) -> glm::ivec2
{ return getSectionCoordFromPos( t.x, t.z ); };

template<>
inline auto Manager::getSectionCoordFromPos( glm::dvec3 t ) -> glm::ivec2
{ return getSectionCoordFromPos( t.x, t.z ); };

template< class X, class Y >
constexpr auto Manager::getSectionCoordFromPos( X x, Y y ) -> glm::ivec2
{
    /* world space is not equal
    to `terrain` array, so this move of origin is needed. So, for center
    of Region we have, eg:
        (0.0 0.0 0.0) point in world space = (250 250) [coords of section] */
    return { (int)x / (int)scene::SECTION_SIZE
                    + scene::REGION_SIDE_SECTION_COUNT / 2,
                (int)y / (int)scene::SECTION_SIZE
                    + scene::REGION_SIDE_SECTION_COUNT / 2 };
};

// getSectionIDFromCoord //

template< class T >
auto Manager::getSectionIDFromCoord( T t ) -> unsigned
{ return getSectionIDFromCoord( t.x, t.y ); };

template<>
inline auto Manager::getSectionIDFromCoord( glm::vec3 t ) -> unsigned
{ return getSectionIDFromCoord( t.x, t.z ); };

template<>
inline auto Manager::getSectionIDFromCoord( glm::dvec3 t ) -> unsigned
{ return getSectionIDFromCoord( t.x, t.z ); };

template< class X, class Y >
constexpr auto Manager::getSectionIDFromCoord( X x, Y y ) -> unsigned
{ return x + y * scene::REGION_SIDE_SECTION_COUNT; };
