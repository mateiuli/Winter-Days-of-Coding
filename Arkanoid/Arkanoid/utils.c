#include "utils.h"
#include <math.h>

int is_intersecting(sfRectangleShape *rectangle, sfCircleShape *ball)
{
	if (!rectangle || !ball)
		return 0;

	sfVector2f r_pos	= sfRectangleShape_getPosition(rectangle);
	sfVector2f r_size	= sfRectangleShape_getSize(rectangle);

	sfVector2f b_pos	= sfCircleShape_getPosition(ball);
	float b_radius		= sfCircleShape_getRadius(ball);

	float r_left	= r_pos.x;
	float r_right	= r_pos.x + r_size.x;
	float r_top		= r_pos.y;
	float r_bottom	= r_pos.y + r_size.y;
	
	float b_left	= b_pos.x - b_radius;
	float b_right	= b_pos.x + b_radius;
	float b_top		= b_pos.y - b_radius;
	float b_bottom	= b_pos.y + b_radius;

	return r_right >= b_left && r_left <= b_right &&
		r_bottom >= b_top && r_top <= b_bottom;
}

collision_t get_collision(sfRectangleShape *rectangle, sfCircleShape *ball)
{
	sfVector2f r_pos = sfRectangleShape_getPosition(rectangle);
	sfVector2f r_size = sfRectangleShape_getSize(rectangle);

	sfVector2f b_pos = sfCircleShape_getPosition(ball);
	float b_radius = sfCircleShape_getRadius(ball);

	float r_left	= r_pos.x;
	float r_right	= r_pos.x + r_size.x;
	float r_top		= r_pos.y;
	float r_bottom	= r_pos.y + r_size.y;

	float b_left	= b_pos.x - b_radius;
	float b_right	= b_pos.x + b_radius;
	float b_top		= b_pos.y - b_radius;
	float b_bottom	= b_pos.y + b_radius;

	// If there's no intersection, get out of the function.
	if (!is_intersecting(rectangle, ball)) return NO_COLLISION;

	// Let's calculate how much the ball intersects the brick
	// in every direction.
	float overlapLeft	= b_right - r_left;
	float overlapRight	= r_left - b_right;
	float overlapTop	= b_bottom - r_top;
	float overlapBottom = r_top - b_bottom;

	// If the magnitude of the left overlap is smaller than the
	// right one we can safely assume the ball hit the brick
	// from the left.
	int ballFromLeft = fabs(overlapLeft) < fabs(overlapRight);

	// We can apply the same idea for top/bottom collisions.
	int ballFromTop = fabs(overlapTop) < fabs(overlapBottom);

	// Let's store the minimum overlaps for the X and Y axes.
	float minOverlapX = ballFromLeft ? overlapLeft : overlapRight;
	float minOverlapY = ballFromTop ? overlapTop : overlapBottom;

	// If the magnitude of the X overlap is less than the magnitude
	// of the Y overlap, we can safely assume the ball hit the brick
	// horizontally - otherwise, the ball hit the brick vertically.

	// Then, upon our assumptions, we change either the X or Y velocity
	// of the ball, creating a "realistic" response for the collision.
	if (fabs(minOverlapX) < fabs(minOverlapY))
		if (ballFromLeft)
			return FROM_LEFT;
		else
			return FROM_RIGHT;
	else
		if (ballFromTop)
			return FROM_TOP;
		else
			return FROM_BOTTOM;
	
	return NO_COLLISION;
}
