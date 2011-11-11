project(KDock)

find_package(KDE4 REQUIRED)
find_package(X11 REQUIRED)

include(KDE4Defaults)
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDES})

set(KDock_SRCS main.cpp AppMatcher.cpp DockApp.cpp DockConfig.cpp DockContainer.cpp DockDragDropEvent.cpp DockGraphicsScene.cpp DockIcon.cpp DockPanelView.cpp DockProxyView.cpp DockPanel.cpp DockTaskIcon.cpp DockTaskManager.cpp)
kde4_add_executable(KDock ${KDock_SRCS})



target_link_libraries(KDock ${KDE4_KDEUI_LIBS} ${KDE4_KUTILS_LIBS} ${X11_X11_LIB} taskmanager)
install(TARGETS KDock ${INSTALL_TARGETS_DEFAULT_ARGS})
