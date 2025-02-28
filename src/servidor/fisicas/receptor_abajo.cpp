#include <config.h>
#include "receptor_abajo.h"
#include "receptor.h"
#include "ids.h"

ReceptorAbajo::ReceptorAbajo(Mundo &mundo, const b2Vec2 &pos) :
        Receptor(mundo ,pos) {
    b2Vec2 pos_sensor(0, (float32)(- (maxHeight / 2) - TAMANIO_SENSOR_Y));
    b2PolygonShape polygon_shape;
    b2FixtureDef fixture_def;
    fixture_def.shape = &polygon_shape;
    fixture_def.userData = sensor.get();
    polygon_shape.SetAsBox(config.tam_receptor, TAMANIO_SENSOR_Y, pos_sensor, 0);
    cuerpo->CreateFixture(&fixture_def);
}

uint8_t ReceptorAbajo::getId() {
    if (activado) {
        return ID_RECEPTORABAJO_ACTIVADO;
    } else {
        return ID_RECEPTORABAJO_DESACTIVADO;
    }
}