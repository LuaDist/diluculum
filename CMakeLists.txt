# Copyright (C) 2007-2012 LuaDist.
# Modified by Peter Kapec, Peter Drahoš
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.
# Original build script by Leandro Motta Barros.

project ( diluculum CXX )
cmake_minimum_required ( VERSION 2.8 )
include ( cmake/dist.cmake )
include ( lua )

# Find Lua
find_package ( Lua REQUIRED )

# Find Boost
set ( Booqst_USE_STATIC_LIBS OFF)
find_package ( Boost 1.39 COMPONENTS unit_test_framework REQUIRED )
add_definitions ( -DBOOST_ALL_DYN_LINK )

# Include directories
include_directories ( ${Boost_INCLUDE_DIRS} ${LUA_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include )

# Link directories
link_directories(${Boost_LIBRARY_DIRS})


# Enable warnings when compiling with G++
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif(CMAKE_COMPILER_IS_GNUCXX)

# Build the library
set(DiluculumSources
    Sources/InternalUtils.cpp
    Sources/LuaExceptions.cpp
    Sources/LuaFunction.cpp
    Sources/LuaState.cpp
    Sources/LuaUserData.cpp
    Sources/LuaUtils.cpp
    Sources/LuaValue.cpp
    Sources/LuaVariable.cpp
    Sources/LuaWrappers.cpp)

add_library ( diluculum ${DiluculumSources} )
target_link_libraries ( diluculum ${LUA_LIBRARIES} )

# Add CTest support
enable_testing ( )

# Now, the unit tests
function(AddUnitTest name)
  add_executable ( ${name} Tests/${name}.cpp )
  target_link_libraries(${name} 
						${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} 
						${LUA_LIBRARIES}
					    diluculum )
  add_test ( ${name} ${name} )
  install ( TARGETS ${name} RUNTIME DESTINATION ${INSTALL_TEST}/${_ARG_INTO}
              COMPONENT Test )
endfunction ( AddUnitTest )

add_library ( ATestModule SHARED Tests/ATestModule.cpp )
target_link_libraries ( ATestModule diluculum ${LUA_LIBRARIES} )
set_target_properties ( ATestModule PROPERTIES PREFIX "" )
install ( TARGETS ATestModule LIBRARY DESTINATION ${INSTALL_TEST}/${_ARG_INTO} COMPONENT Test )

addunittest ( TestLuaFunction )
addunittest ( TestLuaState )
addunittest ( TestLuaUserData )
addunittest ( TestLuaUtils )
addunittest ( TestLuaValue )
addunittest ( TestLuaVariable )
addunittest ( TestLuaWrappers )

# Copy the files needed by the unit tests
configure_file ( ${CMAKE_SOURCE_DIR}/Tests/ReturnThread.lua ${CMAKE_BINARY_DIR}/ReturnThread.lua
  COPYONLY )
configure_file ( ${CMAKE_SOURCE_DIR}/Tests/SyntaxError.lua ${CMAKE_BINARY_DIR}/SyntaxError.lua
  COPYONLY )
configure_file ( ${CMAKE_SOURCE_DIR}/Tests/TestLuaStateDoFile.lua ${CMAKE_BINARY_DIR}/TestLuaStateDoFile.lua
  COPYONLY )
configure_file ( ${CMAKE_SOURCE_DIR}/Tests/TestLuaStateDoFileNoReturn.lua ${CMAKE_BINARY_DIR}/TestLuaStateDoFileNoReturn.lua
  COPYONLY )

# Install
install_library ( diluculum )
install_header ( include/ )
install_data ( AUTHORS.txt COPYING.txt HISTORY.txt README.txt )

FILE(GLOB Tests "Tests/*.lua")
install_test ( ${Tests} )
