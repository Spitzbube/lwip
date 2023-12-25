/*
*****************************************************************************
**
** \file        ./subsystems/net/lwip/src/netif/ethif_fapi.c
**
** \version     $Id: ethif_fapi.c 10100 2012-11-26 11:57:37Z jmorha $
**
** \brief       Ethernet Interface based on FAPI ETH driver
**
** This file provides a sample implementation of lwip ethernet interface
** based on FAPI ETH driver.
**
** \attention   THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
**              ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
**              OMMISSIONS.
**
** (C) Copyright 2006-2010 by Fujitsu Microelectronics Europe GmbH
** (C) Copyright 2010-2012 by Fujitsu Semiconductor Europe GmbH
**
*****************************************************************************
*/

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
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

#define DEBUG 0

//**************************************************************************
//**************************************************************************
//** Standard includes
//**************************************************************************
//**************************************************************************
#include <stdlib.h>
#include <stdint.h>
//#include <stdchar.h>
#include <time.h>

//**************************************************************************
//**************************************************************************
//** FAPI includes
//**************************************************************************
//**************************************************************************
#if 0
#include <fapi/sys_services.h>
#include <fapi/drv_eth.h>
#include <fapi/drv_timer.h>
#endif
#include "eth.h"

//**************************************************************************
//**************************************************************************
//** Local includes
//**************************************************************************
//**************************************************************************
#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/netif/etharp.h"
//#include "lwip/netif/ethif_fapi.h"

//**************************************************************************
//**************************************************************************
//** Defines and Macros
//**************************************************************************
//**************************************************************************

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

//234df188
#if 1 //USE_INPUT_THREAD
static sys_thread_t        ethdev_thread = NULL; //234df188 +0
static sys_thread_t        bData_234df189 = NULL; //234df189 +1
static sys_mbox_t          mbox_recv = NULL; //234df18c +4
#endif /* USE_INPUT_THREAD */

static sys_sem_t           sem_output = NULL; //234df190 +8
static int Data_234df194 = 0; //234df194 +0xc
static void* Data_234df198 = 0; //234df198 +0x10

typedef struct
{
	int Data_0; //0
#if 1
	eth_open_params Data_4; //4
#else
	int Data_4; //4
	int Data_8; //8
	int Data_0xc; //12 = 0x0c
	int Data_0x10; //16 = 0x10
	int Data_0x14; //20 = 0x14
	void (*Data_0x18)(); //24 = 0x18
#endif
	struct pbuf *pbuf; //28 = 0x1c
	uint32_t read_len; //32 = 0x20
	int fill_36; //0x24
	//0x28
} ethif_fapi_state;

typedef struct
{
	int Data_a; //-8
	struct netif* netif; //-4
	ethif_fapi_state state; //8
#if 0
	int Data_0; //0
	int Data_4; //4
	int Data_8; //8
	int Data_0xc; //12 = 0x0c
	int Data_0x10; //16 = 0x10
	int Data_0x14; //20 = 0x14
	void (*Data_0x18)(); //24 = 0x18
	int Data_0x1c; //28 = 0x1c
	int Data_0x20; //32 = 0x20
	int fill_36[1]; //0x24
#endif
	//0x30
} Struct_2396ad0c;

Struct_2396ad0c Data_2396ad0c[2]; //2396ad0c


#if 0

/* just a dummy devive driver handle as no handles are supported by the driver,
 * though the ETH interfaces are still declaring them */
#define ETH_HDL            0
/* just a dummy devive driver index as no handles are supported by the driver,
 * though the ETH interfaces are still declaring them */
#define ETH_IDX            0

//**************************************************************************
//**************************************************************************
//** Local Types
//**************************************************************************
//**************************************************************************

/* Private data exclusively used by FAPI driver,
 * Held in netif->state */
struct ethif_fapi_state {
  FAPI_ETH_OpenParamsT params;
  struct pbuf          *pbuf;        /* yet-to-be-read data by upper layer */
  uint32_t             read_len;     /* data length in pbuf */
  uint32_t             output_flush; /* 1, means previous write() is incompl. */
};

//**************************************************************************
//**************************************************************************
//** Global Data
//**************************************************************************
//**************************************************************************

#if POLL_PHYLINK
static sys_thread_t poll_thread = NULL;
#endif /* POLL_PHYLINK */

static uint64_t last_eth_write_ts = 0;

static struct {
  struct netif             *netif;
  struct ethif_fapi_state  state;
} eth_dev;

#endif

//void * last_netif_read_msg = NULL;

#if 0

//******************************************************************************
//******************************************************************************
//** Local Functions Declaration
//******************************************************************************
//******************************************************************************

static err_t low_level_init(struct netif *netif);
#endif
static err_t low_level_output(struct netif *netif, struct pbuf *out_pbuf);
static struct pbuf * low_level_input(struct netif *netif);
static uint32_t low_level_input_callback(void *);
#if 0
static void ethif_fapi_input(struct netif *netif);
#endif
static void ethif_fapi_thread(void *arg);
static void poll_phylink_thread(void* arg);
void low_level_init(struct netif *netif);
#if 0
#if POLL_PHYLINK
static void poll_phylink_thread(void *arg);
#endif /* POLL_PHYLINK */

//******************************************************************************
//******************************************************************************
//** API Functions
//******************************************************************************
//******************************************************************************

#endif

/*
********************************************************************************
** \brief Initializes the ethernet network interface
**
** This function should be called at the beginning of the program to set up the
** network interface. It calls the function low_level_init() to do the
** actual setup of the hardware.
**
** This function should be passed as a parameter to netif_add().
** netif->state->params and ->index should be initialized before calling
** this function.
**
** \param netif The lwIP network interface structure for this device
**
** \return
**      - ERR_OK, if the netif is initialized
**      - ERR_IF, if low level device detects some error
**      - any other err_t on error
********************************************************************************
*/
/* 2348bef2 - todo */
err_t ethif_fapi_init(struct netif *netif)
{
  err_t result;
#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  //netif->hostname = "lwip"; // set by application
#endif /* LWIP_NETIF_HOSTNAME */

  /* We call NETIF_INIT_SNMP() in low_level_init
   * in order to get link speed.
   *  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, ???); */

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;

  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  if (sem_output == NULL) {
    result = sys_sem_new(&sem_output, 1);
    if (result != 0) {
      LWIP_DEBUGF(NETIF_DEBUG, ("ethif_fapi_init: cannot create a semaphore.\n"));
      return ERR_IF;
    }
  }
  //loc_2348bf22
#if 0

  /* initialize the hardware */
  if (low_level_init(netif)) {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethif_fapi_init: hardware init failed\n"));
    sys_sem_free(&sem_output);
    return ERR_IF;
  }

  /* set default network interface */
  netif_set_default(netif);

#endif

#if 1 //USE_INPUT_THREAD
    /* Create a thread for delayed handling */
    if (mbox_recv == NULL) {
      result = sys_mbox_new(&mbox_recv, 1); //INPUT_THREAD_MBOX_SIZE);
      if (result != 0) {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethif_fapi_init: cannot create a mailbox.\n"));
        sys_sem_free(&sem_output);
        return ERR_IF;
      }
    }
    //loc_2348bf40
    if (ethdev_thread == NULL) {
      ethdev_thread = sys_thread_new(0, //"ethifFapiInputThread",
                                     ethif_fapi_thread,
                                     NULL,
                                     0, //INPUT_THREAD_STACKSIZE,
                                     6); //INPUT_THREAD_PRIORITY);
      if (ethdev_thread == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethif_fapi_init: cannot create a thread.\n"));
        sys_sem_free(&sem_output);
        sys_mbox_free(&mbox_recv);
        return ERR_IF;
      }
    }
#endif /* USE_INPUT_THREAD */
    //loc_2348bf68
    low_level_init(netif);

    ethif_fapi_state* state = netif->state;

    if (state->Data_0 != 0)
    {
    	//0x2348bf76
    	if (bData_234df189 == NULL)
    	{
    		bData_234df189 = sys_thread_new(0,
                    poll_phylink_thread,
					(void*) state->Data_0, //arg
                    0, //POLL_THREAD_STACKSIZE,
                    7); //POLL_THREAD_PRIORITY);
    		if (bData_234df189 == NULL) {
				//loc_2346c0c8
				sys_sem_free(&sem_output);
				sys_mbox_post(&mbox_recv, 0);
		        sys_mbox_free(&mbox_recv);
				return ERR_IF;
			}
    	}
    	//loc_2348bfd4
    }
    else
    {
    	//loc_2348bf94
		sys_sem_free(&sem_output);
		sys_mbox_post(&mbox_recv, 0);
        sys_mbox_free(&mbox_recv);
		return ERR_IF;
    }

#if POLL_PHYLINK
  if (poll_thread == NULL) {
    poll_thread = sys_thread_new("pollPhyLinkThread",
                                 poll_phylink_thread,
                                 NULL,
                                 POLL_THREAD_STACKSIZE,
                                 POLL_THREAD_PRIORITY);
    if (poll_thread == NULL) {
      LWIP_DEBUGF(NETIF_DEBUG, ("ethif_fapi_init: cannot create a poll-link thread.\n"));
      sys_sem_free(&sem_output);

#if USE_INPUT_THREAD
      sys_mbox_post(&mbox_recv, NULL); /* to terminate an input thread */
      sys_mbox_free(&mbox_recv);
#endif /* USE_INPUT_THREAD */

      return ERR_IF;
    }
  }
#endif /* POLL_PHYLINK */

  return ERR_OK;
}

#if 0

/*
********************************************************************************
** \brief Gives access to default ethernet driver instance.
**
** This function returns the handle of the default ethernet driver instance.
**
** \return Returns generic FAPI SYS handle which identifies the default
** ethernet driver instance (obsolete).
********************************************************************************
*/
FAPI_SYS_HandleT ethif_fapi_get_default_handle(void)
{
    return (ETH_HDL);
}

//******************************************************************************
//******************************************************************************
//** Local Functions
//******************************************************************************
//******************************************************************************

#endif


/* 2348bbe6 - todo */
void low_level_init(struct netif *netif)
{
	int i; //r5
	ethif_fapi_state* state; //r4
	uint8_t macAddrChars[12];
	eth_stat_params stat;
	uint8_t digitChar[3];

#if 0
	console_send_string("low_level_init (todo.c): TODO\r\n");
#endif

	state = &Data_2396ad0c[Data_234df194].state;
	netif->state = state;

	if (0 == eth_get_mac_address(state->Data_4.Data_4, &macAddrChars[0]))
	{
		//0x2348bc08
		netif->hwaddr_len = 6; //NETIF_MAX_HWADDR_LEN

		for (i = 0; i < 6/*NETIF_MAX_HWADDR_LEN*/; i++)
		{
			//loc_2348bc16
			digitChar[0] = macAddrChars[i*2];
			digitChar[1] = macAddrChars[i*2 + 1];
			digitChar[2] = 0;

			netif->hwaddr[i] = strtoul(digitChar, 0, 16);
		}
		//0x2348bc3e
		netif->mtu = 1500;
		netif->flags = 0x32;

		state->Data_4.Data_0 = 0x20000;
		state->Data_4.Data_4 = Data_234df194;
		state->Data_4.speed = 0;
		state->Data_4.loop = 0;
		state->Data_4.duplex = 1;
		state->Data_4.recvCallback = low_level_input_callback;

		state->Data_0 = eth_open(&state->Data_4);
		if (state->Data_0 != 0)
		{
			state->pbuf = 0;
			state->read_len = 0;

			eth_get_stat(state->Data_0, &stat);

			Data_2396ad0c[Data_234df194].Data_a = state->Data_0;
			Data_2396ad0c[Data_234df194].netif = netif;
			Data_234df194++;
		}
	}
}

#if 0

/*
 * In this function, the hardware should be initialized.
 * Called from ethif_fapi_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this device
 *
 * @return  0 .. on success,
 *         !0 .. on failure
 */
static err_t low_level_init(struct netif *netif)
{
  struct ethif_fapi_state *state;
  int32_t                 retval;
  FAPI_ETH_MacT           macaddr;
  FAPI_ETH_StatParamsT    devstat;

  /* reset eth dev */
  eth_dev.netif  = 0;

  /* initialize netif */
  state = &eth_dev.state;
  netif->state = (void *)state;

  /* maximum transfer unit */
  netif->mtu = 1500;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  //netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

  /* Open this device */
  state->params.version      = FAPI_ETH_VERSION;
  state->params.devIndex     = ETH_IDX;
  state->params.speed        = FAPI_ETH_SPEED_AUTO;
  /* duplex mode is not relevant as autonegotiation is set in speed parameter */
  state->params.duplex       = FAPI_ETH_FULL_DUPLEX;
  state->params.loop         = FAPI_ETH_LOOP_OFF;

#if INPUT_THREAD_POLLING
  state->params.recvCallback = NULL;
#else
  state->params.recvCallback = low_level_input_callback;
#endif
  if (FAPI_ETH_Open(&state->params, NULL) == NULL)
  {
      LWIP_DEBUGF(NETIF_DEBUG, ("low_level_init: device open failed\n"));
      return ERR_IF;
  }

  /* Set MAC hardware address */
  retval = FAPI_ETH_GetMacAddress(ETH_IDX, macaddr);
  if (retval != FAPI_OK) {
    LWIP_DEBUGF(NETIF_DEBUG, ("low_level_init: device getMacAddress failed\n"));
    return ERR_IF;
  }

  /* assign MAC address to netif mac */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;
  memcpy(netif->hwaddr, macaddr, ETHARP_HWADDR_LEN);

  /* Initialize internal variables for input operation */
  state->pbuf         = NULL;
  state->read_len     = 0;

  /* Initialize the snmp variables and counters inside the struct netif. */
  retval = FAPI_ETH_GetStat(ETH_HDL, &devstat);
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd,
                  (retval == FAPI_OK ? (devstat.speed*1000000) : 0));

  /* store connected netif */
  eth_dev.netif  = netif;

  return ERR_OK;
}


static uint64_t get_ts_interval(uint64_t timeStart, uint64_t timeEnd)
{
    if(timeStart <= timeEnd)
        return (timeEnd - timeStart);
    else
        return (0xFFFFFFFFFFFFFFFF - timeStart + timeEnd + 1);
}

#endif

/*
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this device
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

/* 2348bc8e - todo */
static err_t low_level_output(struct netif *netif, struct pbuf *out_pbuf)
{
#if 0
  ethif_fapi_state */*state*/sp_0x10 = netif->state;
  struct pbuf             *cur_pbuf;
  /*char_t*/uint8_t                  *buf;
  int32_t                 total_len/*r7*/ = 0;
  int32_t                 written_len, len/*r6*/;
  /*FAPI_ETH_FrameEndEnumT*/int  frame;
  uint32_t                retry/*r5*/ = 0;
  uint32_t                write_fail_flag/*sp4*/ = 0;
#endif
  /*err_t*/int                   retval;

	console_send_string("low_level_output (todo.c): TODO\r\n");

#if 0

  LWIP_DEBUGF(NETIF_DEBUG, ("low_level_output: called\n"));
  sys_sem_wait(&sem_output);

#if ETH_PAD_SIZE
  pbuf_header(out_pbuf, -ETH_PAD_SIZE); /* drop the padding word */
#endif

  //->loc_2348bd12
  for ( cur_pbuf = out_pbuf; cur_pbuf != 0/*NULL*/; cur_pbuf = cur_pbuf->next ) {
    /* Transmit pbuf one by one */

#if 0
    LWIP_DEBUGF(NETIF_DEBUG, ("low_level_output: pbuf (%x, tot_len=%d)\n",
                cur_pbuf, cur_pbuf->tot_len));
#else
	{
		extern char debug_string[];
		sprintf(debug_string, "low_level_output: pbuf (%x, tot_len=%d)\r\n",
				cur_pbuf, cur_pbuf->tot_len);
		console_send_string(debug_string);
	}
#endif

    buf = /*(char_t *)*/cur_pbuf->payload;
    len = cur_pbuf->len;
    frame = ( cur_pbuf->next == 0/*NULL*/  ?
                    1/*FAPI_ETH_FRAME_END*/ :
                    0/*FAPI_ETH_FRAME_NOTEND*/ );
    do {
    	//loc_2348bcbc
    	//extern int eth_write(int, void*, int, int);
#if 0
      while (get_ts_interval(last_eth_write_ts, FAPI_TIMER_GetTimeStamp(FAPI_TIMER_RESOLUTION_1_USEC)) < 50) ;
#endif

      written_len = eth_write(/*ETH_HDL*/sp_0x10->Data_0, buf, len, frame);
      LWIP_DEBUGF(NETIF_DEBUG,
                  ("low_level_output: write (len=%d, written_len=%d)\n",
                  len, written_len));

#if 0
      last_eth_write_ts = FAPI_TIMER_GetTimeStamp(FAPI_TIMER_RESOLUTION_1_USEC);
#endif

      if (written_len < 0) {
    	  //0x2348bccc
        ++retry;
        if ((written_len == -108/*FAPI_ETH_ERR_AGAIN*/) ||
          (written_len == -109/*FAPI_ETH_ERR_LOCK_FAIL*/)) {
          /* force context switch to avoid deadlock */
          if (written_len == -109/*FAPI_ETH_ERR_LOCK_FAIL*/) {
#if 0
            RTOS_Sleep(1);
#endif
          }
          if (retry > 10/*ETHIF_FAPI_RETRY_MAX*/) {
        	  //loc_2348bcdc
            /* Fatal Error */
            /* Should we check link status(up) yet? */
            LWIP_DEBUGF(NETIF_DEBUG, ("low_level_output: output retry over\n"));
            write_fail_flag = 1;
            break; //->loc_2348bcfc
          }
          else {
        	  //loc_2348bce2
            /* Try RETRY_MAX times */
            written_len = 0;
          }
        }
        else {
          /* Fatal Error */
          LWIP_DEBUGF(NETIF_DEBUG, ("low_level_output: fatal error occured\n"));
          write_fail_flag = 1;
            break;
        }
      }
#if 0
      else if (written_len == 0)
      {
    	  //loc_2348bce6
          RTOS_Sleep(1);
      }
#endif
      else {
        /* write is OK. */
        retry = 0;
      }

      buf += written_len;
      len -= written_len;
      total_len += written_len;
    } while ( len != 0 );
    //0x2348bcf6
    if (write_fail_flag && (total_len > 0)) {/* Something wrong happened. */
    	//0x2348bd00
      /* Here, we have to cancel already-queued data by write.
       * Write(0) doesn't acutually send data to network. */
      LWIP_DEBUGF(NETIF_DEBUG, ("low_level_output: write failed in mid of data\n"));
      written_len = eth_write(/*ETH_HDL*/sp_0x10->Data_0, buf, 0, 1/*FAPI_ETH_FRAME_END*/);
      //->loc_2348bd16
      /* force context switch to avoid deadlock */
#if 0
      if (written_len == FAPI_ETH_ERR_LOCK_FAIL) {
        RTOS_Sleep(1);
      }
#endif
      break;
    }
    //loc_2348bd10
  } /* for */

  if (total_len == out_pbuf->tot_len) {
    retval = ERR_OK;
  }
  else {
    retval = ERR_MEM;
  }

#if ETH_PAD_SIZE
  pbuf_header(out_pbuf, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
  sys_sem_signal(&sem_output);
#endif

  return retval;
}


#if 0

/*
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this device
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
/* ******************************************* */
/* Enable profiling for the following function */
/* ******************************************* */
#undef PROF_TS_ENABLED
#define PROF_TS_ENABLED 0
#define PROF_PRINT_low_level_input (0 && PROF_TS_ENABLED)
#include <profiling.h>
  /* ******************************************* */
#endif
/* 2348bd34 - todo */
static struct pbuf * low_level_input(struct netif *netif)
{
	ethif_fapi_state *state/*r6*/ = netif->state;
	struct pbuf             *head_pbuf, *new_pbuf, *cur_pbuf;
	/*char_t*/uint8_t       *buf;
	int32_t                 total_len/*r5*/, read_len, len;
	int/*FAPI_ETH_FrameEndEnumT*/  frame;
	eth_stat_params    devstat;
	int32_t                 retval;

#if 1
	console_send_string("low_level_input (todo.c): TODO\r\n");
#endif

	cur_pbuf = 0; //NULL;
	buf = 0; //NULL;
	len = 0;
	if (state->pbuf == 0/*NULL*/) {
	    head_pbuf = new_pbuf = 0/*NULL*/;
	    total_len = 0;
	    //->loc_2348bd8c
	}
	else {
		//loc_2348bd4a
	    head_pbuf = new_pbuf = state->pbuf;
	    total_len = state->read_len;
	    /* Demangle state->pbuf until reaching state->read_len. */
	    if (total_len == 0) {
	    	//->loc_2348bdba
	      new_pbuf = cur_pbuf = head_pbuf;
	      buf = cur_pbuf->payload;
	      len = cur_pbuf->len;
	    }
	    else {
	    	//loc_2348bd58
	      for (cur_pbuf = head_pbuf, len = 0; cur_pbuf != 0/*NULL*/; cur_pbuf = cur_pbuf->next) {
	        len += cur_pbuf->len;
	        if (len >= total_len)
	          break;
	      }
	      //loc_2348bd66
	      if (len > total_len) {
	    	  //0x2348bd6a
	        buf = (/*char_t*/uint8_t*)cur_pbuf->payload + (total_len - (len - cur_pbuf->len));
	        len = len - total_len;
	        //->loc_2348bd86
	      }
	      else {
	    	  //loc_2348bd7a
	        if (cur_pbuf->next != 0/*NULL*/) {
	        	//0x2348bd80
	          cur_pbuf = cur_pbuf->next;
	          buf = cur_pbuf->payload;
	          len = cur_pbuf->len;
	        }
	        else { /* no more buffer */
	        	//loc_2348bd8c
	          new_pbuf = 0; //NULL;
	        }
	      }
	    }
#if 0
	    LWIP_DEBUGF(NETIF_DEBUG,
	        ("low_level_input: saved data exists(read_len=%d, buf=0x%x, len=%d)\n",
	        total_len, buf, len));
#endif
	}

	  /* Loop 1
	   * We don't know the exact size of incoming packet.
	   * So we have to repeat read() as far as we get some data. */
	  do {
	    if (new_pbuf == 0/*NULL*/) {
	    	//loc_2348bd8c
	      /* This means that we need one more pbuf. */
	#if ETH_PAD_SIZE
	      if (head_pbuf == NULL) {
	        /* allow room for Ethernet padding */
	        len = LWIP_MEM_ALIGN_SIZE(PBUF_LINK_HLEN + netif->mtu + ETH_PAD_SIZE);
	      }
	      else {
	        len = LWIP_MEM_ALIGN_SIZE(PBUF_LINK_HLEN + netif->mtu);
	      }
	#else
	      len = LWIP_MEM_ALIGN_SIZE(PBUF_LINK_HLEN + netif->mtu);
	#endif
	      new_pbuf = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
	      if (new_pbuf == NULL) {
	        /* Give up reading more. */
#if 0
	        LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: can't alloc pbuf(len=%d)\n",
	                                                                          len));
#endif
	        //->loc_2348be00
	        state->pbuf = head_pbuf;
	        state->read_len = total_len;
	        return 0; //NULL;
	      }
	      //0x2348bda6
#if 0
	      LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: new pbuf(total_len=%d)\n",
	                                                            new_pbuf->tot_len));
#endif
	      if (head_pbuf == 0/*NULL*/) {
	        head_pbuf = new_pbuf;
	#if ETH_PAD_SIZE
	        pbuf_header(head_pbuf, -ETH_PAD_SIZE); /* drop the padding word */
	#endif
	        //->loc_2348bdb8
	      }
	      else {
	    	  //loc_2348bdb2
	        pbuf_cat(head_pbuf, new_pbuf);
	        LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: pbuf_cat(total_len=%d)\n", head_pbuf->tot_len));
	      }
	      //loc_2348bdb8
	      cur_pbuf = new_pbuf;
	      buf = new_pbuf->payload;
	      len = new_pbuf->len;
	    }
	    //loc_2348be26

	#if PROF_PRINT_low_level_input
	    PROF_TS_SET(2);
	#endif

	    /* Loop 2
	     * Read enough bytes to fill this pbuf chain.
	     * cur_pbuf, buf and len should be set before entering this loop. */
	    for(; cur_pbuf != 0/*NULL*/; cur_pbuf = cur_pbuf->next,
	                            buf = cur_pbuf->payload,
	                            len = cur_pbuf->len) {
	      /* Loop 3
	       * Fill up the current pbuf. */
#if 0
	      LWIP_DEBUGF(NETIF_DEBUG,
	            ("low_level_input: read into pbuf(0x%x, payload=0x%x, len=%d)\n",
	            cur_pbuf, buf, len));
#endif
	      do {
	    	  //loc_2348bdc0
	#if PROF_PRINT_low_level_input
	        PROF_TS_SET(3);
	#endif
	        read_len = eth_read(/*ETH_HDL*/state->Data_0, buf, len, &frame);
	#if PROF_PRINT_low_level_input
	        PROF_TS_SET(4);
	#endif

#if 0
	        LWIP_DEBUGF(NETIF_DEBUG,
	                    ("low_level_input: read (len=%d, read_len=%d, frame=%d)\n",
	                    len, read_len, frame));
#endif
	        if (read_len >= 0) {
	        	//0x2348bdd0
	          if (frame == 0/*FAPI_ETH_FRAME_NOTEND*/) {
	        	  //0x2348bdd6
	            if (read_len > 0) {
	            	//0x2348bdda
#if 0
	              LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: frame=NOTEND\n"));
#endif
	              buf += read_len;
	              len -= read_len;
	              total_len += read_len;
	              continue; //->loc_2348be10
	            }
	            else {
	            	//loc_2348be00
	              state->pbuf = head_pbuf;
	              state->read_len = total_len;
	              return 0; //NULL;
	            }
	          }
	          else {
	        	  //loc_2348bde6
	            /* frame == FAPI_ETH_FRAME_END or FAPI_ETH_FRAME_TERM */
	            buf += read_len;
	            len -= read_len;
	            total_len += read_len;
#if 0
	            LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: frame=%s\n",
	                        ((frame == FAPI_ETH_FRAME_END) ? "END":"TERM")) );
#endif
	            break; //->loc_2348be14
	          }
	        }
	        else {
	          /* Error Case */
	        	//loc_2348bdea
	          if (read_len == -109/*FAPI_ETH_ERR_LOCK_FAIL*/) {
	            /* try again, but force context switch before to avoid deadlock */
#if 0
	            RTOS_Sleep(1);
	            LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: LOCK_FAIL detected\n"));
#endif
	            continue; //->loc_2348be10
	          }
	          //0x2348bdee
	          /* Check the current status of device. */
	          retval = eth_get_stat/*FAPI_ETH_GetStat*/(/*ETH_HDL*/state->Data_0, &devstat);
	          if ((retval == 0/*FAPI_OK*/) && (devstat.linkup == 1/*FAPI_ETH_LINKUP*/)) {
	            /* Give up reading more, but later we may be able to read again. */
	        	  //loc_2348be00
#if 0
	            LWIP_DEBUGF(NETIF_DEBUG,
	                              ("low_level_input: save pbuf due to no data\n"));
#endif
	            state->pbuf = head_pbuf;
	            state->read_len = total_len;
	            return 0; //NULL;
	          }
	          //loc_2348be0a
	          /* Fatal Error */
#if 0
	          LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: fatal error occured\n"));
#endif
	          frame = 2; //FAPI_ETH_FRAME_TERM;
	          break; //->loc_2348be38
	        }
	        //loc_2348be10 -> loc_2348bdc0
	      } while (len != 0); /* Loop 3 */
	      //loc_2348be14
	      if ((frame == 1/*FAPI_ETH_FRAME_END*/) || (frame == 2/*FAPI_ETH_FRAME_TERM*/)) {
	        break;
	      }
	    } /* Loop 2 */

	#if PROF_PRINT_low_level_input
	    PROF_TS_SET(5);
	#endif

	    if ( frame == 0/*FAPI_ETH_FRAME_NOTEND*/ ) {
	      new_pbuf = 0; //NULL;
	      continue;
	    }
	    //loc_2348be34
	    if (frame != 1/*FAPI_ETH_FRAME_END*/) {
	    	//loc_2348be38
#if 0
	      LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: frame TERM if not fatal.\n"));
#endif
	      /*if (new_pbuf != head_pbuf)*/ {
	        pbuf_free(new_pbuf);
	      }
	#if ETH_PAD_SIZE
	      pbuf_header(head_pbuf, ETH_PAD_SIZE); /* reclaim the padding word */
	#endif
	      pbuf_free(head_pbuf);
	      head_pbuf = 0; //NULL;
	      total_len = 0;
	    }
	    //loc_2348be4a
	    state->pbuf = 0; //NULL;
	    state->read_len = 0;
	    break;
	  } while (1); /* Loop 1 */

	  /* We should better trim pbuf to appropriate size of actual packet.
	   * but we don't do this here for efficiency.
	   * while this will be done by ip_input(), we need do this here
	   * because ARP reply. */
	  if (head_pbuf != 0/*NULL*/) {
	    pbuf_realloc(head_pbuf, total_len);
	#if ETH_PAD_SIZE
	    pbuf_header(head_pbuf, ETH_PAD_SIZE); /* reclaim the padding word */
	#endif
	  }
	  //loc_2348be5c
	  return head_pbuf;

#if 0
  FAPI_ETH_FrameEndEnumT  frame;
  FAPI_ETH_StatParamsT    devstat;
  int32_t                 retval;

#if PROF_PRINT_low_level_input
  PROF_TS_INIT(5, 1);
#else
  PROF_TS_INIT(0, 1);
#endif

  PROF_TS_BEGIN();

  /*
   * If we have pending pbuf data, cur_pbuf, buf & len should be set up
   * to the last-read position.
   * Otherwise, head_pbuf & new_pbuf should be nulified.

        head_pbuf(state->pbuf)                      new_pbuf      cur_pbuf
          |                                           |             |
          v                                           v             v
          +--------+    +--------+    +--------+      +--------+    +--------+
          |  pbuf  | => |  pbuf  | => |  pbuf  | - - >|  pbuf  | => |  pbuf  |
          +--------+    +--------+    +--------+  |   +--------+    +--------+
          |<----total_len(state->read_len)---->|  |
                   pbuf_cat()
   *
   */
  cur_pbuf = NULL;
  buf = NULL;
  len = 0;
  if (state->pbuf == NULL) {
    head_pbuf = new_pbuf = NULL;
    total_len = 0;
  }
  else {
    head_pbuf = new_pbuf = state->pbuf;
    total_len = state->read_len;
    /* Demangle state->pbuf until reaching state->read_len. */
    if (total_len == 0) {
      new_pbuf = cur_pbuf = head_pbuf;
      buf = cur_pbuf->payload;
      len = cur_pbuf->len;
    }
    else {
      for (cur_pbuf = head_pbuf, len = 0; cur_pbuf != NULL; cur_pbuf = cur_pbuf->next) {
        len += cur_pbuf->len;
        if (len >= total_len)
          break;
      }
      if (len > total_len) {
        buf = (char_t*)cur_pbuf->payload + (total_len - (len - cur_pbuf->len));
        len = len - total_len;
      }
      else {
        if (cur_pbuf->next != NULL) {
          cur_pbuf = cur_pbuf->next;
          buf = cur_pbuf->payload;
          len = cur_pbuf->len;
        }
        else { /* no more buffer */
          new_pbuf = NULL;
        }
      }
    }
    LWIP_DEBUGF(NETIF_DEBUG,
        ("low_level_input: saved data exists(read_len=%d, buf=0x%x, len=%d)\n",
        total_len, buf, len));
  }

#if PROF_PRINT_low_level_input
  PROF_TS_SET(1);
#endif

  /* Loop 1
   * We don't know the exact size of incoming packet.
   * So we have to repeat read() as far as we get some data. */
  do {
    if (new_pbuf == NULL) {
      /* This means that we need one more pbuf. */
#if ETH_PAD_SIZE
      if (head_pbuf == NULL) {
        /* allow room for Ethernet padding */
        len = LWIP_MEM_ALIGN_SIZE(PBUF_LINK_HLEN + netif->mtu + ETH_PAD_SIZE);
      }
      else {
        len = LWIP_MEM_ALIGN_SIZE(PBUF_LINK_HLEN + netif->mtu);
      }
#else
      len = LWIP_MEM_ALIGN_SIZE(PBUF_LINK_HLEN + netif->mtu);
#endif
      new_pbuf = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
      if (new_pbuf == NULL) {
        /* Give up reading more. */
        LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: can't alloc pbuf(len=%d)\n",
                                                                          len));
        state->pbuf = head_pbuf;
        state->read_len = total_len;
        return NULL;
      }


      LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: new pbuf(total_len=%d)\n",
                                                            new_pbuf->tot_len));
      if (head_pbuf == NULL) {
        head_pbuf = new_pbuf;
#if ETH_PAD_SIZE
        pbuf_header(head_pbuf, -ETH_PAD_SIZE); /* drop the padding word */
#endif
      }
      else {
        pbuf_cat(head_pbuf, new_pbuf);
        LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: pbuf_cat(total_len=%d)\n", head_pbuf->tot_len));
      }
      cur_pbuf = new_pbuf;
      buf = new_pbuf->payload;
      len = new_pbuf->len;
    }

#if PROF_PRINT_low_level_input
    PROF_TS_SET(2);
#endif

    /* Loop 2
     * Read enough bytes to fill this pbuf chain.
     * cur_pbuf, buf and len should be set before entering this loop. */
    for(; cur_pbuf != NULL; cur_pbuf = cur_pbuf->next,
                            buf = cur_pbuf->payload,
                            len = cur_pbuf->len) {
      /* Loop 3
       * Fill up the current pbuf. */
      LWIP_DEBUGF(NETIF_DEBUG,
            ("low_level_input: read into pbuf(0x%x, payload=0x%x, len=%d)\n",
            cur_pbuf, buf, len));

      do {
#if PROF_PRINT_low_level_input
        PROF_TS_SET(3);
#endif
        read_len = FAPI_ETH_Read(ETH_HDL, buf, len, &frame);
#if PROF_PRINT_low_level_input
        PROF_TS_SET(4);
#endif
        LWIP_DEBUGF(NETIF_DEBUG,
                    ("low_level_input: read (len=%d, read_len=%d, frame=%d)\n",
                    len, read_len, frame));

        if (read_len >= 0) {
          if (frame == FAPI_ETH_FRAME_NOTEND) {
            if (read_len > 0) {
              LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: frame=NOTEND\n"));
              buf += read_len;
              len -= read_len;
              total_len += read_len;
              continue;
            }
            else {
              state->pbuf = head_pbuf;
              state->read_len = total_len;
              return NULL;
            }
          }
          else {
            /* frame == FAPI_ETH_FRAME_END or FAPI_ETH_FRAME_TERM */
            buf += read_len;
            len -= read_len;
            total_len += read_len;
            LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: frame=%s\n",
                        ((frame == FAPI_ETH_FRAME_END) ? "END":"TERM")) );
            break;
          }
        }
        else {
          /* Error Case */
          if (read_len == FAPI_ETH_ERR_LOCK_FAIL) {
            /* try again, but force context switch before to avoid deadlock */
            RTOS_Sleep(1);
            LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: LOCK_FAIL detected\n"));
            continue;
          }

          /* Check the current status of device. */
          retval = FAPI_ETH_GetStat(ETH_HDL, &devstat);
          if ((retval == FAPI_OK) && (devstat.linkup == FAPI_ETH_LINKUP)) {
            /* Give up reading more, but later we may be able to read again. */
            LWIP_DEBUGF(NETIF_DEBUG,
                              ("low_level_input: save pbuf due to no data\n"));
            state->pbuf = head_pbuf;
            state->read_len = total_len;
            return NULL;
          }

          /* Fatal Error */
          LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: fatal error occured\n"));
          frame = FAPI_ETH_FRAME_TERM;
          break;
        }
      } while (len != 0); /* Loop 3 */

      if ((frame == FAPI_ETH_FRAME_END) || (frame == FAPI_ETH_FRAME_TERM)) {
        break;
      }
    } /* Loop 2 */

#if PROF_PRINT_low_level_input
    PROF_TS_SET(5);
#endif

    if ( frame == FAPI_ETH_FRAME_NOTEND ) {
      new_pbuf = NULL;
      continue;
    }

    if (frame != FAPI_ETH_FRAME_END) {
      LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input: frame TERM if not fatal.\n"));
      if (new_pbuf != head_pbuf) {
        pbuf_free(new_pbuf);
      }
#if ETH_PAD_SIZE
      pbuf_header(head_pbuf, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
      pbuf_free(head_pbuf);
      head_pbuf = NULL;
      total_len = 0;
    }

    state->pbuf = NULL;
    state->read_len = 0;
    break;
  } while (1); /* Loop 1 */

  /* We should better trim pbuf to appropriate size of actual packet.
   * but we don't do this here for efficiency.
   * while this will be done by ip_input(), we need do this here
   * because ARP reply. */
  if (head_pbuf != NULL) {
    pbuf_realloc(head_pbuf, total_len);
#if ETH_PAD_SIZE
    pbuf_header(head_pbuf, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
  }

  PROF_TS_END(total_len);
  PROF_TS_GVAR_ACCUMULATE_COUNT(low_level_input);

#if PROF_PRINT_low_level_input
  if (total_len > 0) {
    PROF_TS_PRINT_BEGIN("low_level_init");
    PROF_TS_PRINT (1, "pbuf initialized");
    PROF_TS_PRINT (2, "pbuf alloc");
    PROF_TS_PRINT (3, "before eth read");
    PROF_TS_PRINTP(4, "after eth read", total_len);
    PROF_TS_PRINT (5, "pbuf filled");
    PROF_TS_PRINTP_END ("low_level_init");
  }
#endif

  return head_pbuf;
#endif
}

#if 0

/*
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
*
 * @param netif the lwip network interface structure for this device
 */
/* ******************************************* */
/* Enable profiling for the following function */
/* ******************************************* */
#undef PROF_TS_ENABLED
#define PROF_TS_ENABLED 0
#define PROF_PRINT_ethif_fapi_input (0 && PROF_TS_ENABLED)
#include <profiling.h>
 #if PROF_PRINT_ethif_fapi_input
static uint64_t g_mem_error = 0;
static uint64_t g_mem_success = 0;
#endif
/* ******************************************* */
static void ethif_fapi_input(struct netif *netif)
{
  struct eth_hdr *ethhdr;
  struct pbuf    *new_pbuf;
#if PROF_TS_ENABLED
  uint32_t total_len = 0;
#endif
#if PROF_PRINT_ethif_fapi_input
  PROF_TS_INIT(5, 1);
#else
  PROF_TS_INIT(0, 1);
#endif

  PROF_TS_BEGIN();

#if PROF_PRINT_ethif_fapi_input
  PROF_TS_RESET(1);
  PROF_TS_RESET(2);
#endif

  LWIP_DEBUGF(NETIF_DEBUG, ("ethif_fapi_input: called\n"));
  for (;;) {
#if PROF_PRINT_ethif_fapi_input
    PROF_TS_ASSIGN(1, 4);
    PROF_TS_SET(4);
#endif

    /* move a received packet into a new pbuf */
    new_pbuf = low_level_input(netif);

#if PROF_PRINT_ethif_fapi_input
    PROF_TS_ASSIGN(2, 5);
    PROF_TS_SET(5);
#endif

    /* no packet could be read, silently ignore this */
    if (new_pbuf == NULL) {
      LWIP_DEBUGF(NETIF_DEBUG,
                              ("ethif_fapi_input: no more pbuf to retreive\n"));
      PROF_TS_END(total_len);
      PROF_TS_GVAR_ACCUMULATE_TIME(ethif_fapi_input);

#if PROF_PRINT_ethif_fapi_input
      if (total_len > 0) {
        PROF_TS_PRINT_BEGIN("ethif_fapi_input");
        if ((g_mem_error > 0) || (g_mem_success > 0)) {
          PROF_TS_PRINTV(1, "before last lli", "overflows", g_mem_error);
          PROF_TS_PRINTV(2, "after last lli", "success", g_mem_success);
        }
        else {
          PROF_TS_PRINT (1, "before last lli");
          PROF_TS_PRINT (2, "after last lli");
        }
        PROF_TS_PRINT (3, "after netif_input");
        PROF_TS_PRINT (4, "before lli");
        PROF_TS_PRINT (5, "after lli");
        PROF_TS_PRINTP_END ("ethif_fapi_input");
      }
#endif

      break;
    }

#if PROF_TS_ENABLED
    total_len += new_pbuf->tot_len;
#endif

    /* points to packet payload, which starts with an Ethernet header */
    ethhdr = new_pbuf->payload;

    switch (htons(ethhdr->type)) {
    /* IP or ARP packet? */
    case ETHTYPE_IP:
    case ETHTYPE_ARP:
      /* full packet send to tcpip_thread to process */
      LWIP_DEBUGF(NETIF_DEBUG, ("ethif_fapi_input: call upper-level input\n"));
      if (netif->input(new_pbuf, netif)!=ERR_OK) {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethif_fapi_input: IP input error\n"));
        pbuf_free(new_pbuf);
        new_pbuf = NULL;
      }
      break;

    default:
      LWIP_DEBUGF(NETIF_DEBUG, ("ethif_fapi_input: unkown message type(%d)\n",
                  htons(ethhdr->type)));
      pbuf_free(new_pbuf);
      new_pbuf = NULL;
      break;
    }

#if PROF_PRINT_ethif_fapi_input
    PROF_TS_SET(3);
#endif
  }

}

#endif

/* 2348bbb8 - complete */
uint32_t low_level_input_callback(void* a)
{
	uint32_t i;

#if 0
	console_send_string("low_level_input_callback (todo.c): TODO\r\n");
#endif

	for (i = 0; i < 1; i++)
	{
		//loc_2348bbbe
		if (Data_2396ad0c[i].Data_a == (int) a)
		{
			break;
		}
	}

	if (i != 1)
	{
		//0x2348bbd0
		Data_234df198 = Data_2396ad0c[i].netif;

		sys_mbox_trypost(&mbox_recv, Data_234df198);
	}
	//loc_2348bbe2
	return 0;
}

#if 0

/*
 * This function identify netif,  and activates ethif_fapi_thread,
 * or calls ethif_fapi_input() directly.
 */
/* ******************************************* */
/* Enable profiling for the following function */
/* ******************************************* */
#undef PROF_TS_ENABLED
#define PROF_TS_ENABLED 0
#include <profiling.h>
 /* ******************************************* */
static uint32_t low_level_input_callback(void *arg)
{
#if INPUT_THREAD_POLLING
#else
#if USE_INPUT_THREAD
  void *msg;
#endif

  PROF_TS_INIT(0, 1);

  PROF_TS_BEGIN();

#if USE_INPUT_THREAD
  /*
   * Here, we consider a handle as a message itself.
   * It is possible because a handle is actually (void *).
   * Be careful, we have to be consistent with ethif_fapi_thread.
   */
  msg = (void *)eth_dev.netif;

  if (last_netif_read_msg != msg) {
#if PROF_PRINT_ethif_fapi_input
    if (sys_mbox_trypost(mbox_recv, msg) != ERR_OK) {
      g_mem_error++;
    }
    else {
      g_mem_success++;
    }
#else
    (void) sys_mbox_trypost(&mbox_recv, msg);
#endif
  }

  PROF_TS_END(1);
  PROF_TS_GVAR_ACCUMULATE_TIME(low_level_input_callback);

#else
  ethif_fapi_input(eth_dev.netif);

#endif /* USE_INPUT_THREAD */

#endif /* !INPUT_THREAD_POLLING */

  return 0;
}

#endif


/* 2348bec8 - complete */
static void poll_phylink_thread(void* arg)
{
#if 0
	console_send_string("poll_phylink_thread (todo.c): TODO\r\n");
#endif

	eth_stat_params stat;
	int old_link = 2;
//	int r6 = (int) arg;

	while (1)
	{
		//loc_2348bed4
		rtos_task_wait(1000);

#if 0
		console_send_string("poll_phylink_thread (todo.c): after wait\r\n");
#endif

		eth_check_link(arg);

		eth_get_stat(arg, &stat);

		if (old_link != stat.linkup)
		{
			old_link = stat.linkup;
		}
	}

#if 0
	  FAPI_ETH_LinkEnumT   old_link = FAPI_ETH_LINKUNKNOWN;
	  FAPI_ETH_StatParamsT stat;

	  for (;;) {
	    RTOS_Sleep( POLL_THREAD_INTERVAL /* msec */ );
	    FAPI_ETH_CheckLink(ETH_HDL);
	    FAPI_ETH_GetStat(ETH_HDL , &stat);
	    if ( old_link != stat.linkup ) {
	      FAPI_SYS_PRINT_DEBUG(2, "link is %s\n", (stat.linkup == FAPI_ETH_LINKUP ?
	                           "UP" : "DOWN" ) );
	      FAPI_SYS_PRINT_DEBUG(2, "  speed is %d\n", stat.speed );
	      FAPI_SYS_PRINT_DEBUG(2, "  duplex is %d\n", stat.duplex );
	      if (old_link != stat.linkup)
	      {
	        if(stat.linkup == FAPI_ETH_LINKUP)
	        {
	            netif_set_link_up(eth_dev.netif);
	        }
	        else
	        {
	            netif_set_link_down(eth_dev.netif);
	        }
	      }
	      old_link = stat.linkup;
	    }
	  } /* for */
#endif
}


#if 1 //USE_INPUT_THREAD
/*
 * Main function to process incoming packets over network.
 * Task is always waked up by receiving a message
 * from FAPI_ETH interrupt handler.
 *
 * Message is actually a handle for this device.
 */
/* 2348be60 - todo */
static void ethif_fapi_thread(void *arg)
{
  void         *msg;
  struct netif *netifp;
  uint32_t     status_reg;

  if (mbox_recv == NULL)
    return;

#if 1
	console_send_string("ethif_fapi_thread (todo.c): TODO\r\n");
#endif

#if INPUT_THREAD_POLLING
  netifp = eth_dev.netif;
  for (;;) {
    RTOS_Sleep(INPUT_THREAD_POLLING);
    ethif_fapi_input(netifp);
  }

#else
  for (;;) {
    /* Waked up by interrupt handler */
    sys_mbox_fetch(&mbox_recv, &msg); //last_netif_read_msg);
//    msg = last_netif_read_msg;
    LWIP_DEBUGF(NETIF_DEBUG, ("ethif_fapi_thread: waked up by message\n"));
    if (msg == NULL)
    {
      LWIP_DEBUGF(NETIF_DEBUG, ("ethif_fapi_thread: exit mainloop\n"));
      OSTaskDel(0xff);
      break;
    }
    else
    {
        netifp = (struct netif *)msg;
      while (1)
      {
    	  //loc_2348be84
    	  struct eth_hdr* ethhdr;
    	  struct pbuf* p = low_level_input(netifp);
    	  if (p == 0)
    	  {
        	  //loc_2348be6a
    		  break;
    	  }
		  //0x2346bfc2
		  ethhdr = p->payload;

		  uint16_t wType = htons(ethhdr->type);
		  switch (wType)
		  {
		  case ETHTYPE_IP:
		  case ETHTYPE_ARP:
			  if (0 != sub_23479b98(p, netifp))
			  {
				  //loc_2348bec0
				  pbuf_free(p);
			  }
			  else if (0 != netifp->input(p, netifp))
			  {
				  //loc_2348bec0
				  pbuf_free(p);
			  }
			  //->loc_2346bfb8
			  break;

		  default:
			  pbuf_free(p);
			  break;
		  }
      } //while (1)
    }
  }
#endif

  return;
}
#endif /* USE_INPUT_THREAD */

#if 0

#if POLL_PHYLINK
/*
 * This thread periodically check the physical link and
 * restart auto negotiation.
 */
static void poll_phylink_thread(void *arg)
{
  FAPI_ETH_LinkEnumT   old_link = FAPI_ETH_LINKUNKNOWN;
  FAPI_ETH_StatParamsT stat;

  for (;;) {
    RTOS_Sleep( POLL_THREAD_INTERVAL /* msec */ );
    FAPI_ETH_CheckLink(ETH_HDL);
    FAPI_ETH_GetStat(ETH_HDL , &stat);
    if ( old_link != stat.linkup ) {
      FAPI_SYS_PRINT_DEBUG(2, "link is %s\n", (stat.linkup == FAPI_ETH_LINKUP ?
                           "UP" : "DOWN" ) );
      FAPI_SYS_PRINT_DEBUG(2, "  speed is %d\n", stat.speed );
      FAPI_SYS_PRINT_DEBUG(2, "  duplex is %d\n", stat.duplex );
      if (old_link != stat.linkup)
      {
        if(stat.linkup == FAPI_ETH_LINKUP)
        {
            netif_set_link_up(eth_dev.netif);
        }
        else
        {
            netif_set_link_down(eth_dev.netif);
        }
      }
      old_link = stat.linkup;
    }
  } /* for */
}
#endif /* POLL_PHYLINK */

#endif
