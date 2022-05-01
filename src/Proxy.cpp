 /*************************************************************************
 * 
 *  [2017] - [2018] Automy Inc. 
 *  All Rights Reserved.
 * 
 * NOTICE:  All information contained herein is, and remains
 * the property of Automy Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Automy Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Automy Incorporated.
 */


#include <vnx/vnx.h>
#include <vnx/Proxy.h>
#include <vnx/PermissionDenied.hxx>


namespace vnx {


Proxy::Proxy(const std::string& name_, std::shared_ptr<const Endpoint> endpoint_, int socket_)
	:	ProxyBase(name_),
		out(&stream_out)
{
	endpoint = endpoint_;
	stream_out.reset(socket_);
	socket = socket_;
}


void Proxy::handle(std::shared_ptr<const Frame> frame) {
	if(!is_connected) {
		return;
	}
	std::shared_ptr<Frame> copy = vnx::clone(frame);
	if(vnx_pipe) {
		copy->stream = vnx_pipe->get_mac_addr();
	}
	// if we receive a frame it is 100% for the other side
	try {
		vnx::write(out, copy);	// send it to the other side
		out.flush();
	} catch(...) {
		// just ignore it
	}
	num_frames_send++;
}


bool Proxy::handle(std::shared_ptr<const Return> result) {
	if(!is_connected) {
		return false;
	}
	std::shared_ptr<Return> copy = vnx::clone(result);
	if(vnx_pipe) {
		copy->stream = vnx_pipe->get_mac_addr();
	}
	// if we receive a return it is 100% for the other side
	try {
		vnx::write(out, copy);	// send it to the other side
		out.flush();
		return true;
	} catch(...) {
		// just ignore it
	}
	return false;
}


void Proxy::handle(std::shared_ptr<const FlowMessage> flow_msg) {
	if(!is_connected) {
		return;
	}
	if(	flow_msg->dst_mac != service_addr
		&& flow_msg->dst_mac != remote_addr
		&& flow_msg->dst_mac != public_service_addr)
	{
		std::shared_ptr<FlowMessage> copy = vnx::clone(flow_msg);
		if(vnx_pipe) {
			copy->stream = vnx_pipe->get_mac_addr();
		}
		try {
			vnx::write(out, copy);				// send it to the other side
			out.flush();
		} catch(...) {
			// just ignore it
		}
		if(flow_msg->flow_code == FlowMessage::OPEN) {
			outgoing.emplace(flow_msg->src_mac, flow_msg->dst_mac);
		}
		if(flow_msg->flow_code == FlowMessage::CLOSE) {
			outgoing.erase(std::make_pair(flow_msg->src_mac, flow_msg->dst_mac));
		}
	}
}


void Proxy::send_outgoing(std::shared_ptr<Sample> sample) {
	// no try + catch here
	vnx::write(out, sample);
	out.flush();
}


void Proxy::send_outgoing(std::shared_ptr<Request> request) {
	// no try + catch here
	vnx::write(out, request);
	out.flush();
}


void Proxy::connect_reset() {
	// clear buffer and type map
	out.clear();
}


void Proxy::read_socket_until_error(std::shared_ptr<Pipe> service_pipe) {
	SocketInputStream stream_in(socket);
	TypeInput in(&stream_in);
	while(true) {
		std::shared_ptr<Value> value = vnx::read(in);
		if(!value) {
			break;		// should never happen
		}
		if(auto msg = std::dynamic_pointer_cast<Message>(value)) {
			msg->hop_count++;
		}
		const TypeCode* type_code = value->get_type_code();
		while(type_code) {
			switch(type_code->type_hash) {
				case Frame::VNX_TYPE_ID:
					process(std::static_pointer_cast<Frame>(value));
					break;
				case Sample::VNX_TYPE_ID:
					process(std::static_pointer_cast<Sample>(value));
					break;
				case Request::VNX_TYPE_ID:
					process(std::static_pointer_cast<Request>(value), service_pipe);
					break;
				case Return::VNX_TYPE_ID:
					process(std::static_pointer_cast<Return>(value));
					break;
				case FlowMessage::VNX_TYPE_ID:
					process(std::static_pointer_cast<FlowMessage>(value));
					break;
			}
			type_code = type_code->super;
		}
	}
}


} // vnx
