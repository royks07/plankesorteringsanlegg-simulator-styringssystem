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


# --------------------------------------------------------------------


# Define the framework files.
set(Testbed_Framework_SRCS
	Framework/json/json-forwards.h
	Framework/json/json.h
	Framework/b2dJson.h
	Framework/Render.cpp
	Framework/b2dJson.cpp
	Framework/jsoncpp.cpp
	Framework/Main.cpp
	Framework/Render.cpp
	Framework/Render.h
	Framework/Test.cpp
	Framework/Test.h
)

#define the test files.
set(Testbed_Tests_SRCS
	Tests/PlankesorteringsAnlegg/AktuatorFlate.h
	Tests/PlankesorteringsAnlegg/BaneSynkronisator.h
	Tests/PlankesorteringsAnlegg/Kjede.h
	Tests/PlankesorteringsAnlegg/Kommunikator.h
	Tests/PlankesorteringsAnlegg/Pakke.h
	Tests/PlankesorteringsAnlegg/SensorFlate.h
	Tests/PlankesorteringsAnlegg/PlankesorteringsAnlegg.h
	Tests/PlankesorteringsAnlegg/Bane.h
	Tests/BaneTest.h
	Tests/TestEntries.cpp
	Tests/jsonDemoCars.h
	Tests/AddPair.h
	Tests/ApplyForce.h
	Tests/BodyTypes.h
	Tests/Breakable.h
	Tests/Bridge.h
	Tests/BulletTest.h
	Tests/Cantilever.h
	Tests/Car.h
	Tests/Chain.h
	Tests/CharacterCollision.h
	Tests/CollisionFiltering.h
	Tests/CollisionProcessing.h
	Tests/CompoundShapes.h
	Tests/Confined.h
	Tests/ContinuousTest.h
	Tests/DistanceTest.h
	Tests/Dominos.h
	Tests/DumpShell.h
	Tests/DynamicTreeTest.h
	Tests/EdgeShapes.h
	Tests/EdgeTest.h
	Tests/Gears.h
	Tests/OneSidedPlatform.h
	Tests/Pinball.h
	Tests/PolyCollision.h
	Tests/PolyShapes.h
	Tests/Prismatic.h
	Tests/Pulleys.h
	Tests/Pyramid.h
	Tests/RayCast.h
	Tests/Revolute.h
	Tests/Rope.h
	Tests/RopeJoint.h
	Tests/SensorTest.h
	Tests/ShapeEditing.h
	Tests/SliderCrank.h
	Tests/SphereStack.h
	Tests/TheoJansen.h
	Tests/Tiles.h
	Tests/TimeOfImpact.h
	Tests/VaryingFriction.h
	Tests/VaryingRestitution.h
	Tests/VerticalStack.h
	Tests/Web.h
)

# These are used to create visual studio folders.
source_group(Framework FILES ${Testbed_Framework_SRCS})
source_group(Tests FILES ${Testbed_Tests_SRCS})

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

add_executable(Testbed
	${Testbed_Framework_SRCS}
	${Testbed_Tests_SRCS}
)

target_link_libraries (
	Testbed
	Box2D
	freeglut_static
	glui
	${ADDITIONAL_LIBRARIES}
	${OPENGL_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}

) 
