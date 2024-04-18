// Patryk Maciąg 331542
#include "icmp_send.h"


void icmp_send(int sockfd, struct sockaddr_in recipient, int ttl, int pid) {
	for (int i = 0; i < 3; i++) {
		struct icmp header;
		header.icmp_type = ICMP_ECHO;
		header.icmp_code = 0;
		header.icmp_hun.ih_idseq.icd_id = pid; // unikatowy identyfikator, np. 16 mniej znaczących bitów z PID
		header.icmp_hun.ih_idseq.icd_seq = ttl; // numer sekwencyjny
		header.icmp_cksum = 0;
		header.icmp_cksum = compute_icmp_checksum ((u_int16_t*)&header, sizeof(header));


		// Pole TTL jest w nagłówku IP -> brak bezpośredniego dostępu. Zmiana wywołaniem:
		setsockopt (sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));

		// Wysyłanie pakietu przez gniazdo:
		ssize_t bytes_sent = sendto (sockfd, &header, sizeof(header), 0, (struct sockaddr*)&recipient, sizeof(recipient));
		if (bytes_sent < 0) { // 0 moze sie zdarzyc przyprobie wyslania 0 bajtow danych
			fprintf(stderr, "sendto error: %s\n", strerror(errno));
		}
	}
}
