#pragma once
#ifndef VEC3H
#define VEC3H
//==================================================================================================
// Written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is distributed
// without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication along
// with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==================================================================================================
#define _USE_MATH_DEFINES

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <glm/glm.hpp>


inline int maxDimension(const glm::vec3 &v) {
    return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
}

extern glm::vec3 permute(const glm::vec3 &v, int x, int y, int z);

#endif
