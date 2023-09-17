#pragma once

#include <WString.h>
#include <ArduinoJson.h>
#include "time2.h"

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

#define CUSTOM_TYPE(type)									\
template <>													\
struct Converter<type> {									\
	static void toJson(const type& t, JsonVariant dst) { 	\
		dst.set(t.toString()); 								\
	} 														\
															\
	static Time fromJson(JsonVariantConst src) { 			\
		return type::fromString(src.as<const char*>()); 	\
	} 														\
															\
	static bool checkJson(JsonVariantConst src, type &t) {	\
		return type::validate(src.as<const char*>()); 		\
	}														\
};

CUSTOM_TYPE(Time)

#undef CUSTOM_TYPE

ARDUINOJSON_END_PUBLIC_NAMESPACE

struct Configuration {
#define OPTION(name, type, defaultValue, displayName)           \
	static type name;                                           \
	static void (*handler_##name)(type & arg);                  \
	static const type &get##name() {                            \
		return name;                                            \
	}                                                           \
	static void set##name(type value) {                         \
		if(value == name)                                       \
			return;                                             \
		name = value;                                           \
		if(handler_##name)                                      \
			handler_##name(value);                              \
		store();                                                \
	}                                                           \
	static void on##name##Change(void (*handler)(type & arg)) { \
		handler_##name = handler;                               \
	}
#include "config_options.h"

	static void load();
	static void store();
};