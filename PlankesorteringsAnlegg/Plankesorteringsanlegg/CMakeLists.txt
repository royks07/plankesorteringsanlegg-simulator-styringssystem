# Some flags for Freeglut and GLUI.
add_definitions( -DFREEGLUT_EXPORTS -DFREEGLUT_STATIC -D_CRT_SECURE_NO_WARNINGS )


# pthread------------------------------------------------------------
find_package (Threads REQUIRED)

# Boost --------------------------------------------------------------
 set(Boost_USE_STATIC_LIBS OFF)
 set(Boost_USE_STATIC_RUNTIME OFF )
 set(Boost_USE_MULTITHREADED OFF)
 find_package(Boost 1.46.0 COMPONENTS thread system date_time REQUIRED)
 #for 1.53.0 tok FindBoost feil...
 
include_directories(${Boost_INCLUDE_DIRS})

message(STATUS "variable: ${Boost_INCLUDE_DIRS}")
message(STATUS "variable: ${Boost_LIBRARIES}")


# Define Plankesorteringsanlegg.
set(Plankesorteringsanlegg_SRCS
	Main.cpp
	Render.cpp
	Render.h
	SimulatorPage.cpp
	SimulatorPage.h
	Plankesorteringsanlegg.h
	SimulatorPageEntries.cpp
	Actuators.h
	Communicator.h
	Conveyor.h
	ConveyorSynchronizer.h
	Package.h
	Packaging.h
	Sensors.h
	Storages.h
	Text.h
	UserData.h
)


# These are used to create visual studio folders.
source_group(Plankesorteringsanlegg FILES ${Plankesorteringsanlegg_SRCS})

include_directories (
    ${Boost_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	${Box2D_SOURCE_DIR}
)

if(APPLE)
	# We are not using the Apple's framework version, but X11's
	include_directories( /usr/X11/include )
	link_directories( /usr/X11/lib )
	set (OPENGL_LIBRARIES GL GLU GLUT X11)
elseif(WIN32)
	set (ADDITIONAL_LIBRARIES winmm)
endif(APPLE)

add_executable(Plankesorteringsanlegg
	${Plankesorteringsanlegg_SRCS}
)

target_link_libraries (
	Plankesorteringsanlegg
	Box2D
	freeglut_static
	glui
	${ADDITIONAL_LIBRARIES}
	${OPENGL_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
) 
