#include <WiFi.h>
#include <FirebaseESP32.h>

// Thông tin mạng WiFi
const char* ssid = "U12 - L01";
const char* password = "12345678";

// Thông tin Firebase
#define FIREBASE_HOST "remote-car-ee1b9-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Z0eJVyvCR7rcdmZueA7dSrlDjsENZy5kfah2jHOv"

// Khởi tạo đối tượng Firebase
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

void setup() {
  // Khởi động Serial Monitor
  Serial.begin(9600);

  // Kết nối WiFi
  WiFi.begin(ssid, password);

  // Chờ kết nối
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Đang kết nối...");
  }

  // Kết nối thành công
  Serial.println("Đã kết nối!");
  Serial.print("Địa chỉ IP: ");
  Serial.println(WiFi.localIP());

  // Cấu hình Firebase
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.api_key = FIREBASE_AUTH;

  // Kết nối Firebase
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);

  // Kiểm tra kết nối Firebase
  if (Firebase.ready()) {
    Serial.println("Đã kết nối Firebase!");
  } else {
    Serial.println("Kết nối Firebase thất bại!");
  }
}

void loop() {
  // Ví dụ: Ghi dữ liệu vào Firebase
  if (Firebase.setInt(firebaseData, "/test/int", 123)) {
    Serial.println("Ghi dữ liệu thành công!");
  } else {
    Serial.println("Ghi dữ liệu thất bại!");
    Serial.println("Lỗi: " + firebaseData.errorReason());
  }

  delay(2000); // Đợi 2 giây trước khi ghi dữ liệu lần nữa
}
