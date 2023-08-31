
#include "musb/mu_arch.h"
#include "musb/mu_cdi.h"
#include "musb/mu_descs.h"
#include "musb/mu_diag.h"
#include "musb/mu_mem.h"
#include "musb/mu_stdio.h"
#include "musb/class/mu_asix.h"
#include "lwip/tcpip.h"
#include "netif/etharp.h"


static struct netif Data_235b0c60;
static ip_addr_t network_ipaddr;
static ip_addr_t network_netmask;
static ip_addr_t network_gw;

static err_t low_level_init(struct netif *netif);
static err_t low_level_output(struct netif *netif, struct pbuf *p);


int ethif_asixusb_init(MUSB_AsixDevice* pAsixDevice)
{
	console_send_string("ethif_asixusb_init (todo.c): TODO\r\n");

#if 0
	IP4_ADDR(&network_gw, 192,168,2,1);
	IP4_ADDR(&network_ipaddr, 192,168,2,234);
	IP4_ADDR(&network_netmask, 255,255,255,0);
#endif

	pAsixDevice->pNetif = netif_add(&Data_235b0c60,
			&network_ipaddr,
			&network_netmask,
			&network_gw,
			pAsixDevice, //state
			low_level_init,
			tcpip_input);

	if (pAsixDevice->pNetif == NULL)
	{
#if 1
		{
			extern char debug_string[];
			sprintf(debug_string, "MGC_AsixThread: pAsixDevice->pNetif = 0x%p\r\n",
					pAsixDevice->pNetif);
			console_send_string(debug_string);
		}
#endif
	}

	netif_set_default(&Data_235b0c60);

	return 0;
}


int ethif_asixusb_input(MUSB_AsixDevice* pAsixDevice, uint8_t* buf_ptr, uint32_t packet_len)
{
#if 0
	console_send_string("ethif_asixusb_input (todo.c): TODO\r\n");
#endif

#if 0
	{
		extern char debug_string[];
		sprintf(debug_string, "ethif_asixusb_input: packet_len = %d\r\n", packet_len);
		console_send_string(debug_string);
	}
#endif

	struct pbuf *p, *q;

	/* We allocate a pbuf chain of pbufs from the pool. */
	p = pbuf_alloc(PBUF_RAW, packet_len, PBUF_POOL);

	/* We iterate over the pbuf chain until we have read the entire
	* packet into the pbuf. */
	for (q = p; q != NULL; q = q->next)
	{
		/* Read enough bytes to fill this pbuf in the chain. The
		* available data in the pbuf is given by the q->len
		* variable.
		* This does not necessarily have to be a memcpy, you can also preallocate
		* pbufs for a DMA-enabled MAC and after receiving truncate it to the
		* actually received size. In this case, ensure the tot_len member of the
		* pbuf is the sum of the chained pbuf len members.
		*/
		if (q->len != packet_len)
		{
			console_send_string("ethif_asixusb_input: WRONG ASSUMPTION about pbuf chain!!!\r\n");
#if 1
			{
				extern char debug_string[];
				sprintf(debug_string, "ethif_asixusb_input: packet_len=%d, q->len=%d\r\n",
						packet_len, q->len);
				console_send_string(debug_string);
			}
#endif
		}
		memcpy(q->payload, buf_ptr, q->len);

		buf_ptr += q->len;

	} //for (q = p; q != NULL; q = q->next)

	struct netif* netif = pAsixDevice->pNetif;
	struct eth_hdr* ethhdr = p->payload;

	uint16_t wType = htons(ethhdr->type);
#if 0
	{
		extern char debug_string[];
		sprintf(debug_string, "ethif_asixusb_input: wType = 0x%04x\r\n", wType);
		console_send_string(debug_string);
	}
#endif

	switch (wType)
	{
		case ETHTYPE_IP:
		case ETHTYPE_ARP:
			/* full packet send to tcpip_thread to process */
			if (netif->input(p, netif) != ERR_OK)
			{
				console_send_string("ethif_asixusb_input: IP input error\r\n");
				pbuf_free(p);
				p = NULL;
			}
			break;

		default:
			pbuf_free(p);
			p = NULL;
			break;
	}

	return 0;
}


static err_t low_level_init(struct netif *netif)
{
	console_send_string("low_level_init (todo.c): TODO\r\n");

	MUSB_AsixDevice* pAsixDevice = (MUSB_AsixDevice*) netif->state;

	netif->name[0] = 'a';
	netif->name[1] = 'x';

	/* We directly use etharp_output() here to save a function call.
	* You can instead declare your own function an call etharp_output()
	* from it if you have to do some checks before sending (e.g. if link
	* is available...) */
	netif->output = etharp_output;
	netif->linkoutput = low_level_output;

	/* set MAC hardware address length */
	netif->hwaddr_len = ETHARP_HWADDR_LEN;

	/* set MAC hardware address */
	netif->hwaddr[0] = pAsixDevice->abMacAddress[0];
	netif->hwaddr[1] = pAsixDevice->abMacAddress[1];
	netif->hwaddr[2] = pAsixDevice->abMacAddress[2];
	netif->hwaddr[3] = pAsixDevice->abMacAddress[3];
	netif->hwaddr[4] = pAsixDevice->abMacAddress[4];
	netif->hwaddr[5] = pAsixDevice->abMacAddress[5];

	/* maximum transfer unit */
	netif->mtu = 1500;

	/* device capabilities */
	/* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP/* | NETIF_FLAG_LINK_UP*/;

	return ERR_OK;
}


static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
#if 0
	console_send_string("low_level_output (todo.c): TODO\r\n");
#endif

	MUSB_AsixDevice* pAsixDevice = (MUSB_AsixDevice*) netif->state;
	struct pbuf *q;

	if ((pAsixDevice == NULL) || (pAsixDevice->send == NULL))
	{
		return ERR_IF;
	}

	for (q = p; q != NULL; q = q->next)
	{
		/* Send the data from the pbuf to the interface, one pbuf at a
		time. The size of the data in each pbuf is kept in the ->len
		variable. */
#if 0
		{
			extern char debug_string[];
			sprintf(debug_string, "low_level_output: q->len = %d\r\n",
					q->len);
			console_send_string(debug_string);
		}
#endif

		pAsixDevice->send(pAsixDevice, q->payload, q->len);
	}

	return ERR_OK;
}


