#include <vector>
#include <memory>
#include <stdint.h>
#include <iostream>

class Switch;
class Dimmer;

std::tuple<std::vector<Switch>, std::vector<Dimmer>> workingDevices{};

/*This code defines a C++20 concept CDevice that specifies a set of requirements 
for a type T to be considered a device, requiring that the type T has a member function called update()
that returns void.*/
template <typename T>
concept CDevice = requires(T device)
{
	{ device.update() } -> std::same_as<void>;
	{ T::findInEnv() } -> std::same_as<std::vector<T>>;
};

class Switch
{
	public:
		[[nodiscard]] static auto findInEnv() -> std::vector<Switch>
			{
                return std::get<std::vector<Switch>>(workingDevices);
			}

		auto update() -> void { std::cout << "Switch - using switch" << std::endl; }

	private:
		bool m_isOn{false};
};


class Dimmer
{
	public:
		[[nodiscard]] static auto findInEnv() -> std::vector<Dimmer>
			{
                return std::get<std::vector<Dimmer>>(workingDevices);
			}

		auto update() -> void { std::cout << "Dimmer - updating lightness" << std::endl; }

	private:
		uint_fast8_t brightness{0};
};

template <CDevice... TDevices>
class DeviceManager
{
	public:
		DeviceManager() : m_Devices{getDevices()} {}

		auto update() -> void 
		{
			std::apply([this](auto& ... deviceLists) { (updateDevice(deviceLists), ...); },
						m_Devices);
		}

	private:
		using device_list_t = std::tuple<std::vector<TDevices>...>;

		[[nodiscard]] static auto getDevices() -> device_list_t
		{
			return std::tuple{TDevices::findInEnv()...};
		}

		auto updateDevice(auto& deviceList) -> void {
			for(auto& device : deviceList)
			{
				device.update();
			}
		}

		device_list_t m_Devices{};
};

auto main() -> int
{
	std::get<std::vector<Switch>>(workingDevices).push_back(Switch{});
	std::get<std::vector<Switch>>(workingDevices).push_back(Switch{});
	std::get<std::vector<Dimmer>>(workingDevices).push_back(Dimmer{});


    DeviceManager<Switch, Dimmer> manager{};
    manager.update();
}