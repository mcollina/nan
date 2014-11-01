// toys used in testing
// included first, so windows actually defines M_PI
#ifdef _WIN32
# define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <time.h>

#include <nan.h>
#include "nan_new.hpp"

using namespace v8;

template <typename T, typename U>
struct is_same {
  static const bool value = false;
};

template <typename T>
struct is_same<T, T> {
  static const bool value = true;
};

template <typename T, typename U>
bool
assertType(U value) {
  return is_same<v8::Local<T>, U>::value;
}

bool
stringMatches(Local<Value> value, const char * match) {
  String::Utf8Value v(value);
  return std::string(*v) == std::string(match);
}

#define _(e) NAN_TEST_EXPRESSION(e)

NAN_METHOD(testArray) {
  NanScope();
  NanTap t(args[0]);

  t.plan(3);

  t.ok(_( NanNew2<Array>()->Length() == 0));
  t.ok(_( NanNew2<Array>(7)->Length() == 7));
  t.ok(_( assertType<Array>(NanNew2<Array>(7))));

  return_NanUndefined();
}

NAN_METHOD(testBoolean) {
  NanScope();
  NanTap t(args[0]);

  t.plan(6);

  t.ok(_( NanNew2<Boolean>(true)->Value() == true));
  t.ok(_( NanNew2<Boolean>(false)->Value() == false));
  t.ok(_( assertType<Boolean>( NanNew2<Boolean>(true))));

  t.ok(_( NanNew2(true)->Value() == true));
  t.ok(_( NanNew2(false)->Value() == false));
  t.ok(_( assertType<Boolean>( NanNew2(true))));

  return_NanUndefined();
}

NAN_METHOD(testDate) {
  NanScope();
  NanTap t(args[0]);

  t.plan(1);

  t.ok(_( assertType<Date>( NanNew2<Date>(double(time(NULL))))));

  return_NanUndefined();
}

NAN_METHOD(testScript) {
  NanScope();
  NanTap t(args[0]);

  t.plan(2);

  t.ok(_( assertType<Script>(NanNew2<Script>(NanNew2("2 + 3")))));
  t.ok(_( assertType<NanUnboundScript>(NanNew2<NanUnboundScript>(NanNew2("2 + 3")))));

  return_NanUndefined();
}

NAN_METHOD(testString) {
  NanScope();
  NanTap t(args[0]);

  t.plan(8);

  t.ok(_( stringMatches( NanNew2<String>("Hello World"), "Hello World")));
  t.ok(_( stringMatches( NanNew2<String>("Hello World", 4), "Hell")));
  t.ok(_( stringMatches( NanNew2<String>(std::string("foo")), "foo")));
  t.ok(_( assertType<String>( NanNew2<String>("plonk."))));

  //=== Convenience ============================================================

  t.ok(_( stringMatches( NanNew2("using namespace nan; // is poetry"),
          "using namespace nan; // is poetry")));
  t.ok(_( assertType<String>( NanNew2("plonk."))));

  t.ok(_( stringMatches( NanNew2(std::string("bar")), "bar")));
  t.ok(_( assertType<String>( NanNew2(std::string("plonk.")))));

  return_NanUndefined();
}

NAN_METHOD(testNumber) {
  NanScope();
  NanTap t(args[0]);

  t.plan(17);

  t.ok(_( NanNew2<Int32>(5)->Value() == 5 ));
  t.ok(_( NanNew2<Int32>(-42)->Value() == -42 ));
  t.ok(_( assertType<Int32>( NanNew2<Int32>(23) )));

  t.ok(_( NanNew2<Uint32>(5u)->Value() == 5u ));
  t.ok(_( assertType<Uint32>( NanNew2<Uint32>(23u) )));

  t.ok(_( NanNew2<Integer>(5)->Value() == 5 ));
  t.ok(_( NanNew2<Integer>(-1337)->Value() == -1337 ));
  t.ok(_( assertType<Integer>( NanNew2<Integer>(-42) )));

  const double epsilon = 1e-6;
  t.ok(_( NanNew2<Number>(M_PI)->Value() - M_PI < epsilon));
  t.ok(_( NanNew2<Number>(-M_PI)->Value() + M_PI < epsilon));
  t.ok(_( assertType<Number>( NanNew2<Number>(M_E) )));

  //=== Convenience ============================================================

  t.ok(_( NanNew2(5)->Value() == 5 ));
  t.ok(_( assertType<Int32>( NanNew2(23) )));

  t.ok(_( NanNew2(5u)->Value() == 5u ));
  t.ok(_( assertType<Uint32>( NanNew2(23u) )));

  t.ok(_( NanNew2(M_PI)->Value() - M_PI < epsilon));
  t.ok(_( assertType<Number>( NanNew2(M_E) )));

  return_NanUndefined();
}

NAN_METHOD(newIntegerWithValue) {
  NanScope();
  return_NanValue(NanNew2<Integer>(args[0]->Int32Value()));
}

NAN_METHOD(newNumberWithValue) {
  NanScope();
  return_NanValue(NanNew2<Number>(args[0]->NumberValue()));
}

NAN_METHOD(newUint32WithValue) {
  NanScope();
  return_NanValue(NanNew2<Uint32>(args[0]->Uint32Value()));
}

NAN_METHOD(newStringFromChars) {
  NanScope();
  return_NanValue(NanNew2<String>("hello?"));
}

NAN_METHOD(newStringFromCharsWithLength) {
  NanScope();
  return_NanValue(NanNew2<String>("hello?", 4));
}

NAN_METHOD(newStringFromStdString) {
  NanScope();
  return_NanValue(NanNew2<String>(std::string("hello!")));
}

NAN_METHOD(demoDateAndNumber) {
  NanScope();
  Local<Value> number = NanNew<Number>(M_PI);
  Local<Value> date   = NanNew<Date>(double(time(NULL)));
  (void)number; (void)date; // unused
  return_NanUndefined();
}

int ttt = 23;

NAN_METHOD(newExternal) {
  NanScope();
  return_NanValue(NanNew2<External>(&ttt));
}

void Init(Handle<Object> exports) {
  NAN_EXPORT(exports, testArray);
  NAN_EXPORT(exports, testBoolean);
  NAN_EXPORT(exports, testDate);
  NAN_EXPORT(exports, testScript);
  NAN_EXPORT(exports, testString);
  NAN_EXPORT(exports, testNumber);

  NAN_EXPORT(exports, newIntegerWithValue);
  NAN_EXPORT(exports, newNumberWithValue);
  NAN_EXPORT(exports, newUint32WithValue);
  NAN_EXPORT(exports, newStringFromChars);
  NAN_EXPORT(exports, newStringFromCharsWithLength);
  NAN_EXPORT(exports, newStringFromStdString);

  NAN_EXPORT(exports, newExternal);

  NAN_EXPORT(exports, demoDateAndNumber);
}

NODE_MODULE(nan_sketch, Init)

