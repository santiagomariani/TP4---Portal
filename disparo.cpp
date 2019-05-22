#include <iostream>
#include "disparo.h"
#include "Box2D/Box2D.h"
#include <math.h>

#define RADIO_RAYO 0.25f
#define DENSIDAD_RAYO 1
#define CTE_VELOCIDAD_RAYO 0.25


Disparo::Disparo(){
	cuerpo = NULL;
	this->listo = false;
}

void Disparo::activar(Mundo& mundo, const b2Vec2& origen, const b2Vec2& destino){
	if (cuerpo){
		mundo.destruirBody(cuerpo);
	}
	this->listo = false;
	b2BodyDef circle_body_def;
	circle_body_def.type = b2_dynamicBody;
	circle_body_def.position.Set(origen.x, origen.y);
	circle_body_def.fixedRotation = true;
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
Disparo::Disparo(Disparo&& otro){
	if (this == &otro){
		return;
	}
	cuerpo = otro.cuerpo;
	cuerpo->SetUserData(this);
	otro.cuerpo = NULL;
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

Disparo& Disparo::operator=(Disparo& otro){ // Si es el operador asignacion
	if (this == &otro){						// por copia, tendria que poner el const
        return *this;						// y copiar los atributos (no robarlos).
    }
    cuerpo = otro.cuerpo;
    otro.cuerpo = nullptr;
    cuerpo->SetUserData(this);
    return *this;
}

Disparo& Disparo::operator=(Disparo&& otro){
	if (this == &otro){
        return *this;
    }
    cuerpo = otro.cuerpo;
    otro.cuerpo = NULL;
    cuerpo->SetUserData(this);
    return *this;
}
float Disparo::getDiameter(){
	return 2 * RADIO_RAYO;
}

void Disparo::terminar(){
	this->listo = true;
}

bool Disparo::terminado(){
	return this->listo;
}

void Disparo::remover(){
	if (cuerpo && this->terminado()){
		b2World* w = cuerpo->GetWorld();
		w->DestroyBody(cuerpo);
		cuerpo = NULL;
	}
}
