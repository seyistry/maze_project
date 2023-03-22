#include "../inc/linalg.h"

Vector3f vectorAdd(Vector3f *vec1, Vector3f *vec2) {
  Vector3f retVec = {vec1->x + vec2->x, vec1->y + vec2->y, 1};
  return retVec;
}

Vector3f vectorSubtract(Vector3f *vec1, Vector3f *vec2) {
  Vector3f retVec = {vec1->x - vec2->x, vec1->y - vec2->y, 1};
  return retVec;
}

Vector3f homogeneousVectorScale(Vector3f *vec, float scalar) {
  Vector3f retVec = {vec->x * scalar, vec->y * scalar, 1};
  return retVec;
}

Vector3f normalizeVector(Vector3f *vec) {
  return homogeneousVectorScale(vec, 1.0f / homogeneousVectorMagnitude(vec));
}

Vector3f vectorProjection(Vector3f *vec1, Vector3f *vec2) {
  Vector3f pvec = normalizeVector(vec2);

  return homogeneousVectorScale(&pvec, vectorDotProduct(&pvec, vec1));
}
