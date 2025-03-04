#include <WiFi.h>
#include <FirebaseESP32.h>

// WiFi Information
const char* ssid = "iPhone";
const char* password = "987654321";

// Firebase Information
#define FIREBASE_HOST "app-dieu-khien-7306c-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "ikJVw7N0r6MnaAaMMDLXbtG9LMFGt5oC60MWmNAB"

// Motor control pins
const int MR1 = 12;
const int MR2 = 14;
const int ML1 = 27;
const int ML2 = 26;

// Speed control pins
const int ena = 13; // PWM A
const int enb = 25; // PWM B

// Water pump control pin
const int pumpPin = 33;

// Firebase objects
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

// State variables
bool isMoving = false;
bool isSpraying = false;

void setup() {
  Serial.begin(9600);

  // WiFi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Đang kết nối...");
  }
  Serial.println("Đã kết nối WiFi!");
  Serial.print("Địa chỉ IP: ");
  Serial.println(WiFi.localIP());

  // Firebase setup
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);

  if (Firebase.ready()) {
    Serial.println("Đã kết nối Firebase!");
  } else {
    Serial.println("Kết nối Firebase thất bại!");
    Serial.println(firebaseData.errorReason());
  }

  // Motor pin setup
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  // Water pump pin setup
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW);

  // Initial motor speed
  analogWrite(ena, 200); // Tốc độ ban đầu
  analogWrite(enb, 200);
}

void loop() {
  if (Firebase.ready() && Firebase.getJSON(firebaseData, "/controls")) {
    FirebaseJson json = firebaseData.jsonObject();
    FirebaseJsonData jsonData;
    int down = 0, left = 0, right = 0, up = 0, waterSpray = 0;

    // Parse Firebase JSON data
    if (json.get(jsonData, "down")) down = jsonData.to<int>();
    if (json.get(jsonData, "left")) left = jsonData.to<int>();
    if (json.get(jsonData, "right")) right = jsonData.to<int>();
    if (json.get(jsonData, "up")) up = jsonData.to<int>();
    if (json.get(jsonData, "waterSpray")) waterSpray = jsonData.to<int>();

    // **In ra dữ liệu từ Firebase**
    Serial.println("------ Dữ liệu từ Firebase ------");
    Serial.print("down: ");
    Serial.println(down);
    Serial.print("left: ");
    Serial.println(left);
    Serial.print("right: ");
    Serial.println(right);
    Serial.print("up: ");
    Serial.println(up);
    Serial.print("waterSpray: ");
    Serial.println(waterSpray);

    // Movement logic
    if (up == 1 && !isMoving) {
      Forward();
      isMoving = true;
    } else if (down == 1 && !isMoving) {
      Backward();
      isMoving = true;
    } else if (left == 1 && !isMoving) {
      turnLeft();
      isMoving = true;
    } else if (right == 1 && !isMoving) {
      turnRight();
      isMoving = true;
    } else if (up == 0 && down == 0 && left == 0 && right == 0) {
      Stop();
      isMoving = false;
    }

    // Water spray logic
    if (waterSpray == 1 && !isSpraying) {
      pumpOn();
      isSpraying = true;
    } else if (waterSpray == 0) {
      pumpOff();
      isSpraying = false;
    }
  }

  delay(20);
}

// Movement functions
void Forward() {
  digitalWrite(MR1, HIGH);
  digitalWrite(MR2, LOW);
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
}

void Backward() {
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
}

void turnLeft() {
  digitalWrite(MR1, HIGH);
  digitalWrite(MR2, LOW);
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
}

void turnRight() {
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
}

void Stop() {
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, LOW);
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, LOW);
}

// Water pump functions
void pumpOn() {
  digitalWrite(pumpPin, HIGH);
}

void pumpOff() {
  digitalWrite(pumpPin, LOW);
}
