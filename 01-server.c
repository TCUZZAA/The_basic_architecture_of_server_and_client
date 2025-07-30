//服务端程序
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
int main(){

	//创建socket
	int lfd =socket(AF_INET,SOCK_STREAM,0);
	if(lfd<0){
	    perror("socket error");
	    return -1;
	}
	
	//绑定
	struct sockaddr_in serv;
	bzero(&serv,sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port=htons(8888);
	serv.sin_addr.s_addr=htonl(INADDR_ANY);//使用本机任意可用ip
	int ret = bind(lfd,(struct sockaddr *)&serv,sizeof(serv));
	if(ret<0){
		perror("bind error");
		return -1;
	}
	//监听
	listen(lfd,128);
	sleep(30);
	struct sockaddr_in client;
	socklen_t len =sizeof(client);
	int cfd = accept(lfd,(struct sockaddr *)&client ,&len);//len是输入输出参数
	char sIP[16];
	memset(sIP,0x00,sizeof(sIP));
	printf("client:ip:[%s] port:[%d]",inet_ntop(AF_INET,&client.sin_addr.s_addr,sIP,sizeof(sIP)),ntohs(client.sin_port));
	printf("lfd==[%d],cfd==[%d]\n",lfd,cfd);
	int n=0;
	int i=0;
	char buf[1024];
	while(1){
		//读数据
		memset(buf,0x00,sizeof(buf));
		n=read(cfd,buf,sizeof(buf));
		if(n<=0){
		    printf("read error or client close,n==[%d]\n",n);
		    break;
		}
		printf("n==[%d],buf==[%s]",n,buf);

		for(i=0;i<n;i++){
			buf[i] = toupper(buf[i]);
		}
		//发送数据
		write(cfd,buf,n);
	
	}
	close(lfd);
	close(cfd);
}
