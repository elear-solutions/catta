/***
  This file is part of catta.

  catta is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  catta is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
  Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with catta; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA.
***/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>

#include "addr-util.h"

CattaAddress *catta_address_from_sockaddr(const struct sockaddr* sa, CattaAddress *ret_addr) {
    assert(sa);
    assert(ret_addr);

    assert(sa->sa_family == AF_INET || sa->sa_family == AF_INET6);

    ret_addr->proto = catta_af_to_proto(sa->sa_family);
    /* <ES_mod> */
    if(sa->sa_family == AF_INET) {
      struct sockaddr_in sa_in;
      memcpy(&sa_in, sa, sizeof(struct sockaddr));
      memcpy(&ret_addr->data.ipv4, &(sa_in.sin_addr), sizeof(ret_addr->data.ipv4));
    } else {
      struct sockaddr_in6 sa_in6;
      memcpy(&sa_in6, sa, sizeof(struct sockaddr));
      memcpy(&ret_addr->data.ipv6, &(sa_in6.sin6_addr), sizeof(ret_addr->data.ipv6));
    }
    /* </ES_mod> */
    return ret_addr;
}

uint16_t catta_port_from_sockaddr(const struct sockaddr* sa) {
    assert(sa);

    assert(sa->sa_family == AF_INET || sa->sa_family == AF_INET6);
    /* <ES_mod> */
    if(sa->sa_family == AF_INET) {
      struct sockaddr_in sa_in;
      memcpy(&sa_in, sa, sizeof(struct sockaddr));
      return ntohs(sa_in.sin_port);
    } else {
      struct sockaddr_in6 sa_in6;
      memcpy(&sa_in6, sa, sizeof(struct sockaddr));
      return ntohs(sa_in6.sin6_port);
    }
    /* </ES_mod> */
}

int catta_address_is_ipv4_in_ipv6(const CattaAddress *a) {

    static const uint8_t ipv4_in_ipv6[] = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF
    };

    assert(a);

    if (a->proto != CATTA_PROTO_INET6)
        return 0;

    return memcmp(a->data.ipv6.address, ipv4_in_ipv6, sizeof(ipv4_in_ipv6)) == 0;
}

#define IPV4LL_NETWORK 0xA9FE0000L
#define IPV4LL_NETMASK 0xFFFF0000L
#define IPV6LL_NETWORK 0xFE80
#define IPV6LL_NETMASK 0xFFC0

int catta_address_is_link_local(const CattaAddress *a) {
    assert(a);

    if (a->proto == CATTA_PROTO_INET) {
        uint32_t n = ntohl(a->data.ipv4.address);
        return (n & IPV4LL_NETMASK) == IPV4LL_NETWORK;
    }
    else if (a->proto == CATTA_PROTO_INET6) {
        unsigned n = (a->data.ipv6.address[0] << 8) | (a->data.ipv6.address[1] << 0);
        return (n & IPV6LL_NETMASK) == IPV6LL_NETWORK;
    }

    return 0;
}
