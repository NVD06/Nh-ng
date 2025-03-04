import { initializeApp } from "https://www.gstatic.com/firebasejs/9.6.8/firebase-app.js";
import { getDatabase, ref, set, onValue } from "https://www.gstatic.com/firebasejs/9.6.8/firebase-database.js";

const firebaseConfig = {
  apiKey: "AIzaSyCSQAol-nxqMPTq2bV1JAI6RbLvRkEmf0w",
  authDomain: "app-dieu-khien-7306c.firebaseapp.com",
  databaseURL: "https://app-dieu-khien-7306c-default-rtdb.firebaseio.com",
  projectId: "app-dieu-khien-7306c",
  storageBucket: "app-dieu-khien-7306c.firebasestorage.app",
  messagingSenderId: "507311552534",
  appId: "1:507311552534:web:4d0d14164944fe3ec0dd37"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const database = getDatabase(app);
const connectionStatus = document.getElementById('connectionStatus');

// Firebase Connection Check
function checkFirebaseConnection() {
  onValue(ref(database, '/'), (snapshot) => {
    connectionStatus.textContent = "Connected to Firebase!";
    connectionStatus.classList.remove('disconnected');
    connectionStatus.classList.add('connected');
    set(ref(database, `test/connection`), "working")
      .then(() => console.log("Test write successful!"))
      .catch((error) => console.error("Test write failed:", error));
  }, (error) => {
    connectionStatus.textContent = "Failed to connect to Firebase!";
    connectionStatus.classList.remove('connected');
    connectionStatus.classList.add('disconnected');
    console.error("Error connecting to Firebase:", error);
  });
}

// Call the connection check
checkFirebaseConnection();

// Key map for directional controls
const keyMap = {
  ArrowUp: 'up',
  ArrowDown: 'down',
  ArrowLeft: 'left',
  ArrowRight: 'right'
};

// State to track water spray
let isSprayingWater = false;

// Function to send signal to Firebase
function sendSignal(action, state) {
  set(ref(database, `controls/${action}`), state)
    .then(() => console.log(`Successfully sent ${state} to ${action}`))
    .catch((error) => console.error("Error sending data to Firebase:", error));
}

// Event listener for keydown
document.addEventListener('keydown', (event) => {
  const direction = keyMap[event.key];
  console.log(`Keydown event: ${event.key}`); // Log keydown event
  if (direction) {
    console.log(`Sending 1 to ${direction}`);
    sendSignal(direction, 1); // Send 1 when the key is pressed
  }

  // Handle water spray with Space key
  if (event.code === "Space" && !event.repeat) { // Prevent repeat events
    isSprayingWater = !isSprayingWater; // Toggle water spray state
    const state = isSprayingWater ? 1 : 0; // 1 for spray, 0 for stop
    console.log(`Sending ${state} to waterSpray`);
    sendSignal("waterSpray", state);
    console.log(`Water spray ${isSprayingWater ? "started" : "stopped"}`);
  }
});

// Event listener for keyup
document.addEventListener('keyup', (event) => {
  const direction = keyMap[event.key];
  console.log(`Keyup event: ${event.key}`); // Log keyup event
  if (direction) {
    console.log(`Sending 0 to ${direction}`);
    sendSignal(direction, 0); // Send 0 when the key is released
  }
});

// Biến để theo dõi trạng thái chế độ tự động
let isAutoMode = false;

/// Hàm gửi tín hiệu cho chế độ tự động
function toggleAutoMode() {
  isAutoMode = !isAutoMode; // Đảo ngược trạng thái
  const mode = isAutoMode ? "auto" : "manual"; // "auto" nếu bật, "manual" nếu tắt

  // Gửi trạng thái lên Firebase
  const autoModeRef = ref(database, 'Auto/Mode'); // Tham chiếu rõ ràng tới Auto/Mode
  set(autoModeRef, mode)
    .then(() => {
      console.log(`Auto mode set to: ${mode}`);
      alert(`Auto mode is now: ${mode.toUpperCase()}`);
    })
    .catch((error) => {
      console.error("Error updating auto mode:", error);
    });
}

// Thêm sự kiện lắng nghe cho nút "Auto Mode"
const autoModeButton = document.getElementById('autoModeButton');
autoModeButton.addEventListener('click', toggleAutoMode);
