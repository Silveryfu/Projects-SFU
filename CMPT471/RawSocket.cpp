/*
 * Author: Silvery Fu, dif@sfu.ca
 * Date: July 10th, 2014
 *
 * This code is used to generate a ICMP ECHO_REQUEST
 * message via a raw socket.
 */

#include <iostream>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>

#define FRAME_SIZE 1526
#define IP_PACKET_SIZE 20

/*
 * The default destination MAC address
 * is Autumn's eth1 : 00:50:56:a4:67:5e;
 * And IP address is its IP address:
 * 172.17.1.19; The source interface IP
 * and MAC address can be obtained dynamically
 * but the interface is default to the host's eth1
 */

#define DEST_MAC_1 0x00
#define DEST_MAC_2 0x50
#define DEST_MAC_3 0x56
#define DEST_MAC_4 0xA4
#define DEST_MAC_5 0x67
#define DEST_MAC_6 0x5E

const char *DEST_IP = "172.17.1.19";

/* The checksum function */

unsigned short check_sum(unsigned short *word, int length) {
	unsigned long sum;
	for(sum = 0; length > 0; length--)
		sum += *word++;
	sum = (sum >> 16) + (sum &0xffff);
	sum += (sum >> 16);
	return (unsigned short)(~sum);
}

int main() {
	ssize_t ioRtnCode;

	/* Open a packet level socket.
	 * Only the root id has privilege
	 */

	int sd = 0;

	std::cout << "Attempting to create socket; " ;
	sd = socket(PF_PACKET, SOCK_RAW, ETHERTYPE_IP) ;
	if(sd < 0) {
		std::cout << "failed.\n Result is " << sd
				  << ". Are you running as root?\n ";
		std::cout << strerror(errno) << "\n";
		std::exit(1);
	} else {
		std::cout << "descriptor is " << sd << ".\n" ;
	}

	/*
	 * Get the interface number corresponding to an
	 * interface name, in order to send the frame.
	 * Here the name is hard-wired.
	 */

	struct ifreq ifioctl ;
	int ifNdx ;

	/* Obtain the interface by its name */

	ifioctl.ifr_ifrn.ifrn_name[0] = 'e' ;
	ifioctl.ifr_ifrn.ifrn_name[1] = 't' ;
	ifioctl.ifr_ifrn.ifrn_name[2] = 'h' ;
	ifioctl.ifr_ifrn.ifrn_name[3] = '1' ;
	ifioctl.ifr_ifrn.ifrn_name[4] = '\0' ;
	std::cout << "Attempting to obtain interface number for "
			  << ifioctl.ifr_ifrn.ifrn_name << " ... " ;
	ioRtnCode = ioctl(sd,SIOCGIFINDEX, &ifioctl) ;
	if (ioRtnCode < 0) {
		std::cout << "failed.\n Result is " << ioRtnCode
				  << ". Are you running as root?\n " ;
		std::cout << strerror(errno) << "\n" ;
		std::exit(2) ;
	} else {
		ifNdx = ifioctl.ifr_ifru.ifru_ivalue ;
		std::cout << "index is " << ifNdx << "\n" ;
	}

	/*
	 * Define the socket address attribute as a
	 * structure here ourself
	 */

	struct sockaddr_ll
	  {
	    unsigned short int sll_family;
	    unsigned short int sll_protocol;
	    int sll_ifindex;
	    unsigned short int sll_hatype;
	    unsigned char sll_pkttype;
	    unsigned char sll_halen;
	    unsigned char sll_addr[8];
	  };

	/*
	  Define the address structure used for a packet socket. Family
	  and interface index (ifindex) are necessary. sll_addr
	  is unimportant for a raw packet socket. It's the address
	  that's loaded in the frame that counts.
	*/

	struct sockaddr_ll dllAddr ;

	dllAddr.sll_family = AF_PACKET;  // AF_PACKET and PF_PACKET are the same
	dllAddr.sll_protocol = 0;
	dllAddr.sll_ifindex = ifNdx;
	dllAddr.sll_hatype = 0;
	dllAddr.sll_pkttype = 0;
	dllAddr.sll_halen = 6;
	dllAddr.sll_addr[0] = 0x00;
	dllAddr.sll_addr[1] = 0x00;
	dllAddr.sll_addr[2] = 0x00;
	dllAddr.sll_addr[3] = 0x00;
	dllAddr.sll_addr[4] = 0x00;
	dllAddr.sll_addr[5] = 0x00;
	dllAddr.sll_addr[6] = 0;
	dllAddr.sll_addr[7] = 0;

	/*
	 * Declare a generous amount of buffer space in which we can
	 * build the frame, and a variable to hold the length of the
	 * finished frame.
	 */

	char myframe[FRAME_SIZE] ;
	int frameLen = 0;

	/* Construct the ETHERNET header *
	 *
	 * Obtain the MAC address of the source interface
	 * using signal SIOCGIFHWADDR via ioctl
	 */

	ioctl(sd, SIOCGIFHWADDR, &ifioctl);
	const unsigned char* mac_addr=(unsigned char*)ifioctl.ifr_hwaddr.sa_data;

	struct ether_header *eth = (struct ether_header *)myframe;
	memset(myframe, 0, FRAME_SIZE);

	eth->ether_shost[0] = mac_addr[0];
	eth->ether_shost[1] = mac_addr[1];
	eth->ether_shost[2] = mac_addr[2];
	eth->ether_shost[3] = mac_addr[3];
	eth->ether_shost[4] = mac_addr[4];
	eth->ether_shost[5] = mac_addr[5];
	eth->ether_dhost[0] = DEST_MAC_1;
	eth->ether_dhost[1] = DEST_MAC_2;
	eth->ether_dhost[2] = DEST_MAC_3;
	eth->ether_dhost[3] = DEST_MAC_4;
	eth->ether_dhost[4] = DEST_MAC_5;
	eth->ether_dhost[5] = DEST_MAC_6;
	eth->ether_type = htons(ETH_P_IP);

	frameLen += sizeof(struct ether_header);

	/* Construct the IP packet
	 *
	 * Obtain the IP address of the source interface
	 * using signal SIOCGIFADDR via ioctl
	 */

	ioctl(sd,SIOCGIFADDR,&ifioctl);
	struct sockaddr_in* ipaddr = (struct sockaddr_in*)&ifioctl.ifr_addr;
	printf("Source IP address: %s\n",inet_ntoa(ipaddr->sin_addr));


	struct iphdr *ip_header = (struct iphdr *) (myframe + sizeof(struct ether_header));
	ip_header->ihl = 5;      // The IP header length
	ip_header->version = 4;  // Specify the protocol version
	ip_header->tos = 0x00;  // Type of service set to default
	ip_header->id = htons(137); // Give this packet a random id
	ip_header->frag_off = ntohs(IP_DF); // Don't fragment
	ip_header->ttl = 64; // Set it according to the value 'ping' uses
	ip_header->protocol = 1; // Identify the ICMP protocol
	ip_header->saddr = inet_addr(inet_ntoa(ipaddr->sin_addr)); // Set the source IP address
	ip_header->daddr = inet_addr(DEST_IP); // Set the destination IP address

	frameLen += sizeof(struct iphdr); // Increment the frame length

	/* Construct the ICMP message *
	 *
	 * Notice we leave the id and sequence number
	 * to be 0 as default
	 */

    struct icmphdr *icmp_header = (struct icmphdr *)(myframe + sizeof(struct ether_header) + sizeof(struct iphdr));

    icmp_header->type = ICMP_ECHO;
    icmp_header->code = 0; // The type and code together specify an echo_request message
    icmp_header->checksum = 0; // Make the checksum zero then calculate it
    icmp_header->checksum = check_sum((unsigned short*)icmp_header, sizeof(icmphdr));

    frameLen += sizeof(struct icmphdr);

    /*
     * Finally, set the IP packet total length field
     * and calculate the checksum (because it includes
     * the pay load (ICMP message);
     */

	ip_header->tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr));
	ip_header->check = 0; // Set the checksum zero then calculate it
	ip_header->check = check_sum((unsigned short *)ip_header, sizeof(struct iphdr));

    /*
     * Send the frame. Set the length of the finished frame.
     */

    std::cout << "Attempting to send " << frameLen << " bytes ..." ;

    const struct sockaddr *saPtr ;
    size_t saLen ;
    saPtr = reinterpret_cast<const sockaddr *>(&dllAddr) ;
    saLen = sizeof(dllAddr) ;
    ioRtnCode = sendto(sd, myframe, frameLen, 0, saPtr, saLen) ;

    if (ioRtnCode < 0) {
    	std::cout << "failed.\n Result is " << ioRtnCode << "\n " ;
    	std::cout << strerror(errno) << "\n" ;
    	std::exit(3) ;
    } else {
    	std::cout << "success!\n" ;
    }

	return 0;
}





