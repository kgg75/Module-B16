#include <iostream>
#include "User.h"


User::User(
	const int _id,
	const std::string& _name,
	const std::string& _login,
	const SHA1PwdArray& _pass_sha1_hash,
	const bool _locked,
	const bool _logged)
	:
	id(_id),
	name(_name),
	login(_login),
	pass_sha1_hash(_pass_sha1_hash),
	locked(_locked),
	logged(_logged)
{}

