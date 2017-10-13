/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "udpecho.h"

#include "lwip/opt.h"

#if LWIP_NETCONN

#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/ip_addr.h"
/*-----------------------------------------------------------------------------------*/
//static void
//udpecho_thread(void *arg)
//{
//  struct netconn *conn;
//  struct netbuf *buf;
//  char buffer[4096];
//  err_t err;
//  LWIP_UNUSED_ARG(arg);

//#if LWIP_IPV6
//  conn = netconn_new(NETCONN_UDP_IPV6);
//  netconn_bind(conn, IP6_ADDR_ANY, 7);
//#else /* LWIP_IPV6 */
//  conn = netconn_new(NETCONN_UDP);
//  netconn_bind(conn, IP_ADDR_ANY, 7);
//#endif /* LWIP_IPV6 */
//  LWIP_ERROR("udpecho: invalid conn", (conn != NULL), return;);

//  while (1) {
//    err = netconn_recv(conn, &buf);
//    if (err == ERR_OK) {
//      /*  no need netconn_connect here, since the netbuf contains the address */
//      if(netbuf_copy(buf, buffer, sizeof(buffer)) != buf->p->tot_len) {
//        LWIP_DEBUGF(LWIP_DBG_ON, ("netbuf_copy failed\n"));
//      } else {
//        buffer[buf->p->tot_len] = '\0';
//        err = netconn_send(conn, buf);
//      }
//      netbuf_delete(buf);
//    }
//  }
//}
///*-----------------------------------------------------------------------------------*/
//void
//udpecho_init(void)
//{
//  sys_thread_new("udpecho_thread", udpecho_thread, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
//}
extern uint32_t IPaddress;
extern uint32_t Maskaddress;
extern uint32_t GWaddress;
static void
udpecho_thread()
{
  struct netconn *conn;
  struct netbuf *buf;
  char buff[100];
  err_t err;
  uint8_t i;
//	uint32_t BroadCast_Address;
  ip4_addr_t addr;  
	vTaskDelay(5000);	
//	BroadCast_Address = (IPaddress&Maskaddress)|(~Maskaddress);

  IP4_ADDR(&addr, 255,255,255,255); 
	
//	IP4_ADDR(&addr, BroadCast_Address&0xff,
//			BroadCast_Address>>8&0xff,
//			BroadCast_Address>>16&0xff,
//			BroadCast_Address>>24); 
//	printf("Broad_Address:%d.%d.%d.%d\r\n",BroadCast_Address&0xff,
//			BroadCast_Address>>8&0xff,
//			BroadCast_Address>>16&0xff,
//			BroadCast_Address>>24);
	i=10;

  conn = netconn_new(NETCONN_UDP);
  netconn_bind(conn, IP_ADDR_ANY, 6000);
  netconn_connect(conn,&addr,6000);
	while(1)
	{
		if (i)
		{
			i--;

			sprintf(buff,"IP_Address:%d.%d.%d.%d\tMask_Address:%d.%d.%d.%d\tGW_Address:%d.%d.%d.%d\r\n",
			IPaddress&0xff,
			IPaddress>>8&0xff,
			IPaddress>>16&0xff,
			IPaddress>>24,
			Maskaddress&0xff,
			Maskaddress>>8&0xff,
			Maskaddress>>16&0xff,
			Maskaddress>>24,
			GWaddress&0xff,
			GWaddress>>8&0xff,
			GWaddress>>16&0xff,
			GWaddress>>24);			
			buf = netbuf_new();
			netbuf_alloc(buf,strlen((char *)buff));
			memcpy(buf->p->payload,(void*)buff,strlen((char*)buff));	

			err = netconn_send(conn, buf);
			netbuf_delete(buf);
		}
		vTaskDelay(2000);
	}
}
/*-----------------------------------------------------------------------------------*/
void
udpecho_init(void)
{
  sys_thread_new("udpecho_thread", udpecho_thread, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);


}

#endif /* LWIP_NETCONN */
