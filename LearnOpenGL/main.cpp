#include "gameplay.hpp"

int main() noexcept
{
	flat::Gameplay gp;
	gp.initialize();
	gp.mainLoop();
}