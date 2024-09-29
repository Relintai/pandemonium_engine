/*************************************************************************/
/*  ip_unix.cpp                                                          */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "ip_unix.h"

#if defined(UNIX_ENABLED) || defined(WINDOWS_ENABLED) || defined(VITA_ENABLED)

#include <string.h>

#ifdef WINDOWS_ENABLED
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#ifndef UWP_ENABLED
#include <iphlpapi.h>
#endif
#else // UNIX
#include <netdb.h>
#ifdef ANDROID_ENABLED
// We could drop this file once we up our API level to 24,
// where the NDK's ifaddrs.h supports to needed getifaddrs.
#include "thirdparty/misc/ifaddrs-android.h"
#else
#ifdef __FreeBSD__
#include <sys/types.h>
#endif
#ifndef VITA_ENABLED
#include <ifaddrs.h>
#endif
#endif
#include <arpa/inet.h>
#include <sys/socket.h>
#ifdef __FreeBSD__
#include <netinet/in.h>
#endif
#ifndef VITA_ENABLED
#include <net/if.h> // Order is important on OpenBSD, leave as last
#endif
#endif

static IP_Address _sockaddr2ip(struct sockaddr *p_addr) {
	IP_Address ip;

	if (p_addr->sa_family == AF_INET) {
		struct sockaddr_in *addr = (struct sockaddr_in *)p_addr;
		ip.set_ipv4((uint8_t *)&(addr->sin_addr));
	} else if (p_addr->sa_family == AF_INET6) {
		struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)p_addr;
		ip.set_ipv6(addr6->sin6_addr.s6_addr);
	};

	return ip;
};

void IP_Unix::_resolve_hostname(List<IP_Address> &r_addresses, const String &p_hostname, Type p_type) const {
	struct addrinfo hints;
	struct addrinfo *result;

	memset(&hints, 0, sizeof(struct addrinfo));
	if (p_type == TYPE_IPV4) {
		hints.ai_family = AF_INET;
	} else if (p_type == TYPE_IPV6) {
		hints.ai_family = AF_INET6;
		hints.ai_flags = 0;
	} else {
		hints.ai_family = AF_UNSPEC;
		hints.ai_flags = AI_ADDRCONFIG;
	};
	hints.ai_flags &= ~AI_NUMERICHOST;

	int s = getaddrinfo(p_hostname.utf8().get_data(), nullptr, &hints, &result);
	if (s != 0) {
		print_verbose("getaddrinfo failed! Cannot resolve hostname.");
		return;
	};

	if (result == nullptr || result->ai_addr == nullptr) {
		print_verbose("Invalid response from getaddrinfo");
		if (result) {
			freeaddrinfo(result);
		}
		return;
	};

	struct addrinfo *next = result;

	do {
		if (next->ai_addr == NULL) {
			next = next->ai_next;
			continue;
		}
		IP_Address ip = _sockaddr2ip(next->ai_addr);
		if (ip.is_valid() && !r_addresses.find(ip)) {
			r_addresses.push_back(ip);
		}
		next = next->ai_next;
	} while (next);

	freeaddrinfo(result);
}

#if defined(WINDOWS_ENABLED)

#if defined(UWP_ENABLED)

void IP_Unix::get_local_interfaces(RBMap<String, Interface_Info> *r_interfaces) const {
	using namespace Windows::Networking;
	using namespace Windows::Networking::Connectivity;

	// Returns addresses, not interfaces.
	auto hostnames = NetworkInformation::GetHostNames();

	for (int i = 0; i < hostnames->Size; i++) {
		auto hostname = hostnames->GetAt(i);

		if (hostname->Type != HostNameType::Ipv4 && hostname->Type != HostNameType::Ipv6)
			continue;

		String name = hostname->RawName->Data();
		RBMap<String, Interface_Info>::Element *E = r_interfaces->find(name);
		if (!E) {
			Interface_Info info;
			info.name = name;
			info.name_friendly = hostname->DisplayName->Data();
			info.index = String::num_uint64(0);
			E = r_interfaces->insert(name, info);
			ERR_CONTINUE(!E);
		}

		Interface_Info &info = E->get();

		IP_Address ip = IP_Address(hostname->CanonicalName->Data());
		info.ip_addresses.push_front(ip);
	}
}

#else

void IP_Unix::get_local_interfaces(RBMap<String, Interface_Info> *r_interfaces) const {
	ULONG buf_size = 1024;
	IP_ADAPTER_ADDRESSES *addrs;

	while (true) {
		addrs = (IP_ADAPTER_ADDRESSES *)memalloc(buf_size);
		int err = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_SKIP_FRIENDLY_NAME,
				NULL, addrs, &buf_size);
		if (err == NO_ERROR) {
			break;
		};
		memfree(addrs);
		if (err == ERROR_BUFFER_OVERFLOW) {
			continue; // will go back and alloc the right size
		};

		ERR_FAIL_MSG("Call to GetAdaptersAddresses failed with error " + itos(err) + ".");
	};

	IP_ADAPTER_ADDRESSES *adapter = addrs;

	while (adapter != NULL) {
		Interface_Info info;
		info.name = adapter->AdapterName;
		info.name_friendly = adapter->FriendlyName;
		info.index = String::num_uint64(adapter->IfIndex);

		IP_ADAPTER_UNICAST_ADDRESS *address = adapter->FirstUnicastAddress;
		while (address != NULL) {
			int family = address->Address.lpSockaddr->sa_family;
			if (family != AF_INET && family != AF_INET6)
				continue;
			info.ip_addresses.push_front(_sockaddr2ip(address->Address.lpSockaddr));
			address = address->Next;
		}
		adapter = adapter->Next;
		// Only add interface if it has at least one IP
		if (info.ip_addresses.size() > 0)
			r_interfaces->insert(info.name, info);
	};

	memfree(addrs);
};

#endif

#else // UNIX
#if defined(VITA_ENABLED)
void IP_Unix::get_local_interfaces(RBMap<String, Interface_Info> *r_interfaces) const {
	// Ok there copilot
}
#else
void IP_Unix::get_local_interfaces(RBMap<String, Interface_Info> *r_interfaces) const {
	struct ifaddrs *ifAddrStruct = nullptr;
	struct ifaddrs *ifa = nullptr;
	int family;

	getifaddrs(&ifAddrStruct);

	for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr) {
			continue;
		}

		family = ifa->ifa_addr->sa_family;

		if (family != AF_INET && family != AF_INET6) {
			continue;
		}

		RBMap<String, Interface_Info>::Element *E = r_interfaces->find(ifa->ifa_name);
		if (!E) {
			Interface_Info info;
			info.name = ifa->ifa_name;
			info.name_friendly = ifa->ifa_name;
			info.index = String::num_uint64(if_nametoindex(ifa->ifa_name));
			E = r_interfaces->insert(ifa->ifa_name, info);
			ERR_CONTINUE(!E);
		}

		Interface_Info &info = E->get();
		info.ip_addresses.push_front(_sockaddr2ip(ifa->ifa_addr));
	}

	if (ifAddrStruct != nullptr) {
		freeifaddrs(ifAddrStruct);
	}
}
#endif
#endif

void IP_Unix::make_default() {
	_create = _create_unix;
}

IP *IP_Unix::_create_unix() {
	return memnew(IP_Unix);
}

IP_Unix::IP_Unix() {
}

#endif
