#if defined(__linux__) and !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif

#include <thread>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <pthread.h>
#include <sched.h>
#endif

#ifdef _WIN32
static void pinThreadWIN(std::thread& t, unsigned cpuIndex) {
	if (cpuIndex >= 32) return;
	DWORD_PTR mask = (DWORD_PTR)1 << cpuIndex;
	SetThreadAffinityMask((HANDLE)t.native_handle(), mask);
}
#elif defined(__linux__)
void pinThreadLINUX(std::thread& t, int cpu) {
	cpu_set_t set;
	CPU_ZERO(&set);
	CPU_SET(cpu, &set);
	pthread_setaffinity_np(t.native_handle(), sizeof(set), &set);
}
#endif

void pinThread(std::thread& t, unsigned cpuIndex) {
#ifdef _WIN32
	pinThreadWIN(t, cpuIndex);
#elif defined(__linux__)
	pinThreadLINUX(t, cpuIndex);
#endif
}

enum computeType {
	CT_SUM = 0,
	CT_DIF,
	CT_MUL,
	CT_DIV
};

template<typename T>
T computeArray(T* begin, T* end, computeType ct, int threads = -1) {
	int maxThreads = _Thrd_hardware_concurrency();
	int threadsUsed = ((threads == -1) ? maxThreads : ((threads > maxThreads or threads < 1) ? maxThreads : threads));
	if (threadsUsed < 1) return T{};
	long long size = end - begin;
	if (size < 100000) threadsUsed = 1;
	long long sizePerThread = size / threadsUsed;
	std::vector<std::thread> workingThreads; workingThreads.reserve(threadsUsed);
	std::vector<T> parts(threadsUsed);
	T endValue{}; if (ct == CT_MUL or ct == CT_DIV) endValue = 1; if (ct == CT_DIF) endValue = *begin;

	for (int i = 0; i < threadsUsed; i++) {
		workingThreads.emplace_back([ct, i, sizePerThread, threadsUsed, begin, size, &parts] {
			T localEndValue{}; if (ct == CT_MUL or ct == CT_DIV) localEndValue = 1; if (ct == CT_DIF) localEndValue = *begin;
			long long from = i * sizePerThread + (ct == CT_DIF and i == 0);
			long long to = (i == threadsUsed - 1) ? size : (from + sizePerThread);
			if (ct == CT_SUM) {
				for (long long j = from; j < to; j++) {
					localEndValue += begin[j];
				}
			} else
			if (ct == CT_DIF) {
				for (long long j = from; j < to; j++) {
					localEndValue -= begin[j];
				}
			} else
			if (ct == CT_MUL) {
				for (long long j = from; j < to; j++) {
					localEndValue *= begin[j];
				}
			} else
			if (ct == CT_DIV) {
				for (long long j = from; j < to; j++) {
					localEndValue /= begin[j];
				}
			}
			
			parts[i] = localEndValue;
		});

		pinThread(workingThreads.back(), i);
	}

	for (std::thread& workTh : workingThreads) {
		workTh.join();
	}

	for (auto& p : parts) {
		if (ct == CT_SUM) { endValue += p; continue; }
		if (ct == CT_DIF) { endValue += p; continue; }
		if (ct == CT_MUL) { endValue *= p; continue; }
		if (ct == CT_DIV) { endValue /= ((p != 0) ? p : 1); continue; }
	}

	return endValue;
}