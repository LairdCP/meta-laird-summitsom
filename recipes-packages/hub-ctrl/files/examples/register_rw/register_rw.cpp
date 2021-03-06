/*
**********************************************************************************
* © 2015 Microchip Technology Inc. and its subsidiaries.
* You may use this software and any derivatives exclusively with
* Microchip products.
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".
* NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
* INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
* AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
* TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
* CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
* FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
* OF THESE TERMS.
**********************************************************************************
*  register_rw.cpp
*   This file gives the sample code/ test code for using MchpUSB2530 API
*	Interface.
**********************************************************************************
*  $Revision: #1.1 $  $DateTime: 2015/09/21  18:04:00 $  $    $
*  Description: sample code for Register Read/write
**********************************************************************************
* $File:  
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

#include <cstdlib>
// PT2 SDK API Header file
#include "MchpUSBInterface.h"

using namespace std;

int main (int argc, char* argv[])
{
	CHAR sztext[2048];
	CHAR chText[256];
	DWORD dwError = 0;
	WORD vendor_id = 0x424 ,product_id= 0x4504, wAddr=0x413c;
	BYTE byReadData[256] = "\0", byWriteData[256] = "\0";
	BYTE byOp = 0x00;
	int iLen =0,i=0;
	HANDLE hDevice =  INVALID_HANDLE_VALUE;
	BOOL bRet = TRUE;
	

	// Get the version number of the SDK
	if (FALSE == MchpUsbGetVersion(sztext))
	{
		printf ("\nError:SDK Version cannot be obtained,Press any key to exit....");
		exit (1);
	}

	cout << "SDK Version:" <<sztext << endl;
	memset(sztext,0,2048);

	printf("Register read and write Demo\n");

	if(argc < 1)
	{
		printf("ERROR: Invalid Usage.\n");
		printf("Use --help option for further details \n");
		exit (1);
	}
	else if((0 == strcmp(argv[1],"--help")) || (0 == strcmp(argv[1],"/?")))
	{
		printf("Op	: Write \n");
		printf("Usage	: ./register_rw VID(Hex) PID(Hex) Operation(0x01) XDATAAddr Length Data \n");
		printf("Example	: ./register_rw 0x0424 0x4504 0x01 0x3004 2 0x12 0x34 \n\n");
		printf("Op	: Read \n");
		printf("Usage	: ./register_rw VID(Hex) PID(Hex) Operation(0x00) XDATAAddr Length\n");
		printf("Example	: ./register_rw 0x0424 0x4504 0x00 0x3004 2 \n");
		exit (1);
	}
	else
	{
		vendor_id  =  strtol (argv[1], NULL, 0) ;
		product_id =  strtol (argv[2], NULL, 0) ;
		byOp  	   =  strtol (argv[3], NULL, 0) ;
		wAddr	   =  strtol (argv[4], NULL, 0) ;
		iLen	   =  strtol (argv[5], NULL, 0) ;
		if(0x01 == byOp)
		{
			for(i=0; i<iLen; i++)
			{
				byWriteData[i] = strtol (argv[6+i], NULL, 0) ;
			}
		}
	}

	hDevice = MchpUsbOpenID(vendor_id,product_id);

	if(INVALID_HANDLE_VALUE == hDevice) 
	{
		printf ("\nError: MchpUsbOpenID Failed:\n");
		exit (1);
	}

	printf ("MchpUsbOpenID successful...\n");	

	if(0x00 == byOp)
	{
		cout << "Register Read operation \n";
		if (FALSE ==MchpUsbRegisterRead(hDevice,wAddr,iLen, (UINT8 *)&byReadData))
		{
			dwError = MchpUsbGetLastErr(hDevice);
			printf ("Error,%04xn",(unsigned int)dwError);
			exit (1);
		}
		cout << "Success : Register Read operation \n";
		for(i =0; i< iLen; i++)
		{
			sprintf(chText,"0x%02x \t",byReadData[i] );
			strcat(sztext,chText);
		}
		printf("%s \n",sztext);
		printf("\nRegister Read value is %s from 0x%04x \n",sztext,wAddr);
	}
	else
	{
		printf("Register Write operation \n");
		if (FALSE ==MchpUsbRegisterWrite(hDevice,wAddr,iLen,0x00))
		{
			dwError = MchpUsbGetLastErr(hDevice);
			printf ("Failed to write,%04x",(unsigned int)dwError);
			exit (1);
		}
		cout << "\nSuccess :Register Write operation\n";
		sleep(2);	
		cout << "Register Read operation\n";
		if (FALSE ==MchpUsbRegisterRead(hDevice,wAddr,iLen, (UINT8 *)&byReadData))
		{
			dwError = MchpUsbGetLastErr(hDevice);
			printf ("Error,%04xn",(unsigned int)dwError);
			exit (1);
		}
		cout << "\nSuccess : Register Read operation\n";
		for(i =0; i< iLen; i++)
		{
			if(byReadData [i]==byWriteData[i])
			{
				bRet &= TRUE;
			}
			else
			{
				bRet &= FALSE;
			}
		}
		if(bRet == TRUE)
		{
			cout << "Success : Register Read/write operation\n";
		}
		else
		{
			cout << "failed : Register Read/write operation\n";
		}
	}
	//Close device handle
	if(FALSE == MchpUsbClose(hDevice))
	{
		dwError = MchpUsbGetLastErr(hDevice);
		printf ("MchpUsbClose:Error Code,%04x\n",(unsigned int)dwError);
		exit (1);
	}

	return 0;
}

