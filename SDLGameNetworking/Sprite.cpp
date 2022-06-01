#include "Sprite.h"



Sprite::Sprite(SDL_Renderer* renderer, string filename, int xPos, int yPos, bool useTransparency, string name): Bitmap (renderer, filename, xPos, yPos, useTransparency, name)
{
    
}


//Sprite::Sprite(Bitmap& bitmapToCopy): Bitmap(bitmapToCopy)
//{
//
//}


Sprite::~Sprite()
{
   
}


