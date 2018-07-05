#include <stdio.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netpacket/packet.h>

#define KEY "/tmp/HA_Bridge.key"

int execute(){
	return 0;
}

int encrypt(){
	return 0;
}

int getMacAddrs(){
	struct ifaddrs *ifaddr=NULL;
    struct ifaddrs *ifa = NULL;
    int i = 0;
    char mac[12];
    unsigned char *a;

    if (getifaddrs(&ifaddr) == -1){
        perror("getifaddrs");
    }
    else{
        for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){
            if((ifa->ifa_addr) && (ifa->ifa_addr->sa_family == AF_PACKET)){
                struct sockaddr_ll *s = (struct sockaddr_ll*)ifa->ifa_addr;
                //printf("%s ", ifa->ifa_addr);
                for (i=0; i < s->sll_halen; i++){
                    printf("%02x%c", (s->sll_addr[i]), (i+1!=s->sll_halen)?':':'\n');
                    a = s->sll_addr[i];
                    printf("[%x]",a);
                }
            }
        }
       	freeifaddrs(ifaddr);
    }
    return 0;
}

int main(){
	getMacAddrs();
}