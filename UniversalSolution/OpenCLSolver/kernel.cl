void kernel solverKernel
(global float* T, global float* T_new, global int* n, global int* m, global float* h, global float* k, global float* dt, global float* dz, global float* Tb, global float* T0, global float* kL, global float* kR, global float* kU, global float* kD)
{       
    int i = get_global_id(0);
    int j = get_global_id(1);


    double cflaxL = 0;
    double cflaxR = 0;
    double cflaxU = 0;
    double cflaxD = 0;




    float h_fluxR = -*k * *h;
    float h_fluxL = -*k * *h;
    float h_fluxU = -*k * *h;
    float h_fluxD = -*k * *h;

    float h_fluxLgran = -*kL * *h * 2;
    float h_fluxRgran = -*kR * *h * 2;
    float h_fluxUgran = -*kU * *h * 2;
    float h_fluxDgran = -*kD * *h * 2;

    float h_fluxC = -4 * *k * *h;

    float h_fluxCLgran = h_fluxR + h_fluxLgran + h_fluxU + h_fluxD;
    float h_fluxCRgran = h_fluxRgran + h_fluxL + h_fluxU + h_fluxD;
    float h_fluxCUgran = h_fluxR + h_fluxL + h_fluxUgran + h_fluxD;
    float h_fluxDUgran = h_fluxR + h_fluxL + h_fluxU + h_fluxDgran;

    float h_fluxCLUgran = h_fluxR + h_fluxLgran + h_fluxUgran + h_fluxD;
    float h_fluxCRUgran = h_fluxRgran + h_fluxL + h_fluxUgran + h_fluxD;
    float h_fluxCLDgran = h_fluxR + h_fluxLgran + h_fluxU + h_fluxDgran;
    float h_fluxDRUgran = h_fluxRgran + h_fluxL + h_fluxU + h_fluxDgran;


    if (i == 0)
        cflaxL = h_fluxLgran * (*Tb - T[i * *m + j]);
    else
        cflaxL = h_fluxL * (T[(i - 1) * *m + j] - T[i * *m + j]);
    if (i == *n - 1)
        cflaxR = h_fluxRgran * (*T0 - T[i * *m + j]);
    else
        cflaxR = h_fluxR * (T[(i + 1) * *m + j] - T[i * *m + j]);

    if (j == 0)
        cflaxD = h_fluxDgran * (*T0 - T[i * *m + j]);
    else
        cflaxD = h_fluxD * (T[i * *m + j - 1] - T[i * *m + j]);
    if (j == *m - 1)
        cflaxU = h_fluxUgran * (*T0 - T[i * *m + j]);
    else
        cflaxU = h_fluxU * (T[i * *m + j + 1] - T[i * *m + j]);

    T_new[i * *m + j] = T[i * *m + j] - *dt * *h * *h * *dz * (cflaxL + cflaxR + cflaxU + cflaxD); 
}