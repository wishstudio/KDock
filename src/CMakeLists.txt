include_directories(${KDE4_INCLUDES})

set(KDock_SRCS main.cpp DockWindow.cpp)
kde4_add_executable(KDock ${KDock_SRCS})

target_link_libraries(KDock ${KDE4_KDEUI_LIBS})
install(TARGETS KDock ${INSTALL_TARGETS_DEFAULT_ARGS})
