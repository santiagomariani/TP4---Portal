#include <iostream>
#include "Box2D/Box2D.h"

#include "disparo.h"
#include "BloqueRoca.h"
#include "BloqueMetal.h"
#include "chell.h"
#include "mundo.h"
#include "estado_teclado.h"
#include "personajes.h"
#include "cuerpo.h"
#include "SdlWindow.h"
#include "ViewChell.h"
#include "contact_listener.h"
#include "Timer.h"
#include "Camera.h"
#include "CoordConverter.h"
#include "portal.h"
#include "boton.h"
#include "estado_logico.h"
#include "compuerta.h"
#include "ids.h"
#include "or.h"
#include "and.h"
#include "Roca.h"

#include <thread>
#include <pthread.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>

#define CONVERSION 100
#define PI 3.14159265

int main() {

	b2Vec2 gravity(0.0f, -9.8f);
	Mundo world(gravity);
	ContactListener listener;
	world.setContactListener(listener);

	Personajes personajes(world);

	b2Vec2 pos_piedra(-6,-2);
	BloqueRoca rocaPiedra(ID_BLOQUE_ROCA, world, pos_piedra);

	b2Vec2 pos_roca(-8, 0);
	Roca roca(world, pos_roca);

	b2Vec2 pos_roca2(-6, 0);
	Roca roca2(world, pos_roca2);

	std::vector<BloqueMetal> bloques;
	b2Vec2 pos(-40, -3);
	b2Vec2 inc(1, 0);
	for (int j = 0; j < 45; ++j){
		BloqueMetal bloque(ID_BLOQUE_METAL, world, pos);
		bloques.push_back(std::move(bloque));
		pos += inc;
	}

	std::vector<BloqueMetal> pared;
	b2Vec2 pos_bloque(2, -3);
	b2Vec2 inc_pared(0, 1);
	for (int j = 0; j < 2; ++j){
		BloqueMetal roca3(ID_BLOQUE_METAL, world, pos_bloque);
		pared.push_back(std::move(roca3));
		pos_bloque += inc_pared;
	}
	b2Vec2 pos_boton(0, -2.35);
	Boton b(pos_boton, world);
	b2Vec2 pos_boton2(-4, -2.35);
	Boton b2(pos_boton2, world);

	EstadoLogico* estado_boton = new EstadoLogico(b); //usar unique pointer?
	EstadoLogico* estado_boton2 = new EstadoLogico(b2);

	// con botones OR:
	/*Or compuerta_or;
	compuerta_or.agregar(estado_boton);
	compuerta_or.agregar(estado_boton2);

	b2Vec2 pos_compuerta(-1, -2);
	Compuerta comp(pos_compuerta, world, compuerta_or);*/

	// con botones AND:
	And compuerta_and;
	compuerta_and.agregar(estado_boton);
	compuerta_and.agregar(estado_boton2);

	b2Vec2 pos_compuerta(-1, -2);
	Compuerta comp(pos_compuerta, world, compuerta_and);

	int id = personajes.agregar_chell();


//======================================SDL======================================

	const int screenWidth = 800;
	const int screenHeight = 600;

	const int FPS = 60;
	const int TICKS_PER_FRAME = 1000/FPS;


	SdlWindow window(screenWidth, screenHeight);

	std::string bgPath = "assets/industrial-background.jpg";
	SdlTexture background(bgPath, window);

	Camera camera(screenWidth, screenHeight, background);

	// window.fill(0x33, 0x33, 0x33, 0xFF);

	// Disparo
	std::string fxPath = "assets/fx.png";
	SdlTexture fxTexture(fxPath, window);
	Sprite disparoSprite(111, 59, 1, 1920, 3, fxTexture);

	// Bloques roca y metal
	std::string blocksPath = "assets/blocks.png";
	SdlTexture blocksTexture(blocksPath, window);
	Sprite bloqueSprite(193, 193, 1, 172, 1, blocksTexture);
	Sprite bloqueMetalSprite(193, 193, 1, 600, 1, blocksTexture);

	// Boton apagado y prendidos
	std::string botonPath = "assets/miscellaneous.png";
	SdlTexture botonTexture(botonPath, window);
	Sprite botonSprite(175, 55, 1, 116, 1, botonTexture);
	Sprite botonPrendidoSprite(175, 74, 1, 192, 1, botonTexture);

	// Compuerta cerrada y abierta
	std::string compuertaPath = "assets/gate.png";
	SdlTexture compuertaTexture(compuertaPath, window);
	Sprite compuertaCerradaSprite(193, 385, 1, 21, 1, compuertaTexture);
	Sprite compuertaAbiertaSprite(193, 385, 1553, 2437, 1, compuertaTexture);
	Sprite compuertaAbriendoSprite(193, 385, 1, 2051, 19, compuertaTexture);

	//Portal azul
	std::string portalAzulPath = "assets/portAzul.png";
	SdlTexture portalAzulTexture(portalAzulPath, window);
	Sprite portalAzulSprite(193, 193, 0, 0, 1, portalAzulTexture);

	//Portal naranja
	std::string portalNaranjaPath = "assets/portNaranja.png";
	SdlTexture portalNaranjaTexture(portalNaranjaPath, window);
	Sprite portalNaranjaSprite(193, 193, 0, 0, 1, portalNaranjaTexture);

	// Piedra
	Sprite piedra_sprite1(85, 83, 1, 4513, 1, fxTexture);
	Sprite piedra_sprite2(85, 83, 87, 4513, 1, fxTexture);
	Sprite piedra_sprite3(85, 83, 173, 4513, 1, fxTexture);
	Sprite piedra_sprite4(85, 83, 259, 4513, 1, fxTexture);
	Sprite piedra_sprite5(85, 83, 345, 4513, 1, fxTexture);
	Sprite piedra_sprite6(85, 83, 431, 4513, 1, fxTexture);
	Sprite piedra_sprite7(85, 83, 517, 4513, 1, fxTexture);
	Sprite piedra_sprite8(85, 83, 603, 4513, 1, fxTexture);

	ViewChell viewChell(window);

	std::map<int, Renderable*> texturas;
	texturas[ID_CHELL] = &viewChell;
	texturas[ID_BLOQUE_ROCA] = &bloqueSprite;
	texturas[ID_DISPARO] = &disparoSprite;
	texturas[ID_BLOQUE_METAL] = &bloqueMetalSprite;
	texturas[ID_PORTAL_AZUL] = &portalAzulSprite;
	texturas[ID_PORTAL_NARANJA] = &portalNaranjaSprite;
	texturas[ID_BOTON_APAGADO] = &botonSprite;
	texturas[ID_COMPUERTA_CERRADA] = &compuertaCerradaSprite;
	texturas[ID_COMPUERTA_ABIERTA] = &compuertaAbiertaSprite;
	texturas[ID_ROCA] = &piedra_sprite1;
	texturas[ID_BOTON_PRENDIDO] = &botonPrendidoSprite;

	CoordConverter coordConverter(screenWidth, screenHeight);
//======================================Loop======================================


	bool running = true;
	EstadoTeclado teclado;

	Timer fpsTimer;
	Timer capTimer;

	int countedFrames = 1;

	while (running) {
		fpsTimer.start();
		SDL_Event event;
		window.fill(0x33, 0x33, 0x33, 0xFF);
		Chell& chell = personajes.obtener_chell(id);
		SDL_Rect destChell = coordConverter.box2DToSDL(chell);
		camera.renderBg();
		camera.updateCamera(destChell);
		b2Body *cuerpos = world.obtenerBodies();
		while (cuerpos){
			Cuerpo *actual = (Cuerpo*)cuerpos->GetUserData();
			if (!actual){
				cuerpos = cuerpos->GetNext();
				continue;
			}
			SDL_Rect dest = coordConverter.box2DToSDL(*actual);
			int id = actual->getId();
			if (id == 2) {
				camera.render(*texturas[id], dest, (((Disparo*)actual)->getAngle()) * 180/PI * -1);
			}
			else if (id == ID_PORTAL_AZUL || id == ID_PORTAL_NARANJA){
				camera.render(*texturas[id], dest, (((Portal*)actual)->getAnguloSalida()) * 180/PI *-1);
			} else {
				camera.render(*texturas[id], dest);
			}
			cuerpos = cuerpos->GetNext();
		}

		while (SDL_PollEvent(&event) != 0){
			switch(event.type) {
				case SDL_KEYDOWN:{
						SDL_KeyboardEvent& keyEvent = (SDL_KeyboardEvent&) event;
						teclado.agregar_evento(keyEvent);
						break;
				}
				case SDL_KEYUP:{
						SDL_KeyboardEvent& keyEvent = (SDL_KeyboardEvent&) event;
						teclado.agregar_evento(keyEvent);
						break;
				}
				case SDL_MOUSEBUTTONDOWN:{
					SDL_MouseButtonEvent& mouseEvent = (SDL_MouseButtonEvent&) event;
					if ((mouseEvent.button) == SDL_BUTTON_LEFT){
						b2Vec2 click = coordConverter.sdlToBox2D(mouseEvent.x, mouseEvent.y, camera);
						chell.dispararAzul(click);
					}
					if ((mouseEvent.button) == SDL_BUTTON_RIGHT){
						b2Vec2 click = coordConverter.sdlToBox2D(mouseEvent.x, mouseEvent.y, camera);
						chell.dispararNaranja(click);
					}
					break;
				}
				case SDL_QUIT:
					running = false;
					break;
			}
		
		}
		chell.agarrarRoca(teclado);
		chell.mover(teclado);
		world.actualizar();
		window.render();
		

		// Solo para ver cantidad de FPS en terminal.
		//float avgFPS = (countedFrames / (fpsTimer.getTicks() / 1000.f));
		//if (avgFPS > 2000000) {
		//	avgFPS = 0;
		//}
		//std::cout << "FPS: " << avgFPS << std::endl;
		//++countedFrames;
		

		int frameTicks = capTimer.getTicks();

		if (frameTicks < TICKS_PER_FRAME) {
			SDL_Delay(TICKS_PER_FRAME - frameTicks);
		}
	}

	return 0;
}

