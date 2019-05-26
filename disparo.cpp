#include <iostream>
#include "disparo.h"
#include "Box2D/Box2D.h"
#include <math.h>

#define RADIO_RAYO 0.15f
#define DENSIDAD_RAYO 1
#define CTE_VELOCIDAD_RAYO 15


Disparo::Disparo(int identidad, Mundo& mundo) : 
			id(identidad), mundo(mundo),
			Cuerpo(RADIO_RAYO * 2, RADIO_RAYO * 2) {
	cuerpo = nullptr;
}

void Disparo::activar(const b2Vec2& origen, const b2Vec2& destino){
	if (cuerpo){
		mundo.destruirBody(cuerpo);
		cuerpo = nullptr;
	}
	b2BodyDef circle_body_def;
	circle_body_def.type = b2_dynamicBody;
	circle_body_def.position.Set(origen.x, origen.y);
	circle_body_def.fixedRotation = true;
    circle_body_def.bullet = true;
	cuerpo = mundo.agregarBody(circle_body_def);
	//cuerpo = world.CreateBody(&circle_body_def);

	b2CircleShape circleShape;
	circleShape.m_p.Set(origen.x, origen.y);
	circleShape.m_radius = RADIO_RAYO;
	b2FixtureDef circle_fixture_def;
	circle_fixture_def.shape = &circleShape;
	circle_fixture_def.density = DENSIDAD_RAYO;
	circle_fixture_def.isSensor = true; // no choca con nada pero se pueden detectar
	    								// las colisiones
	cuerpo->CreateFixture(&circle_fixture_def);

	b2Vec2 vel = destino;
	vel -= origen;
	vel.Normalize();
	vel *= CTE_VELOCIDAD_RAYO;
	cuerpo->SetGravityScale(0);
	cuerpo->SetLinearVelocity(vel);
	cuerpo->SetUserData(this);
}

Disparo::Disparo(Disparo&& otro) : 
		mundo(otro.mundo),
		Cuerpo(RADIO_RAYO * 2, RADIO_RAYO * 2) {
	if (this == &otro){
		return;
	}
	maxWidth = otro.maxWidth;
	maxHeight = otro.maxHeight;
	cuerpo = otro.cuerpo;
	portal = otro.portal;
	id = otro.id;
	otro.maxWidth = 0;
	otro.maxHeight = 0;
	otro.cuerpo = nullptr;
	cuerpo->SetUserData(this);
}

void Disparo::setPortal(Portal* port){
	portal = port;
}

void Disparo::crearPortal(b2Vec2& pos, b2Vec2& normal){
    portal->establecer(pos, normal);
    this->mundo.agregarPortal(portal);
}

const b2Vec2& Disparo::getPosition(){
	return cuerpo->GetPosition();
}
double Disparo::getAngle(){
	b2Vec2 vel = cuerpo->GetLinearVelocity();
	return atan2(vel.y,vel.x);
}
int Disparo::getId(){
	return 2;
}

Disparo& Disparo::operator=(Disparo&& otro){
	if (this == &otro){
        return *this;
    }

	maxWidth = otro.maxWidth;
	maxHeight = otro.maxHeight;
	cuerpo = otro.cuerpo;
	mundo = std::move(otro.mundo);
	portal = otro.portal;
	id = otro.id;
	otro.maxWidth = 0;
	otro.maxHeight = 0;
	otro.cuerpo = nullptr;
    cuerpo->SetUserData(this);
    return *this;
}

void Disparo::desactivar(){
	if (cuerpo){
		b2World* w = cuerpo->GetWorld();
		w->DestroyBody(cuerpo);
		cuerpo = nullptr;
	}
}

void Disparo::terminar() {
    this->mundo.agregarCuerpoADestruir(this);
}
