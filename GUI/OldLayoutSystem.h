/*
            ,     \    /      ,
           / \    )\__/(     / \   
          /   \  (_\  /_)   /   \                
  _______/_____\__\@  @/___/_____\_______
  |               |\../|                |
  |                \VV/                 |
  |                                     |
  |     Jeśli działa - nie ruszaj       |
  | Jeśli nie działa - sorry            |
  |_____________________________________|
      |    /\ /      \\       \ /\    |
      |  /   V        ))       V   \  |
      |/     `       //        '     \|
      `              V
*/
#ifndef _OLD_LAYOUT_SYS_
#define _OLD_LAYOUT_SYS_

#pragma once

#include "nanogui/nanogui.h"
#include "GUI.h"

namespace OldLayout {
    enum Alignment : short{
        Begin,    ///< up to down, left to right
        Centered, ///< centered, prefered size
        End,      ///< down to up, right to left
        Fill      ///< fill this axis
    };

    struct Anchor{
        Anchor( Alignment mode = Alignment::Begin ) :
        mode{mode} {};
        bool is_margin_rel = true;
        int margin = 0;
        float margin_rel = 0;
        Alignment mode;
    };
    inline void set_x_anchor( Widget* what, Widget* to, OldLayout::Anchor anchor_x );
    inline void set_y_anchor( Widget* what, Widget* to, OldLayout::Anchor anchor_y );
    template< typename T = Vector2i > inline auto _get_axis( T t, int i );
    template< typename T = Vector2i > inline T _get_rel_to_axis( T v1, T v2 , int axis );
    void _set_axis_anchor( Widget& what, Widget& to, Anchor anchor, short axis );

    inline void set_x_anchor( 
            Widget* what,
            Widget* to,
            OldLayout::Anchor anchor_x
    ){ _set_axis_anchor( *what, *to, anchor_x, 0 ); };
    
    inline void set_y_anchor( 
            Widget* what,
            Widget* to,
            OldLayout::Anchor anchor_y
        ){ _set_axis_anchor( *what, *to, anchor_y, 1 ); };
        
    template< typename T = Vector2i >
    inline auto _get_axis( T t, int i ){
        return i == 0 ? t.x() : t.y();
    };

    template< typename T = Vector2i >
    inline T _get_rel_to_axis( T v1, T v2 , int axis ){
        return (axis == 0) ?
            T(
               _get_axis( v1, axis ),
               _get_axis( v2, !axis ) ) :
            T(
               _get_axis( v2, !axis ),
               _get_axis( v1, axis ) );
    };
}

#endif // !_OLD_LAYOUT_SYS_