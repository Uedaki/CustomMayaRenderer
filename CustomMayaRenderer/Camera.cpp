#include "pch.h"
#include "Camera.h"

#include <Maya/MDagPath.h>
#include <Maya/MFloatMatrix.h>
#include <Maya/MFnCamera.h>
#include <Maya/MMatrix.h>
#include <Maya/MPoint.h>
#include <Maya/MSelectionList.h>

#include <glm/gtc/constants.hpp>

#include "Ray.h"
#include "utils.h"

Camera::Camera(const MString &mCameraName)
{
	MDagPath mDag;
	MSelectionList mList;
	mList.add(mCameraName);
	mList.getDagPath(0, mDag);

	MFnCamera mCamera(mDag);
	MPoint mOrigin = mCamera.eyePoint(MSpace::kWorld);
	MVector mDirection = mCamera.viewDirection(MSpace::kWorld);
	MVector mUp = mCamera.upDirection(MSpace::kWorld);
	double vFov = mCamera.verticalFieldOfView();

	origin = glm::vec3(mOrigin[0], mOrigin[1], mOrigin[2]);
	glm::vec3 direction = glm::vec3(mDirection[0], mDirection[1], mDirection[2]);
	glm::vec3 up = glm::vec3(mUp.x, mUp.y, mUp.z);
	float aspect = static_cast<float>(mCamera.aspectRatio());
	
	float theta = static_cast<float>(vFov);
	float halfHeight = glm::tan(theta / 2);
	float halfWidth = aspect * halfHeight;

	glm::vec3 w = -1.0f * glm::normalize(direction);
	u = glm::normalize(glm::cross(up, w));
	v = glm::cross(w, u);

	leftCorner = origin - halfWidth * u - halfHeight * v - w;
	horizontal = 2 * halfWidth * u;
	vertical = 2 * halfHeight * v;
}

Ray Camera::getRay(float s, float t)
{
	return (Ray(origin, leftCorner + s * horizontal + t * vertical - origin));
}