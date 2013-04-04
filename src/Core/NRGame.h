#ifndef SSVNR_CORE_GAME
#define SSVNR_CORE_GAME

#include "Core/NRDependencies.h"
#include "Core/NRFactory.h"
#include "Utils/NRAnimation.h"

namespace nr
{
	class NRAssets;

	class NRGame
	{
		private:
			ssvs::GameWindow& gameWindow;
			ssvs::Camera camera{gameWindow, {{320 / 2, 240 / 2}, {320, 240}}};
			NRAssets& assets;
			NRFactory factory;
			ssvs::GameState gameState;
			ssvsc::World world; // BUG: world must be destroyed after manager, find a way to make this not required!
			ssvsc::Grid& grid;
			sses::Manager manager;

			int inputX{0}, inputY{0}, inputShoot{0}, inputJump{0}, inputWalk{0};
			sf::Text debugText;
			std::vector<std::vector<int>> debugGrid;
			sf::VertexArray debugGridVertices{sf::PrimitiveType::Quads};

			void initInput();
			void initLevel();
			void initDebugGrid();

		public:
			NRGame(ssvs::GameWindow& mGameWindow, NRAssets& mAssets);

			void update(float mFrameTime);
			void updateDebugText(float mFrameTime);
			void drawDebugGrid();
			void draw();

			void render(const sf::Drawable& mDrawable);
			void setDebugGrid(int mX, int mY);
			void clearDebugGrid();

			// Getters
			ssvs::GameWindow& getGameWindow();
			ssvs::GameState& getGameState();
			sses::Manager& getManager();
			ssvsc::World& getWorld();
			NRFactory& getFactory();
			sf::Vector2i getMousePosition();
			int getInputX();
			int getInputY();
			int getInputShoot();
			int getInputJump();
			int getInputWalk();
			void drawDebugText();
	};
}

#endif
