
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_test_ACCEPT_GENERIC_HXX_
#define INCLUDE_vnx_test_ACCEPT_GENERIC_HXX_

#include <vnx/test/TestValue.hxx>


namespace vnx {

template<typename V>
void accept_generic(V& visitor, std::shared_ptr<const ::vnx::test::TestValue> value) {
	if(value) {
		value->accept_generic(visitor);
	} else {
		visitor.accept(nullptr);
	}
}



} // namespace vnx

#endif // INCLUDE_vnx_test_ACCEPT_GENERIC_HXX_