/****Copyright (C) 2022  eggplantcx
 **EventHandler.hpp
 **Full License can be found in LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html>
 */
#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP
#include "./Event.hpp"
#include <functional>
#include <vector>
#include <map>
class EventHandler{
    public:
        typedef unsigned int CallbackID;
        typedef std::function<void(void*)> CallbackFunc;
        typedef struct{
            CallbackFunc call;
            std::vector<Event::EventType> types;
        } Callback;
        
        typedef struct {
            Event::EventType type;
            void *data;
        } AllEventsData;

        static const Event::EventType ALL_EVENTS;
        
        EventHandler();
        void fireEvent(const Event &e);
        void fireEvent(const Event::EventType &type, void *data);
        void fireEvent(AllEventsData *data);
        CallbackID registerCallback(const Callback &c);
        void removeCallback(CallbackID id);

    private:
        bool _idExists();
        std::map<CallbackID, Callback> _callbacks;
        CallbackID _nextID = 1;
        
};
#endif//EVENT_HANDLER_HPP