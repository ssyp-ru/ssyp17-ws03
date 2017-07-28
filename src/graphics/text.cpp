#include "text.h"

namespace re
{

Point2f getFontPos( char ch )
{
    float he = 1.0f/32.0f;
    float we = 1.0f/8.0f;

    switch( ch )
    {
        case 'A': return Point2f( he * 2, we * 2 );
        case 'B': return Point2f( he * 2, we * 3 );
        case 'C': return Point2f( he * 2, we * 4 );
        case 'D': return Point2f( he * 2, we * 5 );
        case 'E': return Point2f( he * 2, we * 6 );
        case 'F': return Point2f( he * 2, we * 7 );
    }
}

void drawText( Point2f pos, std::string text )
{
    static bool init = false;
    static ImagePtr font;

    if( !init )
    {
        font = std::make_shared<Image>( "incoshow.png" );
        init = true;
    }

    for( char ch : text )
    {
        Point2f textPos = getFontPos( ch );
        draw_image_part( pos.x, pos.y, pos.x + 50, pos.y + 100,
                         //textPos.x, textPos.y, 
                         //textPos.x+(1.0f/32.0f), textPos.y+(1.0f/8.0f), 
                         0,0,
                         1.0f/32.0f,
                         1.0f/8.0f,
                         font );
    }
}

}