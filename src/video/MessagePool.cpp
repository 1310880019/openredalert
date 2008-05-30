// MessagePool.cpp
// 1.0

//    This file is part of OpenRedAlert.
//
//    OpenRedAlert is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    OpenRedAlert is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with OpenRedAlert.  If not, see <http://www.gnu.org/licenses/>.

#include "MessagePool.h"

#include <list>
#include <functional>

#include "SDL/SDL_types.h"
#include "SDL/SDL_timer.h"

#include "video/Renderer.h"
#include "include/Logger.h"
#include "ui/RA_Label.h"
#include "video/drawmessage.h"
#include "Message.h"

using std::list;

MessagePool::MessagePool() : updated(false), textimg(0), width(0)
{
    const SDL_Color White = {255, 255, 255, 0};

	msglabel.setColor(White);
	msglabel.UseAntiAliasing(false);
	msglabel.SetFont("grad6fnt.fnt");
}

MessagePool::~MessagePool()
{
    SDL_FreeSurface(textimg);
}

SDL_Surface *MessagePool::getMessages()
{
    Uint32 curtick = SDL_GetTicks();
    SDL_Rect dest;

    if (!updated) {
        /// @TODO Replace this with STL magic
        for (list<Message>::iterator i = msglist.begin(); i != msglist.end();) {
            if (i->expired(curtick)) {
                updated = true;
                i = msglist.erase(i);
            } else {
                ++i;
            }
        }
    }
    if (!updated) { // Check again now we've possibly changed the value in the above loop
        return textimg;
    }
    updated = false;
    SDL_FreeSurface(textimg);
    textimg = NULL;
    if (msglist.empty()) {
        return textimg;
    }
    textimg = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCCOLORKEY, width,
            static_cast<int>(msglist.size()*(msglabel.getHeight()+1)), 16, 0, 0, 0, 0);
    dest.x = 0;
    dest.y = 0;
    dest.w = textimg->w;
    dest.h = textimg->h;
    SDL_FillRect(textimg, &dest, 0);
    SDL_SetColorKey(textimg, SDL_SRCCOLORKEY, 0);
    Uint32 msgy = 0;
    drawMessage dm(msglabel, textimg, msgy);
    for_each(msglist.begin(), msglist.end(), dm);
    return textimg;
}

void MessagePool::postMessage(string msg)
{
	string mess = msg;	
    msglist.push_back(Message(mess, SDL_GetTicks() + 10000));
    updated = true;
}

void MessagePool::clear()
{
    msglist.clear();
    updated = true;
}

void MessagePool::refresh()
{
    //Reload the font
	// msglabel.reload();

    updated = true;
    getMessages(); //Forces the SDL_Surface to reload
}

void MessagePool::setWidth(Uint32 width)
{
	this->width = width;
}

Uint32 MessagePool::getWidth() const 
{
	return width;
}
