
########### next target ###############

SET(cwinterface_LIB_SRCS
   ScriptMgr.cpp
   ScriptMgr.h
   config.h
   system.cpp
   Scripts/sc_default.cpp
   Scripts/sc_defines.cpp
   Scripts/sc_defines.h
   Scripts/on_events.cpp
)

add_library(cwinterface SHARED ${cwinterface_LIB_SRCS})

target_link_libraries(cwinterface)

set_target_properties(cwinterface PROPERTIES VERSION 4.2.0 SOVERSION 4)
install(TARGETS cwinterface DESTINATION lib)
