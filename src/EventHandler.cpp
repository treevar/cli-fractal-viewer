/****Copyright (C) 2022  eggplantcx
 **Event.cpp
 **Full License can be found in LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html>
 */
#include "../inc/EventHandler.hpp"

EventHandler::EventHandler() = default;

void EventHandler::fireEvent(const Event &e){
    fireEvent(e._type, e._dataToVoidPointer());
}

void EventHandler::fireEvent(const Event::EventType &type, void *data){
    for(auto &pair : _callbacks){
        auto &cb = pair.second;
        for(auto &t : cb.types){
            if(t == type){
                cb.call(data);
            }
            else if(t == ALL_EVENTS){
                AllEventsData *anyData = new AllEventsData{type, data};
                cb.call(anyData);
                if(anyData != nullptr){ delete anyData; } 
            }
        }
    }
    if(data != nullptr){ delete data; }   
}

void EventHandler::fireEvent(AllEventsData *data){
    if(data == nullptr){ return; }
    fireEvent(data->type, data->data);
}

EventHandler::CallbackID EventHandler::registerCallback(const Callback &c){
    CallbackID id = _nextID++;
    _callbacks[id] = c;
    return id;
}

void EventHandler::removeCallback(CallbackID id){
    if(id == 0){ return; }
    _callbacks.erase(id);
}

const Event::EventType EventHandler::ALL_EVENTS = "ALL_EVENTS";