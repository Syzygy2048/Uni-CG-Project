#pragma once

#include <rpc.h>

static UUID generateUuid(){
	UUID uuid;
	UuidCreate(&uuid);
	return uuid;
}