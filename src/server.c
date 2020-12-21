#include <stdio.h>      // printf(), fprintf(), perror()
#include <sys/socket.h> // socket(), bind(), accept(), listen()
#include <arpa/inet.h>  // struct sockeraddr_in, struct sockadder, inet_ntoa()
#include <stdlib.h>     // atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>     // memset()
#include <unistd.h>     // close()


#define QUEUELIMIT 5


int main(int argc, char* argv[]){

    int servSock;                       // server socket descriptor
    int clntSock;                       // client socket descriptor
    struct sockaddr_in servSockAddr;  // server internet socket address
    struct sockaddr_in clntSockAddr;  // client internet socket address
    unsigned short servPort;            // server port number
    unsigned int clntLen;               // client internet socket address length

    // Usage : ./[excute file name] [PORT]
    if ( argc != 2 ) {
        fprintf(stderr, "error : argument count mismatch\n");
        exit(EXIT_FAILURE);
    }

    if ( (servPort = (unsigned short) atoi(argv[1])) == 0 ) {
        fprintf(stderr, "invalid port number.\n");
        exit(EXIT_FAILURE);
    }

    /*
    socket()によってOSにソケットの作成が依頼される.
    第1引数 : プロトコルファミリの指定, PF_INETはTCP/IPプロトコルを使うことを意味する.
    第2引数 : ソケットの種類を指定
    第3引数 : プロトコルを指定, TCPを意味するIPPROTO_TCPを指定
    */
    if( (servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ) {
        perror("socket() failed.");
        exit(EXIT_FAILURE);
    }

    /*
    サーバー側のsockaddr_in構造体を初期化
    確保されたメモリ領域にどんな値が入っているかわからないのでmemsetを用いてゼロクリアする.
    */
    memset(&servSockAddr, 0, sizeof(servSockAddr));
    servSockAddr.sin_family         = AF_INET;
    servSockAddr.sin_addr.s_addr    = htonl(INADDR_ANY);
    servSockAddr.sin_port           = htons(servPort);

    /*
    bind()で作成したソケットにIPアドレスとポート番号を紐つける.
    */
    if ( bind(servSock, (struct sockaddr *) &servSockAddr, sizeof(servSockAddr)) < 0 ){
        perror("bind() failed.");
        exit(EXIT_FAILURE);
    }

    /*
    listen()が呼び出されることでクライアントから接続状態を受け付けるようになる.
    */
    if ( listen(servSock, QUEUELIMIT) < 0 ) {
        perror("listen() failed.");
        exit(EXIT_FAILURE);
    }

    while(1) {
        clntLen = sizeof(clntSockAddr);
        if ( (clntSock = accept(servSock, (struct sockaddr *) &clntSockAddr, &clntLen)) < 0 ) {
            perror("accept() failed.");
        }

        printf("connected from %s.\n", inet_ntoa(clntSockAddr.sin_addr));
        close(clntSock);
    }

    return EXIT_SUCCESS;

}