#include <stdio.h>
#include "example_interface.h"

//************First Implementor*****************************//
typedef struct {
   int value;
} Implementor1;

void Implementor1_function1(Implementor1 *self){
   printf("Implementor1::function1()\r\n");
}
void Implementor1_function2(Implementor1 *self, int value){
   self->value = value;
   printf("Implementor1::function2() => %u\r\n",self->value);
}

IMPLEMENT_getExampleInterface(Implementor1,Implementor1)


//************Second Implementor*****************************//
typedef struct {
   int value;
} Implementor2;

void Implementor2_function1(Implementor2 *self){
   printf("Implementor2::function1()\r\n");
}
void Implementor2_function2(Implementor2 *self, int value){
   self->value = value + 25;
   printf("Implementor2::function2() => %u\r\n",self->value);
}

IMPLEMENT_getExampleInterface(Implementor2,Implementor2)


int main(void){
   Implementor1 v1;
   Implementor2 v2;

   // An array of implementors
   ExampleInterface Implementors[2];

   // Assign the implementors at runtime.  These functions
   // are created by the macros used above
   Implementors[0] = Implementor1_getExampleInterface(&v1);
   Implementors[1] = Implementor2_getExampleInterface(&v2);

   printf("Interface Pattern Example\r\n");

   for(int i=0; i<2; i++){
      // Use the actual interface defined functions to call
      // the dispatched versions.
      ExampleInterface_function1(&Implementors[i]);
      ExampleInterface_function2(&Implementors[i],100);
   }
   return 0;
}