// TextTriggerAction.cpp
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

#include "TriggerAction.h"
#include "TextTriggerAction.h"

#include <string>

#include "video/MessagePool.h"

using std::string;

/**
 * Execute the action
 * 
 * For this TextTriggerAction this methode post the saved message 
 * 
 * @see MessagePool::postMessage()
 */
void TextTriggerAction::execute()
{
	// Post the message in the pool
	this->pool->postMessage(this->message);
}

/**
 * Build a TextTriggerAction with a message in string 
 * and a ref to the Message Pool
 * 
 * @param message Message to show
 * @param pool The MessagePool to use
 */
TextTriggerAction::TextTriggerAction(string message, MessagePool* pool) : 
	TriggerAction(TriggerAction::TEXT)
{
	// Set the message
	this->message = message;
	// Set the pool to use
	this->pool = pool;
}

TextTriggerAction::~TextTriggerAction()
{
}