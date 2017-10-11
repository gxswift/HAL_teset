#include "lwip/debug.h"
#include "lwip/apps/httpd.h"
#include "lwip/tcp.h"
#include "fs.h"

#include <string.h>
#include <stdlib.h>

#include "stm32f4xx_hal.h"

#define NUM_CONFIG_CGI_URIS	1 //CGI的URI数量
#define NUM_CONFIG_SSI_TAGS	2  //SSI的TAG数量

//控制LED和BEEP的CGI handler
const char* Login_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
//const char* Set_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);
//const char* Ctrl_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);

static const tCGI ppcURLs[]= //cgi程序
{
	{"/login.cgi",Login_CGI_Handler},
//	{"/set.cgi",Set_CGI_Handler},
//	{"/ctrl.cgi",Ctrl_CGI_Handler}
};
static const char *ppcTAGs[]=  //SSI的Tag
{
	"w", //温度值
	"t"
};


//SSIHandler中需要用到的处理ADC的函数
void ADC_Handler(char *pcInsert)
{ 
    uint32_t temp = 0;   
	temp = HAL_GetTick();
		sprintf(pcInsert,"%d,%d",temp/1000,temp%1000);
}



//SSI的Handler句柄
static u16_t SSIHandler(int iIndex,char *pcInsert,int iInsertLen)
{
	switch(iIndex)
	{
		case 0: 

				break;
		case 1:
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

//当web客户端请求浏览器的时候,使用此函数被CGI handler调用
static int FindCGIParameter(const char *pcToFind,char *pcParam[],int iNumParams)
{
	int iLoop;
	for(iLoop = 0;iLoop < iNumParams;iLoop ++ )
	{
		if(strcmp(pcToFind,pcParam[iLoop]) == 0)
		{
			return (iLoop); //返回iLOOP
		}
	}
	return (-1);
}

const char *Login_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])

{

  uint8_t i=0; //???????????????i???
      uint8_t passchack=0;
      iIndex =FindCGIParameter("username",pcParam,iNumParams);  //??bktime????
      if(iIndex != -1) //??pagingvol???
      {
           for(i = 0;i < iNumParams;i++)
           {
                 if(strcmp(pcParam[i],"username")== 0)  //??CGI??
                 {
                      if(strcmp(pcValue[i],"admin")== 0)//?????
                      {
                            passchack++;
                      }
                 }
                 else if(strcmp(pcParam[i],"password") == 0)  //??CGI??
                 {
                      if(strcmp(pcValue[i],"admin")== 0)//????
                      {
                            passchack++;
                      }
                 }
           }
           if(passchack > 1)//??????????
           {
                 return "/menu.html";     //????????????????????
           }
      }
      return "/index.html";      //????????????????????
}

//CGI句柄初始化
void httpd_cgi_init(void)
{ 
  //配置CGI句柄
  http_set_cgi_handlers(ppcURLs, NUM_CONFIG_CGI_URIS);
}
