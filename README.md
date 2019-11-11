
## nosync::qt - nosync::mt_event_loop on top of Qt event loop

Small header-only library with implementation of nosync::mt_event_loop that
runs on to of Qt event loop.

To create nosync::mt_event_loop you only need to provide a pointer to QObject
that will be used as context for invoking tasks in Qt event loop.
In most cases you'll just provide a pointer to your QCoreApplication, e.g.:

```
QScopedPointer<QCoreApplication> app(new QCoreApplication(argc, argv));
auto mt_evloop = nosync::qt::make_qt_based_event_loop(app.get());
```

Copyright (C) Zbigniew Chyla 2019
