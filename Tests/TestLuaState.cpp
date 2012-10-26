/******************************************************************************\
* TestLuaState.cpp                                                             *
* Unit tests for things declared in 'LuaState.hpp'.                            *
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

#define BOOST_TEST_MODULE LuaState

#include <boost/test/unit_test.hpp>
#include <Diluculum/LuaState.hpp>


// - TestLuaStateNotOwner ------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaStateNotOwner)
{
   using namespace Diluculum;

   // Create a raw Lua state and store a global variable
   lua_State* rls; // raw Lua state

   rls = luaL_newstate();
   lua_pushstring (rls, "x");
   lua_pushnumber (rls, 171);
   lua_settable (rls, LUA_GLOBALSINDEX);

   // Construct a 'LuaState' and check if the variable defined above can be
   // read. Also, store another variable, to see if it can be accessed
   // afterwards.
   {
      LuaState ls (rls);

      BOOST_REQUIRE (ls["x"] == 171);
      ls.doString ("y = 555");
   }

   // At this point, 'ls' is destroyed. Ensure that 'rls' is still alive.
   lua_pushstring (rls, "y");
   lua_gettable (rls, LUA_GLOBALSINDEX);

   BOOST_REQUIRE (lua_isnumber (rls, -1));
   BOOST_CHECK (lua_tonumber(rls, -1) == 555);
}



// - TestLuaStateDoFileMultRet -------------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaStateDoFileMultRet)
{
   using namespace Diluculum;

   LuaState ls;
   const LuaValueList ret = ls.doFile ("TestLuaStateDoFile.lua");

   BOOST_REQUIRE (ret.size() == 5);
   BOOST_CHECK (ret[0] == "foo");
   BOOST_CHECK (ret[1] == 43.21);
   BOOST_CHECK (ret[2][1] == true);
   BOOST_CHECK (ret[2][2] == "bar");
   BOOST_CHECK (ret[3] == "baz");
   BOOST_CHECK (ret[4] == 4.5);

   const LuaValueList empty = ls.doFile(
      "TestLuaStateDoFileNoReturn.lua");
   BOOST_CHECK (empty.size() == 0);
}



// - TestLuaStateDoFile --------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaStateDoFile)
{
   using namespace Diluculum;

   LuaState ls;
   const LuaValue ret = ls.doFile ("TestLuaStateDoFile.lua");

   BOOST_CHECK (ret == "foo");

   const LuaValue empty = ls.doFile("TestLuaStateDoFileNoReturn.lua");
   BOOST_CHECK (empty == Nil);
}



// - TestLuaStateDoStringMultRet -----------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaStateDoStringMultRet)
{
   using namespace Diluculum;

   LuaState ls;

   const LuaValueList nothing = ls.doString ("a = 1 + 1 - 1");
   const LuaValueList one = ls.doString ("return a");
   const LuaValueList oneTwo = ls.doString ("return a, a*2");
   const LuaValueList oneTwoThree = ls.doString ("return a, a+1, 'three'");
   const LuaValueList oneTwoThreeFalse = ls.doString(
      "return a, a+a, 'three', a == 10");
   const LuaValueList nestedTables = ls.doString(
      "return { 'one', 2, { [1] = 'one', two = 2, [true] = 'foo' }, false }");

   // Check the size of the returned data
   BOOST_CHECK (nothing.size() == 0);
   BOOST_REQUIRE (one.size() == 1);
   BOOST_REQUIRE (oneTwo.size() == 2);
   BOOST_REQUIRE (oneTwoThree.size() == 3);
   BOOST_REQUIRE (nestedTables.size() == 1);

   // Check the returned data; first the easy cases...
   BOOST_CHECK (one[0] == 1);

   BOOST_CHECK (oneTwo[0] == 1);
   BOOST_CHECK (oneTwo[1] == 2);

   BOOST_CHECK (oneTwoThree[0] == 1);
   BOOST_CHECK (oneTwoThree[1] == 2);
   BOOST_CHECK (oneTwoThree[2] == "three");

   BOOST_CHECK (oneTwoThreeFalse[0] == 1);
   BOOST_CHECK (oneTwoThreeFalse[1] == 2);
   BOOST_CHECK (oneTwoThreeFalse[2] == "three");
   BOOST_CHECK (oneTwoThreeFalse[3] == false);

   // ...and now that more complicated table
   BOOST_CHECK (nestedTables[0][1] == "one");
   BOOST_CHECK (nestedTables[0][2] == 2);
   BOOST_CHECK (nestedTables[0][3][1.0].asString() == "one");
   BOOST_CHECK (nestedTables[0][3]["two"].asNumber() == 2);
   BOOST_CHECK (nestedTables[0][3][true].asString() == "foo");
   BOOST_CHECK (nestedTables[0][4] == false);
}



// - TestLuaStateDoString ------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaStateDoString)
{
   using namespace Diluculum;

   LuaState ls;

   const LuaValue nothing = ls.doString ("a = math.cos(0)");
   const LuaValue one = ls.doString ("return a");
   const LuaValue two = ls.doString ("return 2*a, { }");
   const LuaValue three = ls.doString ("return 'three', true, 3.0");

   // Check the returned data
   BOOST_CHECK (nothing == Nil);
   BOOST_CHECK (one == 1);
   BOOST_CHECK (two == 2);
   BOOST_CHECK (three == "three");
}



// - TestLuaStateDoExceptions --------------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaStateDoExceptions)
{
   using namespace Diluculum;

   LuaState ls;

   // Force a syntax error
   BOOST_CHECK_THROW (ls.doString ("@#$%#"), LuaSyntaxError);
   BOOST_CHECK_THROW (ls.doFile ("SyntaxError.lua"), LuaSyntaxError);

   // "Lua threads" (coroutines) are not supported by 'LuaValue'. Trying to
   // return one must generate an error.
   BOOST_CHECK_THROW (ls.doString ("return coroutine.create(function() end)"),
                      LuaTypeError);
   BOOST_CHECK_THROW (ls.doFile ("ReturnThread.lua"), LuaTypeError);

   // We are expected to throw in "file not found" situations
   BOOST_CHECK_THROW (ls.doFile ("__THiis_fi1e.doeSNt--exIst.lua"),
                      LuaFileError);
}



// - TestLuaStateSubscriptOperator ---------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaStateSubscriptOperator)
{
   // Some related, but more stressing tests are done in 'TestLuaVariable.cpp'.

   using namespace Diluculum;

   LuaState ls;
   ls.doString ("a = 1; b = 'two'; c = true; d = { 'foo', 3.1415, a = 'baz' }");

   LuaValueMap lvm;
   lvm[1] = "foo";
   lvm[2] = 3.1415;
   lvm["a"] = "baz";

   // Check if the values can be read
   BOOST_CHECK (ls["a"] == 1);
   BOOST_CHECK (ls["b"] == "two");
   BOOST_CHECK (ls["c"] == true);
   BOOST_CHECK (ls["d"] == lvm);

   // Change some values and verify if they were really changed
   ls["a"] = ls["a"].value().asNumber() + 1;
   ls["b"] = ls["d"]["a"].value();

   BOOST_CHECK (ls["a"] == 2);
   BOOST_CHECK (ls["b"] == "baz");

   // Add new values, and read them
   ls["d"][4] = 12.34;
   ls["z"] = 56.78;

   BOOST_CHECK (ls["d"][4] == 12.34);
   BOOST_CHECK (ls["z"] == 56.78);

   // Ensure that accessing a non-existent variable returns \c Nil...
   BOOST_CHECK (ls["nope"] == Nil);

   //... but also certify that we can set such variable's value.
   ls["nope"] = "yup";
   BOOST_CHECK (ls["nope"] == "yup");
}



// - TestCall ------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestCall)
{
   using namespace Diluculum;

   LuaState state;
   state.doString("function doubleNum(i) return i * 2 end");
   state.doString("function doubleHalveNum(i, j) return i * 2, j / 2 end");

   {
      LuaValueList params;
      params.push_back(123);
      LuaFunction f = state["doubleNum"].value().asFunction();
      LuaValueList ret = state.call(f, params);

      BOOST_REQUIRE_EQUAL (ret.size(), 1);
      BOOST_CHECK_EQUAL (ret[0].asInteger(), 246);
   }

   {
      LuaValueList params;
      params.push_back(111);
      params.push_back(888);
      LuaFunction f = state["doubleHalveNum"].value().asFunction();
      LuaValueList ret = state.call(f, params);

      BOOST_REQUIRE_EQUAL (ret.size(), 2);
      BOOST_CHECK_EQUAL (ret[0].asInteger(), 222);
      BOOST_CHECK_EQUAL (ret[1].asInteger(), 444);
   }
}



// - TestGlobalsTableAccess ----------------------------------------------------
BOOST_AUTO_TEST_CASE(TestGlobalsTableAccess)
{
   using namespace Diluculum;

   LuaState state;
   LuaValueMap globals = state.globals();

   BOOST_CHECK_EQUAL (globals["print"].type(), LUA_TFUNCTION);
   BOOST_CHECK_EQUAL (globals["ipairs"].type(), LUA_TFUNCTION);
   BOOST_CHECK_EQUAL (globals["io"].type(), LUA_TTABLE);
   BOOST_CHECK_EQUAL (globals["math"].type(), LUA_TTABLE);
   BOOST_CHECK_EQUAL (globals["math"]["sin"].type(), LUA_TFUNCTION);
   BOOST_CHECK_EQUAL (globals["foo"].type(), LUA_TNIL);

   state["foo"] = "Now I do exist.";
   globals = state.globals();
   BOOST_CHECK_EQUAL (globals["foo"].type(), LUA_TSTRING);
}
