#pragma once

#include <cmath>
#include <numbers>

#include <span>

class Address {
public:
	Address() = default;
	Address(uintptr_t value) : value(value) {}

	uintptr_t get() const {
		return value;
	}

	uintptr_t operator+(uintptr_t offset) const {
		return value + offset;
	}

	uintptr_t operator-(uintptr_t offset) const {
		return value - offset;
	}

	uintptr_t operator*(uintptr_t factor) const {
		return value * factor;
	}

	uintptr_t operator/(uintptr_t divisor) const {
		return value / divisor;
	}

private:
	uintptr_t value = 0;
	std::string type = ""; // Utilized by Lua (for reflection)

	// TODO: use RTTR or Entt Meta for faster reflection
};

class TArray {
public:
	TArray() = default;
	TArray(void* data, int32_t size, int32_t capacity) : data(data), size(size), capacity(capacity) {}

	std::span<void*, std::dynamic_extent> asSpan() {
		return std::span<void*>(static_cast<void**>(data), size);
	}

	void* data = nullptr;
	int32_t size = 0;
	int32_t capacity = 0;
};

class FVector {
public:
	FVector() = default;
	FVector(float x, float y, float z) : x(x), y(y), z(z) {}
	FVector(float val) : x(val), y(val), z(val) {}

	float x = 0;
	float y = 0;
	float z = 0;
};

class FRotator {
public:
	FRotator() = default;
	FRotator(float pitch, float yaw, float roll) : pitch(pitch), yaw(yaw), roll(roll) {}

	FRotator(FVector direction) {
		constexpr double radToDeg = 180.0f / std::numbers::pi;

		pitch = atan2(-direction.z, sqrt(direction.x * direction.x + direction.y * direction.y)) * radToDeg;
		yaw = atan2(direction.y, direction.x) * radToDeg;
		roll = 0;
	}

	FVector getForwardVector() const {
		constexpr double degToRad = std::numbers::pi / 180.0f;

		float cp = cosf(pitch * degToRad);
		float sp = sinf(pitch * degToRad);
		float cy = cosf(yaw * degToRad);
		float sy = sinf(yaw * degToRad);
		return {cp * cy, cp * sy, -sp};
	}

	float pitch = 0;
	float yaw = 0;
	float roll = 0;
};
