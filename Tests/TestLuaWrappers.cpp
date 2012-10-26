/******************************************************************************\
* TestLuaWrappers.cpp                                                          *
* Unit tests for things declared in 'LuaWrappers.hpp'.                         *
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

#define BOOST_TEST_MODULE LuaWrappers

#include <boost/test/unit_test.hpp>
#include <Diluculum/LuaState.hpp>
#include <Diluculum/LuaWrappers.hpp>
#include "WrappedClasses.hpp"
#include "WrappedFunctions.hpp"


// - TestFunctionWrapping ------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestFunctionWrapping)
{
   using namespace Diluculum;
   LuaState ls;

   // The functions used below are defined in 'CLuaFunctions.hpp', which was
   // included in this file. So, we just have to make them available in the
   // 'LuaState' used in this test.
   ls["ZeroTheGlobal"] = DILUCULUM_WRAPPER_FUNCTION (ZeroTheGlobal);
   ls["SetTheGlobal"] = DILUCULUM_WRAPPER_FUNCTION (SetTheGlobal);
   ls["SetTheGlobalToSum"] = DILUCULUM_WRAPPER_FUNCTION (SetTheGlobalToSum);
   ls["ConcatenateThree"] = DILUCULUM_WRAPPER_FUNCTION (ConcatenateThree);
   ls["FibonacciSequence"] = DILUCULUM_WRAPPER_FUNCTION (FibonacciSequence);
   ls["ToOrFromString"] = DILUCULUM_WRAPPER_FUNCTION (ToOrFromString);

   // Here we go...
   Diluculum::LuaValueList res;
   TheGlobal = 171;

   ls.doString ("ZeroTheGlobal()");
   BOOST_CHECK (res.size() == 0);
   BOOST_CHECK (TheGlobal == 0);

   ls.doString ("SetTheGlobal (345)");
   BOOST_CHECK (res.size() == 0);
   BOOST_CHECK (TheGlobal == 345);

   ls.doString ("SetTheGlobalToSum()");
   BOOST_CHECK (res.size() == 0);
   BOOST_CHECK (TheGlobal == 0);

   ls.doString ("SetTheGlobalToSum (3, 8, -5, 22, -11, 7)");
   BOOST_CHECK (res.size() == 0);
   BOOST_CHECK (TheGlobal == 24);

   ls.doString ("SetTheGlobalToSum (171)");
   BOOST_CHECK (res.size() == 0);
   BOOST_CHECK (TheGlobal == 171);

   res = ls.doString ("return ConcatenateThree ('one ', 'um ', 'eins')");
   BOOST_REQUIRE (res.size() == 1);
   BOOST_CHECK (res[0] == "one um eins");

   res = ls.doString ("return FibonacciSequence (2)");
   BOOST_REQUIRE (res.size() == 2);
   BOOST_CHECK (res[0] == 1);
   BOOST_CHECK (res[1] == 1);

   res = ls.doString ("return FibonacciSequence (10)");
   BOOST_REQUIRE (res.size() == 10);
   BOOST_CHECK (res[0] == 1);
   BOOST_CHECK (res[1] == 1);
   BOOST_CHECK (res[2] == 2);
   BOOST_CHECK (res[3] == 3);
   BOOST_CHECK (res[4] == 5);
   BOOST_CHECK (res[5] == 8);
   BOOST_CHECK (res[6] == 13);
   BOOST_CHECK (res[7] == 21);
   BOOST_CHECK (res[8] == 34);
   BOOST_CHECK (res[9] == 55);

   res = ls.doString ("return FibonacciSequence (6)");
   BOOST_REQUIRE (res.size() == 6);
   BOOST_CHECK (res[0] == 1);
   BOOST_CHECK (res[1] == 1);
   BOOST_CHECK (res[2] == 2);
   BOOST_CHECK (res[3] == 3);
   BOOST_CHECK (res[4] == 5);
   BOOST_CHECK (res[5] == 8);

   res = ls.doString ("return ToOrFromString (1)");
   BOOST_REQUIRE (res.size() == 1);
   BOOST_CHECK (res[0] == "one");

   res = ls.doString ("return ToOrFromString (3)");
   BOOST_REQUIRE (res.size() == 1);
   BOOST_CHECK (res[0] == "three");

   res = ls.doString ("return ToOrFromString ('one')");
   BOOST_REQUIRE (res.size() == 1);
   BOOST_CHECK (res[0] == 1);

   res = ls.doString ("return ToOrFromString ('two')");
   BOOST_REQUIRE (res.size() == 1);
   BOOST_CHECK (res[0] == 2);
}



// - TestClassWrapping ---------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestClassWrapping)
{
   using namespace Diluculum;
   LuaState ls;

   // First, make the desired class available in our 'LuaState'
   DILUCULUM_REGISTER_CLASS (ls["Account"], Account);

   // In Lua, create two instances of the class and call some methods
   ls.doString ("a1 = Account.new()");
   ls.doString ("a2 = Account.new(123.45)");

   Diluculum::LuaValueList ret = ls.doString ("return a1:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 0.0);

   ret = ls.doString ("return a2:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 123.45);

   ls.doString ("a1:deposit (55.66)");
   ret = ls.doString ("return a1:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 55.66);

   ls.doString ("a1:withdraw (15.66)");
   ret = ls.doString ("return a1:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 40.0);

   ls.doString ("a2:withdraw (0.45)");
   ret = ls.doString ("return a2:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 123.0);

   // Now, create an instance in C++, make it available in our 'LuaState', call
   // its methods in Lua and C++, and check if the things really change on the
   // other side
   LuaValueList params;
   params.push_back (50.0);
   Account aCppAccount (params);

   DILUCULUM_REGISTER_OBJECT (ls["a3"], Account, aCppAccount);

   ls.doString ("a3:deposit (50.0)");
   BOOST_CHECK (aCppAccount.balance (LuaValueList())[0] == 100.0);

   params.clear();
   params.push_back (110.0);
   aCppAccount.withdraw (params);
   ret = ls.doString ("return a3:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == -10.0);
}



// - TestTwoClasses ------------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestTwoClasses)
{
   using namespace Diluculum;

   LuaState ls;

   // Register two classes here, to ensure that no conflicts arise
   DILUCULUM_REGISTER_CLASS (ls["Account"], Account);
   DILUCULUM_REGISTER_CLASS (ls["NumberProperties"], NumberProperties);

   ls.doString ("a = Account.new (1000.01)");
   ls.doString ("n = NumberProperties.new (1234)");

   // Try calling some 'Account' methods
   LuaValueList ret = ls.doString ("return a:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == 1000.01);

   ls.doString ("a:deposit (0.09)");
   ls.doString ("a:withdraw (500.0)");
   ret = ls.doString ("return a:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == 500.10);

   // And now, some 'NumberProperties' methods
   ret = ls.doString ("return n:isEven()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == true);

   ret = ls.doString ("return n:isOdd()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == false);

   ret = ls.doString ("return n:isBig()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == true);
}



// - TestClassWrappingInTable --------------------------------------------------
BOOST_AUTO_TEST_CASE(TestClassWrappingInTable)
{
   using namespace Diluculum;
   LuaState ls;

   // First, make the desired class available in our 'LuaState'
   ls.doString ("Banking = { }");
   DILUCULUM_REGISTER_CLASS (ls["Banking"]["Account"], Account);

   // In Lua, create two instances of the class and call some methods
   ls.doString ("a1 = Banking.Account.new()");
   ls.doString ("a2 = Banking.Account.new(123.45)");

   Diluculum::LuaValueList ret = ls.doString ("return a1:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 0.0);

   ret = ls.doString ("return a2:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 123.45);

   ls.doString ("a1:deposit (55.66)");
   ret = ls.doString ("return a1:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 55.66);

   ls.doString ("a1:withdraw (15.66)");
   ret = ls.doString ("return a1:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 40.0);

   ls.doString ("a2:withdraw (0.45)");
   ret = ls.doString ("return a2:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 123.0);

   // Now, create an instance in C++, make it available in our 'LuaState', call
   // its methods in Lua and C++, and check if the things really change on the
   // other side
   LuaValueList params;
   params.push_back (50.0);
   Account aCppAccount (params);

   DILUCULUM_REGISTER_OBJECT (ls["a3"], Account, aCppAccount);

   ls.doString ("a3:deposit (50.0)");
   BOOST_CHECK (aCppAccount.balance (LuaValueList())[0] == 100.0);

   params.clear();
   params.push_back (110.0);
   aCppAccount.withdraw (params);
   ret = ls.doString ("return a3:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == -10.0);
}



// - TestClassDestructorObjectInstantiatedInLuaAndGarbageCollected -------------
BOOST_AUTO_TEST_CASE(TestClassDestructorObjectInstantiatedInLuaAndGarbageCollected)
{
   // An object is instantiated from Lua. After the 'LuaState' in which it lives
   // is destroyed, the object is expected to have been garbage-collected and
   // its destructor called.

   using namespace Diluculum;

   DestructorTester::aFlag = false;

   {
      LuaState ls;
      DILUCULUM_REGISTER_CLASS (ls["DestructorTester"], DestructorTester);
      ls.doString ("foo = DestructorTester.new()");

      // Just to be paranoid, ensure that 'aFlag' is still false
      BOOST_REQUIRE (DestructorTester::aFlag == false);
   }

   BOOST_CHECK (DestructorTester::aFlag == true);
}



// - TestClassDestructorObjectInstantiatedInCpp --------------------------------
BOOST_AUTO_TEST_CASE(TestClassDestructorObjectInstantiatedInCpp)
{
   // The object is instantiated in C++ and exported to a 'LuaState'. In this
   // case, the object's destruction is responsibility of the programmer on the
   // C++ side. So, the destructor should not be called after the object is
   // garbage-collected in Lua.

   using namespace Diluculum;

   DestructorTester::aFlag = false;
   LuaValueList params;
   DestructorTester dt (params);

   {
      LuaState ls;
      DILUCULUM_REGISTER_CLASS (ls["DestructorTester"], DestructorTester);

      DILUCULUM_REGISTER_OBJECT (ls["dt"], DestructorTester, dt);

      // Do something to assert that the object is there
      LuaValueList ret = ls.doString ("return type (dt)");
      BOOST_REQUIRE (ret.size() == 1);
      BOOST_REQUIRE (ret[0].asString() == "userdata");

      // Just to be paranoid, ensure that 'aFlag' is still false
      BOOST_REQUIRE (DestructorTester::aFlag == false);
   }

   BOOST_CHECK (DestructorTester::aFlag == false);
}



// - TestClassDestructorObjectInstantiatedInLuaAndDestructorCalled -------------
BOOST_AUTO_TEST_CASE(TestClassDestructorObjectInstantiatedInLuaAndDestructorCalled)
{
   // The object is instantiated in Lua, and its destructor is explicitly
   // called.

   using namespace Diluculum;

   DestructorTester::aFlag = false;

   {
      LuaState ls;
      DILUCULUM_REGISTER_CLASS (ls["DestructorTester"], DestructorTester);
      ls.doString ("foo = DestructorTester.new()");

      // Just to be paranoid, ensure that 'aFlag' is still false
      BOOST_REQUIRE (DestructorTester::aFlag == false);

      // Call the destructor explicitly
      ls.doString ("foo:delete()");

      // Check if the destructor was really called
      BOOST_CHECK (DestructorTester::aFlag == true);
   }

   // Just to be even more paranoid, ensure that 'aFlag' is still true
   BOOST_REQUIRE (DestructorTester::aFlag == true);
}



// - TestClassDestructorObjectInstantiatedInCppAndDestructorCalled -------------
BOOST_AUTO_TEST_CASE(TestClassDestructorObjectInstantiatedInCppAndDestructorCalled)
{
   // The object is instantiated in C++, and its destructor is explicitly
   // called. The destructor should not be called in this case.

   using namespace Diluculum;

   DestructorTester::aFlag = false;
   LuaValueList params;
   DestructorTester dt (params);

   {
      LuaState ls;
      DILUCULUM_REGISTER_CLASS (ls["DestructorTester"], DestructorTester);

      DILUCULUM_REGISTER_OBJECT (ls["dt"], DestructorTester, dt);

      // Do something to assert that the object is there
      LuaValueList ret = ls.doString ("return type (dt)");
      BOOST_REQUIRE (ret.size() == 1);
      BOOST_REQUIRE (ret[0].asString() == "userdata");

      // Just to be paranoid, ensure that 'aFlag' is still false
      BOOST_REQUIRE (DestructorTester::aFlag == false);

      // Explicitly call the destructor
      ls.doString ("dt:delete()");

      // The destructor should not have been called
      BOOST_CHECK (DestructorTester::aFlag == false);
   }

   // Keep being paranoid
   BOOST_CHECK (DestructorTester::aFlag == false);
}



// - TestDynamicModule ---------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestDynamicModule)
{
   using namespace Diluculum;
   LuaState ls;

   LuaValueList ret;

   BOOST_REQUIRE_NO_THROW (ls.doString ("require 'ATestModule'"));

   BOOST_REQUIRE_NO_THROW (ls.doString ("obj = ATestModule.AClass.new (123)"));

   BOOST_REQUIRE_NO_THROW (ret = ls.doString ("return obj:aMethod()"));

   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == 123);

   BOOST_REQUIRE_NO_THROW (ret = ls.doString ("return ATestModule.AFunction()"));
   BOOST_REQUIRE (ret.size() == 2);
   BOOST_CHECK (ret[0] == true);
   BOOST_CHECK (ret[1] == "Ahey!");
}


// - TestMultipleStates --------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestMultipleStates)
{
   // This test case was created in response to a bug report by James
   // Doherty. This code used to cause a crash. (In fact, simply "calling"
   // DILUCULUM_REGISTER_CLASS() in the loop was enough to cause a crash.)

   using namespace Diluculum;

   for (int i = 0; i < 100; ++i)
   {
      LuaState ls;
      DILUCULUM_REGISTER_CLASS (ls["Account"], Account);

      LuaValueList params;
      Account aCppAccount(params);
      DILUCULUM_REGISTER_OBJECT (ls["a1"], Account, aCppAccount);
      ls.doString ("a1:deposit (11.11)");

      LuaValueList ret = ls.doString ("return a1:balance()");
      BOOST_REQUIRE (ret.size() == 1);
      BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
      BOOST_CHECK (ret[0] == 11.11);
   }
}


#if 0

//
// The test case below is disabled because it will not pass. In fact, I am still
// in doubt if I'd call this a bug or a limitation. I'd like to support this
// syntax, but supporting it would complicate the things a bit (as far as I can
// see, at least). Given that there is a reasonable workaround (simply use
// doString()), I don't think I'll make this work anytime soon.
//
// For the record, the problem is that LuaUserData doesn't contain information
// about metatables. So, the call marked with "this fails here!" below fails
// with "attempt to index local 'a' (a userdata value)".
//

// - TestLuaFunctionWithCppObjectParam -----------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaFunctionWithCppObjectParam)
{
   // This test case was created in response to a bug report by Val Razgonyaev.

   using namespace Diluculum;
   LuaState ls;

   DILUCULUM_REGISTER_CLASS (ls["Account"], Account);

   LuaValueList params;
   params.push_back (50.0);
   Account aCppAccount (params);
   DILUCULUM_REGISTER_OBJECT (ls["TheAccount"], Account, aCppAccount);

   ls.doString(
      "function Func(a) return a:balance() * 2 end");

   LuaValueList ret = ls["Func"](ls["TheAccount"].value()); // this fails here!
   BOOST_REQUIRE_EQUAL (ret.size(), 1);
   BOOST_CHECK_EQUAL (ret[0].asNumber(), 100.0);
}

#endif // #if 0
