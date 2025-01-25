
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_ProxyBase_HXX_
#define INCLUDE_vnx_ProxyBase_HXX_

#include <vnx/package.hxx>
#include <vnx/BaseProxy.h>


namespace vnx {

class VNX_EXPORT ProxyBase : public ::vnx::BaseProxy {
public:
	
	
	typedef ::vnx::BaseProxy Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	static constexpr uint64_t VNX_TYPE_ID = 0x7179aa6109e5d72dull;
	
	ProxyBase(const std::string& _vnx_name);
	
	vnx::Hash64 get_type_hash() const override;
	std::string get_type_name() const override;
	const vnx::TypeCode* get_type_code() const override;
	
	void read(std::istream& _in) override;
	void write(std::ostream& _out) const override;
	
	template<typename T>
	void accept_generic(T& _visitor) const;
	void accept(vnx::Visitor& _visitor) const override;
	
	vnx::Object to_object() const override;
	void from_object(const vnx::Object& object) override;
	
	vnx::Variant get_field(const std::string& name) const override;
	void set_field(const std::string& name, const vnx::Variant& value) override;
	
	friend std::ostream& operator<<(std::ostream& _out, const ProxyBase& _value);
	friend std::istream& operator>>(std::istream& _in, ProxyBase& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
protected:
	using Super::handle;
	
	
	void vnx_handle_switch(std::shared_ptr<const vnx::Value> _value) override;
	std::shared_ptr<vnx::Value> vnx_call_switch(std::shared_ptr<const vnx::Value> _method, const vnx::request_id_t& _request_id) override;
	
};

template<typename T>
void ProxyBase::accept_generic(T& _visitor) const {
	_visitor.template type_begin<ProxyBase>(25);
	_visitor.type_field("address", 0); _visitor.accept(address);
	_visitor.type_field("auto_login", 1); _visitor.accept(auto_login);
	_visitor.type_field("import_list", 2); _visitor.accept(import_list);
	_visitor.type_field("export_list", 3); _visitor.accept(export_list);
	_visitor.type_field("forward_list", 4); _visitor.accept(forward_list);
	_visitor.type_field("tunnel_list", 5); _visitor.accept(tunnel_list);
	_visitor.type_field("import_map", 6); _visitor.accept(import_map);
	_visitor.type_field("export_map", 7); _visitor.accept(export_map);
	_visitor.type_field("receive_tunnel", 8); _visitor.accept(receive_tunnel);
	_visitor.type_field("request_tunnel", 9); _visitor.accept(request_tunnel);
	_visitor.type_field("no_retry", 10); _visitor.accept(no_retry);
	_visitor.type_field("auto_import", 11); _visitor.accept(auto_import);
	_visitor.type_field("time_sync", 12); _visitor.accept(time_sync);
	_visitor.type_field("allow_login", 13); _visitor.accept(allow_login);
	_visitor.type_field("use_authentication", 14); _visitor.accept(use_authentication);
	_visitor.type_field("block_until_connect", 15); _visitor.accept(block_until_connect);
	_visitor.type_field("block_until_reconnect", 16); _visitor.accept(block_until_reconnect);
	_visitor.type_field("max_queue_ms", 17); _visitor.accept(max_queue_ms);
	_visitor.type_field("max_queue_size", 18); _visitor.accept(max_queue_size);
	_visitor.type_field("max_hop_count", 19); _visitor.accept(max_hop_count);
	_visitor.type_field("recv_buffer_size", 20); _visitor.accept(recv_buffer_size);
	_visitor.type_field("send_buffer_size", 21); _visitor.accept(send_buffer_size);
	_visitor.type_field("heartbeat_ms", 22); _visitor.accept(heartbeat_ms);
	_visitor.type_field("heartbeat_timeout", 23); _visitor.accept(heartbeat_timeout);
	_visitor.type_field("default_access", 24); _visitor.accept(default_access);
	_visitor.template type_end<ProxyBase>(25);
}


} // namespace vnx


namespace vnx {

} // vnx

#endif // INCLUDE_vnx_ProxyBase_HXX_
