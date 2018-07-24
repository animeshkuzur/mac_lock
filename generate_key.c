#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netpacket/packet.h>
#include "fwriter.h"
#include "base64_encode.h"
#include "freader.h"

char KEY_FILE_NAME[] = "/HA_Bridge.key";
#define APP "sudo systemctl start <service>"

void init();
void convertToString(unsigned char *);
void scrambler(unsigned char *);
int check(char *);
int execute();
int getMacAddrs();

char *path;

int execute(){
    printf("Hello World!\n");
    int status = system("");
	return 0;
}

int generateKey(char *key,char *path){
    if(write_file(path,key)){
    	return 0;
    }
	return 1;
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

                    if(generateKey(key,path)){
                        printf("Key generated\n");
                    }
                    else{
                        printf("Unable to generate Key");
                    }
                }
            }
        }
       	freeifaddrs(ifaddr);
    }
    return 0;
}

void createPath(char *path){
    int len = strlen(path);int i;
    for(i=0;i<strlen(KEY_FILE_NAME);i++){
        path[i+len]=KEY_FILE_NAME[i];
    }
    path[i+len]='\0';
}

void init(){
    int i=0;
    FILE *fp;
    char *homedir = getenv("HOME");
    if (homedir != NULL) {
        path=homedir;
    }
    else{
        uid_t uid = getuid();
        struct passwd *pw = getpwuid(uid);
        if (pw == NULL) {
            printf("Failed\n");
            exit(EXIT_FAILURE);
        }
        path=pw->pw_dir;
    }
    createPath(path);
}

int main(){
    init();
	getMacAddrs();
    return 0;
}