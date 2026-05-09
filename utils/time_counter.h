#pragma once
#include <chrono>

enum timeType {
	TC_MS,
	TC_S,
	TC_mS
};

template <typename F>
long countFunctionTime(F&& f, timeType type) {
	auto start = std::chrono::steady_clock::now();
	f();
	if (type == TC_mS) {
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();
	}
	else if (type == TC_MS) {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
	}
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count();
}