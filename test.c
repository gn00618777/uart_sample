#include <stdio.h>
#include <stdint.h>

// protocol:
// ID1 ID2 OPcode seq length payload checksum
// Header:ID1 + ID2 + OPcode + seq + len, and Header length =  5
// len byte: length of ID1 + ID2 + OPcode + seq + len + payload + checksum 
// 
#define ID1		(0xab)
#define ID2		(0xba)
#define MSG_BUF_LEN	(256)
#define MSG_HEADER_LEN	(5)
#define OK		(0)
#define NG		(-1)

enum {
	MSG_WAIT_ID1,
	MSG_WAIT_ID2,
	MSG_WAIT_OP,
	MSG_WAIT_SEQ,
	MSG_WAIT_LEN,
};

enum {
	ID1,
	ID2,
	OP,
	SEQ,
	MSG_LEN
};

static int32_t process_packet(int8_t *rbuf) {
	//..
}

static void find_header(int8_t *rbuf, int32_t start, int32_t *state, int32_t len) {

	uint32_t i = 0;

	for(i = start ; i < len ; i++) {
		if(*state == MSG_WAIT_ID1) {
			if(rbuf[i] == ID1)
				*state = MSG_WAIT_ID2;
			else
				*state = MSG_WAIT_ID1;

			continue;

		} else if(*state == MSG_WAIT_ID2) {
			if(rbuf[i] == ID2) {
				*state = MSG_WAIT_OP;
			} else
				*state = MSG_WAIT_ID1;

			continue;

		} else if(*state == MSG_WAIT_OP) {
			if(rbuf[i] != ID1)
				*state = MSG_WAIT_SEQ;
			else
				*state = MSG_WAIT_ID1;
			continue;
		} else if(*state == MSG_WAIT_SEQ) {
			if(rbuf[i] != ID1)
                                *state = MSG_WAIT_LEN;
                        else
                                *state = MSG_WAIT_ID1;
			continue;
		} else if(*state == MSG_WAIT_LEN) {
			if(rbuf[i] != ID1)
                                *state = MSG_WAIT_PAYLOAD;
                        else
                                *state = MSG_WAIT_ID1;
			continue;
		} else if(*state == MSG_WAIT_PAYLOAD) {
			if(i == len - 1)
				*state = MSG_READY;
		}
	}
}

int main() 
{
	int32_t idx = 0;
	int32_t len = 0;
	int32_t fd = 0;
	int8_t rbuf[MAX_BUF_LEN];

	fd = uart_init();
	if(fd < 0) {
		printf("init uart fail\n");
		goto exit;
	}

	while(1) {
		res = select(fd, &read_set, NULL, NULL);
		if(res > 0) {
			if(FD_ISSET($read_set)) {
				rcv = FRONT_UART_read(fd, rbuf + idx, sizeof(rbuf)); //non-blocking
				if(rcv >= 0) {
					len += rcv;
					idx += rcv;
					if(len >= grab) {
						memcpy(packet + start_idx, rbuf + a_idx, grab);
						a_idx += grab;
						len -= grab;
						find_header(packet, start_idx, &state, grab + start_idx);

						if(state == MSG_WAIT_ID1) {
							grab = MSG_HEADER_LEN - 0;
							start_idx = 0;
						}
						if(state == MSG_WAIT_ID2) {
							grab = MSG_HEADER_LEN - 1;
							start_idx = 1;
						}
						if(state == MSG_WAIT_OP) {
							grab = MSG_HEADER_LEN - 2;
							start_idx = 2;
						}
						if(state == MSG_WAIT_SEQ) {
							grab = MSG_HEADER_LEN - 3;
							start_idx = 3;
						}
						if(state == MSG_WAIT_LEN) {
							grab = MSG_HEADER_LEN - 4;
							start_idx = 4;
						}
						if(state == MSG_WAIT_PAYLOAD) {
							grab = packet[MSG_LEN] - MSG_HEADER_LEN;
							start_idx = MSG_HEADER_LEN;
						}

						for(i = 0 ; i < MAX_BUF_LEN - grab, i++)
							packet[i] = packet[i + grab];
					}

                        }  else if(rcv < 0) {
                                //...
                        }

                        if(state == MSG_READY) {
                                //get a complete packet
                                process_packet(packet);
                                grab = MSG_HEADER_LEN;
                                state = MSG_WAIT_ID1;
                                start_idx = 0;
                                len = 0;
                                idx = 0;
                        }
			}
		}
exit:
	close(fd);
	return NG;
}
