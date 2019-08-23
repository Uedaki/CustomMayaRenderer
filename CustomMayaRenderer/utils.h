#pragma once

#include <Maya/MStatus.h>

#include <string>

#define LOG_MSG(a, ...) printMsg(a, __VA_ARGS__)

#ifdef _DEBUG
#define DEBUG_MSG(a, ...) printMsg(a, __VA_ARGS__)
#else
#define DEBUG_MSG(a, ...)
#endif

template <typename ...Args>
void printMsg(const char *msg, Args... args)
{
	printf("%s : CustomMayaRenderer : ", __TIME__);
	printf(msg, args...);
	printf("\n");
}

#define STATUS_CHECK(status) if (status != MS::kSuccess) LOG_MSG(status.errorString().asChar());
#define STATUS_BARRIER(status) if (status != MS::kSuccess) { LOG_MSG(status.errorString().asChar()); return (MS::kFailure); }