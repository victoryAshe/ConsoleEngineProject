#include "Target.h"

Target::Target(const Vector2& position)
	:super("T", position, Color::Green)
{
	// Set SortingOrder
	sortingOrder = 3;
}
