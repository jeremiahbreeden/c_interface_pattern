/*****************************************************************************
--                                                                          --
--    Copyright (C) 2014-Present, Jeremiah Breeden                          --
--                                                                          --
-- This is free software;  you can  redistribute it  and/or modify it under --
-- terms of the  GNU General Public License as published  by the Free Soft- --
-- ware  Foundation;  either version 3,  or (at your option) any later ver- --
-- sion.  This is distributed in the hope that it will be useful, but WITH- --
-- OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY --
-- or FITNESS FOR A PARTICULAR PURPOSE.                                     --
--                                                                          --
-- As a special exception under Section 7 of GPL version 3, you are granted --
-- additional permissions described in the GCC Runtime Library Exception,   --
-- version 3.1, as published by the Free Software Foundation.               --
--                                                                          --
-- You should have received a copy of the GNU General Public License and    --
-- a copy of the GCC Runtime Library Exception along with this program;     --
-- see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see    --
-- <http://www.gnu.org/licenses/>.                                          --
--                                                                          --
--  As a special exception, if you link this unit or a modified copy of     --
--  this unit with other files to produce an executable, this unit does     --
--  not by itself cause the resulting executable to be covered by the       --
--  GNU General Public License. This exception does not however invalidate  --
--  any other reasons why the executable file might be covered by the       --
--  GNU Public License.                                                     --
--                                                                          --
*****************************************************************************/

/***************************************************************************
* This file lays out a pattern for creating a runtime dispatching interface
* for any type.  Names used in this file (specifically ExampleInterface, 
* function1, and function2) are used to faciliate find/replace.  You will
* still have to manually modify function declarations, definitions, and
* calls to accomodate parameter lists and return types.  You can delete
* this comment block in your modified files.
***************************************************************************/

/***************************************************************************
* An interface pattern is one method for implementing a "detached" dynamic
* dispatch mechanism for C functions.  It follows the pattern of object
* oriented classes by associating functions to a type (by making sure
* the first parameter of every function is a pointer to the type).  Since
* there is no inheritance in C, this pattern focuses on making FAT pointers
* to the type that wants to implement the interface and all dispatching 
* functions will operate on the FAT pointers.  This allows the designer
* to implement their types as they want without any interfaces in mind and
* then later add the interface capability to them using the MACRO provided
* in this file.
***************************************************************************/

#ifndef ExampleInterface_H
#define ExampleInterface_H

// Forward declare the interface name.  Prefer naming convention that will
// stand out with _ separators between the interface name and the functions.
// This will represent a FAT pointer and must be treated as a pointer normally
// would in C.  The typedef is just to get rid of the need to use "struct"
// in front of the types.
struct ExampleInterface;
typedef struct ExampleInterface ExampleInterface;

// Example interface functions
void ExampleInterface_function1(ExampleInterface *self);
void ExampleInterface_function2(ExampleInterface *self, int param1);

// functions created will use the form:
//
//    <prefix>_<function name>(<typename> *self, ...)
//
// EX:
//    IMPLEMENT_getExampleInterface(AnImplementor,Implementor_t)
//    
// this will make:
//
//    AnImplementor_getExampleInterface(Implementor_t *self);
//
//    both prefix and typename can be the same phrase.
#define IMPLEMENT_getExampleInterface(prefix,typename)         \
   void prefix##_function1_delegate(ExampleInterface *self){   \
      prefix##_function1(self->obj);                           \
   }                                                           \
                                                               \
   void prefix##_function2_delegate(ExampleInterface *self, int param1){   \
      prefix##_function2(self->obj,param1);                                \
   }                                                                       \
                                                                           \
   static const ExampleInterface_vtable_t prefix##_vtable = {  \
      prefix##_function1_delegate,                             \
      prefix##_function2_delegate                              \
   };                                                          \
                                                               \
   ExampleInterface prefix##_getExampleInterface(typename *self){    \
      ExampleInterface result;                                       \
      result.obj = self;                                             \
      result.vtable = &prefix##_vtable;                              \
      return result;                                                 \
   }      

//***************** IMPLEMENTATION:  DO NOT USE DIRECTLY *******************//              

typedef struct {                                      
   void (*function1)(ExampleInterface *);           
   void (*function2)(ExampleInterface *,int);   
} ExampleInterface_vtable_t;                                  
                                                         
struct ExampleInterface {
   void *obj;
   const ExampleInterface_vtable_t *vtable;
};

void ExampleInterface_function1(struct ExampleInterface *self){
   self->vtable->function1(self);
}
void ExampleInterface_function2(struct ExampleInterface *self, int param1){
   self->vtable->function2(self,param1);
}

#endif