/******************************************************************************\
* WrappedFunctions.hpp                                                         *
* C++ functions wrapped for use with Lua for testing purposes.                 *
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

#ifndef _DILUCULUM_TESTS_WRAPPED_FUNCTIONS_HPP_
#define _DILUCULUM_TESTS_WRAPPED_FUNCTIONS_HPP_

#include <Diluculum/LuaWrappers.hpp>

namespace
{
   using Diluculum::LuaValueList;

   /** A global variable used in some "C Lua Functions" defined below. This
    *  global is changed as the side effect of calling these functions, so that
    *  it is possible to check whether a returnless function is being really
    *  executed.
    */
   int TheGlobal;


   /** Sets \c TheGlobal to zero. In Lua, this does not take or return anything
    *  (but no error is issued if parameters are passes: the implementation
    *  simply ignores them).
    */
   LuaValueList ZeroTheGlobal (const LuaValueList& params)
   {
      TheGlobal = 0;
      return LuaValueList();
   }

   DILUCULUM_WRAP_FUNCTION (ZeroTheGlobal)



   /** Sets \c TheGlobal to a given number value. In Lua, this takes one
    *  number parameter and returns nothing. Here, an error is raised if the
    *  parameter is of the wrong type (or if a wrong number of parameters is
    *  passed).
    */
   LuaValueList SetTheGlobal (const LuaValueList& params)
   {
      if (params.size() != 1)
      {
         throw Diluculum::LuaError(
            "Got wrong number of parameters (one was expected).");
      }

      if (params[0].type() != LUA_TNUMBER)
         throw Diluculum::LuaError ("First parameter should be a number.");

      TheGlobal = static_cast<int>(params[0].asNumber());
      return LuaValueList();
   }

   DILUCULUM_WRAP_FUNCTION (SetTheGlobal)



   /** Sets \c TheGlobal to the sum of the parameters passed. In Lua, this takes
    *  any number of parameters, which must be numbers. An error will be raised
    *  if any of the parameters is not a number. If no parameters are passed,
    *  \c TheGlobal is zeroed.
    */
   LuaValueList SetTheGlobalToSum (const LuaValueList& params)
   {
      int acc = 0;

      for (LuaValueList::size_type i = 0; i < params.size(); ++i)
      {
         if (params[i].type() != LUA_TNUMBER)
            throw Diluculum::LuaError ("All parameters should be numbers.");
         else
            acc += static_cast<int>(params[i].asNumber());
      }

      TheGlobal = acc;

      return LuaValueList();
   }

   DILUCULUM_WRAP_FUNCTION (SetTheGlobalToSum)



   /** Concatenates three strings. In Lua, takes three string parameters and
    *  return a string.
    */
   LuaValueList ConcatenateThree (const LuaValueList& params)
   {
      if (params.size() != 3)
      {
         throw Diluculum::LuaError(
            "Got wrong number of parameters (three were expected).");
      }

      if (params[0].type() != LUA_TSTRING
          || params[1].type() != LUA_TSTRING
          || params[2].type() != LUA_TSTRING)
      {
         throw Diluculum::LuaError ("All parameters should be strings.");
      }

      const std::string concatenated = params[0].asString()
         + params[1].asString() + params[2].asString();

      LuaValueList ret;
      ret.push_back (concatenated);

      return ret;
   }

   DILUCULUM_WRAP_FUNCTION (ConcatenateThree)


   /** Returns the first elements of the Fibonacci sequence. This takes one
    *  parameter: a number indicating the number of elements of the sequence
    *  that will be returned. Raises an error if it gets the wrong number or
    *  type of parameters.
    */
   LuaValueList FibonacciSequence (const LuaValueList& params)
   {
      if (params.size() != 1)
      {
         throw Diluculum::LuaError(
            "Got wrong number of parameters (one was expected).");
      }

      if (params[0].type() != LUA_TNUMBER)
         throw Diluculum::LuaError ("First parameter should be a number.");

      unsigned n = static_cast<unsigned>(params[0].asNumber());

      LuaValueList ret;
      int tmo = 1;
      int tmt = 1;
      for (unsigned i = 1; i <= n; ++i)
      {
         if (i <= 2)
            ret.push_back (1);
         else
         {
            int next = tmo+tmt;
            ret.push_back (next);
            tmt = tmo;
            tmo = next;
         }
      }

      return ret;
   }

   DILUCULUM_WRAP_FUNCTION (FibonacciSequence)



   /** Converts a number to a string or a string to a number. Input can be
    *  anything in the incredible range 1 to 3.
    */
   LuaValueList ToOrFromString (const LuaValueList& params)
   {
      if (params.size() != 1)
      {
         throw Diluculum::LuaError(
            "Got wrong number of parameters (one was expected).");
      }

      LuaValueList ret;

      if (params[0].type() == LUA_TNUMBER)
      {
         switch (static_cast<int>(params[0].asNumber()))
         {
            case 1:
               ret.push_back ("one");
               break;
            case 2:
               ret.push_back ("two");
               break;
            case 3:
               ret.push_back ("three");
               break;
            default:
               throw Diluculum::LuaError ("Parameter out of bounds.");
         }
      }
      else if (params[0].type() == LUA_TSTRING)
      {
         if (params[0].asString() == "one")
            ret.push_back (1);
         else if (params[0].asString() == "two")
            ret.push_back (2);
         else if (params[0].asString() == "three")
            ret.push_back (3);
         else
            throw Diluculum::LuaError ("Parameter out of bounds.");
      }
      else
         throw Diluculum::LuaError ("Parameter must be a number or a string.");

      return ret;
   }

   DILUCULUM_WRAP_FUNCTION (ToOrFromString)


} // (anonymous) namespace

#endif // _DILUCULUM_TESTS_WRAPPED_FUNCTIONS_HPP_
