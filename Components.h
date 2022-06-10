#pragma once

#include <cmath>
#include <vector>
#include <gl/freeglut.h>
#include "Geometry.h"
#include "Graphics.h"

/*
  - Component system header
  - Provides easy interact, easy to use components and tools to setup and render OpenGL scene
  - By: EgorDeveloper
  -
  - Components.h:
  - Uses freeglut functions, creates a component system
  -
  - Dependencies:
  - Geometry.h
  - Graphics.h
*/

class Camera {
private:
	Vector3 position, target, axis;
	float perspectiveFov, perspectiveRatio;
	float orthoHalfWidth, orthoHalfHeight;
	float clipNear, clipFar;
	bool isOrtho;

public:
	///<summary>
	///Perspective camera settings. To enable perspective mode call SetPerspective()
	///</summary>
	void SetupPerspective(float FOV, float ScreenRatio, float NearClip, float FarClip) {
		perspectiveFov = FOV;
		perspectiveRatio = ScreenRatio;
		clipNear = NearClip;
		clipFar = FarClip;
	}
	///<summary>
	///Ortho camera settings. To enable ortho mode call SetOrtho()
	///</summary>
	void SetupOrtho(float planeWidth, float planeHeight, float NearClip, float FarClip) {
		orthoHalfHeight = planeHeight / 2.0f;
		orthoHalfWidth = planeWidth / 2.0f;
		clipNear = NearClip;
		clipFar = FarClip;
	}
	///<summary>
	///Default Ortho and Perspective camera settings. Does not affect positions and axis direction
	///</summary>
	void SetupDefault(void) {
		SetupPerspective(60, 1.777f, 0.1f, 50.0f), SetupOrtho(12, 6.75f, 0.1f, 50.0f); isOrtho = false;
	}

	Camera() { position = Vector3(); target = Vector3(1, 0, 0); axis = Vector3(0, 1, 0); SetupDefault(); }
	Camera(Vector3 CameraPosition) { position = CameraPosition; target = Vector3(1, 0, 0); axis = Vector3(0, 1, 0); SetupDefault(); }
	Camera(Vector3 CameraPosition, Vector3 TargetPosition) { position = CameraPosition; target = TargetPosition; axis = Vector3(0, 1, 0); SetupDefault(); }
	Camera(Vector3 CameraPosition, Vector3 TargetPosition, Vector3 AxisDirection) { position = CameraPosition; target = TargetPosition; axis = AxisDirection; SetupDefault(); }

	void UpdatePosition(void) const {
		gluLookAt(
			position.x, position.y, position.z,
			target.x, target.y, target.z,
			axis.x, axis.y, axis.z
		);
	}
	Vector3 GetAxis(void) const { return axis; }
	Vector3 GetCameraPosition(void) const { return position; }
	Vector3 GetTargetPosition(void) const { return target; }
	///<summary>
	///Returns normalized camera look direction
	///</summary>
	Vector3 Normal(void) const { return (Vector3(target) - Vector3(position)).Normal(); }

	///<summary>
	///Sets new axis for this Camera
	///</summary>
	void SetAxis(Vector3 newAxis) { axis = newAxis; }
	///<summary>
	///Sets new position of this Camera
	///</summary>
	void SetCameraPosition(Vector3 newPosition) { position = newPosition; }
	///<summary>
	///Sets new position of this Camera's target
	///</summary>
	void SetTargetPosition(Vector3 newPosition) { target = newPosition; }
	///<summary>
	///Sets new clipping distances for this Camera
	///</summary>
	void SetClipDistance(float NearClip, float FarClip) {
		clipNear = NearClip;
		clipFar = FarClip;
	}
	///<summary>
	///Sets Perspective Camera mode For settings call SetupPerspective()
	///</summary>
	void SetPerspective(void) {
		gluPerspective(perspectiveFov, perspectiveRatio, clipNear, clipFar);
		isOrtho = false;
	}
	///<summary>
	///Sets Ortho Camera mode. For settings call SetupOrtho()
	///</summary>
	void SetOrtho(void) {
		glOrtho(-orthoHalfWidth, orthoHalfWidth, -orthoHalfHeight, orthoHalfHeight, clipNear, clipFar);
		isOrtho = true;
	}
	///<summary>
	///Updates current Camera mode
	///</summary>
	void SetAvailable(void) {
		if (isOrtho) { SetOrtho(); }
		else { SetPerspective(); }
	}
};
class Renderer {
private:
	float clamp(float val) const { return val < -1 ? -1 : val > 1 ? 1 : val; }
	float diffusePoint(float x, float roughness) const { roughness = 1 - roughness; x = (roughness * fabsf(x) - roughness); return 1 - x * x; }
	float realisticPoint(float x, float distClamp, float roughness) const { return (((fabsf(x) + 1) * 1.5f) * diffusePoint(x, roughness) - ((0.5f + roughness) * distClamp)); }
public:
	Camera camera;

	Renderer(Camera cameraToUse) { camera = cameraToUse; }

	static void SendVertex(const Vertex3& vertex) {
		glColor3ub(vertex.color.r, vertex.color.g, vertex.color.b);
		glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
	}
	static void SendVertex(const Vertex3& vertex, const Color& color) {
		glColor3ub(color.r, color.g, color.b);
		glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
	}

	void init(void) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		camera.SetAvailable();
		glMatrixMode(GL_MODELVIEW);
		glPointSize(5);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE); //glDisable(GL_CULL_FACE);
	}

	///<summary>
	///Sends points to render with given color
	///</summary>
	void RenderPoints(const std::vector<Vector3> &points, const Color& color) {
		size_t pSize = points.size();
		glBegin(GL_POINTS);
		glColor3ub(color.r, color.g, color.b);
		while (pSize--) { glVertex3f(points[pSize].x, points[pSize].y, points[pSize].z); }
		glEnd();
	}
	///<summary>
	///Sends Vector3 to render with given color
	///</summary>
	void RenderVector(const Vector3& vector, const Color& color) {
		glBegin(GL_LINES);
		glColor3ub(color.r, color.g, color.b);
		glVertex3f(0, 0, 0);
		glVertex3f(vector.x, vector.y, vector.z);
		glEnd();
	}
	///<summary>
	///Sends Vector3 to render with given color and start position
	///</summary>
	void RenderVector(const Vector3& vector, const Vector3& startPoint, const Color& color) {
		glBegin(GL_LINES);
		glColor3ub(color.r, color.g, color.b);
		glVertex3f(startPoint.x, startPoint.y, startPoint.z);
		glVertex3f(startPoint.x + vector.x, startPoint.y + vector.y, startPoint.z + vector.z);
		glEnd();
	}
private:
	///<summary>
	///Sends triangle to render with given material. Prefer this function. Must be called only in glBegin(GL_TRIANGLES) event
	///</summary>
	void RenderTriangleNoCall(const Triangle& triangle, const Material& material) {
		float normalAngle, centerDist;

		switch (material.shader) {
		case Material::unlit:
			SendVertex(triangle.a);
			SendVertex(triangle.b);
			SendVertex(triangle.c);
			break;
		case Material::diffuse:
			normalAngle = diffusePoint(Vector3::Angle(camera.Normal(), triangle.Normal()), material.roughness);

			SendVertex(triangle.a, Color(triangle.a.color) * normalAngle);
			SendVertex(triangle.b, Color(triangle.b.color) * normalAngle);
			SendVertex(triangle.c, Color(triangle.c.color) * normalAngle);
			break;
		case Material::realistic:
			normalAngle = Vector3::Angle(camera.Normal(), triangle.Normal());
			centerDist = Vector3::Distance(camera.GetCameraPosition(), triangle.Center()) + 0.01f;

			SendVertex(triangle.a, Color::Lerp(triangle.a.color, material.metal, material.metallic) * realisticPoint(normalAngle, Vector3::Distance(camera.GetCameraPosition(), triangle.a.position) / centerDist, material.roughness));
			SendVertex(triangle.b, Color::Lerp(triangle.b.color, material.metal, material.metallic) * realisticPoint(normalAngle, Vector3::Distance(camera.GetCameraPosition(), triangle.b.position) / centerDist, material.roughness));
			SendVertex(triangle.c, Color::Lerp(triangle.c.color, material.metal, material.metallic) * realisticPoint(normalAngle, Vector3::Distance(camera.GetCameraPosition(), triangle.c.position) / centerDist, material.roughness));
		default: break;
		}
	}
public:
	///<summary>
	///Sends triangle to render with given material
	///</summary>
	void RenderTriangle(const Triangle& triangle, const Material& material) {
		glBegin(GL_TRIANGLES);
		RenderTriangleNoCall(triangle, material);
		glEnd();
	}
	///<summary>
	///Renders mesh with given parameters
	///</summary>
	void RenderMesh(const Mesh& mesh, const Vector3& position, const Quaternion& rotation, const Color& color, const Material& material) {
		const size_t vertSz = mesh.vertices.size();
		const size_t triaSz = mesh.triangles.size() + 3;

		std::vector<Vector3> modifiedMesh;
		
		glBegin(GL_TRIANGLES);
		for (size_t i = 0; i < vertSz; ++i) { modifiedMesh.push_back(mesh.vertices[i].Rotation(rotation) + position); }
		for (size_t i = 3; i < triaSz; i += 3) {
			RenderTriangleNoCall(Triangle(modifiedMesh[mesh.triangles[i - 3]], modifiedMesh[mesh.triangles[i - 2]], modifiedMesh[mesh.triangles[i - 1]], color), material);
		}
		glEnd();
	}
	///<summary>
	///Renders grid in XZ axis with given parameters
	///</summary>
	void RenderGrid(float startX, float endX, unsigned amountX, float startZ, float endZ, unsigned amountZ, float height, bool hasBorder, const Color& color) {
		glBegin(GL_LINES);
		glColor3ub(color.r, color.g, color.b);

		if (amountX++) {
			float dx = (endX - startX) / amountX, cx = startX;
			unsigned x = hasBorder ? 0 : 1;

			if (hasBorder) { ++amountX; }
			else { cx += dx; }

			for (x; x < amountX; ++x) {
				glVertex3f(cx, height, startZ);
				glVertex3f(cx, height, endZ);
				cx += dx;
			}
		}
		
		if (amountZ++) {
			float dz = (endZ - startZ) / amountZ, cz = startZ;
			unsigned z = hasBorder ? 0 : 1;

			if (hasBorder) { ++amountZ; }
			else { cz += dz; }

			for (z; z < amountZ; ++z) {
				glVertex3f(startX, height, cz);
				glVertex3f(endX, height, cz);
				cz += dz;
			}
		}
		glEnd();
	}

	void BeginFrame(void) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		camera.UpdatePosition();
	}
	void EndFrame(void) {
		glFlush();
	}
};