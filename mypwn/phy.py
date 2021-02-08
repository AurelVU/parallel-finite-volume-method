def run_phis(T):
    global m, n, h, Tb, dz, dt, kL, T0
    T_new = [[0] * n] * m

    fluxR = -k * h
    fluxL = -k * h
    fluxU = -k * h
    fluxD = -k * h

    fluxLgran = -kL * h * 2
    fluxRgran = -kR * h * 2
    fluxUgran = -kU * h * 2
    fluxDgran = -kD * h * 2

    fluxC = - 4 * k * h

    fluxCLgran = fluxR + fluxLgran + fluxU + fluxD
    fluxCRgran = fluxRgran + fluxL + fluxU + fluxD
    fluxCUgran = fluxR + fluxL + fluxUgran + fluxD
    fluxDUgran = fluxR + fluxL + fluxU + fluxDgran

    fluxCLUgran = fluxR + fluxLgran + fluxUgran + fluxD
    fluxCRUgran = fluxRgran + fluxL + fluxUgran + fluxD
    fluxCLDgran = fluxR + fluxLgran + fluxU + fluxDgran
    fluxDRUgran = fluxRgran + fluxL + fluxU + fluxDgran

    for i in range(n):
        for j in range(m):
            cflaxL = 0
            cflaxR = 0
            cflaxU = 0
            cflaxD = 0
            cflaxC = 0

            if i == 0:
                cflaxL = fluxLgran * (Tb - T[i][j])
            else:
                cflaxL = fluxL * (T[i - 1][j] - T[i][j])
            if i == n - 1:
                cflaxR = fluxRgran * (T0 - T[i][j])
            else:
                cflaxR = fluxR * (T[i + 1][j] - T[i][j])

            if j == 0:
                cflaxD = fluxDgran * (T0 - T[i][j])
            else:
                cflaxD = fluxD * (T[i][j - 1] - T[i][j])
            if j == m - 1:
                cflaxU = fluxUgran * (T0 - T[i][j])
            else:
                cflaxU = fluxU * (T[i][j + 1] - T[i][j])

            T_new[i][j] = T[i][j] - dt * h ** 2 * dz * (cflaxL + cflaxR + cflaxU + cflaxD)
    for arr in T_new:
        print(arr)
    return T_new


n = 40
m = 40
h = 1
k = 1

dt = 1
dz = 1
Tb = 240
T0 = 0

kL = 1
kR = 0.1
kU = 0.1
kD = 0.1

T = [[0] * n] * m
for i in range(50):
    T = run_phis(T)
    for arr in T:
        print(arr)

    print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")

