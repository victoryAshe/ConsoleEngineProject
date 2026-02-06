#include "Ground.h"

Ground::Ground(const Vector2& position)
	:super(" ", position)
{
	// Set SortingOrder
	sortingOrder = 0;
}
