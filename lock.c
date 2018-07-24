#include <stdio.h>
#include<string.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netpacket/packet.h>
#include "base64_encode.h"

#define KEY "/tmp/HA_Bridge.key"
#define APP "sudo systemctl start <service>"

void convertToString(unsigned char *);
void scrambler(unsigned char *);
int execute();
int encrypt();
int getMacAddrs();

int execute(){
    printf("Hello World!\n");
	return 0;
}

int encrypt(){
	return 0;
}

void scrambler(unsigned char *mac){
    char key[]={'1','0','5','2','9','3','4','6','e','d','7','f','a','c','8','b'};
            //   0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
    int i=0;
    while(mac[i] != '\0'){
        char c = mac[i];
        switch(c){
            case '0': mac[i] = key[0];
                    break;
            case '1':mac[i] = key[1];
                    break;
            case '2':mac[i] = key[2];
                    break;
            case '3':mac[i] = key[3];
                    break;
            case '4':mac[i] = key[4];
                    break;
            case '5':mac[i] = key[5];
                    break;
            case '6':mac[i] = key[6];
                    break;
            case '7':mac[i] = key[7];
                    break;
            case '8':mac[i] = key[8];
                    break;
            case '9':mac[i] = key[9];
                    break;
            case 'a':mac[i] = key[10];
                    break;
            case 'b':mac[i] = key[11];
                    break;
            case 'c':mac[i] = key[12];
                    break;
            case 'd':mac[i] = key[13];
                    break;
            case 'e':mac[i] = key[14];
                    break;
            case 'f':mac[i] = key[15];
                    break;
        }
        i++;
    }

}

void convertToString(unsigned char *mac){
    char buffer[13];
    int flag=0;
    if(mac[0]!=NULL){
        sprintf(buffer,"%x%x%x%x%x%x",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
        flag=1;
    }
    int i=0;
    for(i=0;i<13;i++){
        mac[i] = tolower(buffer[i]);
    }
    mac[i]='\0';
}

int getMacAddrs(){
	struct ifaddrs *ifaddr=NULL;
    struct ifaddrs *ifa = NULL;
    int i = 0;
    unsigned char mac[13];
    char key[1024] = "";
    bzero(mac,sizeof(char));

    if (getifaddrs(&ifaddr) == -1){
        printf("getifaddrs");
    }
    else{
        for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){
            if((ifa->ifa_addr) && (ifa->ifa_addr->sa_family == AF_PACKET)){
                struct sockaddr_ll *s = (struct sockaddr_ll*)ifa->ifa_addr;
                //printf("%s ", ifa->ifa_addr);
                for (i=0; i < s->sll_halen; i++){
                    //printf("%02x%c", (s->sll_addr[i]), (i+1!=s->sll_halen)?':':'\n');
                    /*a = s->sll_addr[i];*/
                    if(s->sll_addr[i]==NULL){
                        mac[i]=NULL;
                        continue;
                    }
                    else{
                        //printf("%02x%c", (s->sll_addr[i]), (i+1!=s->sll_halen)?':':'\n');
                        mac[i]=s->sll_addr[i];
                    }
                }
                if(mac[0]!=NULL){
                    convertToString(mac);
                    printf("%s\n",mac);
                    scrambler(mac);
                    printf("%s\n",mac);
                    b64_encode(mac,key);
                    printf("%s\n",key);

                }                
                
                /*unsigned char *mac_addr =*/ /*int a = convertToString(mac);*/
                /*printf("%s",mac_addr);
                printf("\n");*/
            }
        }
       	freeifaddrs(ifaddr);
    }
    return 0;
}

int main(){
	getMacAddrs();
    /*char mysrc[] = "My bonnie is over the          ";
    char myb64[1024] = "";

    b64_encode(mysrc, myb64);
    printf("The string\n[%s]\nencodes into base64 as:\n[%s]\n", mysrc, myb64);*/
}