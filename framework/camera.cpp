#include "camera.h"

Camera::Camera() : Process("camera", GlobalWorkerHandler::Priority::Normal)
{

}

uint8_t Camera::init()
{
    /*if (!SPIFFS.begin()) {
        goutput.warning("!SPIFFSFAIL");
        delay(1000);
    }
    SPIFFS.format();*/
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 15000000;
    config.pixel_format = PIXFORMAT_JPEG;
    //config.frame_size = static_cast<framesize_t>(HSettings::get().imgSize.get()); //TEST
    //config.jpeg_quality = HSettings::get().imgQuality.get();
    config.frame_size = FRAMESIZE_SVGA; //TEST
    config.jpeg_quality = 5;
    
    config.fb_count = 1;

    // camera init
    esp_err_t err = esp_camera_init(&config);
    while (err != ESP_OK) {
        goutput.warning("!CAMERAFAIL");
        esp_err_t err = esp_camera_init(&config);
        delay(1000);
        return 1;
    }

    sensor_t* s = esp_camera_sensor_get();
    if (psramFound()) goutput.info("MEM: " + String(ESP.getPsramSize()));
    else goutput.warning("!PSRAMOFF");
    setSettings(s);
    
    return 0;
}

void Camera::setSettings(sensor_t* s) {

    s->set_exposure_ctrl(s, 0); // 0 = disable , 1 = enable
    s->set_aec2(s, 0); // 0 = disable , 1 = enable
    s->set_aec_value(s, 100);
    s->set_framesize(s, FRAMESIZE_SVGA);
    s->set_quality(s,5);
    s->set_pixformat(s, PIXFORMAT_JPEG);

    /*s->set_brightness(s, 0); // -2 to 2
    s->set_contrast(s, -2); // -2 to 2
    s->set_saturation(s, -2); // -2 to 2
    s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
    s->set_whitebal(s, 1); // 0 = disable , 1 = enable
    s->set_awb_gain(s, 1); // 0 = disable , 1 = enable
    s->set_wb_mode(s, 0); // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
    s->set_exposure_ctrl(s, 0); // 0 = disable , 1 = enable
    s->set_aec2(s, 0); // 0 = disable , 1 = enable
    s->set_gain_ctrl(s, 0); // 0 = disable , 1 = enable
    s->set_agc_gain(s, 0); // 0 to 30
    s->set_gainceiling(s, (gainceiling_t)6); // 0 to 6
    s->set_bpc(s, 1); // 0 = disable , 1 = enable
    s->set_wpc(s, 1); // 0 = disable , 1 = enable
    s->set_raw_gma(s, 1); // 0 = disable , 1 = enable (makes much lighter and noisy)
    s->set_lenc(s, 0); // 0 = disable , 1 = enable
    s->set_hmirror(s, 0); // 0 = disable , 1 = enable
    s->set_vflip(s, 0); // 0 = disable , 1 = enable
    s->set_dcw(s, 0); // 0 = disable , 1 = enable
    s->set_colorbar(s, 0); // 0 = disable , 1 = enable


    s->set_reg(s, 0xff, 0xff, 0x01);//banksel 
    s->set_reg(s, 0x11, 0xff, 01);//frame rate

    s->set_reg(s, 0xff, 0xff, 0x00);//banksel 
    s->set_reg(s, 0x86, 0xff, 1);//disable effects

    s->set_reg(s, 0xd3, 0xff, 5);//clock

    s->set_reg(s, 0x42, 0xff, 0x4f);//image quality (lower is bad)
    s->set_reg(s, 0x44, 0xff, 1);//quality*/

    delay(1200);
}

void Camera::work()
{
}

void Camera::doPicture()
{
    //camera_fb_t* fb = NULL;
    if(!buffer) esp_camera_fb_return(buffer);
    buffer = esp_camera_fb_get();
    if (!buffer) Serial.println("Camera capture failed");
    else pictureDone.emit(buffer);
    esp_camera_fb_return(buffer);
}

camera_fb_t* Camera::getPicture() {
    if (!buffer) esp_camera_fb_return(buffer);
    buffer = esp_camera_fb_get();
    if (!buffer) esp_camera_fb_return(buffer);
    return buffer;
}

void Camera::releaseBuffer(camera_fb_t* fb) {
    //esp_camera_fb_return(fb);
}