#include "gameplay.h"

int main() noexcept
{
	flat::Gameplay gp;
	gp.initialize();
	gp.mainLoop();
}