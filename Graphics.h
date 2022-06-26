#pragma once

#include <vector>
#include <Windows.h>

#include "Geometry.h"

/*
  - Graphics math header
  - Basic color, shader, material, mesh math, rendering tools
  - By: EgorDeveloper
  -
  - Graphics.h:
  - Contains realisations for Color, Material, Triangle, Mesh
  -
  - Dependencies:
  - Geometry.h
*/

typedef struct Color {
private:
	unsigned char ClampColor(long value) {
		if (value < 0) { return 0; }
		return value > 255 ? 255 : (unsigned char)value;
	}
public:
	unsigned char r, g, b;

	Color() { r = 0; g = 0; b = 0; }
	Color(Vector3 fromVector3) { r = (unsigned char)fromVector3.x; g = (unsigned char)fromVector3.y; b = (unsigned char)fromVector3.z; }
	Color(COLORREF fromColorRef) { b = (fromColorRef >> 16) & 0xFF; g = (fromColorRef >> 8) & 0xFF; r = fromColorRef & 0xFF; }
	Color(long R, long G, long B) { r = ClampColor(R); g = ClampColor(G); b = ClampColor(B); }

	Color operator*(const float& value) { return Color((long)(value * r), (long)(value * g), (long)(value * b)); }
	Color operator/(const float& value) { return Color((long)(value / r), (long)(value / g), (long)(value / b)); }
	Color operator+(const Color& second) { return Color((long)r + (long)second.r, (long)g + (long)second.g, (long)b + (long)second.b); }
	Color operator-(const Color& second) { return Color((long)r - (long)second.r, (long)g - (long)second.g, (long)b - (long)second.b); }

	///<summary>
	///Returns Color, which is linear interpolated between two given Colors with parameter t = [0; 1]
	///</summary>
	static Color Lerp(Color a, Color b, float t) { return Color(Vector3::Lerp(a.toVector3(), b.toVector3(), t)); }
	///<summary>
	///Invertes current color instance
	///</summary>
	void invert(void) { r = ~r; g = ~g; b = ~b; }
	///<summary>
	///Returns inverted variant of this Color
	///</summary>
	Color Inverted(void) const { return Color(~r, ~g, ~b); }
	///<summary>
	///Converts current Color (R, G, B) to Vector3 (X, Y, Z)
	///</summary>
	Vector3 toVector3(void) const { return Vector3(r, g, b); }
} Color;

class Material {
public:
	enum Shader {
		unlit = 0,
		diffuse = 1,
		realistic = 2,
		faceorient = 3
	};
	///<summary>
	///Shader of this material. Describes how to render an object
	///</summary>
	Shader shader;
	///<summary>
	///In range [0; 1]. If using Unlit shader, this value is ignored
	///</summary>
	float metallic, roughness;
	///<summary>
	///Metal color lerp to. Grey by default
	///</summary>
	Color metal;
	///<summary>
	///Face orientation indicator color lerp to. Used only in faceorient shader
	///</summary>
	Color facefront = Color(65, 93, 255), faceback = Color(255, 40, 62);
	///<summary>
	///Face orientation t parameter for colors. Used only in faceorient shader
	///</summary>
	float faceorientfactor = 0.9f;

	Material() { shader = Shader::unlit; metallic = 0; roughness = 0; metal = Color(154, 160, 161); }
	Material(Shader shaderType) { shader = shaderType; metallic = 0; roughness = 0.8f; metal = Color(154, 160, 161); }
	Material(Shader shaderType, float Metallic, float Roughness) { shader = shaderType; metallic = Metallic * 0.75f; roughness = Roughness; metal = Color(108, 107, 117); }
};

typedef struct Vertex3 {
	Vector3 position;
	Color color;

	Vertex3() { position = Vector3(); color = Color(); }
	Vertex3(const Vector3& pos) { position = pos; color = Color(); }
	Vertex3(const Vector3& pos, const Color& col) { position = pos, color = col; }
} Vertex3;

typedef struct Triangle {
	Vertex3 a, b, c;

	Triangle(const Vertex3& A, const Vertex3& B, const Vertex3& C) { a = A, b = B, c = C; }
	Triangle(const Vector3& A, const Vector3& B, const Vector3& C, const Color& color) { a = Vertex3(A, color), b = Vertex3(B, color), c = Vertex3(C, color); }

	///<summary>
	///Returns normal vector of current triangle
	///</summary>
	Vector3 Normal(void) const {
		float vx1 = a.position.x - b.position.x, vx2 = b.position.x - c.position.x;
		float vy1 = a.position.y - b.position.y, vy2 = b.position.y - c.position.y;
		float vz1 = a.position.z - b.position.z, vz2 = b.position.z - c.position.z;

		return Vector3(vy1 * vz2 - vz1 * vy2, vz1 * vx2 - vx1 * vz2, vx1 * vy2 - vy1 * vx2).Normal();
	}
	///<summary>
	///Sets all vertex colors to given one
	///</summary>
	void SetColor(const Color& color) {
		a.color = color; b.color = color; c.color = color;
	}
	///<summary>
	///Returns center point of current triangle
	///</summary>
	Vector3 Center(void) const {
		return Vector3(
			a.position.x + b.position.x + c.position.x,
			a.position.y + b.position.y + c.position.y,
			a.position.z + b.position.z + c.position.z
		) / 3.0f;
	}
} Triangle;

class Mesh {
private:
	
public:
	///<summary>
	///List of all mesh vertices
	///</summary>
	std::vector<Vector3> vertices;
	///<summary>
	///List of mesh triangles. Triangles are defined as 3-pair indexes to vertices
	///</summary>
	std::vector<unsigned> triangles;

	Mesh() { vertices = {}; triangles = {}; }
	Mesh(const std::vector<Vector3>& vertexList, const std::vector<unsigned>& triangleList) { vertices = vertexList; triangles = triangleList; }


	Mesh operator+(const Mesh& second) {
		Mesh addCombined = Mesh(vertices, triangles);

		const unsigned szCurVerts = (unsigned)vertices.size();
		const size_t szAddTris = second.triangles.size();

		addCombined.vertices.insert(addCombined.vertices.begin(), second.vertices.begin(), second.vertices.end());

		for (size_t i = 0; i < szAddTris; ++i) {
			addCombined.triangles.push_back(second.triangles[i] + szCurVerts);
		}
		return addCombined;
	}

	///<summary>
	///Clears all vertices and triangles lists
	///</summary>
	void Clear(void) { vertices.clear(); triangles.clear(); }
	///<summary>
	///Shifts current mesh instance by given Vector3
	///</summary>
	void AddPosition(const Vector3& position) {
		const size_t vertSz = vertices.size();
		for (size_t i = 0; i < vertSz; ++i) { vertices[i] = vertices[i] + position; }
	}
	///<summary>
	///Multiplies current mesh instance by given Quaternion
	///</summary>
	void AddRotation(const Quaternion& rotation) {
		const size_t vertSz = vertices.size();
		for (size_t i = 0; i < vertSz; ++i) { vertices[i].Rotatate(rotation); }
	}
	///<summary>
	///Multiplies current mesh instance by scale on each axis
	///</summary>
	void AddScale(const Vector3& scale) {
		const size_t vertSz = vertices.size();
		for (size_t i = 0; i < vertSz; ++i) { vertices[i].MultiplyPairwise(scale); }
	}
	///<summary>
	///Applies transfrom to current mesh instance
	///</summary>
	void ApplyTransform(const Transform& transform) {
		this->AddScale(transform.scale);
		this->AddRotation(transform.rotation);
		this->AddPosition(transform.position);
	}
	///<summary>
	///Generates Cone mesh with given parameters
	///</summary>
	static Mesh GenerateCone(const unsigned& sides, const float& radius, const float& height) {
		Mesh nCone = Mesh();
		
		if (sides < 2) { return nCone; }
		std::vector<Vector3> circle = Vector3::CirclePoints(sides, radius, Vector3(0, -height / 2.0f, 0), Quaternion());
		
		nCone.vertices.push_back(Vector3(0, height / 2.0f, 0));
		nCone.vertices.insert(nCone.vertices.end(), circle.begin(), circle.end());

		if (height > 0) {
			for (unsigned i = 1; i < sides; ++i) {
				nCone.triangles.push_back(i + 1);
				nCone.triangles.push_back(i);
				nCone.triangles.push_back(0);
			}
			nCone.triangles.push_back(1);
			nCone.triangles.push_back(sides);
			nCone.triangles.push_back(0);
		}
		else {
			for (unsigned i = 1; i < sides; ++i) {
				nCone.triangles.push_back(i);
				nCone.triangles.push_back(i + 1);
				nCone.triangles.push_back(0);
			}
			nCone.triangles.push_back(sides);
			nCone.triangles.push_back(1);
			nCone.triangles.push_back(0);
		}
		return nCone;
	}
	///<summary>
	///Generates Cylinder mesh with given parameters
	///</summary>
	static Mesh GenerateCylinder(const unsigned& sides, const float& radius, const float& height) {
		Mesh nCylinder = Mesh();

		if (sides < 2) { return nCylinder; }
		std::vector<Vector3> circle = Vector3::CirclePoints(sides, radius, Vector3(0, -height / 2.0f, 0), Quaternion(0, 0, 0, 1));

		for (unsigned i = 0; i < sides; ++i) {
			nCylinder.vertices.push_back(circle[i]);
			nCylinder.vertices.push_back(circle[i] + Vector3(0, height, 0));
		}

		for (unsigned i = 1; i < sides; ++i) {
			unsigned point = i << 1;

			nCylinder.triangles.push_back(point - 2);
			nCylinder.triangles.push_back(point - 1);
			nCylinder.triangles.push_back(point);
			nCylinder.triangles.push_back(point);
			nCylinder.triangles.push_back(point - 1);
			nCylinder.triangles.push_back(point + 1);
		}
		nCylinder.triangles.push_back((sides << 1) - 2);
		nCylinder.triangles.push_back((sides << 1) - 1);
		nCylinder.triangles.push_back(0);
		nCylinder.triangles.push_back(0);
		nCylinder.triangles.push_back((sides << 1) - 1);
		nCylinder.triangles.push_back(1);

		return nCylinder;
	}
	///<summary>
	///Generates Cuboid mesh with given x, y, z size
	///</summary>
	static Mesh GenerateCuboid(const Vector3& size) {
		Mesh nCube = Mesh();
		
		nCube.triangles = {
			0,2,1,1,2,3,
			2,6,3,3,6,7,
			3,7,1,1,7,5,
			4,5,6,6,5,7,
			5,4,1,1,4,0,
			6,2,4,4,2,0
		};

		for (int sx = -1; sx < 2; sx += 2) {
			for (int sz = -1; sz < 2; sz += 2) {
				for (int sy = -1; sy < 2; sy += 2) {
					nCube.vertices.push_back(Vector3(size.x / 2 * sx, size.y / 2 * sy, size.z / 2 * sz));
				}
			}
		}
		return nCube;
	}
	///<summary>
	///Generates Low-poly Ico-Sphere mesh with given radius
	///</summary>
	static Mesh GenerateIcoSphere(const float& radius) {
		Mesh nIco = Mesh();

		const float sX = radius * 0.5257311f;
		const float sZ = radius * 0.8506508f;

		nIco.triangles = {
			0,1,4,	0,4,9,	9,4,5,	4,8,5,
			4,1,8,	8,1,10, 8,10,3, 5,8,3,
			5,3,2,	2,3,7,	7,3,10, 7,10,6,
			7,6,11, 11,6,0, 0,6,1,	6,10,1,
			9,11,0, 9,2,11, 9,5,2,	7,11,2
		};
		nIco.vertices = {
			Vector3(-sX, 0, sZ),	Vector3(sX, 0, sZ),		Vector3(-sX, 0, -sZ),	Vector3(sX, 0, -sZ),
			Vector3(0, sZ, sX),		Vector3(0, sZ, -sX),	Vector3(0, -sZ, sX),	Vector3(0, -sZ, -sX),
			Vector3(sZ, sX, 0),		Vector3(-sZ, sX, 0),	Vector3(sZ, -sX, 0),	Vector3(-sZ, -sX, 0)
		};
		return nIco;
	}
};