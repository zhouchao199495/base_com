#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

int main(){
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(5000);  //端口
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
   
    int pos = 0, send_bytes, loop = 0;
    char buf[1024] = {0};
    sprintf(buf, "%016x", 10000000);
    int len = strlen(buf);
	do
	{
		send_bytes = send(sock, buf+pos, len-pos, 0);
		if (send_bytes > 0)
		{
			pos += send_bytes;
		}
		else if(send_bytes < 0)
		{
			// if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
			// {
			// 	usleep(1);
			// 	continue;
			// }
			// else
			// {
			// 	fprintf(stderr, "send_bytes=[%d]error[%d][%s]\n", send_bytes, errno, strerror(errno));
			// 	//disconnectSwClient(client);
			// 	return -1;
			// }
		}
		else
		{
			fprintf(stderr, "send_bytes=[%d]error[%d][%s]\n", send_bytes, errno, strerror(errno));
			//disconnectSwClient(client);
			return -1;
		}
	} while (pos < (int)len && ++loop < 1000);
    //读取服务器传回的数据
    char buffer[40];
    read(sock, buffer, sizeof(buffer)-1);
   
    printf("Message form server: %s\n", buffer);
   
    //关闭套接字
    close(sock);
    return 0;
}
