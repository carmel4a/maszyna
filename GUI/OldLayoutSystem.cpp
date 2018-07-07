#include "OldLayoutSystem.h"
#include "nanogui/nanogui.h"
#include "GUI.h"
namespace OldLayout{

    void _set_axis_anchor( 
            Widget& what,
            Widget& to,
            Anchor anchor,
            short axis ){

        switch( anchor.mode ){
            case OldLayout::Alignment::Begin:{

                what.setPosition(
                        _get_rel_to_axis<Vector2i>(
                                ( anchor.is_margin_rel ) ? Vector2i(
                                    to.absolutePosition()
                                    - what.parent()->absolutePosition()
                                    + (anchor.margin_rel * to.size().cast<float>()).cast<int>()
                                ) : Vector2i(
                                    to.absolutePosition() 
                                    - what.parent()->absolutePosition()
                                    + Vector2i( anchor.margin, anchor.margin )
                                ),
                                what.absolutePosition(),
                                axis )
                );
                GUI.update_layout( &what );
                break;
            }
            case OldLayout::Alignment::Centered:{
                what.setPosition(
                        _get_rel_to_axis<Vector2i>(
                                to.absolutePosition() 
                                - what.parent()->absolutePosition() 
                                + ( 0.5F * to.size().cast<float>() ).cast<int>()
                                - ( 0.5F * what.size().cast<float>() ).cast<int>(),
                                what.absolutePosition(),
                                axis )
                );
                GUI.update_layout( &what );
                break;
            }
            case OldLayout::Alignment::End:{

                what.setPosition(
                        _get_rel_to_axis<Vector2i>(
                                ( anchor.is_margin_rel ) ? Vector2i(
                                    to.absolutePosition()
                                    - what.parent()->absolutePosition()
                                    + to.size()
                                    - what.size()
                                    - (anchor.margin_rel * to.size().cast<float>()).cast<int>()
                                ) : Vector2i(
                                    to.absolutePosition() 
                                    - what.parent()->absolutePosition()
                                    + to.size()
                                    - what.size()
                                    + Vector2i( -anchor.margin, -anchor.margin )
                                ),
                                what.absolutePosition(),
                                axis )
                );
                GUI.update_layout( &what );
                break;
            }
            case OldLayout::Alignment::Fill:{
                what.setPosition(
                        _get_rel_to_axis<Vector2i>( to.absolutePosition() - what.parent()->absolutePosition(),
                                what.absolutePosition(),
                                axis )
                );
                what.setFixedSize( 
                        _get_rel_to_axis( to.size(),
                                what.size(),
                                axis )
                );
                GUI.update_layout( &what );
                break;
            }
        }
    }
};