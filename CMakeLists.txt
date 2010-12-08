# Copyright (C) 2007-2009 LuaDist.
# Modified by Peter Kapec
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.
# Original build script by Leandro Motta Barros.

	PROJECT(diluculum)
	CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
	INCLUDE(dist.cmake)

# Find Lua
	fIND_PACKAGE(Lua51 REQUIRED)

# Find Boost
	SET(Boost_USE_STATIC_LIBS OFF)
	FIND_PACKAGE(Boost 1.38 COMPONENTS unit_test_framework REQUIRED)

	ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)

# Include directories
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${LUA_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include)

# Link directories
	LINK_DIRECTORIES(${BOOST_LIBRARY_DIRS})


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
		Sources/LuaWrappers.cpp
	)

	ADD_LIBRARY(diluculum SHARED ${DiluculumSources})
	TARGET_LINK_LIBRARIES(diluculum ${LUA_LIBRARIES})

# Add CTest support
	ENABLE_TESTING()

# Unit tests
	FUNCTION(AddUnitTest name)
		ADD_EXECUTABLE(${name} Tests/${name}.cpp)
		TARGET_LINK_LIBRARIES(${name} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${LUA_LIBRARIES} diluculum)
		ADD_TEST(${name} ${name})
	ENDFUNCTION(AddUnitTest)

	ADD_LIBRARY(ATestModule SHARED Tests/ATestModule.cpp)
	TARGET_LINK_LIBRARIES(ATestModule diluculum ${LUA_LIBRARIES})
	SET_TARGET_PROPERTIES(ATestModule PROPERTIES PREFIX "")

	AddUnitTest(TestLuaFunction)
	AddUnitTest(TestLuaState)
	AddUnitTest(TestLuaUserData)
	AddUnitTest(TestLuaUtils)
	AddUnitTest(TestLuaValue)
	AddUnitTest(TestLuaVariable)
	AddUnitTest(TestLuaWrappers)

# Copy the files needed by the unit tests
	CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Tests/ReturnThread.lua ${CMAKE_BINARY_DIR}/ReturnThread.lua COPYONLY)
	CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Tests/SyntaxError.lua ${CMAKE_BINARY_DIR}/SyntaxError.lua COPYONLY)
	CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Tests/TestLuaStateDoFile.lua ${CMAKE_BINARY_DIR}/TestLuaStateDoFile.lua COPYONLY)
	CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/Tests/TestLuaStateDoFileNoReturn.lua ${CMAKE_BINARY_DIR}/TestLuaStateDoFileNoReturn.lua  COPYONLY)

# Install
	INSTALL(TARGETS diluculum RUNTIME DESTINATION ${INSTALL_BIN} LIBRARY DESTINATION ${INSTALL_LIB} ARCHIVE DESTINATION ${INSTALL_LIB})	
	INSTALL(DIRECTORY include/ DESTINATION ${INSTALL_INC})
	INSTALL(FILES AUTHORS.txt COPYING.txt HISTORY.txt README.txt DESTINATION ${INSTALL_DOC})
	


