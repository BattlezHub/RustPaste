#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

float sqrtn(const float& x)
{
	union
	{
		int i;
		float x;
	} u;
	u.x = x;
	u.i = (1 << 29) + (u.i >> 1) - (1 << 22);

	u.x = u.x + x / u.x;
	u.x = 0.25f * u.x + x / u.x;

	return u.x;
}

//DWORD64 oCos = 0; DWORD64 oAsin = 0; DWORD64 oAtan2 = 0; DWORD64 oSin = 0;
typedef double(__stdcall* COS)(double);
typedef double(__stdcall* ASIN)(double);
typedef double(__stdcall* ATAN2)(double, double);
double m_asin(double d) {
	return ((ASIN)(g_Base + O::System_Math::Asin))(d);
}
double m_atan2(double y, double x) {
	return ((ATAN2)(g_Base + O::System_Math::Atan2))(y, x);
}
double m_sin(double d) {
	return ((COS)(g_Base + O::System_Math::Sin))(d);
}
double m_cos(double d) {
	return ((COS)(g_Base + O::System_Math::Cos))(d);
}
namespace Math {
#define M_PI 3.14159265358979323846f
#define M_RADPI	57.295779513082f
#define M_PI_F ((float)(M_PI))
#define RAD2DEG(x) ((float)(x) * (float)(180.f / M_PI_F))
#define DEG2RAD(x) ((float)(x) * (float)(M_PI_F / 180.f))
#define pow(n) (n)*(n)

float abs(float a) {
		if (a < 0.f) return -a;
		else return a;
	}

Vector3 Cross(Vector3 lhs, Vector3 rhs)
	{
		return Vector3(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x);
	}

float Dot(const Vector3& Vec1, const Vector3& Vec2) {
		return Vec1[0] * Vec2[0] + Vec1[1] * Vec2[1] + Vec1[2] * Vec2[2];
	}

float Calc3D_Dist(const Vector3& Src, const Vector3& Dst) {
		return sqrtn(pow(Src.x - Dst.x) + pow(Src.y - Dst.y) + pow(Src.z - Dst.z));
	}

float Calc2D_Dist(const Vector2& Src, const Vector2& Dst) {
		return sqrtn(pow(Src.x - Dst.x) + pow(Src.y - Dst.y));
	}

Vector2 CalcAngle(const Vector3& Src, const Vector3& Dst) {
		Vector3 dir = Src - Dst;
		return Vector2{ RAD2DEG(m_asin(dir.y / dir.Length())), RAD2DEG(-m_atan2(dir.x, -dir.z)) };
	}


}