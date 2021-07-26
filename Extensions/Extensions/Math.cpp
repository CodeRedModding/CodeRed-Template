#include "Math.hpp"

namespace Math
{
	float GetDistance(const struct FVector& from, const struct FVector& to, const float scaling)
	{
		float distance = std::sqrt(std::pow(to.X - from.X, 2) + std::pow(to.Y - from.Y, 2) + std::pow(to.Z - from.Z, 2));
		return distance / scaling;
	}

	void SinCos(float* scalarSin, float* scalarCos, float value)
	{
		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.

		float quotient = (INV_PI * 0.5f) * value;

		if (value >= 0.0f)
		{
			quotient = (float)((int32_t)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int32_t)(quotient - 0.5f));
		}

		float y = value - (2.0f*PI)*quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).

		float sign;

		if (y > HALF_PI)
		{
			y = PI - y;
			sign = -1.0f;
		}
		else if (y < -HALF_PI)
		{
			y = -PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		// 11-degree minimax approximation
		*scalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*scalarCos = sign*p;
	}

	struct FVector Rotate(struct FVector point, const struct FRotator& rotation, const struct FVector& location)
	{
		double pitch = (double)rotation.Pitch / Rotation180 * PI;
		double yaw = (double)rotation.Yaw / Rotation180 * PI;
		double roll = (double)rotation.Roll / Rotation180 * PI;

		float sz = sin(pitch);
		float cz = cos(pitch);
		float sy = sin(-yaw);
		float cy = cos(-yaw);
		float sx = sin(roll);
		float cx = cos(roll);

		point = FVector{ point.X, point.Y * cx - point.Z * sx, point.Y * sx + point.Z * cx }; // Roll
		point = FVector{ point.X * cz - point.Y * sz, point.X * sz + point.Y * cz, point.Z }; // Pitch
		point = FVector{ point.X * cy + point.Z * sy, point.Y, -point.X * sy + point.Z * cy }; // Yaw

		float tmp = point.Z;
		point.Z = point.Y;
		point.Y = tmp;

		point.X += location.X;
		point.Y += location.Y;
		point.Z += location.Z;

		return point;
	}
}

Vector::Vector() : X(0.f), Y(0.f), Z(0.f) { }

Vector::Vector(const float xyz) : X(xyz), Y(xyz), Z(xyz) { }

Vector::Vector(const float x, const float y, const float z) : X(x), Y(y), Z(z) { }

Vector::Vector(const Vector& other) : X(other.X), Y(other.Y), Z(other.Z) { }

Vector::Vector(const struct FVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }

Vector::~Vector() { }

FVector Vector::UnrealVector()
{
	return FVector { X, Y, Z };
}

Vector Vector::operator+=(const Vector& other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;
	return *this;
}

Vector Vector::operator-=(const Vector& other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;
	return *this;
}

Vector Vector::operator*=(const Vector& other)
{
	X *= other.X;
	Y *= other.Y;
	Z *= other.Z;
	return *this;
}

Vector Vector::operator/=(const Vector& other)
{
	X /= other.X;
	Y /= other.Y;
	Z /= other.Z;
	return *this;
}

Vector Vector::operator+=(const float other)
{
	X += other;
	Y += other;
	Z += other;
	return *this;
}

Vector Vector::operator-=(const float other)
{
	X -= other;
	Y -= other;
	Z -= other;
	return *this;
}

Vector Vector::operator*=(const float other)
{
	X *= other;
	Y *= other;
	Z *= other;
	return *this;
}

Vector Vector::operator/=(const float other)
{
	X /= other;
	Y /= other;
	Z /= other;
	return *this;
}

Vector Vector::operator=(const Vector& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	return *this;
}

Vector Vector::operator=(const struct FVector& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	return *this;
}

Vector Vector::operator=(const float other)
{
	X = other;
	Y = other;
	Z = other;
	return *this;
}

bool Vector::operator==(const Vector& other)
{
	if (X != other.X
		|| Y != other.Y
		|| Z != other.Z)
	{
		return false;
	}

	return true;
}

bool Vector::operator==(const struct FVector& other)
{
	if (X != other.X
		|| Y != other.Y
		|| Z != other.Z)
	{
		return false;
	}

	return true;
}

bool Vector::operator!=(const Vector& other)
{
	if (X != other.X
		|| Y != other.Y
		|| Z != other.Z)
	{
		return true;
	}

	return false;
}

bool Vector::operator!=(const struct FVector& other)
{
	if (X != other.X
		|| Y != other.Y
		|| Z != other.Z)
	{
		return true;
	}

	return false;
}

void Vector::Erase()
{
	X = 0.f;
	Y = 0.f;
	Z = 0.f;
}

Vector Vector::Copy() const
{
	return Vector(X, Y, Z);
}

float Vector::Magnitude() const
{
	return std::sqrtf(X * X + Y * Y + Z * Z);
}

void Vector::Normalize()
{
	float magnitude = Magnitude();
	X = X / magnitude;
	Y = Y / magnitude;
	Z = Z / magnitude;
}

Vector Vector::GetNormalize() const
{
	float magnitude = Magnitude();
	Vector mutableThis = *this;
	mutableThis.X = X / magnitude;
	mutableThis.Y = Y / magnitude;
	mutableThis.Z = Z / magnitude;
	return mutableThis;
}

class Rotator Vector::GetRotator() const
{
	Rotator result;
	result.Yaw = atan2(Y, X) * (180.f / PI);
	result.Pitch = atan2(Z, sqrt(X * X + Y * Y)) * (180.f / PI);
	result.Roll = 0.f;
	return result;
}

Vector Vector::Dot(const Vector& other) const
{
	return Vector(X * other.X + Y * other.Y + Z * other.Z);
}

Vector Vector::Cross(const Vector& other) const
{
	float x = Y * other.Z - Z * other.Y;
	float y = Z * other.X - X * other.Z;
	float z = X * other.Y - Y * other.X;
	return Vector(x, y, z);
}

Vector Vector::Lerp(const Vector& other, const float percentage) const
{
	Vector mutableThis = *this;
	return Vector(mutableThis * percentage + (other * (1.f - percentage)));
}

Vector2D::Vector2D() : X(0.f), Y(0.f) { }

Vector2D::Vector2D(const float xy) : X(xy), Y(xy) { }

Vector2D::Vector2D(const float x, const float y) : X(x), Y(y) { }

Vector2D::Vector2D(const Vector2D& other) : X(other.X), Y(other.Y) { }

Vector2D::Vector2D(const struct FVector2D& other) : X(other.X), Y(other.Y) { }

Vector2D::~Vector2D() { }

FVector2D Vector2D::UnrealVector()
{
	return FVector2D { X, Y };
}

Vector2D Vector2D::operator+=(const Vector2D& other)
{
	X += other.X;
	Y += other.Y;
	return *this;
}

Vector2D Vector2D::operator-=(const Vector2D& other)
{
	X -= other.X;
	Y -= other.Y;
	return *this;
}

Vector2D Vector2D::operator*=(const Vector2D& other)
{
	X *= other.X;
	Y *= other.Y;
	return *this;
}

Vector2D Vector2D::operator/=(const Vector2D& other)
{
	X /= other.X;
	Y /= other.Y;
	return *this;
}

Vector2D Vector2D::operator+=(const float other)
{
	X += other;
	Y += other;
	return *this;
}

Vector2D Vector2D::operator-=(const float other)
{
	X -= other;
	Y -= other;
	return *this;
}

Vector2D Vector2D::operator*=(const float other)
{
	X *= other;
	Y *= other;
	return *this;
}

Vector2D Vector2D::operator/=(const float other)
{
	X /= other;
	Y /= other;
	return *this;
}

Vector2D Vector2D::operator=(const Vector2D& other)
{
	X = other.X;
	Y = other.Y;
	return *this;
}

Vector2D Vector2D::operator=(const struct FVector2D& other)
{
	X = other.X;
	Y = other.Y;
	return *this;
}

Vector2D Vector2D::operator=(const float other)
{
	X = other;
	Y = other;
	return *this;
}

bool Vector2D::operator==(const Vector2D& other)
{
	if (X != other.X
		|| Y != other.Y)
	{
		return false;
	}

	return true;
}

bool Vector2D::operator==(const struct FVector2D& other)
{
	if (X != other.X
		|| Y != other.Y)
	{
		return false;
	}

	return true;
}

bool Vector2D::operator!=(const Vector2D& other)
{
	if (X != other.X
		|| Y != other.Y)
	{
		return true;
	}

	return false;
}

bool Vector2D::operator!=(const struct FVector2D& other)
{
	if (X != other.X
		|| Y != other.Y)
	{
		return true;
	}

	return false;
}

void Vector2D::Erase()
{
	X = 0.f;
	Y = 0.f;
}

Vector2D Vector2D::Copy() const
{
	return Vector2D(X, Y);
}

Rotator::Rotator() : Pitch(0), Yaw(0), Roll(0) { }

Rotator::Rotator(const int32_t pyr) : Pitch(pyr), Yaw(pyr), Roll(pyr) { }

Rotator::Rotator(const int32_t pitch, const int32_t yaw, const int32_t roll) : Pitch(pitch), Yaw(yaw), Roll(roll) { }

Rotator::Rotator(const Rotator& other) : Pitch(other.Pitch), Yaw(other.Yaw), Roll(other.Roll) { }

Rotator::Rotator(const struct FRotator& other) : Pitch(other.Pitch), Yaw(other.Yaw), Roll(other.Roll) { }

Rotator::~Rotator() { }

struct FRotator Rotator::UnrealRotator()
{
	return FRotator{ Pitch, Yaw, Roll };
}

struct Rotator Rotator::operator+=(const Rotator& other)
{
	Pitch += other.Pitch;
	Yaw += other.Yaw;
	Roll += other.Roll;
	return *this;
}

struct Rotator Rotator::operator-=(const Rotator& other)
{
	Pitch -= other.Pitch;
	Yaw -= other.Yaw;
	Roll -= other.Roll;
	return *this;
}

struct Rotator Rotator::operator*=(const Rotator& other)
{
	Pitch *= other.Pitch;
	Yaw *= other.Yaw;
	Roll *= other.Roll;
	return *this;
}

struct Rotator Rotator::operator/=(const Rotator& other)
{
	Pitch /= other.Pitch;
	Yaw /= other.Yaw;
	Roll /= other.Roll;
	return *this;
}

struct Rotator Rotator::operator+=(const int32_t other)
{
	Pitch += other;
	Yaw += other;
	Roll += other;
	return *this;
}

struct Rotator Rotator::operator-=(const int32_t other)
{
	Pitch -= other;
	Yaw -= other;
	Roll -= other;
	return *this;
}

struct Rotator Rotator::operator*=(const int32_t other)
{
	Pitch *= other;
	Yaw *= other;
	Roll *= other;
	return *this;
}

struct Rotator Rotator::operator/=(const int32_t other)
{
	Pitch /= other;
	Yaw /= other;
	Roll /= other;
	return *this;
}

struct Rotator Rotator::operator=(const Rotator& other)
{
	Pitch = other.Pitch;
	Yaw = other.Yaw;
	Roll = other.Roll;
	return *this;
}

struct Rotator Rotator::operator=(const struct FRotator& other)
{
	Pitch = other.Pitch;
	Yaw = other.Yaw;
	Roll = other.Roll;
	return *this;
}

struct Rotator Rotator::operator=(const int32_t other)
{
	Pitch = other;
	Yaw = other;
	Roll = other;
	return *this;
}

bool Rotator::operator==(const Rotator& other)
{
	if (Pitch != other.Pitch
		|| Yaw != other.Yaw
		|| Roll != other.Roll)
	{
		return false;
	}

	return true;
}

bool Rotator::operator==(const struct FRotator& other)
{
	if (Pitch != other.Pitch
		|| Yaw != other.Yaw
		|| Roll != other.Roll)
	{
		return false;
	}

	return true;
}

bool Rotator::operator!=(const Rotator& other)
{
	if (Pitch != other.Pitch
		|| Yaw != other.Yaw
		|| Roll != other.Roll)
	{
		return true;
	}

	return false;
}

bool Rotator::operator!=(const struct FRotator& other)
{
	if (Pitch != other.Pitch
		|| Yaw != other.Yaw
		|| Roll != other.Roll)
	{
		return true;
	}

	return false;
}

void Rotator::Erase()
{
	Pitch = 0;
	Yaw = 0;
	Roll = 0;
}

Rotator Rotator::Copy() const
{
	return Rotator(Pitch, Yaw, Roll);
}

float Rotator::ClampAxis(float a)
{
	a = fmod(a, 360.f);

	if (a < 0.f)
	{
		a += 360.f;
	}

	return a;
}

void Rotator::Normalize()
{
	Pitch = NormalizeAxis(Pitch);
	Yaw = NormalizeAxis(Yaw);
	Roll = NormalizeAxis(Roll);
}

float Rotator::NormalizeAxis(float a)
{
	a = ClampAxis(a);

	if (a > 180.f)
	{
		a -= 360.f;
	}

	return a;
}

Rotator Rotator::GetNormalize() const
{
	Rotator mutableThis = *this;
	mutableThis.Normalize();
	return mutableThis;
}

Vector Rotator::GetVector() const
{
	const float PitchNoWinding = fmod(Pitch, 360.0f);
	const float YawNoWinding = fmod(Yaw, 360.0f);

	float CP, SP, CY, SY;
	Math::SinCos(&SP, &CP, PitchNoWinding * (PI / 180.f));
	Math::SinCos(&SY, &CY, YawNoWinding * (PI / 180.f));

	return Vector(CP * CY, CP * SY, SP);
}

Vector Rotator::Rotate(Vector other) const
{
	double pitch = (double)Pitch / Rotation180 * PI;
	double yaw = (double)Yaw / Rotation180 * PI;
	double roll = (double)Roll / Rotation180 * PI;

	float sz = sin(pitch);
	float cz = cos(pitch);
	float sy = sin(-yaw);
	float cy = cos(-yaw);
	float sx = sin(roll);
	float cx = cos(roll);

	other = Vector(other.X, other.Y * cx - other.Z * sx, other.Y * sx + other.Z * cx); // Roll
	other = Vector(other.X * cz - other.Y * sz, other.X * sz + other.Y * cz, other.Z); // Pitch
	other = Vector(other.X * cy + other.Z * sy, other.Y, -other.X * sy + other.Z * cy); // Yaw

	float tmp = other.Z;
	other.Z = other.Y;
	other.Y = tmp;

	return other;
}

Quat::Quat() : X(0.f), Y(0.f), Z(0.f), W(0.f) { }

Quat::Quat(float xyzw) : X(xyzw), Y(xyzw), Z(xyzw), W(xyzw) { }

Quat::Quat(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }

Quat::Quat(const Quat& other) : X(other.X), Y(other.Y), Z(other.Z), W(other.W) { }

Quat::Quat(const struct FQuat& other) : X(other.X), Y(other.Y), Z(other.Z), W(other.W) { }

Quat::~Quat() { }

struct FQuat Quat::UnrealQuat()
{
	return FQuat{ X, Y, Z, W };
}

Quat Quat::operator+=(const Quat& other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;
	W += other.W;
	return *this;
}

Quat Quat::operator-=(const Quat& other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;
	W -= other.W;
	return *this;
}

Quat Quat::operator*=(const Quat& other)
{
	X *= other.X;
	Y *= other.Y;
	Z *= other.Z;
	W *= other.W;
	return *this;
}

Quat Quat::operator/=(const Quat& other)
{
	X /= other.X;
	Y /= other.Y;
	Z /= other.Z;
	W /= other.W;
	return *this;
}

Quat Quat::operator+=(const float other)
{
	X += other;
	Y += other;
	Z += other;
	W += other;
	return *this;
}

Quat Quat::operator-=(const float other)
{
	X -= other;
	Y -= other;
	Z -= other;
	W -= other;
	return *this;
}

Quat Quat::operator*=(const float other)
{
	X *= other;
	Y *= other;
	Z *= other;
	W *= other;
	return *this;
}

Quat Quat::operator/=(const float other)
{
	X /= other;
	Y /= other;
	Z /= other;
	W /= other;
	return *this;
}

Quat Quat::operator=(const Quat& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	W = other.W;
	return *this;
}

Quat Quat::operator=(const struct FQuat& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	W = other.W;
	return *this;
}

Quat Quat::operator=(const float other)
{
	X = other;
	Y = other;
	Z = other;
	W = other;
	return *this;
}

bool Quat::operator==(const Quat& other)
{
	if (X != other.X
		|| Y != other.Y
		|| Z != other.Z
		|| W != other.W)
	{
		return false;
	}

	return true;
}

bool Quat::operator==(const struct FQuat& other)
{
	if (X != other.X
		|| Y != other.Y
		|| Z != other.Z
		|| W != other.W)
	{
		return false;
	}

	return true;
}

bool Quat::operator!=(const Quat& other)
{
	if (X != other.X
		|| Y != other.Y
		|| Z != other.Z
		|| W != other.W)
	{
		return true;
	}

	return false;
}

bool Quat::operator!=(const struct FQuat& other)
{
	if (X != other.X
		|| Y != other.Y
		|| Z != other.Z
		|| W != other.W)
	{
		return true;
	}

	return false;
}

void Quat::Erase()
{
	X = 0.f;
	Y = 0.f;
	Z = 0.f;
	W = 0.f;
}

Quat Quat::Copy() const
{
	return Quat(X, Y, Z, W);
}

bool Quat::Equals(const Quat& q, float tolerance) const
{
	return (abs(X - q.X) <= tolerance && abs(Y - q.Y) <= tolerance && abs(Z - q.Z) <= tolerance && abs(W - q.W) <= tolerance)
		|| (abs(X + q.X) <= tolerance && abs(Y + q.Y) <= tolerance && abs(Z + q.Z) <= tolerance && abs(W + q.W) <= tolerance);
}

bool Quat::IsIdentity(float tolerance) const
{
	return Equals(Quat(0.f, 0.f, 0.f, 1.f), tolerance);
}

bool Quat::IsNormalized() const
{
	return (abs(1.f - SizeSquared()) < THRESH_QUAT_NORMALIZED);
}

float Quat::Size() const
{
	return sqrt(X * X + Y * Y + Z * Z + W * W);
}

float Quat::SizeSquared() const
{
	return (X * X + Y * Y + Z * Z + W * W);
}

float Quat::GetAngle() const
{
	return (2.f * acos(W));
}

Vector Quat::GetAxisX() const
{
	return Rotate(Vector(1.f, 0.f, 0.f));
}

Vector Quat::GetAxisY() const
{
	return Rotate(Vector(0.f, 1.f, 0.f));
}

Vector Quat::GetAxisZ() const
{
	return Rotate(Vector(0.f, 0.f, 1.f));
}

Vector Quat::GetRotationAxis() const
{
	const float sqrSum = X * X + Y * Y + Z * Z;

	if (sqrSum < SMALL_NUMBER)
	{
		return Vector(1.f, 0.f, 0.f);
	}

	const float scale = 1.f / sqrtf(sqrSum);
	return Vector(X * scale, Y * scale, Z * scale);
}

Quat Quat::GetInverse() const
{
	return Quat(-X, -Y, -Z, W);
}

void Quat::Normalize(float tolerance)
{
	float SquareSum = X * X + Y * Y + Z * Z + W * W;

	if (SquareSum >= tolerance)
	{
		float Scale = 1.f / sqrtf(SquareSum);

		X *= Scale; 
		Y *= Scale; 
		Z *= Scale;
		W *= Scale;
	}
	else
	{
		*this = Quat(0.f, 0.f, 0.f, 1.f);
	}
}

Quat Quat::GetNormalize(float tolerance) const
{
	Quat mutableThis = *this;
	mutableThis.Normalize();
	return mutableThis;
}

Rotator Quat::GetRotator() const
{
	// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/

	const float SingularityTest = Z * X - W * Y;
	const float YawY = 2.f * (W * Z + X * Y);
	const float YawX = (1.f - 2.f * (sqrt(Y) + sqrt(Z)));

	const float SINGULARITY_THRESHOLD = 0.4999995f;
	const float RAD_TO_DEG = (180.f) / PI;
	Rotator RotatorFromQuat;

	if (SingularityTest < -SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch = -90.f;
		RotatorFromQuat.Yaw = atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = RotatorFromQuat.NormalizeAxis(-RotatorFromQuat.Yaw - (2.f * atan2(X, W) * RAD_TO_DEG));
	}
	else if (SingularityTest > SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch = 90.f;
		RotatorFromQuat.Yaw = atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = RotatorFromQuat.NormalizeAxis(RotatorFromQuat.Yaw - (2.f * atan2(X, W) * RAD_TO_DEG));
	}
	else
	{
		RotatorFromQuat.Pitch = asin(2.f * (SingularityTest)) * RAD_TO_DEG;
		RotatorFromQuat.Yaw = atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = atan2(-2.f* (W * X + Y * Z), (1.f - 2.f * (sqrt(X) + sqrt(Y)))) * RAD_TO_DEG;
	}

	return RotatorFromQuat;
}

Vector Quat::Rotate(const Vector& other) const
{
	Vector q(X, Y, Z);
	Vector t = 2.f * q.Cross(other);
	return Vector(other + (W * t) + q.Cross(t));
}