#pragma once
#include "../pch.hpp"

// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/Core/Public/Math/UnrealMathUtility.h
// https://cs.stanford.edu/~acoates/quaternion.h

static constexpr float PI = 3.1415926535897932f;										// Extra digits if needed: 3.1415926535897932384626433832795f
static constexpr float SMALL_NUMBER = 1.e-8f;
static constexpr float KINDA_SMALL_NUMBER = 1.e-4f;
static constexpr float BIG_NUMBER = 3.4e+38f;
static constexpr float EULERS_NUMBER = 2.71828182845904523536f;
static constexpr float UE_GOLDEN_RATIO = 1.6180339887498948482045868343656381f;			// Also known as divine proportion, golden mean, or golden section - related to the Fibonacci Sequence = (1 + sqrt(5)) / 2
static constexpr float FLOAT_NON_FRACTIONAL = 8388608.0f;								// All single-precision floating point numbers greater than or equal to this have no fractional value.

static constexpr float MAX_FLT = 3.402823466e+38f;
static constexpr float INV_PI = 0.31830988618f;
static constexpr float HALF_PI = 1.57079632679f;

static constexpr float UE_SQRT_2 = 1.4142135623730950488016887242097f;
static constexpr float UE_SQRT_3 = 1.7320508075688772935274463415059f;
static constexpr float UE_INV_SQRT_2 = 0.70710678118654752440084436210485f;
static constexpr float UE_INV_SQRT_3 = 0.57735026918962576450914878050196f;
static constexpr float UE_HALF_SQRT_2 = 0.70710678118654752440084436210485f;
static constexpr float UE_HALF_SQRT_3 = 0.86602540378443864676372317075294f;

static constexpr float DELTA = 0.00001f;
static constexpr float FLOAT_NORMAL_THRESH = 0.0001f;

static constexpr float THRESH_POINT_ON_PLANE = 0.10f;									// Thickness of plane for front/back/inside test.
static constexpr float THRESH_POINT_ON_SIDE = 0.20f;									// Thickness of polygon side's side-plane for point-inside/outside/on side test.
static constexpr float THRESH_POINTS_ARE_SAME = 0.00002f;								// Two points are same if within this distance.
static constexpr float THRESH_POINTS_ARE_NEAR = 0.00002f;								// Two points are near if within this distance and can be combined if imprecise math is ok.
static constexpr float THRESH_NORMALS_ARE_SAME = 0.00002f;								// Two normal points are same if within this distance.
static constexpr float THRESH_UVS_ARE_SAME = 0.0009765625f;								// Two UV are same if within this threshold (1.0f/1024f).
static constexpr float THRESH_VECTORS_ARE_NEAR = 0.0004f;								// Two vectors are near if within this distance and can be combined if imprecise math is ok.
static constexpr float THRESH_SPLIT_POLY_WITH_PLANE = 0.25f;							// A plane splits a polygon in half.
static constexpr float THRESH_SPLIT_POLY_PRECISELY = 0.01f;								// A plane exactly splits a polygon.
static constexpr float THRESH_ZERO_NORM_SQUARED = 0.0001f;								// Size of a unit normal that is considered "zero", squared.
static constexpr float THRESH_NORMALS_ARE_PARALLEL = 0.999845f;							// Two unit vectors are parallel if abs(A dot B) is greater than or equal to this. This is roughly cosine(1.0 degrees).
static constexpr float THRESH_NORMALS_ARE_ORTHOGONAL = 0.017455f;						// Two unit vectors are orthogonal (perpendicular) if abs(A dot B) is less than or equal this. This is roughly cosine(89.0f degrees).
static constexpr float THRESH_VECTOR_NORMALIZED = 0.01f;								// Allowed error for a normalized vector (against squared magnitude).
static constexpr float THRESH_QUAT_NORMALIZED = 0.01f;									// Allowed error for a normalized quaternion (against squared magnitude).

static constexpr int32_t Rotation360 = 65535;
static constexpr int32_t Rotation180 = 32768;
static constexpr double RotationToRadians = (PI / Rotation180);
static constexpr double DegreesToRadians = (PI / 180.0);
static constexpr float RotationsToDegrees = (Rotation360 / 360.0f);
static constexpr double RadiansToDegrees = (180.0 / PI);
static constexpr double RadiansToRotation = (Rotation180 * PI);

static constexpr float UU_CM = 1.0f;													// One Unreal Unit is equal to one centimeter.
static constexpr float UU_PIXEL = 6.25f;												// Four pixels are equal to twenty five unreal units.

// Credit to @FrancesElMute/Aberinkula/Megasplat for the rotate function, was pulling my hair out tyring to figure it out on my own.

namespace Math
{
	void SinCos(float* scalarSin, float* scalarCos, float value);
	template <typename T> T RandomRange(T min, T max)
	{
		std::random_device device;
		std::mt19937 random(device());
		std::uniform_int_distribution<T> generator(min, max);
		return generator(random);
	}
}

// Template based 2D vector, a base class for other type-specific vectors with built in operators.
template <typename TElementType>
class TVector2D
{
public:
	using TType = TElementType;
	TType X, Y;

public:
	TVector2D() : X(0), Y(0) {}
	TVector2D(TType xy) : X(xy), Y(xy) {}
	TVector2D(TType x, TType y) : X(x), Y(y) {}
	TVector2D(const TVector2D& other) : X(other.X), Y(other.Y) {}
	~TVector2D() {}

public:
	TVector2D& operator+=(const TVector2D& other) { X += other.X, Y += other.Y; return *this; }
	TVector2D& operator-=(const TVector2D& other) { X -= other.X, Y -= other.Y; return *this; }
	TVector2D& operator*=(const TVector2D& other) { X *= other.X, Y *= other.Y; return *this; }
	TVector2D& operator/=(const TVector2D& other) { X /= other.X, Y /= other.Y; return *this; }
	TVector2D& operator+=(TType other) { X += other; Y += other; return *this; }
	TVector2D& operator-=(TType other) { X -= other; Y -= other; return *this; }
	TVector2D& operator*=(TType other) { X *= other; Y *= other; return *this; }
	TVector2D& operator/=(TType other) { X /= other; Y /= other; return *this; }
	TVector2D& operator=(const TVector2D& other) { X = other.X, Y = other.Y; return *this; }
	TVector2D& operator=(TType other) { X = other; Y = other; return *this; }
	bool operator==(const TVector2D& other) const { return (X == other.X && Y == other.Y); }
	bool operator!=(const TVector2D& other) const { return (X != other.X || Y != other.Y); }
};

// Template based 3D vector, a base class for other type-specific vectors with built in operators.
template <typename TElementType>
class TVector3D
{
public:
	using TType = TElementType;
	TType X, Y, Z;

public:
	TVector3D() : X(0), Y(0), Z(0) {}
	TVector3D(TType xyz) : X(xyz), Y(xyz), Z(xyz) {}
	TVector3D(TType x, TType y, TType z) : X(x), Y(y), Z(z) {}
	TVector3D(const TVector3D& other) : X(other.X), Y(other.Y), Z(other.Z) {}
	~TVector3D() {}

public:
	TVector3D& operator+=(const TVector3D& other) { X += other.X, Y += other.Y; Z += other.Z; return *this; }
	TVector3D& operator-=(const TVector3D& other) { X -= other.X, Y -= other.Y; Z -= other.Z; return *this; }
	TVector3D& operator*=(const TVector3D& other) { X *= other.X, Y *= other.Y; Z *= other.Z; return *this; }
	TVector3D& operator/=(const TVector3D& other) { X /= other.X, Y /= other.Y; Z /= other.Z; return *this; }
	TVector3D& operator+=(TType other) { X += other; Y += other; Z += other; return *this; }
	TVector3D& operator-=(TType other) { X -= other; Y -= other; Z -= other; return *this; }
	TVector3D& operator*=(TType other) { X *= other; Y *= other; Z *= other; return *this; }
	TVector3D& operator/=(TType other) { X /= other; Y /= other; Z /= other; return *this; }
	TVector3D& operator=(const TVector3D& other) { X = other.X, Y = other.Y; Z = other.Z; return *this; }
	TVector3D& operator=(TType other) { X = other; Y = other; Z = other; return *this; }
	bool operator==(const TVector3D& other) const { return (X == other.X && Y == other.Y && Z == other.Z); }
	bool operator!=(const TVector3D& other) const { return (X != other.X || Y != other.Y || Z != other.Z); }
};

// These classes below rely on the templated base classes for ease of use.
// Want to have a vector that uses doubles or even long longs? Not a problem, just copy the class below and replace the variable type.
// All base operators are also templated so they are calculated on compile time.

class Vector2DF : public TVector2D<float>
{
public:
	Vector2DF() : TVector2D(0.0f) {}
	Vector2DF(float xy) : TVector2D(xy) {}
	Vector2DF(float x, float y) : TVector2D(x, y) {}
	Vector2DF(const struct FVector2D& other) : TVector2D(other.X, other.Y) {}
	Vector2DF(const Vector2DF& other) : TVector2D(other) {}
	~Vector2DF() {}

public:
	struct FVector2D UnrealVector() const;

public:
	void Erase();
	Vector2DF Copy() const;
	Vector2DF Lerp(const Vector2DF& other, float percentage) const;
	Vector2DF MidpointTo(const Vector2DF& other) const;

public:
	Vector2DF& operator=(const Vector2DF& other);
	Vector2DF& operator=(const struct FVector2D& other);
};

inline Vector2DF operator+(const Vector2DF& a, const Vector2DF& b) { return Vector2DF(a.X + b.X, a.Y + b.Y); }
inline Vector2DF operator-(const Vector2DF& a, const Vector2DF& b) { return Vector2DF(a.X - b.X, a.Y - b.Y); }
inline Vector2DF operator*(const Vector2DF& a, const Vector2DF& b) { return Vector2DF(a.X * b.X, a.Y * b.Y); }
inline Vector2DF operator/(const Vector2DF& a, const Vector2DF& b) { return Vector2DF(a.X / b.X, a.Y / b.Y); }
inline Vector2DF operator+(const Vector2DF& a, float b) { return Vector2DF(a.X + b, a.Y + b); }
inline Vector2DF operator-(const Vector2DF& a, float b) { return Vector2DF(a.X - b, a.Y - b); }
inline Vector2DF operator*(const Vector2DF& a, float b) { return Vector2DF(a.X * b, a.Y * b); }
inline Vector2DF operator/(const Vector2DF& a, float b) { return Vector2DF(a.X / b, a.Y / b); }

class Vector2DI : public TVector2D<int32_t>
{
public:
	Vector2DI() : TVector2D(0) {}
	Vector2DI(int32_t xy) : TVector2D(xy) {}
	Vector2DI(int32_t x, int32_t y) : TVector2D(x, y) {}
	Vector2DI(const struct FVector2D& other) : TVector2D(static_cast<int32_t>(other.X), static_cast<int32_t>(other.Y)) {}
	Vector2DI(const Vector2DI& other) : TVector2D(other) {}
	~Vector2DI() {}

public:
	struct FVector2D UnrealVector() const;

public:
	void Erase();
	Vector2DI Copy() const;
	Vector2DI Lerp(const Vector2DF& other, int32_t percentage) const;
	Vector2DI MidpointTo(const Vector2DI& other) const;

public:
	Vector2DI& operator=(const Vector2DI& other);
	Vector2DI& operator=(const struct FVector2D& other);
};

inline Vector2DI operator+(const Vector2DI& a, const Vector2DI& b) { return Vector2DI(a.X + b.X, a.Y + b.Y); }
inline Vector2DI operator-(const Vector2DI& a, const Vector2DI& b) { return Vector2DI(a.X - b.X, a.Y - b.Y); }
inline Vector2DI operator*(const Vector2DI& a, const Vector2DI& b) { return Vector2DI(a.X * b.X, a.Y * b.Y); }
inline Vector2DI operator/(const Vector2DI& a, const Vector2DI& b) { return Vector2DI(a.X / b.X, a.Y / b.Y); }
inline Vector2DI operator+(const Vector2DI& a, int32_t b) { return Vector2DI(a.X + b, a.Y + b); }
inline Vector2DI operator-(const Vector2DI& a, int32_t b) { return Vector2DI(a.X - b, a.Y - b); }
inline Vector2DI operator*(const Vector2DI& a, int32_t b) { return Vector2DI(a.X * b, a.Y * b); }
inline Vector2DI operator/(const Vector2DI& a, int32_t b) { return Vector2DI(a.X / b, a.Y / b); }

class VectorF : public TVector3D<float>
{
public:
	VectorF() : TVector3D(0.0f) {}
	VectorF(TType xyz) : TVector3D(xyz) {}
	VectorF(TType x, TType y, TType z) : TVector3D(x, y, z) {}
	VectorF(const struct FVector& other) : TVector3D(other.X, other.Y, other.Z) {}
	VectorF(const VectorF& other) : TVector3D(other) {}
	~VectorF() {}

public:
	struct FVector UnrealVector() const;

public:
	void Erase();
	VectorF Copy() const;
	float Magnitude() const;
	void Normalize();
	class Rotator GetRotator() const;
	VectorF GetNormalize() const;
	VectorF Dot(const VectorF& other) const;
	VectorF Cross(const VectorF& other) const;
	VectorF Lerp(const VectorF& other, float percentage) const;
	VectorF MidpointTo(const VectorF& other) const;
	VectorF Rotate(const Rotator& rotation, const VectorF& location); // Credit to @FrancesElMute/Aberinkula/Megasplat for this rotate function.
	float DistanceTo(const VectorF& other, float scaling = 1.0f) const;

public:
	VectorF& operator=(const VectorF& other);
	VectorF& operator=(const struct FVector& other);
};

inline VectorF operator+(const VectorF& a, const VectorF& b) { return VectorF(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
inline VectorF operator-(const VectorF& a, const VectorF& b) { return VectorF(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline VectorF operator*(const VectorF& a, const VectorF& b) { return VectorF(a.X * b.X, a.Y * b.Y, a.Z * b.Z); }
inline VectorF operator/(const VectorF& a, const VectorF& b) { return VectorF(a.X / b.X, a.Y / b.Y, a.Z / b.Z); }
inline VectorF operator+(const VectorF& a, float b) { return VectorF(a.X + b, a.Y + b, a.Z + b); }
inline VectorF operator-(const VectorF& a, float b) { return VectorF(a.X - b, a.Y - b, a.Z - b); }
inline VectorF operator*(const VectorF& a, float b) { return VectorF(a.X * b, a.Y * b, a.Z * b); }
inline VectorF operator/(const VectorF& a, float b) { return VectorF(a.X / b, a.Y / b, a.Z / b); }

class VectorI : public TVector3D<int32_t>
{
public:
	VectorI() : TVector3D(0) {}
	VectorI(TType xyz) : TVector3D(xyz) {}
	VectorI(TType x, TType y, TType z) : TVector3D(x, y, z) {}
	VectorI(const struct FVector& other) : TVector3D(static_cast<int32_t>(other.X), static_cast<int32_t>(other.Y), static_cast<int32_t>(other.Z)) {}
	VectorI(const VectorI& other) : TVector3D(other) {}
	~VectorI() {}

public:
	struct FVector UnrealVector() const;

public:
	void Erase();
	VectorI Copy() const;
	float Magnitude() const;
	void Normalize();
	class Rotator GetRotator() const;
	VectorI GetNormalize() const;
	VectorI Dot(const VectorI& other) const;
	VectorI Cross(const VectorI& other) const;
	VectorI Lerp(const VectorI& other, int32_t percentage) const;
	VectorI MidpointTo(const VectorI& other) const;
	float DistanceTo(const VectorI& other, float scaling = 1.0f) const;

public:
	VectorI& operator=(const VectorI& other);
	VectorI& operator=(const struct FVector& other);
};

inline VectorI operator+(const VectorI& a, const VectorI& b) { return VectorI(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
inline VectorI operator-(const VectorI& a, const VectorI& b) { return VectorI(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline VectorI operator*(const VectorI& a, const VectorI& b) { return VectorI(a.X * b.X, a.Y * b.Y, a.Z * b.Z); }
inline VectorI operator/(const VectorI& a, const VectorI& b) { return VectorI(a.X / b.X, a.Y / b.Y, a.Z / b.Z); }
inline VectorI operator+(const VectorI& a, int32_t b) { return VectorI(a.X + b, a.Y + b, a.Z + b); }
inline VectorI operator-(const VectorI& a, int32_t b) { return VectorI(a.X - b, a.Y - b, a.Z - b); }
inline VectorI operator*(const VectorI& a, int32_t b) { return VectorI(a.X * b, a.Y * b, a.Z * b); }
inline VectorI operator/(const VectorI& a, int32_t b) { return VectorI(a.X / b, a.Y / b, a.Z / b); }

class Rotator
{
public:
	int32_t Pitch, Yaw, Roll;

public:
	Rotator();
	Rotator(int32_t pyr);
	Rotator(int32_t pitch, int32_t yaw, int32_t roll);
	Rotator(const struct FRotator& other);
	Rotator(const Rotator& other);
	~Rotator();

public:
	struct FRotator UnrealRotator(bool bURR) const;
	Rotator& FromUnrealRotator(const struct FRotator& other);

public:
	void Erase();
	Rotator Copy() const;
	float ClampAxis(float a);
	Rotator& Normalize();
	float NormalizeAxis(float a);
	class Quat GetQuat() const;
	Rotator GetNormalize() const;
	VectorF GetVector() const;
	VectorF Rotate(VectorF other) const; // Credit to @FrancesElMute/Aberinkula/Megasplat for this rotate function.

public:
	Rotator& operator+=(const Rotator& other);
	Rotator& operator-=(const Rotator& other);
	Rotator& operator*=(const Rotator& other);
	Rotator& operator/=(const Rotator& other);
	Rotator& operator+=(int32_t other);
	Rotator& operator-=(int32_t other);
	Rotator& operator*=(int32_t other);
	Rotator& operator/=(int32_t other);
	Rotator& operator=(const Rotator& other);
	Rotator& operator=(const struct FRotator& other);
	Rotator& operator=(int32_t other);
	bool operator==(const Rotator& other) const;
	bool operator==(const struct FRotator& other) const;
	bool operator!=(const Rotator& other) const;
	bool operator!=(const struct FRotator& other) const;
};

inline Rotator operator+(const Rotator& a, const Rotator& b) { return Rotator(a.Pitch + b.Pitch, a.Yaw + b.Yaw, a.Roll + b.Roll); }
inline Rotator operator-(const Rotator& a, const Rotator& b) { return Rotator(a.Pitch - b.Pitch, a.Yaw - b.Yaw, a.Roll - b.Roll); }
inline Rotator operator*(const Rotator& a, const Rotator& b) { return Rotator(a.Pitch * b.Pitch, a.Yaw * b.Yaw, a.Roll * b.Roll); }
inline Rotator operator/(const Rotator& a, const Rotator& b) { return Rotator(a.Pitch / b.Pitch, a.Yaw / b.Yaw, a.Roll / b.Roll); }
inline Rotator operator+(const Rotator& a, int32_t b) { return Rotator(a.Pitch + b, a.Yaw + b, a.Roll + b); }
inline Rotator operator-(const Rotator& a, int32_t b) { return Rotator(a.Pitch - b, a.Yaw - b, a.Roll - b); }
inline Rotator operator*(const Rotator& a, int32_t b) { return Rotator(a.Pitch * b, a.Yaw * b, a.Roll * b); }
inline Rotator operator/(const Rotator& a, int32_t b) { return Rotator(a.Pitch / b, a.Yaw / b, a.Roll / b); }
inline extern VectorF Rotate(VectorF point, const Rotator& rotation, const VectorF& location);

class Quat
{
public:
	float X, Y, Z, W;

public:
	Quat();
	Quat(float xyzw);
	Quat(float x, float y, float z, float w);
	Quat(const struct FQuat& other);
	Quat(const Quat& other);
	~Quat();

public:
	struct FQuat UnrealQuat() const;

public:
	void Erase();
	Quat Copy() const;
	bool Equals(const Quat& q, float tolerance = SMALL_NUMBER) const;
	bool IsIdentity(float tolerance = SMALL_NUMBER) const;
	bool IsNormalized() const;
	float Size() const;
	float SizeSquared() const;
	float GetAngle() const;
	VectorF GetAxisX() const;
	VectorF GetAxisY() const;
	VectorF GetAxisZ() const;
	VectorF GetRotationAxis() const;
	Quat Conjugate() const;
	Quat GetInverse() const;
	void Normalize(float tolerance = SMALL_NUMBER);
	Quat GetNormalize(float tolerance = SMALL_NUMBER) const;
	Rotator GetRotator() const;
	VectorF Rotate(const VectorF& other) const;

public:
	Quat& operator+=(const Quat& other);
	Quat& operator-=(const Quat& other);
	Quat& operator*=(const Quat& other);
	Quat& operator/=(const Quat& other);
	Quat& operator+=(float other);
	Quat& operator-=(float other);
	Quat& operator*=(float other);
	Quat& operator/=(float other);
	Quat& operator=(const Quat& other);
	Quat& operator=(const struct FQuat& other);
	Quat& operator=(float other);
	bool operator==(const Quat& other) const;
	bool operator==(const struct FQuat& other) const;
	bool operator!=(const Quat& other) const;
	bool operator!=(const struct FQuat& other) const;
};

inline Quat operator+(const Quat& a, const Quat& b) { return Quat(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W); }
inline Quat operator-(const Quat& a, const Quat& b) { return Quat(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W); }
inline Quat operator*(const Quat& a, const Quat& b) { return Quat(a.X * b.X, a.Y * b.Y, a.Z * b.Z, a.W * b.W); }
inline Quat operator/(const Quat& a, const Quat& b) { return Quat(a.X / b.X, a.Y / b.Y, a.Z / b.Z, a.W / b.W); }
inline Quat operator+(const Quat& a, float b) { return Quat(a.X + b, a.Y + b, a.Z + b, a.W + b); }
inline Quat operator-(const Quat& a, float b) { return Quat(a.X - b, a.Y - b, a.Z - b, a.W - b); }
inline Quat operator*(const Quat& a, float b) { return Quat(a.X * b, a.Y * b, a.Z * b, a.W * b); }
inline Quat operator/(const Quat& a, float b) { return Quat(a.X / b, a.Y / b, a.Z / b, a.W / b); }