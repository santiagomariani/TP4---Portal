#include "portal.h"
#include "chell.h"
#include <math.h>
#include "Box2D/Box2D.h"
#define TAMANIO_PORTAL_X 0.5
#define TAMANIO_PORTAL_Y 0.05

#define PI 3.14159265


Portal::Portal(int identidad, Mundo& mundo) : id(identidad), mundo(mundo){
	hermano = nullptr;
	cuerpo = nullptr;
}

void Portal::activar(){
	if (cuerpo){
		mundo.destruirBody(cuerpo);
		cuerpo = nullptr;
	}
	b2BodyDef cuerpo_def;
	cuerpo_def.type = b2_staticBody;
	cuerpo_def.position.Set(pos.x, pos.y);
	cuerpo = mundo.agregarBody(cuerpo_def);

	b2PolygonShape polygonShape;
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &polygonShape;
	myFixtureDef.density = 1;

	polygonShape.SetAsBox(TAMANIO_PORTAL_X, TAMANIO_PORTAL_Y);
	cuerpo->CreateFixture(&myFixtureDef);

	orientacion = atan2(normal.y, normal.x);
	if (orientacion < 0){
		orientacion += 2 * PI;
	}
}

void Portal::conectar(Portal* otro){
	hermano = otro;
}

float Portal::getAnguloSalida(){
	return orientacion;
}

int Portal::getId(){
	return id;
}

const b2Vec2& Portal::getPosition(){
	return cuerpo->GetPosition();
}

float Portal::getAnguloEntrada(){
	if (orientacion - PI < 0){
		return orientacion + PI;
	}
	return orientacion - PI;
}

void Portal::expulsar(b2Body* otro, float orientacion_otro){
	float nuevo_ang = getAnguloSalida() - orientacion_otro;
	b2Rot rotador(nuevo_ang);
	b2Vec2 vel = otro->GetLinearVelocity();
	b2Vec2 nueva_vel = b2Mul(rotador, vel);
	otro->SetLinearVelocity(nueva_vel);
	otro->SetTransform(cuerpo->GetPosition(), 0.0f);
}

void Portal::teletransportar(b2Body* otro){
	hermano->expulsar(otro, getAnguloEntrada());
}

void Portal::establecer(b2Vec2 &pos, b2Vec2 &normal) {
    pos.Set(pos.x, pos.y);
    normal.Set(normal.x, normal.y);
}

Portal& Portal::operator=(Portal&& otro){
    mundo = std::move(otro.mundo);
    pos = otro.pos;
    normal = otro.normal;
    id = otro.id;
    hermano = otro.hermano;
    cuerpo = otro.cuerpo;
    otro.cuerpo = nullptr;
    orientacion = otro.orientacion;

}
