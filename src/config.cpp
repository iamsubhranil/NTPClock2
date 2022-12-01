#include "config.h"

#define OPTION(name, type, defaultValue)                              \
	type Configuration::name                          = defaultValue; \
	void (*Configuration::handler_##name)(type & arg) = nullptr;
#include "config_options.h"