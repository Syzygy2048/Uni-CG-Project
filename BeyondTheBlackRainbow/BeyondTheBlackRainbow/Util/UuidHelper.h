#pragma once

#include <rpc.h>

UUID generateUuid(){
	UUID uuid;
	UuidCreate(&uuid);
	return uuid;
}