#pragma once

static enum EventTrigger {
	EVENT,
	RAYTRACE_HIT,
	TRIGGER_ENTER,
	TRIGGER_EXIT,
};

static enum EventIdentifier {
	DOOR_TRIGGER,
	OPEN_DOOR,
};