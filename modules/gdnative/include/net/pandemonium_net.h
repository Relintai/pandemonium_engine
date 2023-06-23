/**************************************************************************/
/*  pandemonium_net.h                                                           */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PANDEMONIUM ENGINE                               */
/*                        https://pandemoniumengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef PANDEMONIUM_NET_H
#define PANDEMONIUM_NET_H

#include <gdn/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

// For future versions of the API we should only add new functions at the end of the structure and use the
// version info to detect whether a call is available

// Use these to populate version in your plugin
#define PANDEMONIUM_NET_API_MAJOR 1
#define PANDEMONIUM_NET_API_MINOR 0

typedef struct {
	pandemonium_gdnative_api_version version; /* version of our API */
	pandemonium_object *data; /* User reference */

	/* This is StreamPeer */
	pandemonium_error (*get_data)(void *user, uint8_t *p_buffer, int p_bytes);
	pandemonium_error (*get_partial_data)(void *user, uint8_t *p_buffer, int p_bytes, int *r_received);
	pandemonium_error (*put_data)(void *user, const uint8_t *p_data, int p_bytes);
	pandemonium_error (*put_partial_data)(void *user, const uint8_t *p_data, int p_bytes, int *r_sent);

	int (*get_available_bytes)(const void *user);

	void *next; /* For extension? */
} pandemonium_net_stream_peer;

/* Binds a StreamPeerGDNative to the provided interface */
void pandemonium_net_bind_stream_peer(pandemonium_object *p_obj, const pandemonium_net_stream_peer *p_interface);

typedef struct {
	pandemonium_gdnative_api_version version; /* version of our API */

	pandemonium_object *data; /* User reference */

	/* This is PacketPeer */
	pandemonium_error (*get_packet)(void *, const uint8_t **, int *);
	pandemonium_error (*put_packet)(void *, const uint8_t *, int);
	pandemonium_int (*get_available_packet_count)(const void *);
	pandemonium_int (*get_max_packet_size)(const void *);

	void *next; /* For extension? */
} pandemonium_net_packet_peer;

/* Binds a PacketPeerGDNative to the provided interface */
void GDAPI pandemonium_net_bind_packet_peer(pandemonium_object *p_obj, const pandemonium_net_packet_peer *);

typedef struct {
	pandemonium_gdnative_api_version version; /* version of our API */

	pandemonium_object *data; /* User reference */

	/* This is PacketPeer */
	pandemonium_error (*get_packet)(void *, const uint8_t **, int *);
	pandemonium_error (*put_packet)(void *, const uint8_t *, int);
	pandemonium_int (*get_available_packet_count)(const void *);
	pandemonium_int (*get_max_packet_size)(const void *);

	/* This is NetworkedMultiplayerPeer */
	void (*set_transfer_mode)(void *, pandemonium_int);
	pandemonium_int (*get_transfer_mode)(const void *);
	// 0 = broadcast, 1 = server, <0 = all but abs(value)
	void (*set_target_peer)(void *, pandemonium_int);
	pandemonium_int (*get_packet_peer)(const void *);
	pandemonium_bool (*is_server)(const void *);
	void (*poll)(void *);
	// Must be > 0, 1 is for server
	int32_t (*get_unique_id)(const void *);
	void (*set_refuse_new_connections)(void *, pandemonium_bool);
	pandemonium_bool (*is_refusing_new_connections)(const void *);
	pandemonium_int (*get_connection_status)(const void *);

	void *next; /* For extension? Or maybe not... */
} pandemonium_net_multiplayer_peer;

/* Binds a MultiplayerPeerGDNative to the provided interface */
void GDAPI pandemonium_net_bind_multiplayer_peer(pandemonium_object *p_obj, const pandemonium_net_multiplayer_peer *);

#ifdef __cplusplus
}
#endif

#endif // PANDEMONIUM_NET_H
