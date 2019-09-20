#pragma once
#ifndef MATERIALH
#define MATERIALH

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const = 0;
};

#endif // !MATERIALH
