#include "../pch.hpp"

#define UCONST_Pi 3.1415926
#define URotation180 32768 
#define URotationToRadians UCONST_Pi / URotation180 

namespace Math
{
	float GetDistance(const struct FVector& from, const struct FVector& to, const float scaling);
	struct FVector Rotate(const struct FVector& point, const struct FRotator& rotation, const struct FVector& location);
}

struct CRVector
{
public:
	float X, Y, Z;

public:
	CRVector();
	CRVector(const float xyz);
	CRVector(const float x, const float y, const float z);
	~CRVector();

public:
	struct FVector UnrealVector();

public:
	struct CRVector operator+=(const struct CRVector& other);
	struct CRVector operator-=(const struct CRVector& other);
	struct CRVector operator*=(const struct CRVector& other);
	struct CRVector operator/=(const struct CRVector& other);
	struct CRVector operator+=(const float other);
	struct CRVector operator-=(const float other);
	struct CRVector operator*=(const float other);
	struct CRVector operator/=(const float other);
	struct CRVector operator=(const struct CRVector& other);
	struct CRVector operator=(const struct FVector& other);
	struct CRVector operator=(const float other);
	bool operator==(const struct CRVector& other);
	bool operator==(const struct FVector& other);
	bool operator!=(const struct CRVector& other);
	bool operator!=(const struct FVector& other);

public:
	void Erase();
	struct CRVector Copy() const;
	float Magnitude() const;
	struct CRVector Normalize();
	struct CRVector NormalizeMutable() const;
	struct CRVector Dot(const struct CRVector& other) const;
	struct CRVector Cross(const struct CRVector& other) const;
	struct CRVector Lerp(const struct CRVector& other, const float percentage) const;
};

inline struct CRVector operator+(const struct CRVector& a, const struct CRVector& b) { return CRVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
inline struct CRVector operator-(const struct CRVector& a, const struct CRVector& b) { return CRVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline struct CRVector operator*(const struct CRVector& a, const struct CRVector& b) { return CRVector(a.X * b.X, a.Y * b.Y, a.Z * b.Z); }
inline struct CRVector operator/(const struct CRVector& a, const struct CRVector& b) { return CRVector(a.X / b.X, a.Y / b.Y, a.Z / b.Z); }
inline struct CRVector operator+(const struct CRVector& a, const float b) { return CRVector(a.X + b, a.Y + b, a.Z + b); }
inline struct CRVector operator-(const struct CRVector& a, const float b) { return CRVector(a.X - b, a.Y - b, a.Z - b); }
inline struct CRVector operator*(const struct CRVector& a, const float b) { return CRVector(a.X * b, a.Y * b, a.Z * b); }
inline struct CRVector operator/(const struct CRVector& a, const float b) { return CRVector(a.X / b, a.Y / b, a.Z / b); }

struct CRVector2D
{
public:
	float X, Y;

public:
	CRVector2D();
	CRVector2D(const float xy);
	CRVector2D(const float x, const float y);
	~CRVector2D();

public:
	struct FVector2D UnrealVector();

public:
	struct CRVector2D operator+=(const struct CRVector2D& other);
	struct CRVector2D operator-=(const struct CRVector2D& other);
	struct CRVector2D operator*=(const struct CRVector2D& other);
	struct CRVector2D operator/=(const struct CRVector2D& other);
	struct CRVector2D operator+=(const float other);
	struct CRVector2D operator-=(const float other);
	struct CRVector2D operator*=(const float other);
	struct CRVector2D operator/=(const float other);
	struct CRVector2D operator=(const struct CRVector2D& other);
	struct CRVector2D operator=(const struct FVector2D& other);
	struct CRVector2D operator=(const float other);
	bool operator==(const struct CRVector2D& other);
	bool operator==(const struct FVector2D& other);
	bool operator!=(const struct CRVector2D& other);
	bool operator!=(const struct FVector2D& other);

public:
	void Erase();
	struct CRVector2D Copy() const;
};

inline struct CRVector2D operator+(const struct CRVector2D& a, const struct CRVector2D& b) { return CRVector2D(a.X + b.X, a.Y + b.Y); }
inline struct CRVector2D operator-(const struct CRVector2D& a, const struct CRVector2D& b) { return CRVector2D(a.X - b.X, a.Y - b.Y); }
inline struct CRVector2D operator*(const struct CRVector2D& a, const struct CRVector2D& b) { return CRVector2D(a.X * b.X, a.Y * b.Y); }
inline struct CRVector2D operator/(const struct CRVector2D& a, const struct CRVector2D& b) { return CRVector2D(a.X / b.X, a.Y / b.Y); }
inline struct CRVector2D operator+(const struct CRVector2D& a, const float b) { return CRVector2D(a.X + b, a.Y + b); }
inline struct CRVector2D operator-(const struct CRVector2D& a, const float b) { return CRVector2D(a.X - b, a.Y - b); }
inline struct CRVector2D operator*(const struct CRVector2D& a, const float b) { return CRVector2D(a.X * b, a.Y * b); }
inline struct CRVector2D operator/(const struct CRVector2D& a, const float b) { return CRVector2D(a.X / b, a.Y / b); }

// Credit to @FrancesElMute/Aberinkula/Megasplat for the rotate function, was pulling my hair out tyring to figure it out on my own.

struct CRRotator
{
public:
	int32_t Pitch, Yaw, Roll;

public:
	CRRotator();
	CRRotator(const int32_t pyr);
	CRRotator(const int32_t pitch, const int32_t yaw, const int32_t roll);
	~CRRotator();

public:
	struct FRotator UnrealRotator();

public:
	struct CRRotator operator+=(const struct CRRotator& other);
	struct CRRotator operator-=(const struct CRRotator& other);
	struct CRRotator operator*=(const struct CRRotator& other);
	struct CRRotator operator/=(const struct CRRotator& other);
	struct CRRotator operator+=(const int32_t other);
	struct CRRotator operator-=(const int32_t other);
	struct CRRotator operator*=(const int32_t other);
	struct CRRotator operator/=(const int32_t other);
	struct CRRotator operator=(const struct CRRotator& other);
	struct CRRotator operator=(const struct FRotator& other);
	struct CRRotator operator=(const int32_t other);
	bool operator==(const struct CRRotator& other);
	bool operator==(const struct FRotator& other);
	bool operator!=(const struct CRRotator& other);
	bool operator!=(const struct FRotator& other);

public:
	void Erase();
	struct CRRotator Copy() const;
	struct CRVector Rotate(const struct CRVector& point, const struct CRVector& location) const;
};

inline struct CRRotator operator+(const struct CRRotator& a, const struct CRRotator& b) { return CRRotator(a.Pitch + b.Pitch, a.Yaw + b.Yaw, a.Roll + b.Roll); }
inline struct CRRotator operator-(const struct CRRotator& a, const struct CRRotator& b) { return CRRotator(a.Pitch - b.Pitch, a.Yaw - b.Yaw, a.Roll - b.Roll); }
inline struct CRRotator operator*(const struct CRRotator& a, const struct CRRotator& b) { return CRRotator(a.Pitch * b.Pitch, a.Yaw * b.Yaw, a.Roll * b.Roll); }
inline struct CRRotator operator/(const struct CRRotator& a, const struct CRRotator& b) { return CRRotator(a.Pitch / b.Pitch, a.Yaw / b.Yaw, a.Roll / b.Roll); }
inline struct CRRotator operator+(const struct CRRotator& a, const int32_t b) { return CRRotator(a.Pitch + b, a.Yaw + b, a.Roll + b); }
inline struct CRRotator operator-(const struct CRRotator& a, const int32_t b) { return CRRotator(a.Pitch - b, a.Yaw - b, a.Roll - b); }
inline struct CRRotator operator*(const struct CRRotator& a, const int32_t b) { return CRRotator(a.Pitch * b, a.Yaw * b, a.Roll * b); }
inline struct CRRotator operator/(const struct CRRotator& a, const int32_t b) { return CRRotator(a.Pitch / b, a.Yaw / b, a.Roll / b); }