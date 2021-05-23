#include "Math.hpp"

namespace Math
{
	float GetDistance(const struct FVector& from, const struct FVector& to, const float scaling)
	{
		float distance = std::sqrt(std::pow(to.X - from.X, 2) + std::pow(to.Y - from.Y, 2) + std::pow(to.Z - from.Z, 2));
		return distance / scaling;
	}

	struct FVector Rotate(const struct FVector& point, const struct FRotator& rotation, const struct FVector& location)
	{
		double pitch = (double)rotation.Pitch / 32768.0 * 3.14159;
		double yaw = (double)rotation.Yaw / 32768.0 * 3.14159;
		double roll = (double)rotation.Roll / 32768.0 * 3.14159;

		float sx = sin(roll);
		float cx = cos(roll);
		float sy = sin(-yaw);
		float cy = cos(-yaw);
		float sz = sin(pitch);
		float cz = cos(pitch);

		FVector newPoint;

		newPoint = FVector{ point.X, point.Y * cx - point.Z * sx, point.Y * sx + point.Z * cx };
		newPoint = FVector{ point.X * cz - point.Y * sz, point.X * sz + point.Y * cz, point.Z };
		newPoint = FVector{ point.X * cy + point.Z * sy, point.Y, -point.X * sy + point.Z * cy };

		float temp = point.Z;
		newPoint.Z = point.Y;
		newPoint.Y = temp;

		newPoint.X += location.X;
		newPoint.Y += location.Y;
		newPoint.Z += location.Z;

		return newPoint;
	}
}

CRVector::CRVector() : X(0.f), Y(0.f), Z(0.f) { }

CRVector::CRVector(const float xyz) : X(xyz), Y(xyz), Z(xyz) { }

CRVector::CRVector(const float x, const float y, const float z) : X(x), Y(y), Z(z) { }

CRVector::~CRVector() { }

struct FVector CRVector::UnrealVector()
{
	return FVector { X, Y, Z };
}

struct CRVector CRVector::operator+=(const struct CRVector& other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;
	return *this;
}

struct CRVector CRVector::operator-=(const struct CRVector& other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;
	return *this;
}

struct CRVector CRVector::operator*=(const struct CRVector& other)
{
	X *= other.X;
	Y *= other.Y;
	Z *= other.Z;
	return *this;
}

struct CRVector CRVector::operator/=(const struct CRVector& other)
{
	X /= other.X;
	Y /= other.Y;
	Z /= other.Z;
	return *this;
}

struct CRVector CRVector::operator+=(const float other)
{
	X += other;
	Y += other;
	Z += other;
	return *this;
}

struct CRVector CRVector::operator-=(const float other)
{
	X -= other;
	Y -= other;
	Z -= other;
	return *this;
}

struct CRVector CRVector::operator*=(const float other)
{
	X *= other;
	Y *= other;
	Z *= other;
	return *this;
}

struct CRVector CRVector::operator/=(const float other)
{
	X /= other;
	Y /= other;
	Z /= other;
	return *this;
}

struct CRVector CRVector::operator=(const struct CRVector& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	return *this;
}

struct CRVector CRVector::operator=(const struct FVector& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	return *this;
}

struct CRVector CRVector::operator=(const float other)
{
	X = other;
	Y = other;
	Z = other;
	return *this;
}

bool CRVector::operator==(const struct CRVector& other)
{
	if (X != other.X
		|| Y != other.Y
		|| Z != other.Z)
	{
		return false;
	}

	return true;
}

bool CRVector::operator==(const struct FVector& other)
{
	if (X != other.X
		|| Y != other.Y
		|| Z != other.Z)
	{
		return false;
	}

	return true;
}

bool CRVector::operator!=(const struct CRVector& other)
{
	if (X != other.X
		|| Y != other.Y
		|| Z != other.Z)
	{
		return true;
	}

	return false;
}

bool CRVector::operator!=(const struct FVector& other)
{
	if (X != other.X
		|| Y != other.Y
		|| Z != other.Z)
	{
		return true;
	}

	return false;
}

void CRVector::Erase()
{
	X = 0.f;
	Y = 0.f;
	Z = 0.f;
}

struct CRVector CRVector::Copy() const
{
	return CRVector(X, Y, Z);
}

float CRVector::Magnitude() const
{
	return std::sqrtf(X * X + Y * Y + Z * Z);
}

struct CRVector CRVector::Normalize()
{
	float magnitude = Magnitude();
	X = X / magnitude;
	Y = Y / magnitude;
	Z = Z / magnitude;
	return *this;
}

struct CRVector CRVector::NormalizeMutable() const
{
	float magnitude = Magnitude();
	CRVector mutableThis = *this;
	mutableThis.X = X / magnitude;
	mutableThis.Y = Y / magnitude;
	mutableThis.Z = Z / magnitude;
	return mutableThis;
}

struct CRVector CRVector::Dot(const struct CRVector& other) const
{
	return CRVector(X * other.X + Y * other.Y + Z * other.Z);
}

struct CRVector CRVector::Cross(const struct CRVector& other) const
{
	float x = Y * other.Z - Z * other.Y;
	float y = Z * other.X - X * other.Z;
	float z = X * other.Y - Y * other.X;
	return CRVector(x, y, z);
}

struct CRVector CRVector::Lerp(const struct CRVector& other, const float percentage) const
{
	CRVector mutableThis = *this;
	return CRVector(mutableThis * percentage + (other * (1.f - percentage)));
}

CRVector2D::CRVector2D() : X(0.f), Y(0.f) { }

CRVector2D::CRVector2D(const float xy) : X(xy), Y(xy) { }

CRVector2D::CRVector2D(const float x, const float y) : X(x), Y(y) { }

CRVector2D::~CRVector2D() { }

struct FVector2D CRVector2D::UnrealVector()
{
	return FVector2D { X, Y };
}

struct CRVector2D CRVector2D::operator+=(const struct CRVector2D& other)
{
	X += other.X;
	Y += other.Y;
	return *this;
}

struct CRVector2D CRVector2D::operator-=(const struct CRVector2D& other)
{
	X -= other.X;
	Y -= other.Y;
	return *this;
}

struct CRVector2D CRVector2D::operator*=(const struct CRVector2D& other)
{
	X *= other.X;
	Y *= other.Y;
	return *this;
}

struct CRVector2D CRVector2D::operator/=(const struct CRVector2D& other)
{
	X /= other.X;
	Y /= other.Y;
	return *this;
}

struct CRVector2D CRVector2D::operator+=(const float other)
{
	X += other;
	Y += other;
	return *this;
}

struct CRVector2D CRVector2D::operator-=(const float other)
{
	X -= other;
	Y -= other;
	return *this;
}

struct CRVector2D CRVector2D::operator*=(const float other)
{
	X *= other;
	Y *= other;
	return *this;
}

struct CRVector2D CRVector2D::operator/=(const float other)
{
	X /= other;
	Y /= other;
	return *this;
}

struct CRVector2D CRVector2D::operator=(const struct CRVector2D& other)
{
	X = other.X;
	Y = other.Y;
	return *this;
}

struct CRVector2D CRVector2D::operator=(const struct FVector2D& other)
{
	X = other.X;
	Y = other.Y;
	return *this;
}

struct CRVector2D CRVector2D::operator=(const float other)
{
	X = other;
	Y = other;
	return *this;
}

bool CRVector2D::operator==(const struct CRVector2D& other)
{
	if (X != other.X
		|| Y != other.Y)
	{
		return false;
	}

	return true;
}

bool CRVector2D::operator==(const struct FVector2D& other)
{
	if (X != other.X
		|| Y != other.Y)
	{
		return false;
	}

	return true;
}

bool CRVector2D::operator!=(const struct CRVector2D& other)
{
	if (X != other.X
		|| Y != other.Y)
	{
		return true;
	}

	return false;
}

bool CRVector2D::operator!=(const struct FVector2D& other)
{
	if (X != other.X
		|| Y != other.Y)
	{
		return true;
	}

	return false;
}

void CRVector2D::Erase()
{
	X = 0.f;
	Y = 0.f;
}

struct CRVector2D CRVector2D::Copy() const
{
	return CRVector2D(X, Y);
}

CRRotator::CRRotator() : Pitch(0), Yaw(0), Roll(0) { }

CRRotator::CRRotator(const int32_t pyr) : Pitch(pyr), Yaw(pyr), Roll(pyr) { }

CRRotator::CRRotator(const int32_t pitch, const int32_t yaw, const int32_t roll) : Pitch(pitch), Yaw(yaw), Roll(roll) { }

CRRotator::~CRRotator() { }

struct FRotator CRRotator::UnrealRotator()
{
	return FRotator{ Pitch, Yaw, Roll };
}

struct CRRotator CRRotator::operator+=(const struct CRRotator& other)
{
	Pitch += other.Pitch;
	Yaw += other.Yaw;
	Roll += other.Roll;
	return *this;
}

struct CRRotator CRRotator::operator-=(const struct CRRotator& other)
{
	Pitch -= other.Pitch;
	Yaw -= other.Yaw;
	Roll -= other.Roll;
	return *this;
}

struct CRRotator CRRotator::operator*=(const struct CRRotator& other)
{
	Pitch *= other.Pitch;
	Yaw *= other.Yaw;
	Roll *= other.Roll;
	return *this;
}

struct CRRotator CRRotator::operator/=(const struct CRRotator& other)
{
	Pitch /= other.Pitch;
	Yaw /= other.Yaw;
	Roll /= other.Roll;
	return *this;
}

struct CRRotator CRRotator::operator+=(const int32_t other)
{
	Pitch += other;
	Yaw += other;
	Roll += other;
	return *this;
}

struct CRRotator CRRotator::operator-=(const int32_t other)
{
	Pitch -= other;
	Yaw -= other;
	Roll -= other;
	return *this;
}

struct CRRotator CRRotator::operator*=(const int32_t other)
{
	Pitch *= other;
	Yaw *= other;
	Roll *= other;
	return *this;
}

struct CRRotator CRRotator::operator/=(const int32_t other)
{
	Pitch /= other;
	Yaw /= other;
	Roll /= other;
	return *this;
}

struct CRRotator CRRotator::operator=(const struct CRRotator& other)
{
	Pitch = other.Pitch;
	Yaw = other.Yaw;
	Roll = other.Roll;
	return *this;
}

struct CRRotator CRRotator::operator=(const struct FRotator& other)
{
	Pitch = other.Pitch;
	Yaw = other.Yaw;
	Roll = other.Roll;
	return *this;
}

struct CRRotator CRRotator::operator=(const int32_t other)
{
	Pitch = other;
	Yaw = other;
	Roll = other;
	return *this;
}

bool CRRotator::operator==(const struct CRRotator& other)
{
	if (Pitch != other.Pitch
		|| Yaw != other.Yaw
		|| Roll != other.Roll)
	{
		return false;
	}

	return true;
}

bool CRRotator::operator==(const struct FRotator& other)
{
	if (Pitch != other.Pitch
		|| Yaw != other.Yaw
		|| Roll != other.Roll)
	{
		return false;
	}

	return true;
}

bool CRRotator::operator!=(const struct CRRotator& other)
{
	if (Pitch != other.Pitch
		|| Yaw != other.Yaw
		|| Roll != other.Roll)
	{
		return true;
	}

	return false;
}

bool CRRotator::operator!=(const struct FRotator& other)
{
	if (Pitch != other.Pitch
		|| Yaw != other.Yaw
		|| Roll != other.Roll)
	{
		return true;
	}

	return false;
}

void CRRotator::Erase()
{
	Pitch = 0;
	Yaw = 0;
	Roll = 0;
}

struct CRRotator CRRotator::Copy() const
{
	return CRRotator(Pitch, Yaw, Roll);
}

struct CRVector CRRotator::Rotate(const struct CRVector& point, const struct CRVector& location) const
{
	double pitch = (double)Pitch / 32768.0 * 3.14159;
	double yaw = (double)Yaw / 32768.0 * 3.14159;
	double roll = (double)Roll / 32768.0 * 3.14159;

	float sx = sin(roll);
	float cx = cos(roll);
	float sy = sin(-yaw);
	float cy = cos(-yaw);
	float sz = sin(pitch);
	float cz = cos(pitch);

	CRVector newPoint;

	newPoint = CRVector(point.X, point.Y * cx - point.Z * sx, point.Y * sx + point.Z * cx);
	newPoint = CRVector(point.X * cz - point.Y * sz, point.X * sz + point.Y * cz, point.Z);
	newPoint = CRVector(point.X * cy + point.Z * sy, point.Y, -point.X * sy + point.Z * cy);

	float temp = point.Z;
	newPoint.Z = point.Y;
	newPoint.Y = temp;

	newPoint.X += location.X;
	newPoint.Y += location.Y;
	newPoint.Z += location.Z;

	return newPoint;
}