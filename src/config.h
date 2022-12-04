#pragma once

#include <WString.h>

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