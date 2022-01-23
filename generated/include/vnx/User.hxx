
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_User_HXX_
#define INCLUDE_vnx_User_HXX_

#ifdef _MSC_VER
#include <vnx_base_export.h>
#else
#define VNX_BASE_EXPORT
#endif

#include <vnx/package.hxx>
#include <vnx/Value.h>


namespace vnx {

class VNX_BASE_EXPORT User : public ::vnx::Value {
public:
	
	std::string name;
	std::string hashed_password;
	std::vector<std::string> access_roles;
	std::set<std::string> permissions;
	
	typedef ::vnx::Value Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	static constexpr uint64_t VNX_TYPE_ID = 0x500d208d47b945a2ull;
	
	User() {}
	
	vnx::Hash64 get_type_hash() const override;
	std::string get_type_name() const override;
	const vnx::TypeCode* get_type_code() const override;
	
	static std::shared_ptr<User> create();
	std::shared_ptr<vnx::Value> clone() const override;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) override;
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const override;
	
	void read(std::istream& _in) override;
	void write(std::ostream& _out) const override;
	
	void accept(vnx::Visitor& _visitor) const override;
	
	vnx::Object to_object() const override;
	void from_object(const vnx::Object& object) override;
	
	vnx::Variant get_field(const std::string& name) const override;
	void set_field(const std::string& name, const vnx::Variant& value) override;
	
	friend std::ostream& operator<<(std::ostream& _out, const User& _value);
	friend std::istream& operator>>(std::istream& _in, User& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
};


} // namespace vnx


namespace vnx {

} // vnx

#endif // INCLUDE_vnx_User_HXX_
