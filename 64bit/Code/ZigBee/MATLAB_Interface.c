/*
 *
 *	This is a simple program that illustrates how to call the MATLAB
 *	Engine functions from NetSim C Code.
 *
 */
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"
#include "mat.h"
#include "mex.h"
#include "main.h"
#include "802_15_4.h"
#include "../BatteryModel/BatteryModel.h"
#include "direct.h"

char buf[100];
Engine *ep;
int status;
mxArray *Xc = NULL, *Yc = NULL, *Pwr = NULL;
double **x, **y, **p;
mxArray *out;
double *result[3];
int dim = 0;
double fn_netsim_matlab_init()
{
	/*
	 * Start the MATLAB engine Process
	 */
	fprintf(stderr, "\nPress any key to start MATLAB...\n");
	_getch();
	if (!(ep = engOpen(NULL))) {
		MessageBox((HWND)NULL, (LPCWSTR)"Can't start MATLAB engine",
			(LPCWSTR) "MATLAB_Interface.c", MB_OK);
		exit(-1);
	}

	engEvalString(ep, "desktop");
	return 0;
}

void fn_netsim_matlab_run()
{
	int i = 0;
	ptrIEEE802_15_4_PHY_VAR phy;
	ptrBATTERY battery;
	dim = NETWORK->nDeviceCount - 1;

	Xc = mxCreateNumericMatrix((mwSize)dim, 1, mxDOUBLE_CLASS, mxREAL);
	x = (double**)mxMalloc(dim * sizeof(*x));
	x[0] = (double*)mxGetPr(Xc);

	Yc = mxCreateNumericMatrix((mwSize)dim, 1, mxDOUBLE_CLASS, mxREAL);
	y = (double**)mxMalloc(dim * sizeof(*y));
	y[0] = (double*)mxGetPr(Yc);

	Pwr = mxCreateNumericMatrix((mwSize)dim, 1, mxDOUBLE_CLASS, mxREAL);
	p = (double**)mxMalloc(dim * sizeof(*p));
	p[0] = (double*)mxGetPr(Pwr);

	for (i = 1; i < dim; i++)
	{
		x[i] = x[i - 1] + 1;
		y[i] = y[i - 1] + 1;
		p[i] = p[i - 1] + 1;
	}


	for (i = 0; i < NETWORK->nDeviceCount; i++)
	{
		if (strcmp(DEVICE(i+1)->type, "SENSOR"))
			continue;
		phy = WSN_PHY(i+1);
		x[i][0] = DEVICE_POSITION(i + 1)->X;
		y[i][0] = DEVICE_POSITION(i + 1)->Y;
		p[i][0] = battery_get_consumed_energy((ptrBATTERY)phy->battery,0);
	}

	engPutVariable(ep, "Xc", Xc);
	engPutVariable(ep, "Yc", Yc);
	engPutVariable(ep, "Pwr", Pwr);
	
	sprintf(buf, "energy_consumption([Xc,Yc],Pwr)");
	status = engEvalString(ep, buf);
		
}

double fn_netsim_matlab_finish()
{
	//Close the MATLAB Engine Process
	fprintf(stderr, "\nPress any key to close MATLAB...\n");
	_getch();
	status = engEvalString(ep, "exit");
	return 0;
}

