#include "../inc/raycaster.h"

float calculateDrawHeight(float rayLength) {
  return distFromViewplane * WALL_SIZE / rayLength;
}

float getUndistortedRayLength(Vector3f *ray) {
  Vector3f undistortedRay;
  Vector3f proj = vectorProjection(ray, &viewplaneDir);
  undistortedRay = vectorSubtract(ray, &proj);

  return homogeneousVectorMagnitude(&undistortedRay);
}