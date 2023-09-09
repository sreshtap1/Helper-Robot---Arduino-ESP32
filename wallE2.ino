#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <Servo.h>

Servo leftServo, rightServo, faceServo;
const int leftServoPin = 5;
const int rightServoPin = 6;
const int faceServoPin = 7;

const int motorPinA = 8;
const int motorPinB = 9;
const int motorPinC = 10;
const int motorPinD = 11;

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Wifi ka Naam 2.4G";
const char* password = "wifikapassword";

const char* identifier = "key";
const char* value = "value";

// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>

<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" />

    <link rel="icon" href="data:," />

    <link
      href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.1/dist/css/bootstrap.min.css"
      rel="stylesheet"
      integrity="sha384-4bw+/aepP/YC94hEpVNVgiZdgIC5+VKNBQNGCHeKRQN+PtmoHDEXuppvnDJzQIu9"
      crossorigin="anonymous"
    />
    <title>  Wall-E</title>

    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
  </head>

  <body>
    <div class="container">
      <div class="row">
        <div class="col-lg-12 d-flex justify-content-center">
          <h1>  Systemes Wall - E</h1>
        </div>
      </div>

      <div class="row">
        <div class="col-lg-6">
          <h3>Movements Control</h3>

          <div class="row">
            <div
              class="col-lg-3 d-flex align-items-center justify-content-center"
            >
              <button class="btn btn-primary" id="movementBtnL">LEFT</button>
            </div>

            <div
              class="col-lg-3 d-flex align-items-center justify-content-center"
            >
              <div class="row">
                <div class="col-lg-12 d-flex justify-content-center mb-4">
                  <button class="btn btn-primary" id="movementBtnU">UP</button>
                </div>

                <div class="col-lg-12 d-flex justify-content-center">
                  <button class="btn btn-primary" id="movementBtnD">
                    DOWN
                  </button>
                </div>
              </div>
            </div>

            <div
              class="col-lg-3 d-flex align-items-center justify-content-center"
            >
              <button class="btn btn-primary" id="movementBtnR">Right</button>
            </div>
          </div>
        </div>

        <div class="col-lg-6">
          <h3>Face and Hand Control</h3>

          <div class="row">
            <div class="col-lg-4">
              <div class="row">
                <div
                  class="col-lg-12 d-flex align-items-center justify-content-center"
                >
                  <h4>Left Hand</h4>
                </div>
              </div>

              <div class="row">
                <div
                  class="col-lg-12 d-flex align-items-center justify-content-center"
                >
                  <input
                    type="range"
                    orient="vertical"
                    value="50"
                    id="leftHand"
                  />
                </div>
              </div>
            </div>

            <div class="col-lg-4">
              <div class="row">
                <div
                  class="col-lg-12 d-flex align-items-center justify-content-center"
                >
                  <h4>Face</h4>
                </div>
              </div>

              <div class="row">
                <div
                  class="col-lg-12 d-flex align-items-center justify-content-center"
                >
                  <input type="range" value="50" id="face" />
                </div>
              </div>
            </div>

            <div class="col-lg-4">
              <div class="row">
                <div
                  class="col-lg-12 d-flex align-items-center justify-content-center"
                >
                  <h4>Right Hand</h4>
                </div>
              </div>

              <div class="row">
                <div
                  class="col-lg-12 d-flex align-items-center justify-content-center"
                >
                  <input
                    type="range"
                    orient="vertical"
                    value="50"
                    id="rightHand"
                  />
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>

    <script
      src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.1/dist/js/bootstrap.bundle.min.js"
      integrity="sha384-HwwvtgBNo3bZJJLYd8oVXjrBZt8cqVSpeBNS5n7C8IVInixGAoxmnlMuBnhbgrkm"
      crossorigin="anonymous"
    ></script>

    <script>
      !(function (t, n, i, o) {
        function e(n, i) {
          t.isFunction(i) && (i = { onHold: i }),
            (this.$e = t(n)),
            (this.ms = !1),
            (this.options = t.extend({}, u, i)),
            (this.interval = null),
            (this.indicator = 0),
            this.init();
        }
        var l = "clickAndHold",
          u = {
            timeout: 200,
            onHold: function (t, n) {},
            onRelease: function (t) {},
          },
          s = e.prototype;
        (s.init = function () {
          var t = this;
          this.$e
            .mousedown(function (n) {
              (t.ms = !0),
                (t.e = this),
                (t.event = n),
                (t.initialTimeout = t.options.timeout),
                t.callAndCheck();
            })
            .mouseup(function (n) {
              (t.ms = !1),
                t.options.onRelease.call(this, n),
                clearInterval(t.interval),
                (t.options.timeout = t.initialTimeout),
                (t.indicator = 0);
            });
        }),
          (s.callAndCheck = function (t, n) {
            var i = this,
              o = i.options;
            i.ms &&
              (i.indicator++,
              o.onHold.call(i.e, i.event, i.indicator),
              i.ms &&
                (o.timeout > 10 && (o.timeout -= 10),
                clearInterval(i.interval),
                (i.interval = setInterval(function () {
                  i.callAndCheck();
                }, o.timeout))));
          }),
          (t.fn[l] = function (n) {
            return this.each(function () {
              t.data(this, "plugin_" + l) ||
                t.data(this, "plugin_" + l, new e(this, n));
            });
          });
      })(jQuery, window, document);
    </script>

    <script>
      $.ajaxSetup({ timeout: 1000 });
      const sendData = (key, value) => {
        $.get(`/get?key=${key}&value=${value}`);        
        console.log("data sent");
      }
    </script>

    <script>
      

      $("#rightHand").on("input change", function () {
        sendData("right", this.value);
      });

      $("#leftHand").on("input change", function () {
        sendData("left", this.value);
      });

      $("#face").on("input change", function () {
        sendData("face", this.value);
      });

      $("#movementBtnL").clickAndHold({
        timeout: 200,

        onHold: () => {
          sendData("move", "left");
        },

        onRelease: () => {
          sendData("move", "stop");
        },
      });

      $("#movementBtnU").clickAndHold({
        timeout: 200,

        onHold: () => {
          sendData("move", "up");
        },

        onRelease: () => {
          sendData("move", "stop");
        },
      });

      $("#movementBtnD").clickAndHold({
        timeout: 200,

        onHold: () => {
          sendData("move", "down");
        },

        onRelease: () => {
          sendData("move", "stop");
        },
      });

      $("#movementBtnR").clickAndHold({
        timeout: 200,

        onHold: () => {
          sendData("move", "right");
        },

        onRelease: () => {
          sendData("move", "stop");
        },
      });
    </script>
  </body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

void moveForward() {
  digitalWrite(motorPinA, HIGH);
  digitalWrite(motorPinB, LOW);
  digitalWrite(motorPinC, HIGH);
  digitalWrite(motorPinD, LOW);
}

void moveRight() {
  digitalWrite(motorPinA, LOW);
  digitalWrite(motorPinB, HIGH);
  digitalWrite(motorPinC, HIGH);
  digitalWrite(motorPinD, LOW);
}

void moveLeft() {
  digitalWrite(motorPinA, HIGH);
  digitalWrite(motorPinB, LOW);
  digitalWrite(motorPinC, LOW);
  digitalWrite(motorPinD, HIGH);
}

void moveBack() {
  digitalWrite(motorPinA, LOW);
  digitalWrite(motorPinB, HIGH);
  digitalWrite(motorPinC, LOW);
  digitalWrite(motorPinD, HIGH);
}

void moveStop(){
  digitalWrite(motorPinA, LOW);
  digitalWrite(motorPinB, LOW);
  digitalWrite(motorPinC, LOW);
  digitalWrite(motorPinD, LOW);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
    String inputMessage;
    String inputParam;

    inputParam = request->getParam(identifier)->value();
    inputMessage = request->getParam(value)->value();

    if (inputParam == "move") {
      if (inputMessage == "up") {
        moveForward();
      } else if (inputMessage == "back") {
        moveBack();
      } else if (inputMessage == "right") {
        moveRight();
      } else if (inputMessage == "left") {
        moveLeft();
      } else {
        moveStop();
      }
    } else if (inputParam == "rightHand") {
      int angle = map(inputMessage.toInt(), 0, 100, 0, 180);
      rightServo.write(angle);
    } else if (inputParam == "leftHand") {
      int angle = map(inputMessage.toInt(), 0, 100, 0, 180);
      leftServo.write(angle);
    } else if (inputParam == "face") {
      int angle = map(inputMessage.toInt(), 0, 100, 0, 180);
      faceServo.write(angle);
    }

    Serial.print(inputParam);
    Serial.print(": ");
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" + inputParam + ") with value: " + inputMessage + "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
}