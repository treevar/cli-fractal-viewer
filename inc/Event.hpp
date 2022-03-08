/****Copyright (C) 2022  eggplantcx
 **Event.hpp
 **Full License can be found in LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html>
 */
#ifndef EVENT_HPP
#define EVENT_HPP
#include <string>
class Event{
    public:
        typedef std::string EventType;

    protected: 
        EventType _type;

        Event();
        Event(const EventType &eType);
        virtual ~Event();
        
        virtual void *_dataToVoidPointer() const = 0;
    friend class EventHandler;
};
#endif//EVENT_HPP