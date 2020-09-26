#define MAX_STRING_LEN 64
class Rect {
public:
	float x; // 0x10
	float y; // 0x14
	float wid; // 0x18
	float hei; // 0x1C
	Rect(float x, float y/*top left*/, float x_rightsize, float y_downsize) {
		this->x = x;
		this->y = y;
		wid = x_rightsize;
		hei = y_downsize;
	}
	Rect() {
		this->x = 0;
		this->y = 0;
		wid = 0;
		hei = 0;
	}
	bool Contains(Vector2 point)
	{
		return point.x >= x && point.x < (x + wid) && point.y >= y && point.y < (y + hei);
	}
};
class Color {
public:
	float r;
	float g;
	float b;
	float a;
	Color(float rr, float gg, float bb, float aa) {
		r = rr;
		g = gg;
		b = bb;
		a = aa;
	}
};
class Str {
	char zpad[0x10];
public:
	int size;
	wchar_t str[MAX_STRING_LEN + 1];
	Str(const wchar_t* st) {
		size = min(m_wcslen((wchar_t*)st), MAX_STRING_LEN);
		for (int i = 0; i < size; i++) {
			str[i] = st[i];
		}
		str[size] = 0;
	}
};

enum class EventType : int {
	MouseDown = 0,
	MouseUp = 1,
	MouseDrag = 3,
	KeyDown = 4,
	KeyUp = 5,
	Repaint = 7
};

enum class KeyCode : int {
	Insert = 0x115,
	End = 0x117,
};

struct SliderData {
	float IO;
	Vector2 fillPos;
};