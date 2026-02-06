#include "Wall.h"

Wall::Wall(const Vector2& position)
	:super("#", position)
{
	// Set SortingOrder.
	// 벽은 어차피 위에 못 올라가도록 설정할 거니까 
	// sortingOrder 의미 없음.
	sortingOrder = 0;
}
