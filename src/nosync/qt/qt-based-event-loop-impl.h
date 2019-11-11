#ifndef NOSYNC__QT__QT_BASED_EVENT_LOOP_IMPL_H
#define NOSYNC__QT__QT_BASED_EVENT_LOOP_IMPL_H

#include <QElapsedTimer>
#include <QTimer>
#include <algorithm>
#include <cstdint>
#include <functional>
#include <nosync/memory-utils.h>
#include <nosync/os/dependent-mt-event-loop.h>
#include <nosync/qt/qt-based-event-loop.h>
#include <utility>


namespace nosync::qt
{

std::shared_ptr<os::mt_event_loop> make_qt_based_event_loop(QObject *qt_context)
{
    namespace ch = std::chrono;
    using std::move;

    struct evloop_context
    {
        QObject *qt_context;
        QElapsedTimer etime_timer;
        std::function<void()> wakeup_func;
    };

    auto ctx = std::make_shared<evloop_context>();
    ctx->qt_context = qt_context;
    ctx->etime_timer.start();

    return os::make_dependent_mt_event_loop(
        [ctx]() {
            return ch::milliseconds(ctx->etime_timer.elapsed());
        },
        [ctx](ch::nanoseconds etime, std::function<void()> wakeup_func) {
            ctx->wakeup_func = move(wakeup_func);
            auto rel_time_ms = std::max<std::int64_t>(
                ch::ceil<ch::milliseconds>(etime).count() - ctx->etime_timer.elapsed(), 0);
            QTimer::singleShot(
                rel_time_ms, ctx->qt_context,
                [ctx]() {
                    if (ctx->wakeup_func) {
                        auto wakeup_func = move(ctx->wakeup_func);
                        ctx->wakeup_func = nullptr;
                        wakeup_func();
                    }
                });
        },
        [ctx](std::function<void()> task) {
            QMetaObject::invokeMethod(ctx->qt_context, move(task), Qt::QueuedConnection);
        });
}

}

#endif
