// Patryk Maciąg 331542
#include "utils.h"
#include "icmp_send.h"
#include "icmp_receive.h"


int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage: sudo ./traceroute <IP>\n");
		return EXIT_FAILURE;
    }
    
	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0) {
		fprintf(stderr, "socket error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}

	int final_ip = FALSE;
	int pid = getpid();

	// Wpisujemy adres odbiorcy do struktury adresowej:
	struct sockaddr_in recipient;
	bzero (&recipient, sizeof(recipient));
	recipient.sin_family = AF_INET;
	if (inet_pton(AF_INET, argv[1], &recipient.sin_addr) != 1) {
		fprintf(stderr, "IP error\n");
		return EXIT_FAILURE;
	}

	for (int ttl = 1; ttl <= 30 && final_ip == FALSE; ttl++) {
		for (int i = 0; i < 3; i++) {
			icmp_send(sockfd, recipient, ttl, pid);
		}

		int time_sum = 0;
		int packets = 0;
		char ip[3][20];

		struct timespec start_time, end_time;
		clock_gettime(CLOCK_MONOTONIC, &start_time);

		struct pollfd ps;
		ps.fd = sockfd;
		ps.events = POLLIN;
		ps.revents = 0;


		while (packets < 3) {
			int ready = poll (&ps, 1, 1000);

			if (ready > 0 && ps.revents == POLLIN) {
    			// Deskryptor jest gotowy do czytania.
				icmp_receive(sockfd, ttl, pid, &packets, &final_ip, ip + packets);
			
				clock_gettime(CLOCK_MONOTONIC, &end_time);	
				long time_sec = end_time.tv_sec - start_time.tv_sec;
    			long time_nsec = end_time.tv_nsec - start_time.tv_nsec;
				time_sum += 1000 * time_sec + time_nsec / 1000000;


			} else if (ready == 0) {
    			// Timeout - żaden deskryptor nie jest gotowy.
				break;
			} else {
				fprintf(stderr, "socket read error\n");
				return EXIT_FAILURE;
			}
		}

		int average_time = (packets < 3) ? -1 : time_sum / 3;

	
		printf("%d. ", ttl);
		if (packets == 0) {
			printf("*");
		}
		else {
			printf("%s", ip[0]);
			if (packets > 1 && strcmp(ip[0],ip[1])) {
				printf(" %s", ip[1]);
			}
			if (packets > 2 && strcmp(ip[0],ip[2]) && strcmp(ip[1],ip[2])) {
				printf(" %s", ip[2]);
			}

			if (average_time < 0) {
				printf(" ???");	
			} else {
				printf(" %dms", average_time);
			}
		}
		printf("\n");

		packets = 0;
	}

	return 0;
}
