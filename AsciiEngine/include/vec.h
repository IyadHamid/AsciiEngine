#pragma once

template <typename T = double> struct vec2;
template <typename T = double> struct vec3;
template <typename T = double> struct vec4;
template <typename T = double> struct mat4;

template <typename T> struct vec2 {
	T x, y;

	vec2<T>() : x{ (T)0 }, y{ (T)0 } {}
	vec2<T>(T a, T b) : x{ a }, y{ b } {}

	vec2<T> operator+(vec2<T> other) { return vec2<T>{ x + other.x, y + other.y }; }
	vec2<T> operator-(vec2<T> other) { return vec2<T>{ x - other.x, y - other.y }; }

	vec2<T> operator*(T other) { return vec2<T>{ x * other, y * other }; }
	vec2<T> operator/(T other) { return vec2<T>{ x / other, y / other }; }

	//Dot product
	T operator*(vec2<T> other) { return vec2<T>{ x * other.x, y * other.y }.sum(); }

	void operator+=(vec2<T> other) { *this = *this + other; }
	void operator-=(vec2<T> other) { *this = *this - other; }

	T sum() { return x + y; }
	double magnitude() { return sqrt(x * x + y * y);  }
	vec2<T> normalize() { return *this / (T)magnitude(); }

	template <typename S> vec2<S> cast() { return vec2<S>{ (S)x, (S)y }; }
};

template <typename T> struct vec3 {
	T x, y, z;

	vec3<T>() : x{ (T)0 }, y{ (T)0 }, z{ (T)0 } {}
	vec3<T>(T a, T b, T c) : x{ a }, y{ b }, z{ c } {}
	vec3<T>(vec2<T> a, T b) : x{ a.x }, y{ a.y }, z{ b } {}

	vec3<T> operator+(vec3<T> other) { return vec3<T>{ x + other.x, y + other.y, z + other.z }; }
	vec3<T> operator-(vec3<T> other) { return vec3<T>{ x - other.x, y - other.y, z - other.z }; }

	vec3<T> operator*(T other) { return vec3<T>{ x * other, y * other, z * other }; }
	vec3<T> operator/(T other) { return vec3<T>{ x / other, y / other, z / other }; }

	//Dot product
	T operator*(vec3<T> other) { return vec3<T>{ x * other.x, y * other.y, z * other.z }.sum(); }
	//Cross product
	vec3<T> cross(vec3<T> other) { return vec3<T>{ y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x }; };

	void operator+=(vec3<T> other) { *this = *this + other; }
	void operator-=(vec3<T> other) { *this = *this - other; }

	T sum() { return x + y + z; }
	double magnitude() { return sqrt(x * x + y * y + z * z); }
	vec3<T> normalize() { return *this / (T)magnitude(); }

	template <typename S> vec3<S> cast() { return vec3<S>{ (S)x, (S)y, (S)z }; }
};

template <typename T> struct vec4 {
	T x, y, z, w;

	vec4<T>() : x{ (T)0 }, y{ (T)0 }, z{ (T)0 }, w{ (T)0 } {}
	vec4<T>(T a, T b, T c, T d) : x{ a }, y{ b }, z{ c }, w{ d } {}
	vec4(vec3<T> a, T b) : x{ a.x }, y{ a.y }, z{ a.z }, w{ b } {}

	vec4<T> operator+(vec4<T> other) { return vec4<T>{ x + other.x, y + other.y, z + other.z, w + other.w }; }
	vec4<T> operator-(vec4<T> other) { return vec4<T>{ x - other.x, y - other.y, z - other.z, w - other.w }; }

	vec4<T> operator*(T other) { return vec4<T>{ x * other, y * other, z * other, w * other }; }
	vec4<T> operator/(T other) { return vec4<T>{ x / other, y / other, z / other, w / other }; }

	//Dot product
	T operator*(vec4<T> other) { return vec4<T>{ x * other.x, y * other.y, z * other.z, w * other.w }.sum(); }

	void operator+=(vec4<T> other) { *this = *this + other; }
	void operator-=(vec4<T> other) { *this = *this - other; }

	//mat4<T> operator*(mat4<T> other) { return mat4<T>{ vec4<T>{ (other.x() * x).sum(), (other.y() * x).sum(), (other.z() * x).sum(), (other.w() * x).sum() },
	//												   vec4<T>{ (other.x() * y).sum(), (other.y() * y).sum(), (other.z() * y).sum(), (other.w() * y).sum() },
	//												   vec4<T>{ (other.x() * z).sum(), (other.y() * z).sum(), (other.z() * z).sum(), (other.w() * z).sum() },
	//												   vec4<T>{ (other.x() * w).sum(), (other.y() * w).sum(), (other.z() * w).sum(), (other.w() * w).sum() } }; }

	T sum() { return x + y + z + w; }
	double magnitude() { return sqrt(x * x + y * y + z * z + w * w); }
	vec4<T> normalize() { return *this / (T)magnitude(); }

	template <typename S> vec4<S> cast() { return vec4<S>{ (S)x, (S)y, (S)z, (S)w }; }
};

template <typename T> struct mat4 {
	vec4<T> a, b, c, d;

	mat4(vec4<T> a, vec4<T> b, vec4<T> c, vec4<T> d) : a{ a }, b{ b }, c{ c }, d{ d } {}

	vec4<T> x() { return vec4<T>{ a.x, b.x, c.x, d.x }; }
	vec4<T> y() { return vec4<T>{ a.y, b.y, c.y, d.y }; }
	vec4<T> z() { return vec4<T>{ a.z, b.z, c.z, d.z }; }
	vec4<T> w() { return vec4<T>{ a.w, b.w, c.w, d.w }; }

	mat4<T> operator+(mat4<T> other) { return mat4<T>{ a + other.a, b + other.b, c + other.c, d + other.d }; }
	mat4<T> operator-(mat4<T> other) { return mat4<T>{ a - other.a, b - other.b, c - other.c, d - other.d }; }

	mat4<T> operator*(mat4<T> other) { return mat4<T>{ vec4<T>{ a * other.x(), a * other.y(), a * other.z(), a * other.w() },
											           vec4<T>{ b * other.x(), b * other.y(), b * other.z(), b * other.w() },
											           vec4<T>{ c * other.x(), c * other.y(), c * other.z(), c * other.w() },
											           vec4<T>{ d * other.x(), d * other.y(), d * other.z(), d * other.w() } }; }
	//Treats vector as a 4x1 vector
	vec4<T> operator*(vec4<T> other) { return vec4<T>{ a * other, b * other, c * other, d * other }; }
};