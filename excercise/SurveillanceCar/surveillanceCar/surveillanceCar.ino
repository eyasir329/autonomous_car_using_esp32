#include "esp_camera.h"
#include <WiFi.h>
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "soc/soc.h"             // disable brownout problems
#include "soc/rtc_cntl_reg.h"    // disable brownout problems
#include "esp_http_server.h"
#include <ESP32Servo.h>

// Replace with your network credentials
const char* ssid = "SUB1";
const char* password = "newtown123";

#define PART_BOUNDARY "123456789000000000000987654321"

#define CAMERA_MODEL_AI_THINKER

#if defined(CAMERA_MODEL_AI_THINKER)
  #define PWDN_GPIO_NUM     32
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      0
  #define SIOD_GPIO_NUM     26
  #define SIOC_GPIO_NUM     27
  
  #define Y9_GPIO_NUM       35
  #define Y8_GPIO_NUM       34
  #define Y7_GPIO_NUM       39
  #define Y6_GPIO_NUM       36
  #define Y5_GPIO_NUM       21
  #define Y4_GPIO_NUM       19
  #define Y3_GPIO_NUM       18
  #define Y2_GPIO_NUM        5
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     23
  #define PCLK_GPIO_NUM     22

#else
  #error "Camera model not selected"
#endif

#define SERVO_1      14
#define SERVO_2      15

#define SERVO_STEP   5

Servo servo1;
Servo servo2;

int servo1Pos = 0;
int servo2Pos = 0;

static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

httpd_handle_t camera_httpd = NULL;
httpd_handle_t stream_httpd = NULL;

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<html>

<head>
  <title>ESP32-CAM Robot</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 0;
      padding: 10px;
    }

    .container {
      display: flex;
      justify-content: center;
      gap: 30px;
      margin-top: 20px;
    }

    .section {
      text-align: center;
      margin: 10px 0;
    }

    th {
      font-size: medium;
      background-color: aquamarine;
      padding: 8px;
      margin-bottom: 15px;
    }

    td {
      padding: 10px;
    }

    .button {
      background-color: #2f4468;
      border: none;
      color: white;
      padding: 14px 24px;
      font-size: 18px;
      cursor: pointer;
      border-radius: 4px;
      user-select: none;
      margin: 5px;
    }

    .button:hover {
      background-color: #3b587a;
    }

    .slider-label {
      font-size: small;
      margin-bottom: 5px;
      display: block;
    }

    .slider {
      width: 60%;
      margin-top: 5px;
    }

    img {
      max-width: 100%;
      height: auto;
      margin-bottom: 20px;
    }

    table {
      margin: auto;
    }

    .light-control {
      margin-top: 30px;
    }
  </style>
</head>

<body>
  <h2>ESP32 Surveillance Car</h2>
  <img src="" id="photo" alt="Live Video Stream">

  <div class="container">
    <!-- Pan Tilt Control -->
    <div class="section">
      <table>
        <tr>
          <th>Pan Tilt Control</th>
        </tr>
        <tr>
          <td>
            <button class="button" onmousedown="toggleCheckbox('up');" ontouchstart="toggleCheckbox('up');">Up</button>
          </td>
        </tr>
        <tr>
          <td>
            <button class="button" onmousedown="toggleCheckbox('left');" ontouchstart="toggleCheckbox('left');"
              style="margin-right: 20px;">Left</button>
            <button class="button" onmousedown="toggleCheckbox('right');" ontouchstart="toggleCheckbox('right');"
              style="margin-left: 20px;">Right</button>
          </td>
        </tr>
        <tr>
          <td>
            <button class="button" onmousedown="toggleCheckbox('down');"
              ontouchstart="toggleCheckbox('down');">Down</button>
          </td>
        </tr>
      </table>
      <!-- Light Control -->
      <div class="light-control">
        <table>
          <tr>
            <th>Light Control</th>
          </tr>
          <tr>
            <td>
              <label class="slider-label">Brightness</label>
              <input type="range" id="lightBrightness" class="slider" name="lightBrightness" min="0" max="50" style="width: 100%;">
            </td>
          </tr>
        </table>
      </div>
    </div>

    <!-- Controlling Car -->
    <div class="section">
      <table>
        <tr>
          <th>Controlling Car</th>
        </tr>
        <tr>
          <td>
            <button class="button" onmousedown="toggleCheckbox('up');"
              ontouchstart="toggleCheckbox('up');">Forward</button>
          </td>
        </tr>
        <tr>
          <td>
            <label class="slider-label">Forward Speed</label>
            <input type="range" id="forwardSpeed" class="slider" name="forwardSpeed" min="0" max="50">
          </td>
        </tr>
        <tr>
          <td>
            <button class="button" onmousedown="toggleCheckbox('left');"
              ontouchstart="toggleCheckbox('left');">Left</button>
            <button class="button" onmousedown="toggleCheckbox('stop');" ontouchstart="toggleCheckbox('stop');"
              style="margin:0px 20px;">Stop</button>
            <button class="button" onmousedown="toggleCheckbox('right');"
              ontouchstart="toggleCheckbox('right');">Right</button>
          </td>
        </tr>
        <tr>
          <td>
            <button class="button" onmousedown="toggleCheckbox('down');"
              ontouchstart="toggleCheckbox('down');">Backward</button>
          </td>
        </tr>
        <tr>
          <td>
            <label class="slider-label">Backward Speed</label>
            <input type="range" id="backwardSpeed" class="slider" name="backwardSpeed" min="0" max="50">
          </td>
        </tr>
      </table>
    </div>
  </div>
</body>

</html>

)rawliteral";

// Web server handlers
static esp_err_t index_handler(httpd_req_t *req){
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

static esp_err_t stream_handler(httpd_req_t *req){
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != ESP_OK){
    return res;
  }

  while(true){
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      if(fb->width > 400){
        if(fb->format != PIXFORMAT_JPEG){
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if(!jpeg_converted){
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if(res == ESP_OK){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if(fb){
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf){
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if(res != ESP_OK){
      break;
    }
  }
  return res;
}

static esp_err_t cmd_handler(httpd_req_t *req){
  char* buf;
  size_t buf_len;
  char variable[32] = {0,};

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if(!buf){
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "go", variable, sizeof(variable)) == ESP_OK) {
      } else {
        free(buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
      }
    } else {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
    }
    free(buf);
  } else {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }

  sensor_t * s = esp_camera_sensor_get();
  int res = 0;

  if(!strcmp(variable, "up")) {
    if(servo1Pos <= 170) {
      servo1Pos += 10;
      servo1.write(servo1Pos);
    }
    Serial.println("Up");
  }
  else if(!strcmp(variable, "left")) {
    if(servo2Pos <= 170) {
      servo2Pos += 10;
      servo2.write(servo2Pos);
    }
    Serial.println("Left");
  }
  else if(!strcmp(variable, "right")) {
    if(servo2Pos >= 10) {
      servo2Pos -= 10;
      servo2.write(servo2Pos);
    }
    Serial.println("Right");
  }
  else if(!strcmp(variable, "down")) {
    if(servo1Pos >= 10) {
      servo1Pos -= 10;
      servo1.write(servo1Pos);
    }
    Serial.println("Down");
  }
  else {
    res = -1;
  }

  if(res){
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

void startCameraServer(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
  };

  httpd_uri_t cmd_uri = {
    .uri       = "/action",
    .method    = HTTP_GET,
    .handler   = cmd_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t stream_uri = {
    .uri       = "/stream",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };

  if (httpd_start(&camera_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(camera_httpd, &index_uri);
    httpd_register_uri_handler(camera_httpd, &cmd_uri);
    httpd_register_uri_handler(camera_httpd, &stream_uri);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connected to Wi-Fi");
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());
 
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
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // Initialize the camera
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }

  servo1.attach(SERVO_1);
  servo2.attach(SERVO_2);
  
  startCameraServer();
}

void loop() {
  delay(10000);
}
