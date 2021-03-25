#pragma once
#include "hprocess.h"
#define CAMERA_MODEL_AI_THINKER
#include "esp_camera.h"
#include "slotable.h"
#include "camera_pins.h"
#include "SPIFFS.h"

class Camera: public Process {
	camera_fb_t* buffer=nullptr;
public:
	Signal<void, camera_fb_t*> pictureDone;
	Camera();
	uint8_t init() override;
	void work() override;
	void doPicture();
	void setSettings(sensor_t* s);
	camera_fb_t* getPicture();
	void releaseBuffer(camera_fb_t*);
};
#ifdef ARDUINO_IDE 
#include "camera.cpp" 
#endif
