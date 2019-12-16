#pragma once
template <typename T>
T clamp(T a, T min, T max) {
	return std::max(min, std::min(a, max));
}