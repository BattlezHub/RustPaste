
float sqrtn(const float& x);
class Vector3
{
public:
	float x, y, z;

	Vector3() {
		x = y = z = 0.0f;
	}

	Vector3(float X, float Y, float Z) {
		x = X; y = Y; z = Z;
	}

	float operator[](int i) const {
		return ((float*)this)[i];
	}

	Vector3& operator-=(float v) {
		x -= v; y -= v; z -= v; return *this;
	}

	

	Vector3 operator*(float v) const {
		return Vector3(x * v, y * v, z * v);
	}

	Vector3 operator/(float v) const
	{
		return Vector3(x / v, y / v, z / v);
	}

	Vector3& operator+=(const Vector3& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}

	Vector3 operator-(const Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	bool operator==(const Vector3& b) const {
		return x == b.x && y == b.y && z == b.z;
	}
	Vector3& operator-=(const Vector3& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}
	Vector3 operator+(const Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	float Length() {
		return sqrtn(x * x + y * y + z * z);
	}
	Vector3 Normalized() {
		float len = Length();
		return Vector3(x / len, y / len, z / len);
	}
};

class Vector2 {
public:
	float x, y;

	Vector2() {
		x = y = 0.0f;
	}

	Vector2 operator*(float v) const {
		return Vector2(x * v, y * v);
	}

	Vector2 operator/(float v) const {
		return Vector2(x / v, y / v);
	}
	Vector2(float X, float Y) {
		x = X; y = Y;
	}

	Vector2 operator-(const Vector2& v) const {
		return Vector2(x - v.x, y - v.y);
	}

	Vector2 operator+(const Vector2& v) const {
		return Vector2(x + v.x, y + v.y);
	}

	Vector2& operator+=(const Vector2& v) {
		x += v.x; y += v.y; return *this;
	}

	bool Zero() const {
		return (x > -0.1f && x < 0.1f && y > -0.1f && y < 0.1f);
	}

	float Length() {
		return sqrtn(x * x + y * y);
	}


	void Normalize() {
		float len = Length();
		this->x /= len;
		this->y /= len;
	}

private:
	float sqrLength() {
		return (x * x + y * y);
	}

};

class Vector4 {
public:
	float x, y, z, w;
};
struct Matrix4x4 {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		}; float m[4][4];
	};
};

enum class Button : int {
	FORWARD = 2,
	BACKWARD = 4,
	LEFT = 8,
	RIGHT = 16,
	JUMP = 32,
	DUCK = 64,
	SPRINT = 128,
	USE = 256,
	FIRE_PRIMARY = 1024,
	FIRE_SECONDARY = 2048,
	RELOAD = 8192,
	FIRE_THIRD = 134217728
};

struct Ray {
	Vector3 origin;
	Vector3 dir;
	Ray(Vector3 o, Vector3 d) {
		origin = o;
		dir = d;
	}
	Ray() {}
};

struct TraceInfo {

	bool valid; // 0x10
	float distance; // 0x14
	DWORD64 entity; // 0x18   //BasePlayer
	Vector3 point; // 0x20
	Vector3 normal; // 0x2C
	DWORD64/*Transform*/ bone; // 0x38
	DWORD64 material; // 0x40
	unsigned int partID; // 0x48
	DWORD64 collider; // 0x50
};