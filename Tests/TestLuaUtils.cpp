/******************************************************************************\
* TestLuaUtils.cpp                                                             *
* Unit tests for things declared in 'LuaUtils.hpp'.                            *
*                                                                              *
*                                                                              *
* Copyright (C) 2005-2011 by Leandro Motta Barros.                             *
*                                                                              *
* Permission is hereby granted, free of charge, to any person obtaining a copy *
* of this software and associated documentation files (the "Software"), to     *
* deal in the Software without restriction, including without limitation the   *
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or  *
* sell copies of the Software, and to permit persons to whom the Software is   *
* furnished to do so, subject to the following conditions:                     *
*                                                                              *
* The above copyright notice and this permission notice shall be included in   *
* all copies or substantial portions of the Software.                          *
*                                                                              *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS *
* IN THE SOFTWARE.                                                             *
\******************************************************************************/

#define BOOST_TEST_MODULE LuaUtils

#include <boost/test/unit_test.hpp>
#include <cstring>
#include <Diluculum/LuaExceptions.hpp>
#include <Diluculum/LuaState.hpp>
#include <Diluculum/LuaUserData.hpp>
#include <Diluculum/LuaUtils.hpp>


int CLuaFunctionExample (lua_State* ls)
{
   return 0;
}


// - TestToLuaValue ------------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestToLuaValue)
{
   using namespace Diluculum;

   lua_State* ls = luaL_newstate();

   lua_pushboolean (ls, true);
   lua_pushnumber (ls, 171.171);
   lua_pushstring (ls, "The book is on the table.");
   lua_pushnil (ls);
   lua_pushcfunction (ls, CLuaFunctionExample);
   void* mem = lua_newuserdata (ls, 555);
   memset (mem, 2, 555);

   LuaValue lvTrue (true);
   LuaValue lv171 (171.171);
   LuaValue lvTBIOTT ("The book is on the table.");
   LuaValue lvCLua (CLuaFunctionExample);
   LuaValue lvUserData555Twos (LuaUserData (555));
   memset (lvUserData555Twos.asUserData().getData(), 2, 555);

   // Try reading using positive (absolute) indexes
   BOOST_CHECK (ToLuaValue (ls, 1) == lvTrue);
   BOOST_CHECK (ToLuaValue (ls, 2) == lv171);
   BOOST_CHECK (ToLuaValue (ls, 3) == lvTBIOTT);
   BOOST_CHECK (ToLuaValue (ls, 4) == Nil);
   BOOST_CHECK (ToLuaValue (ls, 5) == lvCLua);
   BOOST_CHECK (ToLuaValue (ls, 6) == lvUserData555Twos);

   // And now, try with negative indexes
   BOOST_CHECK (ToLuaValue (ls, -1) == lvUserData555Twos);
   BOOST_CHECK (ToLuaValue (ls, -2) == lvCLua);
   BOOST_CHECK (ToLuaValue (ls, -3) == Nil);
   BOOST_CHECK (ToLuaValue (ls, -4) == lvTBIOTT);
   BOOST_CHECK (ToLuaValue (ls, -5) == lv171);
   BOOST_CHECK (ToLuaValue (ls, -6) == lvTrue);

   // Just to be complete, do everything once more using "constants"
   BOOST_CHECK (ToLuaValue (ls, 1) == true);
   BOOST_CHECK (ToLuaValue (ls, 2) == 171.171);
   BOOST_CHECK (ToLuaValue (ls, 3) == "The book is on the table.");
   BOOST_CHECK (ToLuaValue (ls, 5) == CLuaFunctionExample);
   BOOST_CHECK (ToLuaValue (ls, -2) == CLuaFunctionExample);
   BOOST_CHECK (ToLuaValue (ls, -4) == "The book is on the table.");
   BOOST_CHECK (ToLuaValue (ls, -5) == 171.171);
   BOOST_CHECK (ToLuaValue (ls, -6) == true);

   // Ensure that trying to convert unsupported types throws an exception
   lua_newthread (ls);
   BOOST_CHECK_THROW (ToLuaValue (ls, -1), LuaTypeError);

   // Close the Lua state used in this test
   lua_close (ls);
}



// - TestPushLuaValue ----------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestPushLuaValue)
{
   using namespace Diluculum;

   lua_State* ls = luaL_newstate();

   PushLuaValue (ls, Nil);
   PushLuaValue (ls, false);
   PushLuaValue (ls, "The sky is blue.");
   PushLuaValue (ls, 2.7183);
   PushLuaValue (ls, CLuaFunctionExample);

   LuaUserData fiveFiveFiveZeros (555);
   memset (fiveFiveFiveZeros.getData(), 0, 555);
   LuaValue lvUserData555Zeros (fiveFiveFiveZeros);
   memset (lvUserData555Zeros.asUserData().getData(), 0, 555);
   PushLuaValue (ls, lvUserData555Zeros);

   // Check if the values were properly pushed
   BOOST_CHECK (lua_isnil (ls, 1));

   BOOST_REQUIRE (lua_isboolean (ls, 2));
   BOOST_CHECK (lua_toboolean (ls, 2) == false);

   BOOST_REQUIRE (lua_isstring (ls, 3));
   BOOST_CHECK (std::strcmp (lua_tostring (ls, 3), "The sky is blue.") == 0);

   BOOST_REQUIRE (lua_isnumber (ls, 4));
   BOOST_CHECK (lua_tonumber (ls, 4) == 2.7183);

   BOOST_REQUIRE (lua_iscfunction (ls, 5));
   BOOST_CHECK (lua_tocfunction (ls, 5) == CLuaFunctionExample);

   BOOST_REQUIRE (lua_type (ls, 6) == LUA_TUSERDATA);
   unsigned char zeros[555] = { 0 };
   BOOST_CHECK (memcmp (lua_touserdata (ls, 6), zeros, 555) == 0);

   // Close the Lua state used in this test
   lua_close (ls);
}



// - TestPushLuaValueLuaFunction -----------------------------------------------
BOOST_AUTO_TEST_CASE(TestPushLuaValueLuaFunction)
{
   // 'LuaFunction's are a bit different from the other things that a 'LuaValue'
   // can hold. In particular, one normally cannot construct a 'LuaFunction'
   // from scratch. Instead, one is constructed through a 'LuaVariable'. Hence,
   // 'LuaFunction' deserves exclusive test cases.

   using namespace Diluculum;

   LuaState tmpLS;
   tmpLS.doString ("function Squarer(n) return n * n end");

   LuaValue val = tmpLS["Squarer"].value();

   BOOST_REQUIRE (val.type() == LUA_TFUNCTION);

   lua_State* ls = luaL_newstate();
   PushLuaValue (ls, val);

   BOOST_CHECK_EQUAL (lua_isfunction (ls, 1), true);
   BOOST_CHECK_EQUAL (lua_iscfunction (ls, 1), false);
}



// - TestToLuaValueLuaFunction -------------------------------------------------
BOOST_AUTO_TEST_CASE(TestToLuaValueLuaFunction)
{
   // 'LuaFunction's are a bit different from the other things that a 'LuaValue'
   // can hold. In particular, one normally cannot construct a 'LuaFunction'
   // from scratch. Instead, one is constructed through a 'LuaVariable'. Hence,
   // 'LuaFunction' deserves exclusive test cases.

   using namespace Diluculum;

   LuaState tmpLS;
   tmpLS.doString("function Squarer(n) return n * n end");

   LuaValue val = tmpLS["Squarer"].value();

   lua_State* ls = luaL_newstate();
   PushLuaValue (ls, val);

   // Up this point, everything should work (another test already tests this)
   LuaValue readBack = ToLuaValue (ls, 1);
   BOOST_REQUIRE (readBack.type() == LUA_TFUNCTION);
   LuaFunction readBackFunc = readBack.asFunction();

   LuaState newLS;
   LuaValueList params;
   params.push_back(-5);
   LuaValueList ret = newLS.call(readBackFunc, params, "Squarer chunk");

   BOOST_REQUIRE_EQUAL (ret.size(), 1u);
   BOOST_CHECK_EQUAL (ret[0].asInteger(), 25);
}
