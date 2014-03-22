TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += \
    main.cpp

HEADERS += \
    FCHistogram.hpp \
    FCSystem.hpp \
    FCStatistics.hpp \
    FCDetector.hpp \
    FCSignal.hpp \
    FCFilter.hpp \
    FCMath.hpp \
    FCTrace.hpp \
    FCCounter.hpp

OTHER_FILES +=
    
QMAKE_CXXFLAGS += -std=c++0x