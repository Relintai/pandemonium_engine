#ifndef MULTIPLAYER_PEER_GDNATIVE_H
#define MULTIPLAYER_PEER_GDNATIVE_H

/*************************************************************************/
/*  multiplayer_peer_gdnative.h                                          */
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

#include "core/io/networked_multiplayer_peer.h"

#include "gdn/gdnative.h"
#include "net/pandemonium_net.h"

class MultiplayerPeerGDNative : public NetworkedMultiplayerPeer {
	GDCLASS(MultiplayerPeerGDNative, NetworkedMultiplayerPeer);

protected:
	static void _bind_methods();
	const pandemonium_net_multiplayer_peer *interface;

public:
	MultiplayerPeerGDNative();
	~MultiplayerPeerGDNative();

	/* Sets the interface implementation from GDNative */
	void set_native_multiplayer_peer(const pandemonium_net_multiplayer_peer *p_impl);

	/* Specific to PacketPeer */
	virtual Error get_packet(const uint8_t **r_buffer, int &r_buffer_size);
	virtual Error put_packet(const uint8_t *p_buffer, int p_buffer_size);
	virtual int get_max_packet_size() const;
	virtual int get_available_packet_count() const;

	/* Specific to NetworkedMultiplayerPeer */
	virtual void set_transfer_mode(TransferMode p_mode);
	virtual TransferMode get_transfer_mode() const;
	virtual void set_target_peer(int p_peer_id);

	virtual int get_packet_peer() const;

	virtual bool is_server() const;

	virtual void poll();

	virtual int get_unique_id() const;

	virtual void set_refuse_new_connections(bool p_enable);
	virtual bool is_refusing_new_connections() const;

	virtual ConnectionStatus get_connection_status() const;
};

#endif // MULTIPLAYER_PEER_GDNATIVE_H
