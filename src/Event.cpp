/****Copyright (C) 2022  eggplantcx
 **Event.cpp
 **Full License can be found in LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html>
 */
#include "../inc/Event.hpp"
#include "../inc/EventHandler.hpp"

Event::Event(){}

Event::Event(const EventType &type) : _type(type) {}

