#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"

#include <string.h>
#include <stdlib.h>

#include "stm32f4xx_hal.h"

#define NUM_CONFIG_CGI_URIS	0  //CGI的URI数量
#define NUM_CONFIG_SSI_TAGS	1  //SSI的TAG数量

//控制LED和BEEP的CGI handler
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char* BEEP_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);

static const char *ppcTAGs[]=  //SSI的Tag
{
	"w" //温度值
};


//SSIHandler中需要用到的处理ADC的函数
void ADC_Handler(char *pcInsert)
{ 
    uint32_t temp = 0;   
	temp = HAL_GetTick();
		sprintf(pcInsert,"%d",temp);
}




//SSI的Handler句柄
static u16_t SSIHandler(int iIndex,char *pcInsert,int iInsertLen)
{
	switch(iIndex)
	{
		case 0: 
				ADC_Handler(pcInsert);
				break;
	}
	return strlen(pcInsert);
}


//SSI句柄初始化
void httpd_ssi_init(void)
{  
	//配置SSI句柄
	http_set_ssi_handler(SSIHandler,ppcTAGs,NUM_CONFIG_SSI_TAGS);
}


