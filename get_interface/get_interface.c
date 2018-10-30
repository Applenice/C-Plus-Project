#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>  
#include <pcap.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define STRNCASECMP strncasecmp
#define MAC_ADDR_STR_LEN 32
#define IFL_MAX 16
#define INTFACENAMESIZE 64

struct intrface 
{ 
    unsigned char name [INTFACENAMESIZE];
    unsigned char mac_addr[MAC_ADDR_STR_LEN];
    unsigned char ip_addr4[INET_ADDRSTRLEN];
    unsigned char ip_addr6[INET6_ADDRSTRLEN];
    unsigned char active;
};

/*
 * Local interface globals
 */
static struct intrface ifl[IFL_MAX];
static unsigned int num_interfaces = 0;

/* logfile definitions */
FILE *info = NULL;

static int find_interface_in_list(char *name) 
{
    for (int i = 0; i < IFL_MAX; ++i) 
    {
        if (STRNCASECMP((char*)ifl[i].name, name, strlen(name)) == 0) 
        {
            return i;
        }
    }
   return -1;
}

/**
 * \fn void get_mac_address (char *name, unsigned char mac_addr)
 * \param name interface name
 * \param mac_addr MAC address of interface
 * \return none
 */
void get_mac_address(char *name, unsigned char mac_addr[MAC_ADDR_STR_LEN])
{
    struct ifreq ifr;
    int sock;

    sock=socket(PF_INET, SOCK_STREAM, 0);
    if (sock >= 0) 
    {
        strncpy(ifr.ifr_name,name,sizeof(ifr.ifr_name)-1);
        ifr.ifr_name[sizeof(ifr.ifr_name)-1]='\0';
        ioctl(sock, SIOCGIFHWADDR, &ifr);

        sprintf((char*)mac_addr, "%02x%02x%02x%02x%02x%02x",
            (int)(unsigned char)ifr.ifr_hwaddr.sa_data[0],
            (int)(unsigned char)ifr.ifr_hwaddr.sa_data[1],
            (int)(unsigned char)ifr.ifr_hwaddr.sa_data[2],
            (int)(unsigned char)ifr.ifr_hwaddr.sa_data[3],
            (int)(unsigned char)ifr.ifr_hwaddr.sa_data[4],
            (int)(unsigned char)ifr.ifr_hwaddr.sa_data[5]);
        
        close(sock);
    } 
    else {
	    perror("Failed to create socket\n");
    }
}

static unsigned int interface_list_get() 
{
    pcap_if_t *alldevs;
    pcap_if_t *d;
    int i;
    unsigned int num_ifs = 0;
    char errbuf[PCAP_ERRBUF_SIZE];

    /* Retrieve the device list on the local machine */
    if (pcap_findalldevs(&alldevs, errbuf) == -1) 
    {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        return num_ifs;
    }
    memset(&ifl, 0x00, sizeof(ifl));

    /* store off the interface list */
    for (d = alldevs; d; d = d->next)
    {
        char ip_string[INET6_ADDRSTRLEN];
        pcap_addr_t *dev_addr = NULL; //interface address that used by pcap_findalldevs()

        /* check if the device is suitable for live capture */
        for (dev_addr = d->addresses; dev_addr != NULL; dev_addr = dev_addr->next)
        {
            /* skip the loopback interfaces */
            /* MacOS */
            if (STRNCASECMP(d->name,"lo0",3) == 0) 
            {
                continue;
            }
            /* Linux */
            if (STRNCASECMP(d->name,"lo",2) == 0) 
            {
                continue;
            }
            if (dev_addr->addr && (dev_addr->addr->sa_family == AF_INET || 
				   dev_addr->addr->sa_family == AF_INET6) && dev_addr->netmask) 
            {
                i = find_interface_in_list(d->name);
                if (i > -1) 
                {
                    /* seen this interface before */
                    memset(ip_string, 0x00, INET6_ADDRSTRLEN);
                    if (dev_addr->addr->sa_family == AF_INET6)
                    {
                        inet_ntop(AF_INET6, &((struct sockaddr_in6 *)dev_addr->addr)->sin6_addr, ip_string, INET6_ADDRSTRLEN);
                        snprintf((char*)ifl[i].ip_addr6, INET6_ADDRSTRLEN, "%s", (unsigned char*)ip_string);
                    } else {
                        inet_ntop(AF_INET, &((struct sockaddr_in *)dev_addr->addr)->sin_addr, ip_string, INET_ADDRSTRLEN);
                        snprintf((char*)ifl[i].ip_addr4, INET_ADDRSTRLEN, "%s", (unsigned char*)ip_string);
                    }
                    get_mac_address((char*)ifl[i].name,ifl[i].mac_addr);
                } else {
                    /* first time seeing this interface add to list */
                    snprintf((char*)ifl[num_ifs].name, INTFACENAMESIZE, "%s", d->name);
                    memset(ip_string, 0x00, INET6_ADDRSTRLEN);
                    if (dev_addr->addr->sa_family == AF_INET6) 
                    {
                        inet_ntop(AF_INET6, &((struct sockaddr_in6 *)dev_addr->addr)->sin6_addr, ip_string, INET6_ADDRSTRLEN);
                        snprintf((char*)ifl[num_ifs].ip_addr6, INET6_ADDRSTRLEN, "%s", (unsigned char*)ip_string);
                    } else {
                        inet_ntop(AF_INET, &((struct sockaddr_in *)dev_addr->addr)->sin_addr, ip_string, INET_ADDRSTRLEN);
                        snprintf((char*)ifl[num_ifs].ip_addr4, INET_ADDRSTRLEN, "%s", (unsigned char*)ip_string);
                    }
                    ifl[num_ifs].active = IFF_UP;
                    get_mac_address((char*)ifl[num_ifs].name,ifl[num_ifs].mac_addr);
                    ++num_ifs;
                }
            }
        }
    }

    if (num_ifs == 0)
    {
       fprintf(info, "No suitable interfaces found.\n\n");
    }

    pcap_freealldevs(alldevs);
    return num_ifs;
}

/**
 * \fn void save_interfaces (FILE *f, int num_ifs)
 * \param f file to print to
 * \param num_ifs number of interfaces available
 * \return none
 */
void save_interfaces(FILE *info, int num_ifs) {
{
    int i;
    fprintf(info, "\nInterfaces\n");
    fprintf(info, "==========\n");
    for (i = 0; i < num_ifs; ++i) 
    {
        fprintf(info, "Interface: %s\n", ifl[i].name);
        if (ifl[i].ip_addr4[0] != 0) 
        {
            fprintf(info, "  IPv4 Address: %s\n", ifl[i].ip_addr4);
        }
        if (ifl[i].ip_addr6[0] != 0) 
        {
            fprintf(info, "  IPv6 Address: %s\n", ifl[i].ip_addr6);
        }
        if (ifl[i].mac_addr[0] != 0) 
        {
            fprintf(info, "  MAC Address: %c%c:%c%c:%c%c:%c%c:%c%c:%c%c\n",
                    ifl[i].mac_addr[0], ifl[i].mac_addr[1],
                    ifl[i].mac_addr[2], ifl[i].mac_addr[3],
                    ifl[i].mac_addr[4], ifl[i].mac_addr[5],
                    ifl[i].mac_addr[6], ifl[i].mac_addr[7],
                    ifl[i].mac_addr[8], ifl[i].mac_addr[9],
                    ifl[i].mac_addr[10], ifl[i].mac_addr[11]);
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    info = fopen("test_interface.txt", "a");
    if (info == NULL) 
    {
        printf("could not open interface info file\n");
        return 1;
    }
    num_interfaces = interface_list_get();
    printf("interface nums: %d\n", num_interfaces);
    save_interfaces(info, num_interfaces);
    fclose(info);
    return 0;
}