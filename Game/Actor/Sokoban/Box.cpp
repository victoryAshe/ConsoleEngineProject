#include "Box.h"

Box::Box(const Vector2& newPosition)
	:super("B", newPosition, Color::Blue)
{
	// Set SortingOrder
	sortingOrder = 5;
}
