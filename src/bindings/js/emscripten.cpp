#include <emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

struct MyStruct {
  double a, b, c;
};

MyStruct Foo(const MyStruct x, double y) {
  MyStruct r;
  r.a = x.a;
  r.b = x.b;
  r.c = y;
  return r;
}

class FilterWrapper {
public:
    int getNumElements() const {
        return num;
    }

    double getValue(int i) {
        return vals[i];
    }
    
  int getX() const { return x; }
  void setX(int x_) { x = x_; }
private:
    int num = 5;
    double vals[5] = {1.0,2.0,3.0,4.0,5.0};
    int x = 4;
};


float lerp(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("lerp", &lerp);
}

EMSCRIPTEN_BINDINGS(my_struct) {
  class_<MyStruct>("MyStruct")
    .constructor<>()
    .property("a", &MyStruct::a)
    .property("b", &MyStruct::b)
    .property("c", &MyStruct::c)
    ;


  class_<FilterWrapper>("FilterWrapper")
    .constructor<>()
    .function("getNumElements", &FilterWrapper::getNumElements)
    .function("getValue", &FilterWrapper::getValue)
    .property("size", &FilterWrapper::getNumElements)
    ;

  function("Foo", &Foo);
}



