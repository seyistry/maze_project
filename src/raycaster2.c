#include <stdio.h>

#include "../inc/config.h"
#include "../inc/player.h"
#include "../inc/raycaster.h"

Vector3f findVerticalRayStepVector(Vector3f *ray) {
  Vector3f stepVector = HOMOGENEOUS_V3;
  if (ray->x < 0) { /* Ray is facing left */
    stepVector.x = -1 * WALL_SIZE;

  } else { /* Ray is facing right */
    stepVector.x = WALL_SIZE;
  }

  return homogeneousVectorScale(
      ray, vectorDotProduct(&stepVector, &stepVector) /
               MAKE_FLOAT_NONZERO(vectorDotProduct(&stepVector, ray)));
}

Vector3f findHorizontalRayStepVector(Vector3f *ray) {
  Vector3f stepVector = HOMOGENEOUS_V3;
  if (ray->y < 0) { /* Ray is facing up */
    stepVector.y = -1 * WALL_SIZE;

  } else { /* Ray is facing down */
    stepVector.y = WALL_SIZE;
  }

  return homogeneousVectorScale(
      ray, vectorDotProduct(&stepVector, &stepVector) /
               MAKE_FLOAT_NONZERO(vectorDotProduct(&stepVector, ray)));
}

Vector3f getTileCoordinateForVerticalRay(Vector3f *ray) {
  Vector3f pos = vectorAdd(&playerPos, ray);
  Vector3f coord;
  coord.x =
      (int)(pos.x + ((ray->x < 0) ? (-1 * RAY_EPS) : (RAY_EPS))) / WALL_SIZE;
  coord.y = (int)(pos.y + ((ray->y < 0) ? (-1 * EPS) : (EPS))) / WALL_SIZE;

  return coord;
}

Vector3f getTileCoordinateForHorizontalRay(Vector3f *ray) {
  Vector3f pos = vectorAdd(&playerPos, ray);
  Vector3f coord;
  coord.x = (int)(pos.x + ((ray->x < 0) ? (-1 * EPS) : EPS)) / WALL_SIZE;
  coord.y =
      (int)(pos.y + ((ray->y < 0) ? (-1 * RAY_EPS) : (RAY_EPS))) / WALL_SIZE;

  return coord;
}