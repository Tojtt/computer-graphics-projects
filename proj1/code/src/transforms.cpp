#include "transforms.h"

#include "CGL/matrix3x3.h"
#include "CGL/vector2D.h"
#include "CGL/vector3D.h"

namespace CGL {

Vector2D operator*(const Matrix3x3 &m, const Vector2D &v) {
	Vector3D mv = m * Vector3D(v.x, v.y, 1);
	return Vector2D(mv.x / mv.z, mv.y / mv.z);
}

Matrix3x3 translate(float dx, float dy) {
	// Part 3: Fill this in.
	Matrix3x3 trans_matrix;
	for (int i; i < 3; i++) {
		for (int j; j < 3; j++) {
			if (i == j) {
				trans_matrix(i, j) = 1.0;
			}
			else
				trans_matrix(i, j) = 0.0;
		}
	}
	trans_matrix(0, 2) = dx;
	trans_matrix(1, 2) = dy;

	return trans_matrix;
}

Matrix3x3 scale(float sx, float sy) {
	// Part 3: Fill this in.
	Matrix3x3 scale_matrix;
	for (int i; i < 3; i++) {
		for (int j; j < 3; j++) {
			if (i == j) {
				scale_matrix(i, j) = 1.0;
			}
			else
				scale_matrix(i, j) = 0.0;
		}
	}
	scale_matrix(0, 0) = sx;
	scale_matrix(1, 1) = sy;
	return scale_matrix;
}

// The input argument is in degrees counterclockwise
Matrix3x3 rotate(float deg) {
	// Part 3: Fill this in.
	Matrix3x3 rotate_matrix;
	for (int i; i < 3; i++) {
		for (int j; j < 3; j++) {
			if (i == j) {
				rotate_matrix(i, j) = 1.0;
			}
			else
				rotate_matrix(i, j) = 0.0;
		}
	}
	double rad = radians(deg);
	rotate_matrix(0, 0) = cos(rad);
	rotate_matrix(0, 1) = -sin(rad);
	rotate_matrix(1, 0) = sin(rad);
	rotate_matrix(1, 1) = cos(rad);
	return rotate_matrix;
}

}
