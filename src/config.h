#pragma once
#include <string>

struct Configuration {
#define OPTION(name, type, defaultValue)                        \
	static type name;                                           \
	static void (*handler_##name)(type & arg);                  \
	static const type &get##name() {                            \
		return name;                                            \
	}                                                           \
	static void set##name(type value) {                         \
		name = value;                                           \
		if(handler_##name)                                      \
			handler_##name(value);                              \
	}                                                           \
	static void on##name##Change(void (*handler)(type & arg)) { \
		handler_##name = handler;                               \
	}
#include "config_options.h"
};