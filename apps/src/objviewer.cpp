/*
NOTE THIS PROJECT IS NOT WORKING DUE TO MATRICIES.
THIS FILE HAS BEEN DISABLED FROM CMAKE
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "AsciiRender.h"

using namespace std;

#define PI 3.141592654
#define deg2rad(A) (A * PI/180.0)

mat4<double> view(vec3<double> eye, vec2<double> rot) {
	//Look out function
	vec3 center{0.0, 0.0, 0.0};

	vec3 f = (center - eye).normalize();
	vec3 u = vec3{ 0.0, 0.0, 1.0 }.normalize();
	vec3 r = (f.cross(u)).normalize();
	u = r.cross(f);
	return mat4( vec4{ r.x       , u.x            , -f.x     , 0.0 },
			     vec4{ r.y       , u.y            , -f.x     , 0.0 },
			     vec4{ r.z       , u.z            , -f.x     , 0.0 },
			     vec4{ -(r * eye), -(u * eye)     , (f * eye), 0.0 } );
	//return mat4( vec4{ 6.0, 0.0, 0.0, 10.0 },
	//		     vec4{ 0.0, 6.0, 0.0, 10.0 },
	//		     vec4{ 0.0, 0.0, 6.0, 0.0 },
	//		     vec4{ 0.0, 0.0, 0.0, 1.0 } );
}

mat4<double> proj(vec2<double> res, double fov, double n, double f) {
	//Perspective projection matrix
	//Screen coordinates
	double b, t, l, r;
	double scale = tan(fov * 0.5) * n;
	r = (res.x / res.y) * scale;
	l = -r;
	t = scale;
	b = -t;
	return mat4(vec4{ (2 * n) / (r - l), 0.0              ,  (r + l) / (r - l), 0.0 },
				vec4{ 0.0              , (2 * n) / (t - b),  (t + b) / (t - b), 0.0 },
				vec4{ 0.0              , 0.0              , -(f + n) / (f - n), (-2 * f * n) / (f - n) },
				vec4{ 0.0              , 0.0              , -1.0              , 0.0 });
}

int main() {

	vec2<int> res{ 100, 100 };
	//vec3 eye{ 7.359, -6.926, 4.958 };
	vec3<double> eye{ 5.0, 5.0, 0.0 };
	//vec3 eye{ 0, 0, 0 };
	//vec2 rot{ 63.6, 46.7 }; //Pitch, yaw (using FPS camera)
	vec2 rot{ 0.0, 0.0 };
	double fov = 70.0;

	rot = vec2(deg2rad(rot.x), deg2rad(rot.y));
	fov = deg2rad(fov);
	ifstream fin("../../../cube.obj", ios::in);
	string line;
	vector<vec3<double>> verticies, normalizes;
	vector<vector<vec3<int>>> faces;

	while (getline(fin, line)) {
		istringstream ss(line);
		string inst;
		ss >> inst;
		if (inst == "v") {
			vec3 v;
			ss >> v.x >> v.y >> v.z;
			verticies.push_back(v);
		}
		else if (inst == "vn") {
			vec3 vn;
			ss >> vn.x >> vn.y >> vn.z;
			normalizes.push_back(vn);
		}
		else if (inst == "f") {
			vec3<int> f;
			vector<vec3<int>> triangles;
			string seg;
			while (ss >> seg) {
				//Parses seg (eg: '2/4/6')
				istringstream segStream(seg), numStream;
				string num;
				vec3<int> triangle;
				getline(segStream, num, '/');
				numStream = istringstream(num);
				numStream >> triangle.x;
				getline(segStream, num, '/');
				numStream = istringstream(num);
				numStream >> triangle.y;
				getline(segStream, num, '/');
				numStream = istringstream(num);
				numStream >> triangle.z;
				triangle = triangle - vec3<int>{ 1, 1, 1 }; //All indicies are 1 off
				triangles.push_back(triangle);
			}
			faces.push_back(triangles);
		}
	}
	fin.close();

	mat4 viewMat = view(eye.cast<double>(), rot);
	mat4 projMat = proj(res.cast<double>(), fov, .1, 100);
	
	mat4 scale{ vec4{ 6.0, 0.0, 0.0, 1.0 },
				vec4{ 0.0, 6.0, 0.0, 20.0 },
				vec4{ 0.0, 0.0, 6.0, 0.0 },
				vec4{ 0.0, 0.0, 0.0, 1.0 } };

	mat4 finalMat = viewMat * projMat;


	AsciiRender ar(res); int i = 0;
	for (vector<vec3<int>> face : faces) {
		vector<vec2<int>> polyVerticies;
		for (vec3<int> vertex : face) {
			vec4 result;
			result = finalMat * vec4(verticies[vertex.x], 1.0);
			vec2 pt = { result.x / result.w, result.y / result.w };
			pt = pt + vec2{ .5, .5 };
			pt = pt * 100;
			polyVerticies.push_back(pt.cast<int>());
			cout << pt.x << " " << pt.y << endl;
		}
		char* a = "012345";
		ar.fillPolygon(polyVerticies, a[i]);

		i++;
	}

	cout << ar;
	cin >> eye.x;
	cin >> eye.y;
	cin >> eye.z;
	return 0;
}
