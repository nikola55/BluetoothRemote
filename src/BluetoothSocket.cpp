#include "BluetoothSocket.h"

#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <cstdio>

bluetooth::BluetoothSocket::
BluetoothSocket(BluetoothAddress addr) : addr(addr),  fd(-1)
{
     // connect();
}

bluetooth::BluetoothSocket::
BluetoothSocket(BluetoothAddress addr, int fd) : addr(addr), fd(fd)
{

}

unsigned char bluetooth::BluetoothSocket::read()
{
	printf("bluetooth::BluetoothSocket::read()");
    unsigned char byte;
    int res = ::read(fd, &byte, 1);

    if(res == -1) {
        perror("read");
        throw std::runtime_error("cannot read");
    } else if (res == EOF) {
        throw std::runtime_error("EOF encountered");
    }

    return byte;
}

int bluetooth::BluetoothSocket::read(size_t numBytes, void *dstBuf) {
	printf(">>>>bluetooth::BluetoothSocket::read(%d, void *dstBuf)\n", numBytes);
    int numRead = 0;
    int numMore = numBytes;
    int rd;
    unsigned char * buf = (unsigned char*)dstBuf;
    while((rd=::read(fd, &buf[numRead], numMore)) > 0) {
        numRead+=rd;
        numMore-=rd;
    }
    if(rd == -1) {
		printf("~~~~bluetooth::BluetoothSocket::read(%d, void *dstBuf) cannot read\n", numBytes);
        throw std::runtime_error("cannot read");
    } else if (rd == EOF) {
		printf("~~~~bluetooth::BluetoothSocket::read(%d, void *dstBuf) EOF read\n", numBytes);
		throw std::runtime_error("EOF read");
	}
	printf("~~~~bluetooth::BluetoothSocket::read(%d, void *dstBuf)\n", numBytes);
    return numRead;
}

int bluetooth::BluetoothSocket::write(size_t numBytes, const void *srcBuf)
{
    int szSent,
        offset = 0,
        toSend = numBytes;
    const unsigned char *buf = (const unsigned char *)srcBuf;
    while((szSent=::write(fd, &buf[offset], toSend)) > 0)
    {
        offset+=szSent;
        toSend-=szSent;
    }
    if(szSent==-1)
    {
        perror("write");
        throw std::runtime_error("cannot perform send");
    }
    return offset;
}

bluetooth::BluetoothSocket::~BluetoothSocket()
{
    if(fd != -1)
    {
        ::close(fd);
		fd = -1;
    }
}
