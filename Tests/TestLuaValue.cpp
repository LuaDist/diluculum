/******************************************************************************\
* TestLuaValue.cpp                                                             *
* Unit tests for things declared in 'LuaValue.hpp'.                            *
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

#define BOOST_TEST_MODULE LuaValue

#include <cstring>
#include <boost/test/unit_test.hpp>
#include <Diluculum/LuaExceptions.hpp>
#include <Diluculum/LuaValue.hpp>
#include "WrappedClasses.hpp"


// - CLuaFunctionExample -------------------------------------------------------
int CLuaFunctionExample (lua_State* ls)
{
   return 0;
}



// - AnotherCLuaFunctionExample ------------------------------------------------
int AnotherCLuaFunctionExample (lua_State* ls)
{
   return 0;
}



// - TestLuaValueCopyConstructor -----------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaValueCopyConstructor)
{
   using namespace Diluculum;

   LuaValue aNilValue;
   LuaValue anotherNilValue (aNilValue);
   BOOST_CHECK (aNilValue == anotherNilValue);

   LuaValue aNumberValue (1.0);
   LuaValue anotherNumberValue (aNumberValue);
   BOOST_CHECK (aNumberValue == anotherNumberValue);

   LuaValue aStringValue ("Foo");
   LuaValue anotherStringValue (aStringValue);
   BOOST_CHECK (aStringValue == anotherStringValue);

   LuaValue aBooleanValue (true);
   LuaValue anotherBooleanValue (aBooleanValue);
   BOOST_CHECK (aBooleanValue == anotherBooleanValue);

   LuaValueMap aLuaValueMap;
   aLuaValueMap[1] = "one";
   aLuaValueMap[2] = "three";
   aLuaValueMap["confusing"] = true;
   LuaValue aTableValue (aLuaValueMap);
   LuaValue anotherTableValue (aTableValue);
   BOOST_CHECK (aTableValue == anotherTableValue);

   LuaValue aFunctionValue (CLuaFunctionExample);
   LuaValue anotherFunctionValue (aFunctionValue);
   BOOST_CHECK (aFunctionValue == anotherFunctionValue);

   LuaValue anUserDataValue (LuaUserData (1024));
   LuaValue anotherUserDataValue (anUserDataValue);
   BOOST_CHECK (anUserDataValue == anotherUserDataValue);

   char fbc[] = "fake bytecode";
   LuaValue aLuaFunctionValue (LuaFunction (fbc, strlen(fbc)));
   LuaValue anotherLuaFunctionValue (aLuaFunctionValue);
   BOOST_CHECK (aLuaFunctionValue == anotherLuaFunctionValue);
}



// - TestLuaValueAssignmentOperator --------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaValueAssignmentOperator)
{
   using namespace Diluculum;

   LuaValue anotherValue;

   LuaValue aNilValue;
   anotherValue = aNilValue;
   BOOST_CHECK (aNilValue == anotherValue);

   LuaValue aNumberValue (1.0);
   anotherValue = aNumberValue;
   BOOST_CHECK (aNumberValue == anotherValue);

   LuaValue aStringValue ("Foo");
   anotherValue = aStringValue;
   BOOST_CHECK (aStringValue == anotherValue);

   LuaValue aBooleanValue (true);
   anotherValue = aBooleanValue;
   BOOST_CHECK (aBooleanValue == anotherValue);

   LuaValueMap aLuaValueMap;
   aLuaValueMap[1] = "one";
   aLuaValueMap[2] = "three";
   aLuaValueMap["confusing"] = true;
   LuaValue aTableValue (aLuaValueMap);
   anotherValue = aLuaValueMap;
   BOOST_CHECK (aTableValue == anotherValue);

   LuaValue aFunctionValue (CLuaFunctionExample);
   anotherValue = aFunctionValue;
   BOOST_CHECK (aFunctionValue == anotherValue);

   LuaValue anUserDataValue (LuaUserData (1024));
   anotherValue = anUserDataValue;
   BOOST_CHECK (anUserDataValue == anotherValue);
}



// - TestLuaValueAndValueLists -------------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaValueAndValueLists)
{
   using namespace Diluculum;

   LuaValueList valueList;
   valueList.push_back (123.456);
   valueList.push_back ("Well, well, well...");
   valueList.push_back (false);
   valueList.push_back (-543);

   const LuaValue lv1 (valueList);
   BOOST_CHECK (lv1 == 123.456);

   LuaValue lv2;
   lv2 = valueList;
   BOOST_CHECK (lv2 == 123.456);
}



// - TestLuaValueType ----------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaValueType)
{
   using namespace Diluculum;

   LuaValue aNilValue;
   BOOST_CHECK (aNilValue.type() == LUA_TNIL);

   LuaValue aNumberValue1 (1.0);
   BOOST_CHECK (aNumberValue1.type() == LUA_TNUMBER);

   LuaValue aNumberValue2 (1.0f);
   BOOST_CHECK (aNumberValue2.type() == LUA_TNUMBER);

   LuaValue aNumberValue3 (1);
   BOOST_CHECK (aNumberValue3.type() == LUA_TNUMBER);

   LuaValue aNumberValue4 (1UL);
   BOOST_CHECK (aNumberValue4.type() == LUA_TNUMBER);

   LuaValue aStringValue ("Foo");
   BOOST_CHECK (aStringValue.type() == LUA_TSTRING);

   LuaValue aBooleanValue (true);
   BOOST_CHECK (aBooleanValue.type() == LUA_TBOOLEAN);

   LuaValueMap aLuaValueMap;
   LuaValue aTableValue (aLuaValueMap);
   BOOST_CHECK (aTableValue.type() == LUA_TTABLE);

   LuaValue aCFunctionValue (CLuaFunctionExample);
   BOOST_CHECK (aCFunctionValue.type() == LUA_TFUNCTION);

   char fbc[] = "fake bytecode";
   LuaValue aLuaFunctionValue (LuaFunction (fbc, strlen(fbc)));
   BOOST_CHECK (aCFunctionValue.type() == LUA_TFUNCTION);

   LuaValue anUserDataValue (LuaUserData (1024));
   BOOST_CHECK (anUserDataValue.type() == LUA_TUSERDATA);
}



// - TestLuaValueTypeName ------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaValueTypeName)
{
   using namespace Diluculum;

   LuaValue aNilValue;
   BOOST_CHECK (aNilValue.typeName() == "nil");

   LuaValue aNumberValue1 (1.0);
   BOOST_CHECK (aNumberValue1.typeName() == "number");

   LuaValue aNumberValue2 (1.0f);
   BOOST_CHECK (aNumberValue2.typeName() == "number");

   LuaValue aNumberValue3 (1);
   BOOST_CHECK (aNumberValue3.typeName() == "number");

   LuaValue aNumberValue4 (1U);
   BOOST_CHECK (aNumberValue4.typeName() == "number");

   LuaValue aStringValue ("Foo");
   BOOST_CHECK (aStringValue.typeName() == "string");

   LuaValue aBooleanValue (true);
   BOOST_CHECK (aBooleanValue.typeName() == "boolean");

   LuaValueMap aLuaValueMap;
   LuaValue aTableValue (aLuaValueMap);
   BOOST_CHECK (aTableValue.typeName() == "table");

   LuaValue aCFunctionValue (CLuaFunctionExample);
   BOOST_CHECK (aCFunctionValue.typeName() == "function");

   char fbc[] = "fake bytecode";
   LuaValue aLuaFunctionValue (LuaFunction (fbc, strlen(fbc)));
   BOOST_CHECK (aCFunctionValue.typeName() == "function");

   LuaValue anUserDataValue (LuaUserData (1024));
   BOOST_CHECK (anUserDataValue.typeName() == "userdata");
}



// - TestLuaValueAsSomethingFunctions ------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaValueAsSomethingFunctions)
{
   using namespace Diluculum;

   lua_State* ls = luaL_newstate();

   LuaValue aNilValue;
   LuaValue anotherNilValue = Nil;
   LuaValue aNumberValue1 (1.1);
   LuaValue aNumberValue2 (-2.0);
   LuaValue aStringValueFoo ("Foo");
   LuaValue aStringValueBar ("Bar");
   LuaValue aBooleanValue (true);
   LuaValue anotherBooleanValue (false);
   LuaValue aCFunctionValue (CLuaFunctionExample);

   LuaValueMap lvm;
   lvm["Foo"] = false;
   lvm[2.3] = 4.3;
   lvm[5.4] = static_cast<lua_Number>(4);
   lvm[171] = "Hey!";
   lvm[false] = Nil;
   lvm[true] = std::string("Ahhhh!");
   LuaValue tableValue (lvm);

   int ints[] = { 12, 13, 14, 15, 16, 17, 18, 19 };
   LuaUserData ud (sizeof(ints));
   memcpy (ud.getData(), &ints, sizeof(ints));
   LuaValue anUserDataValue (ud);

   char fbc[] = "fake bytecode";
   LuaValue aLuaFunctionValue (LuaFunction (fbc, strlen(fbc)));

   // Try to get the proper values.
   BOOST_CHECK (aNumberValue1.asNumber() == 1.1);
   BOOST_CHECK (aNumberValue2.asNumber() == -2.0);
   BOOST_CHECK (aNumberValue1.asInteger() == 1);
   BOOST_CHECK (aNumberValue2.asInteger() == -2);
   BOOST_CHECK (aStringValueFoo.asString() == "Foo");
   BOOST_CHECK (aStringValueBar.asString() == "Bar");
   BOOST_CHECK (aBooleanValue.asBoolean() == true);
   BOOST_CHECK (anotherBooleanValue.asBoolean() == false);
   BOOST_CHECK (aCFunctionValue.asFunction() == CLuaFunctionExample);

   BOOST_CHECK (tableValue.asTable()["Foo"].asBoolean() == false);
   BOOST_CHECK (tableValue.asTable()[2.3].asNumber() == 4.3);
   BOOST_CHECK (tableValue.asTable()[5.4].asNumber() == 4);
   BOOST_CHECK (tableValue.asTable()[171].asString() == "Hey!");
   BOOST_CHECK (tableValue.asTable()[true].asString() == "Ahhhh!");
   BOOST_CHECK (memcmp (anUserDataValue.asUserData().getData(), ints,
                        sizeof(ints)) == 0);
   BOOST_CHECK (memcmp (aLuaFunctionValue.asFunction().getData(), fbc,
                        strlen(fbc)) == 0);

   // Ensure that we get the proper exception when doing the Wrong Thing
   BOOST_CHECK_THROW (aNilValue.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (aNilValue.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (aNilValue.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (aNilValue.asTable(), TypeMismatchError);
   BOOST_CHECK_THROW (aNilValue.asFunction(), TypeMismatchError);
   BOOST_CHECK_THROW (aNilValue.asUserData(), TypeMismatchError);

   BOOST_CHECK_THROW (anotherNilValue.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (anotherNilValue.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (anotherNilValue.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (anotherNilValue.asTable(), TypeMismatchError);
   BOOST_CHECK_THROW (anotherNilValue.asFunction(), TypeMismatchError);
   BOOST_CHECK_THROW (anotherNilValue.asUserData(), TypeMismatchError);

   BOOST_CHECK_THROW (aNumberValue1.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (aNumberValue1.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (aNumberValue1.asTable(), TypeMismatchError);
   BOOST_CHECK_THROW (aNumberValue1.asFunction(), TypeMismatchError);
   BOOST_CHECK_THROW (aNumberValue1.asUserData(), TypeMismatchError);

   BOOST_CHECK_THROW (aStringValueFoo.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (aStringValueFoo.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (aStringValueFoo.asTable(), TypeMismatchError);
   BOOST_CHECK_THROW (aStringValueFoo.asFunction(), TypeMismatchError);
   BOOST_CHECK_THROW (aStringValueFoo.asUserData(), TypeMismatchError);

   BOOST_CHECK_THROW (aBooleanValue.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (aBooleanValue.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (aBooleanValue.asTable(), TypeMismatchError);
   BOOST_CHECK_THROW (aBooleanValue.asFunction(), TypeMismatchError);
   BOOST_CHECK_THROW (aBooleanValue.asUserData(), TypeMismatchError);

   BOOST_CHECK_THROW (aCFunctionValue.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (aCFunctionValue.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (aCFunctionValue.asTable(), TypeMismatchError);
   BOOST_CHECK_THROW (aCFunctionValue.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (aCFunctionValue.asUserData(), TypeMismatchError);

   BOOST_CHECK_THROW (aLuaFunctionValue.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (aLuaFunctionValue.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (aLuaFunctionValue.asTable(), TypeMismatchError);
   BOOST_CHECK_THROW (aLuaFunctionValue.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (aLuaFunctionValue.asUserData(), TypeMismatchError);

   BOOST_CHECK_THROW (anUserDataValue.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (anUserDataValue.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (anUserDataValue.asTable(), TypeMismatchError);
   BOOST_CHECK_THROW (anUserDataValue.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (anUserDataValue.asFunction(), TypeMismatchError);

   lua_close (ls);
}



// - TestLuaValueAsObjectPtr ---------------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaValueAsObjectPtr)
{
   using namespace Diluculum;

   LuaState ls;

   // First, register our class into our 'LuaState'
   DILUCULUM_REGISTER_CLASS (ls["Account"], Account);

   // In Lua, instantiate the class and call some methods
   ls.doString ("a = Account.new (100)");
   ls.doString ("a:deposit (75)");
   ls.doString ("a:withdraw (25)");

   // Sanity check if our account has the right balance
   Diluculum::LuaValueList ret = ls.doString ("return a:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 150.0);

   // Now, take a pointer to the object stored in the 'LuaState' and use it
   Account* account = ls["a"].value().asObjectPtr<Account*>();
   LuaValueList params;
   params.push_back (50.0);
   account->deposit (params);
   account->deposit (params);
   account->withdraw (params);

   ret = ls.doString ("return a:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 200.0);

   // Ensure that calls from Lua are still working
   ls.doString ("a:withdraw (125)");
   ls.doString ("a:deposit (25)");

   ret = ls.doString ("return a:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 100.0);

   // Finally, try the const version of asObjectPtr()
   const Account* constAccount = ls["a"].value().asObjectPtr<const Account*>();
   ret = constAccount->balance (LuaValueList());
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 100);
}



// - TestLuaValueRelationalOperators -------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaValueRelationalOperators)
{
   using namespace Diluculum;

   LuaValue aNilValue;
   LuaValue anotherNilValue;
   LuaValue aNumberValue1 (1.0);
   LuaValue aNumberValue2 (2);
   LuaValue aStringValueFoo ("Foo");
   LuaValue aStringValueBar ("Bar");
   LuaValue aBooleanValue (true);
   LuaValue anotherBooleanValue (true);
   LuaValue aCFunctionValue (CLuaFunctionExample);
   LuaValue anotherCFunctionValue (AnotherCLuaFunctionExample);

   LuaValueMap aLuaValueMap;
   LuaValue aTableValue (aLuaValueMap);

   char fbc1[] = "fake bytecode";
   LuaValue aLuaFunctionValue (LuaFunction (fbc1, strlen(fbc1)));

   char fbc2[] = "more fake bytecode";
   LuaValue anotherLuaFunctionValue (LuaFunction (fbc2, strlen(fbc2)));

   LuaValue sameLuaFunctionValue (LuaFunction (fbc1, strlen(fbc1)));

   int ints[] = { 12, 13, 14, 15, 16, 17, 18, 19 };
   LuaUserData ud1 (sizeof(ints));
   memcpy (ud1.getData(), ints, sizeof(ints));
   LuaValue anUserDataValue (ud1);

   int otherInts[] = { 12, 12, 14, 15, 16, 17, 18, 19 };
   LuaUserData ud2 (sizeof(otherInts));
   memcpy (ud2.getData(), otherInts, sizeof(otherInts));
   LuaValue anotherUserDataValue (ud2);

   int sameInts[] = { 12, 13, 14, 15, 16, 17, 18, 19 };
   LuaUserData ud3 (sizeof(sameInts));
   memcpy (ud3.getData(), sameInts, sizeof(sameInts));
   LuaValue sameUserDataValue (ud3);

   // operator<
   BOOST_CHECK (aBooleanValue < aNilValue);
   BOOST_CHECK (aBooleanValue < aCFunctionValue);
   BOOST_CHECK (aBooleanValue < aLuaFunctionValue);
   BOOST_CHECK (aNilValue < aNumberValue1);
   BOOST_CHECK (aCFunctionValue < aNilValue);
   BOOST_CHECK (aNumberValue2 < aStringValueFoo);
   BOOST_CHECK (aCFunctionValue < aNumberValue2);
   BOOST_CHECK (aCFunctionValue < aStringValueFoo);
   BOOST_CHECK (aLuaFunctionValue < aNumberValue2);
   BOOST_CHECK (aLuaFunctionValue < aStringValueFoo);
   BOOST_CHECK (aNilValue < aStringValueFoo);
   BOOST_CHECK (aBooleanValue < aStringValueFoo);
   BOOST_CHECK (aBooleanValue < aTableValue);
   BOOST_CHECK (aNilValue < aTableValue);
   BOOST_CHECK (aBooleanValue < aTableValue);
   BOOST_CHECK (aNumberValue1 < aNumberValue2);
   BOOST_CHECK (aStringValueBar < aStringValueFoo);
   BOOST_CHECK (aNilValue < anUserDataValue);
   BOOST_CHECK (aNumberValue1 < anUserDataValue);
   BOOST_CHECK (aStringValueFoo < anUserDataValue);
   BOOST_CHECK (aBooleanValue < anUserDataValue);
   BOOST_CHECK (aCFunctionValue < anUserDataValue);
   BOOST_CHECK (aLuaFunctionValue < anUserDataValue);
   BOOST_CHECK (aNumberValue1 < 1.01);
   BOOST_CHECK (aNumberValue2 < 2.5L);
   BOOST_CHECK (aStringValueFoo < "Zzz");

   // ! operator>
   BOOST_CHECK (!(aBooleanValue > aNilValue));
   BOOST_CHECK (!(aBooleanValue > aCFunctionValue));
   BOOST_CHECK (!(aBooleanValue > aLuaFunctionValue));
   BOOST_CHECK (!(aNilValue > aNumberValue1));
   BOOST_CHECK (!(aCFunctionValue > aNilValue));
   BOOST_CHECK (!(aLuaFunctionValue > aNilValue));
   BOOST_CHECK (!(aNumberValue2 > aStringValueFoo));
   BOOST_CHECK (!(aCFunctionValue > aNumberValue2));
   BOOST_CHECK (!(aCFunctionValue > aStringValueFoo));
   BOOST_CHECK (!(aLuaFunctionValue > aNumberValue2));
   BOOST_CHECK (!(aLuaFunctionValue > aStringValueFoo));
   BOOST_CHECK (!(aNilValue > aStringValueFoo));
   BOOST_CHECK (!(aBooleanValue > aStringValueFoo));
   BOOST_CHECK (!(aBooleanValue > aBooleanValue));
   BOOST_CHECK (!(aNilValue > aNilValue));
   BOOST_CHECK (!(aNumberValue1 > aNumberValue1));
   BOOST_CHECK (!(aNumberValue2 > aNumberValue2));
   BOOST_CHECK (!(aBooleanValue > anotherBooleanValue));
   BOOST_CHECK (!(aTableValue > aTableValue));
   BOOST_CHECK (!(aNumberValue1 > aNumberValue2));
   BOOST_CHECK (!(aStringValueBar > aStringValueFoo));
   BOOST_CHECK (!(aNilValue > anUserDataValue));
   BOOST_CHECK (!(aNumberValue1 > anUserDataValue));
   BOOST_CHECK (!(aStringValueFoo > anUserDataValue));
   BOOST_CHECK (!(aBooleanValue > anUserDataValue));
   BOOST_CHECK (!(aCFunctionValue > anUserDataValue));
   BOOST_CHECK (!(aLuaFunctionValue > anUserDataValue));
   BOOST_CHECK (!(aNumberValue1 > 1.01));
   BOOST_CHECK (!(aNumberValue2 > 2.5L));
   BOOST_CHECK (!(aStringValueFoo > "Zzz"));

   // operator>
   BOOST_CHECK (aNilValue > aBooleanValue);
   BOOST_CHECK (aNilValue > aCFunctionValue);
   BOOST_CHECK (aNilValue > aLuaFunctionValue);
   BOOST_CHECK (aNumberValue1 > aNilValue);
   BOOST_CHECK (aNumberValue1 > aCFunctionValue);
   BOOST_CHECK (aNumberValue1 > aLuaFunctionValue);
   BOOST_CHECK (aStringValueFoo > aNumberValue2);
   BOOST_CHECK (aStringValueFoo > aNilValue);
   BOOST_CHECK (aStringValueFoo > aBooleanValue);
   BOOST_CHECK (aStringValueFoo > aCFunctionValue);
   BOOST_CHECK (aStringValueFoo > aLuaFunctionValue);
   BOOST_CHECK (aCFunctionValue > aBooleanValue);
   BOOST_CHECK (aLuaFunctionValue > aBooleanValue);
   BOOST_CHECK (aTableValue > aBooleanValue);
   BOOST_CHECK (aTableValue > aNilValue);
   BOOST_CHECK (aTableValue > aBooleanValue);
   BOOST_CHECK (aTableValue > aStringValueFoo);
   BOOST_CHECK (aNumberValue2 > aNumberValue1);
   BOOST_CHECK (aStringValueFoo > aStringValueBar);
   BOOST_CHECK (anUserDataValue > aNilValue);
   BOOST_CHECK (anUserDataValue > aNumberValue1);
   BOOST_CHECK (anUserDataValue > aStringValueFoo);
   BOOST_CHECK (anUserDataValue > aBooleanValue);
   BOOST_CHECK (anUserDataValue > aCFunctionValue);
   BOOST_CHECK (anUserDataValue > aLuaFunctionValue);
   BOOST_CHECK (aNumberValue1 > -10);
   BOOST_CHECK (aNumberValue2 > 1e-10f);
   BOOST_CHECK (aStringValueFoo > "");

   // ! operator<
   BOOST_CHECK (!(aNilValue < aBooleanValue));
   BOOST_CHECK (!(aNilValue < aCFunctionValue));
   BOOST_CHECK (!(aNilValue < aLuaFunctionValue));
   BOOST_CHECK (!(aNumberValue1 < aNilValue));
   BOOST_CHECK (!(aNumberValue1 < aCFunctionValue));
   BOOST_CHECK (!(aNumberValue1 < aLuaFunctionValue));
   BOOST_CHECK (!(aStringValueFoo < aNumberValue2));
   BOOST_CHECK (!(aStringValueFoo < aNilValue));
   BOOST_CHECK (!(aStringValueFoo < aBooleanValue));
   BOOST_CHECK (!(aStringValueFoo < aCFunctionValue));
   BOOST_CHECK (!(aStringValueFoo < aLuaFunctionValue));
   BOOST_CHECK (!(aCFunctionValue < aBooleanValue));
   BOOST_CHECK (!(aLuaFunctionValue < aBooleanValue));
   BOOST_CHECK (!(aBooleanValue < aBooleanValue));
   BOOST_CHECK (!(aNilValue < aNilValue));
   BOOST_CHECK (!(aTableValue < aStringValueFoo));
   BOOST_CHECK (!(aNumberValue1 < aNumberValue1));
   BOOST_CHECK (!(aNumberValue2 < aNumberValue2));
   BOOST_CHECK (!(aBooleanValue < anotherBooleanValue));
   BOOST_CHECK (!(aTableValue < aTableValue));
   BOOST_CHECK (!(aNumberValue2 < aNumberValue1));
   BOOST_CHECK (!(aStringValueFoo < aStringValueBar));
   BOOST_CHECK (!(anUserDataValue < aNilValue));
   BOOST_CHECK (!(anUserDataValue < aNumberValue1));
   BOOST_CHECK (!(anUserDataValue < aStringValueFoo));
   BOOST_CHECK (!(anUserDataValue < aBooleanValue));
   BOOST_CHECK (!(anUserDataValue < aCFunctionValue));
   BOOST_CHECK (!(anUserDataValue < aLuaFunctionValue));
   BOOST_CHECK (!(aNumberValue1 < -10));
   BOOST_CHECK (!(aNumberValue2 < 1e-10f));
   BOOST_CHECK (!(aStringValueFoo < ""));

   // operator==
   short sOne = 1;
   unsigned short usOne = 1;
   BOOST_CHECK (aNumberValue1 == 1);
   BOOST_CHECK (aNumberValue1 == 1.0f);
   BOOST_CHECK (aNumberValue1 == 1.0);
   BOOST_CHECK (aNumberValue1 == 1.0L);
   BOOST_CHECK (aNumberValue1 == 1U);
   BOOST_CHECK (aNumberValue1 == 1L);
   BOOST_CHECK (aNumberValue1 == 1UL);
   BOOST_CHECK (aNumberValue1 == sOne);
   BOOST_CHECK (aNumberValue1 == usOne);

   BOOST_CHECK (aStringValueFoo == "Foo");
   BOOST_CHECK (aStringValueFoo == std::string("Foo"));
   BOOST_CHECK (aBooleanValue == true);
   BOOST_CHECK (aCFunctionValue == CLuaFunctionExample);
   BOOST_CHECK (aLuaFunctionValue == sameLuaFunctionValue);
   BOOST_CHECK (anUserDataValue == sameUserDataValue);

   // ! operator!=
   BOOST_CHECK (!(aNumberValue1 != 1));
   BOOST_CHECK (!(aNumberValue1 != 1.0f));
   BOOST_CHECK (!(aNumberValue1 != 1.0));
   BOOST_CHECK (!(aNumberValue1 != 1.0L));
   BOOST_CHECK (!(aNumberValue1 != 1U));
   BOOST_CHECK (!(aNumberValue1 != 1L));
   BOOST_CHECK (!(aNumberValue1 != 1UL));
   BOOST_CHECK (!(aStringValueFoo != "Foo"));
   BOOST_CHECK (!(aStringValueFoo != std::string("Foo")));
   BOOST_CHECK (!(aBooleanValue != true));
   BOOST_CHECK (!(aCFunctionValue != CLuaFunctionExample));
   BOOST_CHECK (!(aLuaFunctionValue != sameLuaFunctionValue));
   BOOST_CHECK (!(anUserDataValue != sameUserDataValue));

   // operator !=
   BOOST_CHECK (aCFunctionValue != 0);
   BOOST_CHECK (anotherCFunctionValue != 0);
   BOOST_CHECK (aCFunctionValue != anotherCFunctionValue);
   BOOST_CHECK (aLuaFunctionValue != anotherLuaFunctionValue);
   BOOST_CHECK (anUserDataValue != anotherUserDataValue);

   // some tests with more complex tables
   LuaValueMap lvmEmpty;
   LuaValue emptyTableValue (lvmEmpty);

   LuaValueMap lvmShort;
   lvmShort["Foo"] = 1.2;
   lvmShort[false] = "Bar";
   LuaValue shortTableValue (lvmShort);

   BOOST_CHECK (emptyTableValue < shortTableValue);
   BOOST_CHECK (!(emptyTableValue > shortTableValue));
   BOOST_CHECK (shortTableValue > emptyTableValue);
   BOOST_CHECK (!(shortTableValue < emptyTableValue));

   LuaValueMap lvmShortClone;
   lvmShortClone["Foo"] = 1.2;
   lvmShortClone[false] = "Bar";
   LuaValue shortCloneTableValue (lvmShortClone);

   BOOST_CHECK (!(shortTableValue > shortCloneTableValue));
   BOOST_CHECK (!(shortCloneTableValue < shortTableValue));

   LuaValueMap lvmLong;
   lvmLong["Foo"] = false;
   lvmLong[2.3] = 4.3;
   lvmLong[5.4] = lvmShort;
   lvmLong[true] = std::string("Ahhhh!");
   LuaValue longTableValue (lvmLong);

   BOOST_CHECK (shortTableValue < longTableValue);
   BOOST_CHECK (!(shortTableValue > longTableValue));
   BOOST_CHECK (longTableValue > shortTableValue);
   BOOST_CHECK (!(longTableValue < shortTableValue));

   LuaValueMap lvmShortGreater;
   lvmShortGreater["Foo"] = 1.5;
   lvmShortGreater[false] = "Bar";
   LuaValue shortGreaterTableValue (lvmShortGreater);

   BOOST_CHECK (shortTableValue < shortGreaterTableValue);
   BOOST_CHECK (!(shortTableValue > shortGreaterTableValue));
   BOOST_CHECK (shortGreaterTableValue > shortTableValue);
   BOOST_CHECK (!(shortGreaterTableValue < shortTableValue));

   LuaValueMap lvmLongLess;
   lvmLongLess["Foo"] = false;
   lvmLongLess[2.3] = 4.3;
   lvmLongLess[5.4] = lvmShort;
   lvmLongLess[false] = std::string("Ahhhh!");
   LuaValue longLessTableValue (lvmLongLess);

   BOOST_CHECK (longLessTableValue < longTableValue);
   BOOST_CHECK (!(longLessTableValue > longTableValue));
   BOOST_CHECK (longTableValue > longLessTableValue);
   BOOST_CHECK (!(longTableValue < longLessTableValue));
}



// - TestLuaValueSubscriptOperator ---------------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaValueSubscriptOperator)
{
   using namespace Diluculum;

   LuaValueMap nestedLMV;
   nestedLMV["foo"] = "bar";
   nestedLMV[true] = 123.45;
   nestedLMV[2.0] = false;

   LuaValueMap lvm;
   lvm[4.0] = 6.5;
   lvm[555u] = "FiveFiveFive";
   lvm["nested"] = nestedLMV;
   lvm[true] = false;

   LuaValue tableValue (lvm);

   // Simple data access
   BOOST_CHECK (tableValue[4.0] == 6.5);
   BOOST_CHECK (tableValue[555] == "FiveFiveFive");
   BOOST_CHECK (tableValue["nested"]["foo"] == "bar");
   BOOST_CHECK (tableValue["nested"][true] == 123.45);
   BOOST_CHECK (tableValue["nested"][2.0] == false);
   BOOST_CHECK (tableValue[true] == false);

   // Change some fields
   tableValue[true] = 7.89;
   BOOST_CHECK (tableValue[true] == 7.89);
   tableValue["nested"][2.0] = "baz";
   BOOST_CHECK (tableValue["nested"][2.0] == "baz");

   // Add some new fields
   tableValue["dummy"];
   BOOST_CHECK (tableValue["dummy"] == Nil);

   tableValue["aaa"] = 2.5;
   BOOST_CHECK (tableValue["aaa"] == 2.5);

   tableValue["nested"][7.8] = 1.23;
   BOOST_CHECK (tableValue["nested"][7.8] == 1.23);

   // Try to subscript non-table values
   LuaValue nilValue;
   BOOST_CHECK_THROW (nilValue[1.0], TypeMismatchError);

   LuaValue numberValue(1.0);
   BOOST_CHECK_THROW (numberValue[true], TypeMismatchError);

   LuaValue stringValue("abc");
   BOOST_CHECK_THROW (stringValue["xyz"], TypeMismatchError);

   LuaValue boolValue(true);
   BOOST_CHECK_THROW (boolValue[false], TypeMismatchError);
}



// - TestLuaValueConstSubscriptOperator ----------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaValueConstSubscriptOperator)
{
   using namespace Diluculum;

   LuaValueMap nestedLMV;
   nestedLMV["foo"] = "bar";
   nestedLMV[true] = 123.45;
   nestedLMV[2.0] = false;

   LuaValueMap lvm;
   lvm[4.0] = 6.5;
   lvm["nested"] = nestedLMV;
   lvm[true] = false;
   lvm[123L] = 456;

   const LuaValue tableValue (lvm);

   // Simple data access
   BOOST_CHECK (tableValue[4.0] == 6.5);
   BOOST_CHECK (tableValue["nested"]["foo"] == "bar");
   BOOST_CHECK (tableValue["nested"][true] == 123.45);
   BOOST_CHECK (tableValue["nested"][2.0] == false);
   BOOST_CHECK (tableValue[true] == false);
   BOOST_CHECK (tableValue[123] == 456);

   // Then, ensure that accessing an invalid key returns 'Nil'
   BOOST_CHECK (tableValue["dummy"] == Nil);
   BOOST_CHECK (tableValue[123.456] == Nil);
   BOOST_CHECK (tableValue["nested"][7.8] == Nil);

   // Try to subscript non-table values
   LuaValue nilValue;
   BOOST_CHECK_THROW (nilValue[1.0], TypeMismatchError);

   LuaValue numberValue(1.0);
   BOOST_CHECK_THROW (numberValue[true], TypeMismatchError);

   LuaValue stringValue("abc");
   BOOST_CHECK_THROW (stringValue["xyz"], TypeMismatchError);

   LuaValue boolValue(true);
   BOOST_CHECK_THROW (boolValue[false], TypeMismatchError);
}



// - TestLuaValueWithStringWithEmbeddedNull ------------------------------------
BOOST_AUTO_TEST_CASE(TestLuaValueWithStringWithEmbeddedNull)
{
   // This test was added to catch a problem reported by Christof Warlich (who
   // also pinpointed the bug and shown the solution).
   using namespace Diluculum;

   LuaState state;
   state.doString ("stringWithNull = 'ab\\0cd'");

   LuaValue stringWithNull = state["stringWithNull"].value();
   BOOST_REQUIRE_EQUAL (stringWithNull.type(), LUA_TSTRING);

   std::string s = stringWithNull.asString();

   BOOST_REQUIRE_EQUAL (s.size(), 5);
   BOOST_CHECK_EQUAL (s[0], 'a');
   BOOST_CHECK_EQUAL (s[1], 'b');
   BOOST_CHECK_EQUAL (s[2], 0);
   BOOST_CHECK_EQUAL (s[3], 'c');
   BOOST_CHECK_EQUAL (s[4], 'd');
}
