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

Camera::Camera(const MString &camName)
{
	MDagPath dag;

	MSelectionList list;
	list.add(camName);
	list.getDagPath(0, dag);

	MFnCamera mCamera(dag);
	MPoint mOrigin = mCamera.eyePoint(MSpace::kWorld);
	MVector mDirection = mCamera.viewDirection(MSpace::kWorld);
	MVector mUp = mCamera.upDirection(MSpace::kWorld);
	MVector mRight = mCamera.rightDirection(MSpace::kWorld);
	double vFov = mCamera.verticalFieldOfView();

	origin = glm::vec3(mOrigin[0], mOrigin[1], mOrigin[2]);
	direction = glm::vec3(mDirection[0], mDirection[1], mDirection[2]);
	up = glm::vec3(mUp.x, mUp.y, mUp.z);
	right = glm::vec3(mRight.x, mRight.y, mRight.z);
	
	aspect = static_cast<float>(mCamera.aspectRatio());
	hAperture = static_cast<float>(mCamera.horizontalFilmAperture());
	vAperture = static_cast<float>(mCamera.verticalFilmAperture());
	focusDistance = static_cast<float>(mCamera.focusDistance());

	float theta = static_cast<float>(vFov) * glm::pi<float>() / 180;
	float halfHeight = glm::tan(theta / 2);
	float halfWidth = aspect * halfHeight;

	glm::vec3 w = glm::normalize(origin - direction);
	glm::vec3 u = glm::normalize(glm::cross(up, w));
	glm::vec3 v = glm::cross(w, u);

	lowerLeft = origin - halfWidth * u - halfHeight * v - w;
	LOG_MSG("fov %f", vFov);
	LOG_MSG("Origin {%f, %f, %f}}", origin.x,origin.y, origin.z);
	LOG_MSG("Up {%f, %f, %f}}", up.x,up.y, up.z);
	LOG_MSG("Right {%f, %f, %f}}", right.x,right.y, right.z);
	LOG_MSG("lowerLeft {%f, %f, %f}}", lowerLeft.x,lowerLeft.y, lowerLeft.z);
}

Ray Camera::getRay(float u, float v)
{
	Ray ray;
	ray.origin = origin;
	ray.direction = lowerLeft + right * u + up * v - origin;
	//LOG_MSG("ray {%f, %f, %f}}", ray.direction.x, ray.direction.y, ray.direction.z);
	return (ray);
}