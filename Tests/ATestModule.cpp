/******************************************************************************\
* ATestModule.cpp                                                              *
* A Lua module as a shared library, created using Diluculum facilities.        *
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


#include <Diluculum/LuaWrappers.hpp>


/** A class that will be exported by the Lua module. Does nothing interesting.
 *  It's just a test...
 */
class AClass
{
   public:
      /// Constructor. Takes a number and stores it in \c val_.
      AClass (const Diluculum::LuaValueList& params)
      {
         if (params.size() != 1 || params[0].type() != LUA_TNUMBER)
         {
            throw Diluculum::LuaError(
               "Constructor expected a single number parameter.");
         }
         val_ = params[0].asNumber();
      }

      /// A test method; returns val_.
      Diluculum::LuaValueList aMethod (const Diluculum::LuaValueList& params)
      {
         Diluculum::LuaValueList ret;
         ret.push_back (val_);
         return ret;
      }

   private:
      /** Just a value. If you want to read something with meaning, try
       *  Literature, instead of my test classes.
       */
      double val_;
};



/// A function that always returns two values: \c true and <tt>"Ahey!"</tt>.
Diluculum::LuaValueList AFunction (const Diluculum::LuaValueList& params)
{
   Diluculum::LuaValueList ret;
   ret.push_back (true);
   ret.push_back ("Ahey!");

   return ret;
}

DILUCULUM_BEGIN_CLASS (AClass);
   DILUCULUM_CLASS_METHOD (AClass, aMethod);
DILUCULUM_END_CLASS (AClass);

DILUCULUM_WRAP_FUNCTION (AFunction);

DILUCULUM_BEGIN_MODULE (ATestModule);
   DILUCULUM_MODULE_ADD_CLASS (AClass, "AClass");
   DILUCULUM_MODULE_ADD_FUNCTION (DILUCULUM_WRAPPER_FUNCTION(AFunction), "AFunction");
DILUCULUM_END_MODULE();
