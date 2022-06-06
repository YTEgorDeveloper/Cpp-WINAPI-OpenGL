#pragma once

#include <cmath>
#include <vector>

#define PI 3.14159265f

/*
  - Geometry math header
  - Basic vector, quaternion, matrix math
  - By: EgorDeveloper
  -
  - Geometry.h:
  - Contains realisations for Vector2, Vector3, Quaternion, Matrix
*/

typedef struct Quaternion {
	float x, y, z, w;

	Quaternion() { x = 0; y = 0; z = 0; w = 1; }
	Quaternion(float X, float Y, float Z, float W) { x = X; y = Y; z = Z; w = W; }

	Quaternion operator*(const Quaternion& second) {
		return Quaternion (
			w * second.w - x * second.x - y * second.y - z * second.z,
			w * second.x + x * second.w + y * second.z - z * second.y,
			w * second.y - x * second.z + y * second.w + z * second.x,
			w * second.z + x * second.y - y * second.x + z * second.w
		);
	}

	///<summary>
	///Returns quaternion from given axis angles in radians
	///</summary>
	static Quaternion EulerAngles(const float& angleX, const float& angleY, const float& angleZ) {
		float cy = cosf(angleZ / 2.0f), sy = sinf(angleZ / 2.0f);
		float cp = cosf(angleY / 2.0f), sp = sinf(angleY / 2.0f);
		float cr = cosf(angleX / 2.0f), sr = sinf(angleX / 2.0f);

		return Quaternion(
			sr * cp * cy - cr * sp * sy,
			cr * sp * cy + sr * cp * sy,
			cr * cp * sy - sr * sp * cy,
			cr * cp * cy + sr * sp * sy
		);
	}
	///<summary>
	///Converts Radians to Degrees
	///</summary>
	static float Rad2Deg(const float& radians) { return radians * 57.2957795f; }
	///<summary>
	///Converts Degrees to Radians
	///</summary>
	static float Deg2Rad(const float& degrees) { return degrees / 57.2957795f; }

} Quaternion;

typedef struct Vector2 {
	float x, y;

	Vector2() { x = 0; y = 0; }
	Vector2(float X) { x = X; y = 0; }
	Vector2(float X, float Y) { x = X; y = Y; }

	Vector2 operator*(const float& value) { return Vector2(x * value, y * value); }
	Vector2 operator/(const float& value) { return Vector2(x / value, y / value); }
	Vector2 operator+(const Vector2& second) { return Vector2(x + second.x, y + second.y); }
	Vector2 operator-(const Vector2& second) { return Vector2(x - second.x, y - second.y); }

	///<summary>
	///Returns dot product between two given Vector2 points
	///</summary>
	static float Dot(Vector2 a, Vector2 b) { return a.x * b.x + a.y * b.y; }
	///<summary>
	///Returns distance from (0, 0) to given Vector2 point
	///</summary>
	static float Distance(Vector2 a) { return sqrtf(a.x * a.x + a.y * a.y); }
	///<summary>
	///Returns distance between two given Vector2 points
	///</summary>
	static float Distance(Vector2 a, Vector2 b) { return Distance(a - b); }
	///<summary>
	///Returns Vector2, which is linear interpolated between two given Vector2 points with parameter t = [0; 1]
	///</summary>
	static Vector2 Lerp(Vector2 a, Vector2 b, float t) { return a + (b - a) * t; }
	///<summary>
	///Returns new Vector2, which is a given Vector3 with length of 1
	///</summary>
	static Vector2 Normal(Vector2 vector) { float d = Distance(vector); if (d == 0) { return vector; } return vector / d; }
	///<summary>
	///Returns distance from (0, 0) to current Vector2 instance
	///</summary>
	float Length(void) const { return Distance(*this); }
	///<summary>
	///Applies linear interpolation between given Vector2 and current Vector2 instance points with parameter t = [0; 1]
	///</summary>
	void Lerp(Vector2 b, float t) { *this = Lerp(*this, b, t); }
	///<summary>
	///Returns new Vector2, which is a current Vector2 instance with length of 1
	///</summary>
	Vector2 Normal(void) const { return Normal(*this); }
	///<summary>
	///Applies normalisation to current Vector2 instance
	///</summary>
	void Normalise(void) { *this = Normal(*this); }
} Vector2;

typedef struct Vector3 {
	float x, y, z;

	Vector3() { x = 0; y = 0; z = 0; }
	Vector3(float X) { x = X; y = 0; z = 0; }
	Vector3(float X, float Y) { x = X; y = Y; z = 0; }
	Vector3(float X, float Y, float Z) { x = X; y = Y; z = Z; }

	Vector3 operator*(const float& value) { return Vector3(x * value, y * value, z * value); }
	Vector3 operator/(const float& value) { return Vector3(x / value, y / value, z / value); }
	Vector3 operator+(const Vector3& second) { return Vector3(x + second.x, y + second.y, z + second.z); }
	Vector3 operator-(const Vector3& second) { return Vector3(x - second.x, y - second.y, z - second.z); }

	///<summary>
	///Returns dot product between two given Vector3 points
	///</summary>
	static float Dot(const Vector3& a, const Vector3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
	///<summary>
	///Returns cross product between two given Vector3 points
	///</summary>
	static Vector3 Cross(const Vector3& a, const Vector3& b) { return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
	///<summary>
	///Returns multiplication of Vector3 pairwise components (a.x * b.x, a.y * b.y, a.z * b.z)
	///</summary>
	static Vector3 MultiplyPairwise(const Vector3& a, const Vector3& b) { return Vector3(a.x * b.x, a.y * b.y, a.z * b.z); }
	///<summary>
	///Returns distance from (0, 0, 0) to given Vector3 point
	///</summary>
	static float Distance(const Vector3& a) { return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z); }
	///<summary>
	///Returns distance between two given Vector3 points
	///</summary>
	static float Distance(Vector3 a, Vector3 b) { return Distance(a - b); }
	///<summary>
	///Returns Vector3, which is linear interpolated between two given Vector3 points with parameter t = [0; 1]
	///</summary>
	static Vector3 Lerp(Vector3 a, Vector3 b, float t) { return a + (b - a) * t; }
	///<summary>
	///Returns new Vector3, which is a given Vector3 with length of 1
	///</summary>
	static Vector3 Normal(Vector3 vector) { float d = Distance(vector); if (d == 0) { return vector; } return vector / d; }
	///<summary>
	///Returns angle in ratio [-1; 1] between two given Vector3 points (use acos to convert ratio to radians)
	///</summary>
	static float Angle(const Vector3& a, const Vector3& b) {
		float magnitude = Distance(a) + Distance(b);
		if (magnitude == 0) { return 0; }
		return Dot(a, b) / magnitude;
	}
	///<summary>
	///Returns vector multiplied by given Quaternion
	///</summary>
	Vector3 Rotation(const Quaternion& rotation) const {
		Vector3 qv(rotation.x, rotation.y, rotation.z);
		Vector3 cv(*this);
		return (qv * 2.0f * Dot(qv, cv)) + (cv * (rotation.w * rotation.w - Dot(qv, qv))) + (Cross(qv, cv) * 2.0f * rotation.w);
	}
	///<summary>
	///Multiplies current vector instance by given Quaternion
	///</summary>
	void Rotatate(const Quaternion& rotation) {
		Vector3 qv(rotation.x, rotation.y, rotation.z);
		*this = (qv * 2.0f * Dot(qv, *this)) + (*this * (rotation.w * rotation.w - Dot(qv, qv))) + (Cross(qv, *this) * 2.0f * rotation.w);
	}
	///<summary>
	///Returns quaternion from given axis angles in radians
	///</summary>
	static Quaternion EulerAngles(const Vector3& angles) {
		float cy = cosf(angles.z), sy = sinf(angles.z);
		float cp = cosf(angles.y), sp = sinf(angles.y);
		float cr = cosf(angles.x), sr = sinf(angles.x);

		return Quaternion(
			sr * cp * cy - cr * sp * sy,
			cr * sp * cy + sr * cp * sy,
			cr * cp * sy - sr * sp * cy,
			cr * cp * cy + sr * sp * sy
		);
	}
	///<summary>
	///Converts quaternion to Euler angles
	///</summary>
	static Vector3 EulerAngles(const Quaternion& q) {
		Vector3 eulerangles;

		float sinp = 2 * (q.w * q.y - q.z * q.x);
		float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
		float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
		float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
		float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);

		eulerangles.y = atan2f(sinr_cosp, cosr_cosp);
		eulerangles.x = atan2f(siny_cosp, cosy_cosp);
		eulerangles.y = (fabs(sinp) >= 1) ? (copysignf(1.5707963f, sinp)) : (std::asinf(sinp)); // use 90 degrees if out of range
		
		return eulerangles / 2;
	}
	///<summary>
	///Multiplies pairwise components of current Vector3 instance with given Vector3 (a.x * b.x, a.y * b.y, a.z * b.z)
	///</summary>
	void MultiplyPairwise(const Vector3& b) { *this = MultiplyPairwise(*this, b); }
	///<summary>
	///Returns distance from (0, 0, 0) to current Vector3 instance
	///</summary>
	float Length(void) const { return Distance(*this); }
	///<summary>
	///Applies linear interpolation between given Vector3 and current Vector3 instance points with parameter t = [0; 1]
	///</summary>
	void Lerp(const Vector3& b, float t) { *this = Lerp(*this, b, t); }
	///<summary>
	///Returns new Vector3, which is a current Vector3 instance with length of 1
	///</summary>
	Vector3 Normal(void) const { return Normal(*this); }
	///<summary>
	///Applies normalisation to current Vector3 instance
	///</summary>
	void Normalise(void) { *this = Normal(*this); }
	///<summary>
	///Returns list of Vector3 points that form a circle
	///</summary>
	static std::vector<Vector3> CirclePoints(const unsigned& n, const float& radius, const Vector3& position, const Quaternion& rotation) {
		std::vector<Vector3> points;
		if (n < 3) { return points; }

		float angleDelta = 6.2831853f / n;
		float currentAngle = 0;

		for (unsigned i = 0; i < n; ++i) {
			points.push_back(Vector3(cosf(currentAngle), 0, sinf(currentAngle)).Rotation(rotation) * radius + position);
			currentAngle += angleDelta;
		}

		return points;
	}
} Vector3;

typedef struct Matrix {
	///<summary>
	///Returns Determinant of 2x2 Matrix
	///</summary>
	static float Determinat2(float a, float b, float c, float d) { return a * d - b * c; }
	///<summary>
	///Returns Determinant of 2x2 Matrix
	///</summary>
	static float Determinat2(Vector2 a, Vector2 b) { return a.x * b.y - a.y * b.x; }
	///<summary>
	///Returns Determinant of 3x3 Matrix
	///</summary>
	static float Determinat3(const Vector3& a, const Vector3& b, const Vector3& c) {
		return
			a.x * Determinat2(b.y, b.z, c.y, c.z) -
			a.y * Determinat2(b.x, b.z, c.x, c.z) +
			a.z * Determinat2(b.x, b.y, c.x, c.y);
	}
} Matrix;

class Transform {
private:

public:
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	Transform() { position = Vector3(); rotation = Quaternion(); scale = Vector3(1, 1, 1); }
	Transform(const Vector3& nPos) { position = nPos; rotation = Quaternion(); scale = Vector3(1, 1, 1); }
	Transform(const Vector3& nPos, const Quaternion& nRot) { position = nPos; rotation = nRot; scale = Vector3(1, 1, 1); }
	Transform(const Vector3& nPos, const Quaternion& nRot, const Vector3& nScale) { position = nPos; rotation = nRot; scale = nScale; }
};