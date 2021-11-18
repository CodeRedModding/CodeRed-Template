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
		// Map Value to y in [-pi, pi], x = 2 * pi * quotient + remainder.

		float quotient = (INV_PI * 0.5f) * value;

		if (value >= 0.0f)
		{
			quotient = (float)((int32_t)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int32_t)(quotient - 0.5f));
		}

		float y = value - (2.0f * PI) * quotient;

		// Map y to [-pi / 2, pi / 2] with sin(y) = sin(Value).

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
		*scalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.f) * y;

		// 10-degree minimax approximation
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.f;
		*scalarCos = sign*p;
	}

	struct FVector RotateUnreal(struct FVector point, const struct FRotator& rotation, const struct FVector& location)
	{
		double pitch = (double)rotation.Pitch / RadiansToRotation;
		double yaw = (double)rotation.Yaw / RadiansToRotation;
		double roll = (double)rotation.Roll / RadiansToRotation;

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

FVector2D Vector2DF::UnrealVector() const
{
	return FVector2D { X, Y };
}

void Vector2DF::Erase()
{
	X = 0.0f;
	Y = 0.0f;
}

Vector2DF Vector2DF::Copy() const
{
	return Vector2DF(X, Y);
}

Vector2DF Vector2DF::operator=(const Vector2DF& other)
{
	X = other.X;
	Y = other.Y;
	return *this;
}

Vector2DF Vector2DF::operator=(const struct FVector2D& other)
{
	X = other.X;
	Y = other.Y;
	return *this;
}

FVector2D Vector2DI::UnrealVector() const
{
	return FVector2D { static_cast<float>(X), static_cast<float>(Y) };
}

void Vector2DI::Erase()
{
	X = 0;
	Y = 0;
}

Vector2DI Vector2DI::Copy() const
{
	return Vector2DI(X, Y);
}

Vector2DI Vector2DI::operator=(const Vector2DI& other)
{
	X = other.X;
	Y = other.Y;
	return *this;
}

Vector2DI Vector2DI::operator=(const struct FVector2D& other)
{
	X = other.X;
	Y = other.Y;
	return *this;
}

struct FVector VectorF::UnrealVector() const
{
	return { X, Y, Z };
}

void VectorF::Erase()
{
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
}

VectorF VectorF::Copy() const
{
	return VectorF(X, Y, Z);
}

float VectorF::Magnitude() const
{
	return std::sqrtf(X * X + Y * Y + Z * Z);
}

void VectorF::Normalize()
{
	float magnitude = Magnitude();
	X = X / magnitude;
	Y = Y / magnitude;
	Z = Z / magnitude;
}

VectorF VectorF::GetNormalize() const
{
	float magnitude = Magnitude();
	VectorF mutableThis = *this;
	mutableThis.X = X / magnitude;
	mutableThis.Y = Y / magnitude;
	mutableThis.Z = Z / magnitude;
	return mutableThis;
}

class Rotator VectorF::GetRotator() const
{
	Rotator result;
	result.Yaw = atan2(Y, X) * (180.0f / PI);
	result.Pitch = atan2(Z, sqrt(X * X + Y * Y)) * (180.0f / PI);
	result.Roll = 0.f;
	return result;
}

VectorF VectorF::Dot(const VectorF& other) const
{
	return VectorF(X * other.X + Y * other.Y + Z * other.Z);
}

VectorF VectorF::Cross(const VectorF& other) const
{
	float x = Y * other.Z - Z * other.Y;
	float y = Z * other.X - X * other.Z;
	float z = X * other.Y - Y * other.X;
	return VectorF(x, y, z);
}

VectorF VectorF::Lerp(const VectorF& other, float percentage) const
{
	VectorF mutableThis = *this;
	return VectorF(mutableThis * percentage + (other * (1.f - percentage)));
}

VectorF VectorF::operator=(const VectorF& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	return *this;
}

VectorF VectorF::operator=(const struct FVector& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	return *this;
}

struct FVector VectorI::UnrealVector() const
{
	return { static_cast<float>(X), static_cast<float>(Y), static_cast<float>(Z) };
}

void VectorI::Erase()
{
	X = 0;
	Y = 0;
	Z = 0;
}

VectorI VectorI::Copy() const
{
	return VectorI(X, Y, Z);
}

float VectorI::Magnitude() const
{
	return std::sqrtf(X * X + Y * Y + Z * Z);
}

void VectorI::Normalize()
{
	float magnitude = Magnitude();
	X = X / magnitude;
	Y = Y / magnitude;
	Z = Z / magnitude;
}

VectorI VectorI::GetNormalize() const
{
	float magnitude = Magnitude();
	VectorI mutableThis = *this;
	mutableThis.X = X / magnitude;
	mutableThis.Y = Y / magnitude;
	mutableThis.Z = Z / magnitude;
	return mutableThis;
}

class Rotator VectorI::GetRotator() const
{
	Rotator result;
	result.Yaw = atan2(Y, X) * (180.0f / PI);
	result.Pitch = atan2(Z, sqrt(X * X + Y * Y)) * (180.0f / PI);
	result.Roll = 0;
	return result;
}

VectorI VectorI::Dot(const VectorI& other) const
{
	return VectorI(X * other.X + Y * other.Y + Z * other.Z);
}

VectorI VectorI::Cross(const VectorI& other) const
{
	int32_t x = Y * other.Z - Z * other.Y;
	int32_t y = Z * other.X - X * other.Z;
	int32_t z = X * other.Y - Y * other.X;
	return VectorI(x, y, z);
}

VectorI VectorI::Lerp(const VectorI& other, int32_t percentage) const
{
	VectorI mutableThis = *this;
	return VectorI(mutableThis * percentage + (other * (1 - percentage)));
}

VectorI VectorI::operator=(const VectorI& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	return *this;
}

VectorI VectorI::operator=(const struct FVector& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	return *this;
}

Rotator::Rotator() : Pitch(0), Yaw(0), Roll(0) {}

Rotator::Rotator(int32_t pyr) : Pitch(pyr), Yaw(pyr), Roll(pyr) {}

Rotator::Rotator(int32_t pitch, int32_t yaw, int32_t roll) : Pitch(pitch), Yaw(yaw), Roll(roll) {}

Rotator::Rotator(const Rotator& other) : Pitch(other.Pitch), Yaw(other.Yaw), Roll(other.Roll) {}

Rotator::Rotator(const struct FRotator& other) : Pitch(other.Pitch), Yaw(other.Yaw), Roll(other.Roll) {}

Rotator::~Rotator() {}

struct FRotator Rotator::UnrealRotator() const
{
	return FRotator{ Pitch, Yaw, Roll };
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
	a = fmod(a, 360.0f);

	if (a < 0.0f)
	{
		a += 360.0f;
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

	if (a > 180.0f)
	{
		a -= 360.0f;
	}

	return a;
}

Rotator Rotator::GetNormalize() const
{
	Rotator mutableThis = *this;
	mutableThis.Normalize();
	return mutableThis;
}

VectorF Rotator::GetVector() const
{
	const float PitchNoWinding = fmod(Pitch, 360.0f);
	const float YawNoWinding = fmod(Yaw, 360.0f);

	float CP, SP, CY, SY;
	Math::SinCos(&SP, &CP, PitchNoWinding * (PI / 180.0f));
	Math::SinCos(&SY, &CY, YawNoWinding * (PI / 180.0f));

	return VectorF(CP * CY, CP * SY, SP);
}

VectorF Rotator::Rotate(VectorF other) const
{
	double pitch = (double)Pitch / RadiansToRotation;
	double yaw = (double)Yaw / RadiansToRotation;
	double roll = (double)Roll / RadiansToRotation;

	float sz = sin(pitch);
	float cz = cos(pitch);
	float sy = sin(-yaw);
	float cy = cos(-yaw);
	float sx = sin(roll);
	float cx = cos(roll);

	other = VectorF(other.X, other.Y * cx - other.Z * sx, other.Y * sx + other.Z * cx); // Roll
	other = VectorF(other.X * cz - other.Y * sz, other.X * sz + other.Y * cz, other.Z); // Pitch
	other = VectorF(other.X * cy + other.Z * sy, other.Y, -other.X * sy + other.Z * cy ); // Yaw

	float tmp = other.Z;
	other.Z = other.Y;
	other.Y = tmp;

	return other;
}

Rotator Rotator::operator+=(const Rotator& other)
{
	Pitch += other.Pitch;
	Yaw += other.Yaw;
	Roll += other.Roll;
	return *this;
}

Rotator Rotator::operator-=(const Rotator& other)
{
	Pitch -= other.Pitch;
	Yaw -= other.Yaw;
	Roll -= other.Roll;
	return *this;
}

Rotator Rotator::operator*=(const Rotator& other)
{
	Pitch *= other.Pitch;
	Yaw *= other.Yaw;
	Roll *= other.Roll;
	return *this;
}

Rotator Rotator::operator/=(const Rotator& other)
{
	Pitch /= other.Pitch;
	Yaw /= other.Yaw;
	Roll /= other.Roll;
	return *this;
}

Rotator Rotator::operator+=(int32_t other)
{
	Pitch += other;
	Yaw += other;
	Roll += other;
	return *this;
}

Rotator Rotator::operator-=(int32_t other)
{
	Pitch -= other;
	Yaw -= other;
	Roll -= other;
	return *this;
}

Rotator Rotator::operator*=(int32_t other)
{
	Pitch *= other;
	Yaw *= other;
	Roll *= other;
	return *this;
}

Rotator Rotator::operator/=(int32_t other)
{
	Pitch /= other;
	Yaw /= other;
	Roll /= other;
	return *this;
}

Rotator Rotator::operator=(const Rotator& other)
{
	Pitch = other.Pitch;
	Yaw = other.Yaw;
	Roll = other.Roll;
	return *this;
}

Rotator Rotator::operator=(const struct FRotator& other)
{
	Pitch = other.Pitch;
	Yaw = other.Yaw;
	Roll = other.Roll;
	return *this;
}

Rotator Rotator::operator=(int32_t other)
{
	Pitch = other;
	Yaw = other;
	Roll = other;
	return *this;
}

bool Rotator::operator==(const Rotator& other) const
{
	return (Pitch == other.Pitch && Yaw == other.Yaw && Roll == other.Roll);
}

bool Rotator::operator==(const struct FRotator& other) const
{
	return (Pitch == other.Pitch && Yaw == other.Yaw && Roll == other.Roll);
}

bool Rotator::operator!=(const Rotator& other) const
{
	return (Pitch != other.Pitch || Yaw != other.Yaw || Roll != other.Roll);
}

bool Rotator::operator!=(const struct FRotator& other) const
{
	return (Pitch != other.Pitch || Yaw != other.Yaw || Roll != other.Roll);
}

VectorF Rotate(VectorF point, const Rotator& rotation, const VectorF& location)
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

	point = VectorF(point.X, point.Y * cx - point.Z * sx, point.Y * sx + point.Z * cx); // Roll
	point = VectorF(point.X * cz - point.Y * sz, point.X * sz + point.Y * cz, point.Z); // Pitch
	point = VectorF(point.X * cy + point.Z * sy, point.Y, -point.X * sy + point.Z * cy ); // Yaw

	float tmp = point.Z;
	point.Z = point.Y;
	point.Y = tmp;

	point.X += location.X;
	point.Y += location.Y;
	point.Z += location.Z;

	return point;
}

Quat::Quat() : X(0.0f), Y(0.0f), Z(0.0f), W(0.0f) {}

Quat::Quat(float xyzw) : X(xyzw), Y(xyzw), Z(xyzw), W(xyzw) {}

Quat::Quat(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}

Quat::Quat(const Quat& other) : X(other.X), Y(other.Y), Z(other.Z), W(other.W) {}

Quat::Quat(const struct FQuat& other) : X(other.X), Y(other.Y), Z(other.Z), W(other.W) {}

Quat::~Quat() {}

struct FQuat Quat::UnrealQuat() const
{
	return FQuat{ X, Y, Z, W };
}

void Quat::Erase()
{
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
	W = 0.0f;
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
	return Equals(Quat(0.0f, 0.0f, 0.0f, 1.0f), tolerance);
}

bool Quat::IsNormalized() const
{
	return (abs(1.0f - SizeSquared()) < THRESH_QUAT_NORMALIZED);
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
	return (2.0f * acos(W));
}

VectorF Quat::GetAxisX() const
{
	return Rotate(VectorF(1.0f, 0.0f, 0.0f));
}

VectorF Quat::GetAxisY() const
{
	return Rotate(VectorF(0.0f, 1.0f, 0.0f));
}

VectorF Quat::GetAxisZ() const
{
	return Rotate(VectorF(0.0f, 0.0f, 1.0f));
}

VectorF Quat::GetRotationAxis() const
{
	const float sqrSum = X * X + Y * Y + Z * Z;

	if (sqrSum < SMALL_NUMBER)
	{
		return VectorF(1.0f, 0.0f, 0.0f);
	}

	const float scale = 1.0f / sqrtf(sqrSum);
	return VectorF(X * scale, Y * scale, Z * scale);
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
		float Scale = 1.0f / sqrtf(SquareSum);

		X *= Scale; 
		Y *= Scale; 
		Z *= Scale;
		W *= Scale;
	}
	else
	{
		*this = Quat(0.0f, 0.0f, 0.0f, 1.0f);
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
	const float YawY = 2.0f * (W * Z + X * Y);
	const float YawX = (1.0f - 2.0f * (sqrt(Y) + sqrt(Z)));

	const float SINGULARITY_THRESHOLD = 0.4999995f;
	const float RAD_TO_DEG = (180.0f) / PI;
	Rotator RotatorFromQuat;

	if (SingularityTest < -SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch = -90.0f;
		RotatorFromQuat.Yaw = atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = RotatorFromQuat.NormalizeAxis(-RotatorFromQuat.Yaw - (2.0f * atan2(X, W) * RAD_TO_DEG));
	}
	else if (SingularityTest > SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch = 90.0f;
		RotatorFromQuat.Yaw = atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = RotatorFromQuat.NormalizeAxis(RotatorFromQuat.Yaw - (2.0f * atan2(X, W) * RAD_TO_DEG));
	}
	else
	{
		RotatorFromQuat.Pitch = asin(2.0f * (SingularityTest)) * RAD_TO_DEG;
		RotatorFromQuat.Yaw = atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = atan2(-2.0f* (W * X + Y * Z), (1.f - 2.f * (sqrt(X) + sqrt(Y)))) * RAD_TO_DEG;
	}

	return RotatorFromQuat;
}

VectorF Quat::Rotate(const VectorF& other) const
{
	VectorF q(X, Y, Z);
	VectorF t = 2.0f * q.Cross(other);
	return VectorF(other + (W * t) + q.Cross(t));
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

Quat Quat::operator+=(float other)
{
	X += other;
	Y += other;
	Z += other;
	W += other;
	return *this;
}

Quat Quat::operator-=(float other)
{
	X -= other;
	Y -= other;
	Z -= other;
	W -= other;
	return *this;
}

Quat Quat::operator*=(float other)
{
	X *= other;
	Y *= other;
	Z *= other;
	W *= other;
	return *this;
}

Quat Quat::operator/=(float other)
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

Quat Quat::operator=(float other)
{
	X = other;
	Y = other;
	Z = other;
	W = other;
	return *this;
}

bool Quat::operator==(const Quat& other) const
{
	return (X == other.X && Y == other.Y && Z == other.Z && W == other.W);
}

bool Quat::operator==(const struct FQuat& other) const
{
	return (X == other.X && Y == other.Y && Z == other.Z && W == other.W);
}

bool Quat::operator!=(const Quat& other) const
{
	return (X != other.X || Y != other.Y || Z != other.Z || W != other.W);
}

bool Quat::operator!=(const struct FQuat& other) const
{
	return (X != other.X || Y != other.Y || Z != other.Z || W != other.W);
}