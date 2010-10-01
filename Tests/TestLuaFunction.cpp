/******************************************************************************\
* TestLuaFunction.cpp                                                          *
* Unit tests for things declared in 'LuaFunction.hpp'.                         *
*                                                                              *
*                                                                              *
* Copyright (C) 2005-2010 by Leandro Motta Barros.                             *
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

#define BOOST_TEST_MODULE LuaFunction

#include <cstring>
#include <boost/test/unit_test.hpp>
#include <Diluculum/LuaState.hpp>
#include <Diluculum/LuaFunction.hpp>


// - TestPureLuaFunctionCallViaVariableArity0 ----------------------------------
BOOST_AUTO_TEST_CASE(TestPureLuaFunctionCallViaVariableArity0)
{
   using namespace Diluculum;

   LuaState ls;
   ls.doString("function func() return 171 end");
   LuaVariable luaFunc = ls["func"];
   LuaValueList ret = luaFunc();
   BOOST_REQUIRE_EQUAL (ret.size(), 1u);
   BOOST_CHECK_EQUAL (ret[0].asNumber(), 171);
}



// - TestPureLuaFunctionCallViaVariableArity1 ----------------------------------
BOOST_AUTO_TEST_CASE(TestPureLuaFunctionCallViaVariableArity1)
{
   using namespace Diluculum;

   LuaState ls;
   ls.doString("function func(p) return p*2 end");
   LuaVariable luaFunc = ls["func"];
   LuaValueList ret = luaFunc(222);
   BOOST_REQUIRE_EQUAL (ret.size(), 1u);
   BOOST_CHECK_EQUAL (ret[0].asNumber(), 444);
}



// - TestCallAnonymousLuaFunction ----------------------------------------------
BOOST_AUTO_TEST_CASE(TestCallAnonymousLuaFunction)
{
   using namespace Diluculum;

   LuaState ls;
   LuaValueList doStringRet =
      ls.doString("return function(p1, p2) return p1 + p2, p1 * p2 end");
   BOOST_REQUIRE_EQUAL (doStringRet.size(), 1u);
   BOOST_REQUIRE_EQUAL (doStringRet[0].type(), LUA_TFUNCTION);

   LuaFunction func = doStringRet[0].asFunction();

   LuaValueList params;
   params.push_back(3);
   params.push_back(-4);

   LuaValueList ret = ls.call (func, params);

   BOOST_REQUIRE_EQUAL (ret.size(), 2u);
   BOOST_REQUIRE_EQUAL (ret[0].type(), LUA_TNUMBER);
   BOOST_REQUIRE_EQUAL (ret[1].type(), LUA_TNUMBER);
   BOOST_CHECK_EQUAL (ret[0].asNumber(), -1);
   BOOST_CHECK_EQUAL (ret[1].asNumber(), -12);
}



// - TestCallAnonymousLuaFunctionNested ----------------------------------------
BOOST_AUTO_TEST_CASE(TestCallAnonymousLuaFunctionNested)
{
   using namespace Diluculum;

   LuaState ls;
   LuaValueList doStringRet =
      ls.doString(
         "return function(p) return '<emph>'..tostring(p)..'</emph>' end");
   BOOST_REQUIRE_EQUAL (doStringRet.size(), 1u);
   BOOST_REQUIRE_EQUAL (doStringRet[0].type(), LUA_TFUNCTION);

   LuaValue emphFunc = doStringRet[0];

   doStringRet =
      ls.doString("return function(t, f) return t..' and '..f(t) end");

   BOOST_REQUIRE_EQUAL (doStringRet.size(), 1u);
   BOOST_REQUIRE_EQUAL (doStringRet[0].type(), LUA_TFUNCTION);

   LuaFunction f = doStringRet[0].asFunction();

   LuaValueList params;
   params.push_back("blah");
   params.push_back(emphFunc);

   LuaValueList ret = ls.call (f, params);

   BOOST_REQUIRE_EQUAL (ret.size(), 1u);
   BOOST_REQUIRE_EQUAL (ret[0].type(), LUA_TSTRING);
   BOOST_CHECK_EQUAL (ret[0].asString(), "blah and <emph>blah</emph>");
}



// - TestLuaFunctionConstruction -----------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaFunctionConstruction)
{
   using namespace Diluculum;

   // Create three 'LuaFunction's, of different sizes
   const char pseudoBytecode1[] = "1234567890";
   const char pseudoBytecode2[] = "qazwsx";
   char pseudoBytecode3[] = "vgui8u4jt930,cf9irt 7y843ynothkf9723q1ms##$_sf 12";

   LuaFunction lf1 (pseudoBytecode1, strlen(pseudoBytecode1));
   const LuaFunction lf2 (pseudoBytecode2, strlen(pseudoBytecode2));
   LuaFunction lf3 (pseudoBytecode3, strlen(pseudoBytecode3));

   // Ensure that the size returned by 'getSize()' matches the expected
   BOOST_CHECK_EQUAL (lf1.getSize(), strlen(pseudoBytecode1));
   BOOST_CHECK_EQUAL (lf2.getSize(), strlen(pseudoBytecode2));
   BOOST_CHECK_EQUAL (lf3.getSize(), strlen(pseudoBytecode3));

   // The address returned by 'getData()' should be valid (non-NULL)
   BOOST_CHECK_NE (lf1.getData(), reinterpret_cast<void*>(0));
   BOOST_CHECK_NE (lf2.getData(), reinterpret_cast<void*>(0));
   BOOST_CHECK_NE (lf3.getData(), reinterpret_cast<void*>(0));

   // Just in case, check that the address of the various 'LuaFunction's are
   // different.
   BOOST_CHECK_NE (lf1.getData(), lf2.getData());
   BOOST_CHECK_NE (lf1.getData(), lf3.getData());
   BOOST_CHECK_NE (lf2.getData(), lf3.getData());
}



// - TestLuaFunctionGetData ----------------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaFunctionGetData)
{
   using namespace Diluculum;

   const char pseudoBytecode1[] = "1234567890";
   const char pseudoBytecode2[] = "1234567890";
   char pseudoBytecode3[] = "qwertyuiop";

   LuaFunction lf1 (pseudoBytecode1, strlen(pseudoBytecode1));
   const LuaFunction lf2 (pseudoBytecode2, strlen(pseudoBytecode2));
   LuaFunction lf3 (pseudoBytecode3, strlen(pseudoBytecode3));

   // Check every Lua function with the data it is expected to contain
   BOOST_CHECK_EQUAL (memcmp (lf1.getData(), pseudoBytecode1,
                              strlen(pseudoBytecode1)),
                      0);

   BOOST_CHECK_EQUAL (memcmp (lf1.getData(), pseudoBytecode2,
                              strlen(pseudoBytecode2)),
                      0);

   BOOST_CHECK_EQUAL (memcmp (lf3.getData(), pseudoBytecode3,
                              strlen(pseudoBytecode3)),
                      0);

   // Compare data among the 'LuaFunction's
   BOOST_CHECK_EQUAL (memcmp (lf1.getData(), lf2.getData(),
                              strlen(pseudoBytecode1)),
                      0);
   BOOST_CHECK_NE (memcmp (lf1.getData(), lf3.getData(),
                           strlen(pseudoBytecode1)),
                   0);
   BOOST_CHECK_NE (memcmp (lf2.getData(), lf3.getData(),
                           strlen(pseudoBytecode2)),
                   0);
}



// - TestLuaFunctionCopies -----------------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaFunctionCopies)
{
   using namespace Diluculum;

   const char pseudoBytecode1[] = "1234567890";
   const char pseudoBytecode2[] = "qazwsx";
   char pseudoBytecode3[] = "vgui8u4jt930,cf9irt 7y843ynothkf9723q1ms##$_sf 12";

   LuaFunction lf1 (pseudoBytecode1, strlen(pseudoBytecode1));
   LuaFunction lf2 (pseudoBytecode2, strlen(pseudoBytecode2));
   LuaFunction lf3 (pseudoBytecode3, strlen(pseudoBytecode3));

   // Try a "chained assignment"
   BOOST_REQUIRE (lf2 != lf1);
   BOOST_REQUIRE (lf3 != lf1);

   lf2 = lf3 = lf1;

   BOOST_CHECK_EQUAL (memcmp (lf1.getData(), pseudoBytecode1,
                              strlen(pseudoBytecode1)),
                      0);
   BOOST_CHECK (lf2 == lf1);
   BOOST_CHECK (lf3 == lf1);

   // And a simple one...
   LuaFunction lf4 (pseudoBytecode1, strlen(pseudoBytecode1));
   LuaFunction lf5 (pseudoBytecode3, strlen(pseudoBytecode3));

   BOOST_REQUIRE (lf4 != lf5);

   lf5 = lf4;

   BOOST_CHECK_EQUAL (memcmp (lf4.getData(), pseudoBytecode1,
                              strlen(pseudoBytecode1)),
                      0);
   BOOST_REQUIRE (lf5 == lf4);
}
