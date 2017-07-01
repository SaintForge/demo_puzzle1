#ifndef VECTOR
#define VECTOR

#include <math.h>
#include <stdlib.h>

class Vector2{
public:
     float x, y;
public:
     Vector2(){
	  x = 0;
	  y = 0;
     }
     ~Vector2(){}
     Vector2(const Vector2 &vec){
	  x = vec.x;
	  y = vec.y;
     }
     Vector2(float x_, float y_){
	  x = x_;
	  y = y_;
     }

     void init(float x_, float y_){
	  x = x_;
	  y = y_;
     }

     void add(float x_, float y_){
	  x += x_;
	  y += y_;
     }
     void add(Vector2 vec){
	  x += vec.x;
	  y += vec.y;
     }
     
     void sub(float x_, float y_){
	  x -= x_;
	  y -= y_;
     }
     void sub(Vector2 vec){
	  x -= vec.x;
	  y -= vec.y;
     }

     void mult(float mult){
	  x *= mult;
	  y *= mult;
     }
     void div(float div){
	  x /= div;
	  y /= div;
     }

     float mag(){
	  return sqrt(x*x + y*y);
     }

     void normalize(){
	  float m = mag();
	  if(m != 0) div(m);
     }

     void limit(float max){
	  if(mag() > max){
	       normalize();
	       mult(max);
	  }
     }

     static Vector2 random2(){
	  Vector2 rand((rand() % 50) - 25, (rand() % 50) - 25);
	  rand.normalize();
	  return rand;
     }

};

#endif
