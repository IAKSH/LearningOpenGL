#pragma once

#include <string_view>
#include <cinttypes>
#include <chrono>
#include <deque>

namespace flat
{
	class Animation
	{
	private:
		std::deque<uint32_t> textures;
		uint32_t intervalMS = 1000;
		std::chrono::steady_clock::time_point lastUpdate;
		uint32_t currentIndice = 0;
	public:
		Animation();
		Animation(std::initializer_list<std::string_view>&& pathes);
		~Animation();
		void setIntervalMS(uint32_t interval);
		void load(std::initializer_list<std::string_view>& pathes);
		void load(std::initializer_list<std::string_view>&& pathes);
		void checkUpdate();
		const uint32_t& getCurrentTexture();
	};
}