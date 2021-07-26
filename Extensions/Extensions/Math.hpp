#pragma once
#include "../pch.hpp"

// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/Core/Public/Math/UnrealMathUtility.h

#define PI 								(3.1415926535897932f)	/* Extra digits if needed: 3.1415926535897932384626433832795f */
#define SMALL_NUMBER					(1.e-8f)
#define KINDA_SMALL_NUMBER				(1.e-4f)
#define BIG_NUMBER						(3.4e+38f)
#define EULERS_NUMBER					(2.71828182845904523536f)
#define UE_GOLDEN_RATIO					(1.6180339887498948482045868343656381f)	/* Also known as divine proportion, golden mean, or golden section - related to the Fibonacci Sequence = (1 + sqrt(5)) / 2 */
#define FLOAT_NON_FRACTIONAL			(8388608.f) /* All single-precision floating point numbers greater than or equal to this have no fractional value. */

#define MAX_FLT							(3.402823466e+38F)
#define INV_PI							(0.31830988618f)
#define HALF_PI							(1.57079632679f)

#define UE_SQRT_2						(1.4142135623730950488016887242097f)
#define UE_SQRT_3						(1.7320508075688772935274463415059f)
#define UE_INV_SQRT_2					(0.70710678118654752440084436210485f)
#define UE_INV_SQRT_3					(0.57735026918962576450914878050196f)
#define UE_HALF_SQRT_2					(0.70710678118654752440084436210485f)
#define UE_HALF_SQRT_3					(0.86602540378443864676372317075294f)

#define DELTA							(0.00001f)
#define FLOAT_NORMAL_THRESH				(0.0001f)

#define THRESH_POINT_ON_PLANE			(0.10f)		/* Thickness of plane for front/back/inside test */
#define THRESH_POINT_ON_SIDE			(0.20f)		/* Thickness of polygon side's side-plane for point-inside/outside/on side test */
#define THRESH_POINTS_ARE_SAME			(0.00002f)	/* Two points are same if within this distance */
#define THRESH_POINTS_ARE_NEAR			(0.015f)	/* Two points are near if within this distance and can be combined if imprecise math is ok */
#define THRESH_NORMALS_ARE_SAME			(0.00002f)	/* Two normal points are same if within this distance */
#define THRESH_UVS_ARE_SAME			    (0.0009765625f)/* Two UV are same if within this threshold (1.0f/1024f) */
#define THRESH_VECTORS_ARE_NEAR			(0.0004f)	/* Two vectors are near if within this distance and can be combined if imprecise math is ok */
#define THRESH_SPLIT_POLY_WITH_PLANE	(0.25f)		/* A plane splits a polygon in half */
#define THRESH_SPLIT_POLY_PRECISELY		(0.01f)		/* A plane exactly splits a polygon */
#define THRESH_ZERO_NORM_SQUARED		(0.0001f)	/* Size of a unit normal that is considered "zero", squared */
#define THRESH_NORMALS_ARE_PARALLEL		(0.999845f)	/* Two unit vectors are parallel if abs(A dot B) is greater than or equal to this. This is roughly cosine(1.0 degrees). */
#define THRESH_NORMALS_ARE_ORTHOGONAL	(0.017455f)	/* Two unit vectors are orthogonal (perpendicular) if abs(A dot B) is less than or equal this. This is roughly cosine(89.0 degrees). */
#define THRESH_VECTOR_NORMALIZED		(0.01f)		/** Allowed error for a normalized vector (against squared magnitude) */
#define THRESH_QUAT_NORMALIZED			(0.01f)		/** Allowed error for a normalized quaternion (against squared magnitude) */

#define Rotation360						(65535)
#define Rotation180						(32768)
#define RotationToRadians				(PI / Rotation180)

// Credit to @FrancesElMute/Aberinkula/Megasplat for the rotate function, was pulling my hair out tyring to figure it out on my own.

namespace Math
{
	float GetDistance(const struct FVector& from, const struct FVector& to, const float scaling);
	void SinCos(float* scalarSin, float* scalarCos, float value);
	struct FVector Rotate(struct FVector point, const struct FRotator& rotation, const struct FVector& location);
}

class Vector
{
public:
	float X, Y, Z;

public:
	Vector();
	Vector(const float xyz);
	Vector(const float x, const float y, const float z);
	Vector(const Vector& other);
	Vector(const struct FVector& other);
	~Vector();

public:
	struct FVector UnrealVector();

public:
	Vector operator+=(const Vector& other);
	Vector operator-=(const Vector& other);
	Vector operator*=(const Vector& other);
	Vector operator/=(const Vector& other);
	Vector operator+=(const float other);
	Vector operator-=(const float other);
	Vector operator*=(const float other);
	Vector operator/=(const float other);
	Vector operator=(const Vector& other);
	Vector operator=(const struct FVector& other);
	Vector operator=(const float other);
	bool operator==(const Vector& other);
	bool operator==(const struct FVector& other);
	bool operator!=(const Vector& other);
	bool operator!=(const struct FVector& other);

public:
	void Erase();
	Vector Copy() const;
	float Magnitude() const;
	void Normalize();
	class Rotator GetRotator() const;
	Vector GetNormalize() const;
	Vector Dot(const Vector& other) const;
	Vector Cross(const Vector& other) const;
	Vector Lerp(const Vector& other, const float percentage) const;
};

inline Vector operator+(const Vector& a, const Vector& b) { return Vector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
inline Vector operator-(const Vector& a, const Vector& b) { return Vector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline Vector operator*(const Vector& a, const Vector& b) { return Vector(a.X * b.X, a.Y * b.Y, a.Z * b.Z); }
inline Vector operator/(const Vector& a, const Vector& b) { return Vector(a.X / b.X, a.Y / b.Y, a.Z / b.Z); }
inline Vector operator+(const Vector& a, const float b) { return Vector(a.X + b, a.Y + b, a.Z + b); }
inline Vector operator-(const Vector& a, const float b) { return Vector(a.X - b, a.Y - b, a.Z - b); }
inline Vector operator*(const Vector& a, const float b) { return Vector(a.X * b, a.Y * b, a.Z * b); }
inline Vector operator/(const Vector& a, const float b) { return Vector(a.X / b, a.Y / b, a.Z / b); }

class Vector2D
{
public:
	float X, Y;

public:
	Vector2D();
	Vector2D(const float xy);
	Vector2D(const float x, const float y);
	Vector2D(const Vector2D& other);
	Vector2D(const struct FVector2D& other);
	~Vector2D();

public:
	struct FVector2D UnrealVector();

public:
	Vector2D operator+=(const Vector2D& other);
	Vector2D operator-=(const Vector2D& other);
	Vector2D operator*=(const Vector2D& other);
	Vector2D operator/=(const Vector2D& other);
	Vector2D operator+=(const float other);
	Vector2D operator-=(const float other);
	Vector2D operator*=(const float other);
	Vector2D operator/=(const float other);
	Vector2D operator=(const Vector2D& other);
	Vector2D operator=(const struct FVector2D& other);
	Vector2D operator=(const float other);
	bool operator==(const Vector2D& other);
	bool operator==(const struct FVector2D& other);
	bool operator!=(const Vector2D& other);
	bool operator!=(const struct FVector2D& other);

public:
	void Erase();
	Vector2D Copy() const;
};

inline Vector2D operator+(const Vector2D& a, const Vector2D& b) { return Vector2D(a.X + b.X, a.Y + b.Y); }
inline Vector2D operator-(const Vector2D& a, const Vector2D& b) { return Vector2D(a.X - b.X, a.Y - b.Y); }
inline Vector2D operator*(const Vector2D& a, const Vector2D& b) { return Vector2D(a.X * b.X, a.Y * b.Y); }
inline Vector2D operator/(const Vector2D& a, const Vector2D& b) { return Vector2D(a.X / b.X, a.Y / b.Y); }
inline Vector2D operator+(const Vector2D& a, const float b) { return Vector2D(a.X + b, a.Y + b); }
inline Vector2D operator-(const Vector2D& a, const float b) { return Vector2D(a.X - b, a.Y - b); }
inline Vector2D operator*(const Vector2D& a, const float b) { return Vector2D(a.X * b, a.Y * b); }
inline Vector2D operator/(const Vector2D& a, const float b) { return Vector2D(a.X / b, a.Y / b); }

class Rotator
{
public:
	int32_t Pitch, Yaw, Roll;

public:
	Rotator();
	Rotator(const int32_t pyr);
	Rotator(const int32_t pitch, const int32_t yaw, const int32_t roll);
	Rotator(const Rotator& other);
	Rotator(const struct FRotator& other);
	~Rotator();

public:
	struct FRotator UnrealRotator();

public:
	Rotator operator+=(const Rotator& other);
	Rotator operator-=(const Rotator& other);
	Rotator operator*=(const Rotator& other);
	Rotator operator/=(const Rotator& other);
	Rotator operator+=(const int32_t other);
	Rotator operator-=(const int32_t other);
	Rotator operator*=(const int32_t other);
	Rotator operator/=(const int32_t other);
	Rotator operator=(const Rotator& other);
	Rotator operator=(const struct FRotator& other);
	Rotator operator=(const int32_t other);
	bool operator==(const Rotator& other);
	bool operator==(const struct FRotator& other);
	bool operator!=(const Rotator& other);
	bool operator!=(const struct FRotator& other);

public:
	void Erase();
	Rotator Copy() const;
	float ClampAxis(float a);
	void Normalize();
	float NormalizeAxis(float a);
	Rotator GetNormalize() const;
	Vector GetVector() const;
	Vector Rotate(Vector other) const;
};

inline Rotator operator+(const Rotator& a, const Rotator& b) { return Rotator(a.Pitch + b.Pitch, a.Yaw + b.Yaw, a.Roll + b.Roll); }
inline Rotator operator-(const Rotator& a, const Rotator& b) { return Rotator(a.Pitch - b.Pitch, a.Yaw - b.Yaw, a.Roll - b.Roll); }
inline Rotator operator*(const Rotator& a, const Rotator& b) { return Rotator(a.Pitch * b.Pitch, a.Yaw * b.Yaw, a.Roll * b.Roll); }
inline Rotator operator/(const Rotator& a, const Rotator& b) { return Rotator(a.Pitch / b.Pitch, a.Yaw / b.Yaw, a.Roll / b.Roll); }
inline Rotator operator+(const Rotator& a, const int32_t b) { return Rotator(a.Pitch + b, a.Yaw + b, a.Roll + b); }
inline Rotator operator-(const Rotator& a, const int32_t b) { return Rotator(a.Pitch - b, a.Yaw - b, a.Roll - b); }
inline Rotator operator*(const Rotator& a, const int32_t b) { return Rotator(a.Pitch * b, a.Yaw * b, a.Roll * b); }
inline Rotator operator/(const Rotator& a, const int32_t b) { return Rotator(a.Pitch / b, a.Yaw / b, a.Roll / b); }

class Quat
{
public:
	float X, Y, Z, W;

public:
	Quat();
	Quat(float xyzw);
	Quat(float x, float y, float z, float w);
	Quat(const Quat& other);
	Quat(const struct FQuat& other);
	~Quat();

public:
	struct FQuat UnrealQuat();

public:
	Quat operator+=(const Quat& other);
	Quat operator-=(const Quat& other);
	Quat operator*=(const Quat& other);
	Quat operator/=(const Quat& other);
	Quat operator+=(const float other);
	Quat operator-=(const float other);
	Quat operator*=(const float other);
	Quat operator/=(const float other);
	Quat operator=(const Quat& other);
	Quat operator=(const struct FQuat& other);
	Quat operator=(const float other);
	bool operator==(const Quat& other);
	bool operator==(const struct FQuat& other);
	bool operator!=(const Quat& other);
	bool operator!=(const struct FQuat& other);

public:
	void Erase();
	Quat Copy() const;
	bool Equals(const Quat& q, float tolerance = SMALL_NUMBER) const;
	bool IsIdentity(float tolerance = SMALL_NUMBER) const;
	bool IsNormalized() const;
	float Size() const;
	float SizeSquared() const;
	float GetAngle() const;
	Vector GetAxisX() const;
	Vector GetAxisY() const;
	Vector GetAxisZ() const;
	Vector GetRotationAxis() const;
	Quat GetInverse() const;
	void Normalize(float tolerance = SMALL_NUMBER);
	Quat GetNormalize(float tolerance = SMALL_NUMBER) const;
	Rotator GetRotator() const;
	Vector Rotate(const Vector& other) const;
};

inline Quat operator+(const Quat& a, const Quat& b) { return Quat(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W); }
inline Quat operator-(const Quat& a, const Quat& b) { return Quat(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W); }
inline Quat operator*(const Quat& a, const Quat& b) { return Quat(a.X * b.X, a.Y * b.Y, a.Z * b.Z, a.W * b.W); }
inline Quat operator/(const Quat& a, const Quat& b) { return Quat(a.X / b.X, a.Y / b.Y, a.Z / b.Z, a.W / b.W); }
inline Quat operator+(const Quat& a, const float b) { return Quat(a.X + b, a.Y + b, a.Z + b, a.W + b); }
inline Quat operator-(const Quat& a, const float b) { return Quat(a.X - b, a.Y - b, a.Z - b, a.W - b); }
inline Quat operator*(const Quat& a, const float b) { return Quat(a.X * b, a.Y * b, a.Z * b, a.W * b); }
inline Quat operator/(const Quat& a, const float b) { return Quat(a.X / b, a.Y / b, a.Z / b, a.W / b); }