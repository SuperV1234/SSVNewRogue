#include <sstream>
#include <string>
#include "Core/NRDependencies.h"
#include "Core/NRAssets.h"
#include "Core/NRGame.h"
#include "Utils/NRUtils.h"

using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvuj;
using namespace sses;
using namespace ssvs;
using namespace ssvsc;
using namespace ssvsc::Utils;

namespace nr
{
	NRGame::NRGame(GameWindow& mGameWindow, NRAssets& mAssets) : gameWindow(mGameWindow), assets(mAssets), world(createResolver<Retro>(), createSpatial<Grid>(600, 600, 1600, 300)),
		factory{assets, *this, manager, world}
	{
		gameState.onUpdate += [&](float mFrameTime){ update(mFrameTime); };
		gameState.onPostUpdate += [&]{ inputX = inputY = inputShoot = inputJump = inputWalk = 0; };
		gameState.onDraw += [&]{ draw(); };

		initInput();
		initLevel();
	}
	
	void NRGame::initInput()
	{
		using k = Keyboard::Key;
		using b = Mouse::Button;
		using t = Input::Trigger::Types;
		
		gameState.addInput({{k::Escape}}, [&](float){ gameWindow.stop(); });

		gameState.addInput({{k::A}},	[=](float mFrameTime){ camera.move(Vector2f{-10, 0} * mFrameTime); });
		gameState.addInput({{k::D}},	[=](float mFrameTime){ camera.move(Vector2f{10, 0} * mFrameTime); });
		gameState.addInput({{k::W}},	[=](float mFrameTime){ camera.move(Vector2f{0, -10} * mFrameTime); });
		gameState.addInput({{k::S}},	[=](float mFrameTime){ camera.move(Vector2f{0, 10} * mFrameTime); });
		gameState.addInput({{k::Q}},	[=](float mFrameTime){ camera.zoom(pow(1.1f, mFrameTime)); });
		gameState.addInput({{k::E}},	[=](float mFrameTime){ camera.zoom(pow(0.9f, mFrameTime)); });

		gameState.addInput({{k::Left}},		[&](float){ inputX = -1; });
		gameState.addInput({{k::Right}},	[&](float){ inputX = 1; });
		gameState.addInput({{k::Up}},		[&](float){ inputY = -1; });
		gameState.addInput({{k::Down}},		[&](float){ inputY = 1; });
		gameState.addInput({{k::Z}, {b::Left}},		[&](float){ inputShoot = 1; }, t::SINGLE);
		gameState.addInput({{k::X}},		[&](float){ inputJump = 1; });
		gameState.addInput({{k::LShift}},	[&](float){ inputWalk = 1; });
		
		// BUG: fix input problems (try crouching and moving, it uncrouches!)

		gameState.addInput({{k::Num1}}, [&](float)
		{
			auto mousePosition = camera.getMousePosition() * 100.f;
			factory.createWall(Vector2i(mousePosition.x, mousePosition.y));
		}, t::SINGLE);
		
		gameState.addInput({{k::Num2}}, [&](float)
		{
			auto mousePosition = camera.getMousePosition() * 100.f;
			factory.createWanderer(Vector2i(mousePosition.x, mousePosition.y));
		}, t::SINGLE);
		
		gameState.addInput({{k::Num3}}, [&](float)
		{
			auto mousePosition = camera.getMousePosition() * 100.f;
			factory.createPlayer(Vector2i(mousePosition.x, mousePosition.y));
		}, t::SINGLE);
		
		gameState.addInput({{k::Num4}}, [&](float)
		{
			auto mousePosition = camera.getMousePosition() * 100.f;
			int xx = (static_cast<Grid&>(world.getSpatial()).getIndex(mousePosition.x));
			int yy =(static_cast<Grid&>(world.getSpatial()).getIndex(mousePosition.y));
			string ff = toStr(static_cast<Grid&>(world.getSpatial()).getCell((int)xx, (int)yy).getBodies().size());
			log(toStr(xx )+ " " + toStr(yy) + "  :: " + toStr(ff));
		}, t::SINGLE);
	}
	void NRGame::initLevel()
	{		
		int tilesX{320 / 16}, tilesY{240 / 16};
		//tilesX = static_cast<Grid&>(world.getSpatial()).getXMaxIndex() / 2;
		//tilesY = static_cast<Grid&>(world.getSpatial()).getYMaxIndex() /2;
		
		for(int iY{0}; iY < tilesY; iY++)
			for(int iX{0}; iX < tilesX; iX++)
				if(iX == 0 || iY == 0 || iX == tilesX - 1 || iY == tilesY - 1)
					factory.createWall({1600 * iX + 800, 1600 * iY + 800});
				else
				{
					if(getRnd(0, 100) > 5) { }
					else
					{
						if(getRnd(0, 100) > 50) factory.createWall({1600 * iX + 800, 1600 * iY + 800});
						//else factory.createWanderer({1600 * iX + 800, 1600 * iY + 800});
					}
				}
				
		factory.createWall({1600 * 7 + 800, 1600 * 7 + 800});
		factory.createWall({1600 * 8 + 800, 1600 * 7 + 800});
		factory.createWall({1600 * 2 + 800, 1600 * 10 + 800});
		factory.createWall({1600 * 3 + 800, 1600 * 10 + 800});
		
		factory.createWall({1600 * 5, 1600 * 12});
		factory.createWall({1600 * 7, 1600 * 13 + 800 - 1300 + 1});
		factory.createWall({1600 * 7, 1600 * 13 + 800 - 1300 - 1300 - 1 - 1600});
		factory.createWall({1600 * 9, 1600 * 12 + 1200});
		
		factory.createPlayer({1600 * 4, 1600 * 5}); 
		//factory.createWanderer({1600 * 2, 1600 * 5});
		//factory.createWanderer({1600 * 3, 1600 * 5});
	}

	void NRGame::update(float mFrameTime)
	{
		gameWindow.setTitle(toStr(gameWindow.getFPS()));
		lastFT = mFrameTime;
		
		for(auto& cPhysics : manager.getComponents<NRCPhysics>("physics")) if(cPhysics->isAffectedByGravity() && cPhysics->getBody().getVelocity().y < 1000) cPhysics->getBody().applyForce({0, 25});
		world.update(mFrameTime);
		manager.update(mFrameTime);		
	}
	void NRGame::draw()
	{
		camera.apply();
		manager.draw();
		camera.unapply();
		
		ostringstream s;
		auto& entities(manager.getEntities());
		auto& bodies(world.getBodies());
		int componentCount{0}, dynamicBodiesCount{0}; 
		for(auto& entity : entities) componentCount += entity->getComponents().size();
		for(auto& body : bodies) if(!body->isStatic()) ++dynamicBodiesCount;
		
		s << "FPS: "				<< toStr(gameWindow.getFPS()) << endl;
		s << "FrameTime: "			<< toStr(lastFT) << endl;
		s << "Bodies(all): "		<< toStr(bodies.size()) << endl;
		s << "Bodies(static): "		<< toStr(bodies.size() - dynamicBodiesCount) << endl;
		s << "Bodies(dynamic): "	<< toStr(dynamicBodiesCount) << endl;	
		s << "Entities: "			<< toStr(entities.size()) << endl;
		s << "Components: "			<< toStr(componentCount) << endl;
		
		Text debugText(s.str(), assets.getAssetManager().getFont("bitxmap.ttf"));
		debugText.setCharacterSize(200);
		debugText.scale(0.033f, 0.033f);
		
		vector<Vector2f> offsets{{-1.f, -1.f}, {-1.f, 1.f}, {1.f, -1.f}, {1.f, 1.f}};
		for(auto& offset : offsets)
		{
			debugText.setColor(Color::Black);
			debugText.setPosition({offset.x * 0.3f, offset.y * 0.3f});
			render(debugText);
			debugText.setPosition({offset.x * 0.4f, offset.y * 0.4f});
			render(debugText);
			debugText.setPosition({offset.x * 0.5f, offset.y * 0.5f});
			render(debugText);
		}
		
		debugText.setColor(Color::White);
		debugText.setPosition({0, 0});
		render(debugText);
	}

	void NRGame::render(const Drawable& mDrawable) { gameWindow.draw(mDrawable); }

	// Getters
	GameWindow& NRGame::getGameWindow()	{ return gameWindow; }
	GameState& NRGame::getGameState()	{ return gameState; }
	Manager& NRGame::getManager()		{ return manager; }
	World& NRGame::getWorld()			{ return world; }
	NRFactory& NRGame::getFactory()		{ return factory; }
	Vector2i NRGame::getMousePosition() { return toCoords(camera.getMousePosition()); }
	int NRGame::getInputX() 			{ return inputX; }
	int NRGame::getInputY() 			{ return inputY; }
	int NRGame::getInputShoot() 		{ return inputShoot; }
	int NRGame::getInputJump() 			{ return inputJump; }
	int NRGame::getInputWalk() 			{ return inputWalk; }
}
