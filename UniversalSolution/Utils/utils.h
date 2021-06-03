#pragma once

struct params
{
	float h = 0.1;
	float k = 50;

	float dt = 1;
	float dz = 1;
	float Tb = 240;
	float T0 = 0;

	float kL = 1;
	float kR = 0;
	float kU = 10;
	float kD = 10;
};

template <typename T>
inline T** allocate2DArray(int n, int m)
{
    T* t = new T[n * m];
    T** T_new = new T* [n];
    for (int i = 0; i < n; i++)
        T_new[i] = t + i * m;
    return T_new;
}

template <typename T>
inline T** copy2DArray(T** T_inp, int n, int m) // работает некорректно
{
    T** T_new = allocate2DArray<T>(n, m);
    for (int i = 0; i < n; i++)
        memcpy(T_new[i], T_inp[i], m);
    return T_new;
}

