#include <vector>
#include <memory>
#include <stdint.h>
#include <iostream>

class DeviceInterface;
using device_list_t = std::vector<std::shared_ptr<DeviceInterface>>;

device_list_t workingDevices{};

class DeviceInterface
{
	public:
		virtual auto update() -> void = 0;
        virtual ~DeviceInterface() {}
};

class Switch final : public DeviceInterface
{
	public:
		[[nodiscard]] static auto findInEnv() -> device_list_t
		{
			device_list_t switches;
			for (const auto& : workingDevices)
			{
				if(std::dynamic_pointer_cast<Switch>(device))
				{
					switches.push_back(device);
				}
			}
			return switches;
		}

		auto update() -> void override { std::cout << "Switch - using switch" << std::endl; }
        ~Switch() {}
	private:
		bool m_isOn{false};
};

class Dimmer final : public DeviceInterface
{
	public:
		[[nodiscard]] static auto findInEnv() -> device_list_t
		{
			device_list_t dimmers;
			for (const auto& device : workingDevices)
			{
				if(std::dynamic_pointer_cast<Dimmer>(device))
				{
					dimmers.push_back(device);
				}
			}
			return dimmers;
		}

		auto update() -> void override { std::cout << "Dimmer - updating lightness" << std::endl; }
        ~Dimmer() {}
	private:
		uint_fast8_t brightness{0};
};

class DeviceManager
{
	public:
		DeviceManager(device_list_t devices)
		: m_Devices{std::move(devices)} {}

		auto update() -> void 
		{ 
			for (auto& device : m_Devices)
			{
				device->update();
			}
		}

		[[nodiscard]] static auto getDevices() -> device_list_t 
		{
			device_list_t output{};
			{
				auto switch_list = Switch::findInEnv();
				output.insert(std::end(output),
					std::make_move_iterator(std::begin(switch_list)),
					std::make_move_iterator(std::end(switch_list)));
			}
			{
				auto dimmer_list = Dimmer::findInEnv();
				output.insert(std::end(output),
					std::make_move_iterator(std::begin(dimmer_list)),
					std::make_move_iterator(std::end(dimmer_list)));
			}
			return output;
		}
	private:
		device_list_t m_Devices{};
};

auto main() -> int
{
	workingDevices.emplace_back(std::make_shared<Switch>());
	workingDevices.emplace_back(new Switch{});
	workingDevices.emplace_back(std::make_shared<Dimmer>());
	workingDevices.emplace_back(std::make_shared<Dimmer>());
	workingDevices.emplace_back(std::make_shared<Switch>());
	workingDevices.emplace_back(new Switch{});

	DeviceManager manager(DeviceManager::getDevices());
	manager.update();
}