// Patryk Maciąg 331542
#include "icmp_receive.h"
#include <netinet/ip.h>
#include <sys/types.h>

void print_as_bytes(unsigned char* buff, ssize_t length)
{
	for (ssize_t i = 0; i < length; i++, buff++)
		printf ("%.2x ", *buff);	
}


void icmp_receive(int sockfd, int ttl, int pid, int *packets, int *final_ip, char (*ip)[20])
{
	struct sockaddr_in sender; // info o nadawcy
	socklen_t sender_len = sizeof(sender);
	u_int8_t buffer[IP_MAXPACKET]; // pakiet jako ciąg bajtów

	ssize_t packet_len = recvfrom (sockfd, buffer, IP_MAXPACKET, 0, (struct sockaddr*)&sender, &sender_len); // odbiera kolejny pakiet z kolejki związanej z gniazdem
	if (packet_len < 0) {
		fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	struct ip *ip_header = (struct ip*) buffer;
	u_int8_t* icmp_packet = buffer + 4 * ip_header->ip_hl;
	struct icmp *icmp_header = (struct icmp *) icmp_packet;

	// Pakiet IP z startowym TTL = j, zostaje odrzucony przez j-ty router na trasie do celu. Router ten odeśle komunikat ICMP time exceeded zawierający nagłówek IP, więc przesówamy się
	if (icmp_header->icmp_type == ICMP_TIME_EXCEEDED) {
		struct ip *ip_header2 = (struct ip *)(icmp_packet + 8);
		u_int8_t buffer2[IP_MAXPACKET];
		memcpy(buffer2, ip_header2, IP_MAXPACKET);
		u_int8_t* icmp_packet2 = buffer2 + 4 * ip_header2->ip_hl;
		struct icmp *icmp_header2 = (struct icmp *)icmp_packet2;
		if (icmp_header2->icmp_seq == ttl && icmp_header2->icmp_id == pid) {
			inet_ntop(AF_INET, &(ip_header->ip_src), *ip, sizeof(*ip));
			(*packets)++;
		}
	}
	else if (icmp_header->icmp_type == ICMP_ECHOREPLY) { // komputer docelowy odpowie komunikatem ICMP echo reply
		if (icmp_header->icmp_seq == ttl && icmp_header->icmp_id == pid) {		
			inet_ntop(AF_INET, &(ip_header->ip_src), *ip, sizeof(*ip));
			*final_ip = TRUE;
			(*packets)++;
		}
	}
}
