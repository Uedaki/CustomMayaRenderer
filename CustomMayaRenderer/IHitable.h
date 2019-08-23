#pragma once

struct Ray;
struct HitRecord;

class IHitable
{
public:
	virtual bool hit(const Ray &ray, float min, float max, HitRecord &record) const = 0;
};