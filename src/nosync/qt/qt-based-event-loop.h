#ifndef NOSYNC__QT__QT_BASED_EVENT_LOOP_H
#define NOSYNC__QT__QT_BASED_EVENT_LOOP_H

#include <QObject>
#include <memory>
#include <nosync/os/mt-event-loop.h>


namespace nosync::qt
{

std::shared_ptr<os::mt_event_loop> make_qt_based_event_loop(QObject *qt_context);

}

#include <nosync/qt/qt-based-event-loop-impl.h>

#endif
