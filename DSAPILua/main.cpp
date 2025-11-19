#include <print>

#include <sol/sol.hpp>

#include "types.h"

#include "driver.h"

static auto exec(sol::state_view sv, std::string_view code) -> bool {
	auto res = sv.safe_script(code, sol::script_pass_on_error);
	if (!res.valid()) {
		sol::error err = res;
		std::println("[Script fail] {}", err.what());
		return false;
	}

	// Try to print res if there is any
	sol::object return_value = res;
	if (return_value.get_type() == sol::type::number) {
		uintptr_t number = return_value.as<uintptr_t>();
		std::println("=> 0x{:X}", number);
		return true;
	}
	if (return_value.get_type() == sol::type::string) {
		std::string str = return_value.as<std::string>();
		std::println("=> {}", str);
		return true;
	}
	if (return_value.get_type() == sol::type::boolean) {
		bool boolean = return_value.as<bool>();
		std::println("=> {}", boolean);
		return true;
	}
	if (return_value.get_type() == sol::type::userdata) {
		// Check if it's an Address
		if (return_value.is<Address>()) {
			Address addr = return_value.as<Address>();
			std::println("=> 0x{:X}", addr.get());
			return true;
		}

		std::println("=> userdata");
		return true;
	}

	std::println("Type: {}", static_cast<int>(return_value.get_type()));

	return true;
}

auto main() -> int {
	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::string, sol::lib::math);

	lua.new_usertype<TArray>("TArray",
		sol::constructors<TArray(), TArray(void*, int32_t, int32_t)>(),
		"data", &TArray::data,
		"size", &TArray::size,
		"capacity", &TArray::capacity,
		"asSpan", &TArray::asSpan);

	lua.new_usertype<FVector>("FVector",
		sol::constructors<FVector(), FVector(float, float, float)>(),
		"x", &FVector::x,
		"y", &FVector::y,
		"z", &FVector::z);

	lua.new_usertype<FRotator>("FRotator",
		sol::constructors<FRotator(), FRotator(float, float, float), FRotator(FVector)>(),
		"pitch", &FRotator::pitch,
		"yaw", &FRotator::yaw,
		"roll", &FRotator::roll,
		"getForwardVector", &FRotator::getForwardVector);

	lua.new_usertype<DriverManager>("DriverManager",
		sol::no_constructor,
		"isValid", &DriverManager::isValid,
		"getTargetPID", &DriverManager::getTargetPID,
		"read", &DriverManager::read<FVector>,
		"write", &DriverManager::write<FVector>,
		"attach", &DriverManager::attachToProcess,
		"getBase", &DriverManager::getBaseAddress);

	// Address type
	lua.new_usertype<Address>("Address",
		sol::constructors<Address(), Address(uintptr_t)>(),
		"get", &Address::get,
		sol::meta_function::addition, &Address::operator+,
		sol::meta_function::subtraction, &Address::operator-,
		sol::meta_function::multiplication, &Address::operator*,
		sol::meta_function::division, &Address::operator/);

	lua["mem"] = &g_Driver;

	while (true) {
		std::string line;
		std::print("LUA > ");
		std::getline(std::cin, line);
		if (line == "exit" || line == "quit") {
			break;
		}
		line = "return " + line;
		exec(lua, line);
	}
}