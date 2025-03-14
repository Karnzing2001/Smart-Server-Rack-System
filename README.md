# Smart Server Rack System
<p>"Smart Server Rack System" เป็นระบบการจัดการสภาพแวดล้อมของตู้แร็คเซิร์ฟเวอร์แบบอัจฉริยะ ที่สามารถตรวจสอบและควบคุมสภาพแวดล้อมในตู้แร็คเซิร์ฟเวอร์อย่างสมบูรณ์และมีประสิทธิภาพ ในการออกแบบและพัฒนาระบบ มีเซ็นเซอร์และอุปกรณ์ตรวจจับภายในตู้แร็คเซิร์ฟเวอร์ ซึ่งสามารถตรวจจับอุณหภูมิ ความชื้น ควัน และการใช้พลังงานของเซิร์ฟเวอร์และอุปกรณ์ไอทีได้อย่างอัตโนมัติ ข้อมูลที่ได้จากเซ็นเซอร์นี้จะถูกส่งไปยังระบบควบคุมที่อยู่ภายในตู้เซิร์ฟเวอร์ และให้ผู้ดูแลระบบเข้าถึงข้อมูลเหล่านี้ผ่านอินเทอร์เน็ต ทำให้สามารถตรวจสอบสถานะและประสิทธิภาพของระบบได้อย่างสะดวก ระบบ "Smart Server Rack" สามารถแจ้งเตือนผู้ดูแลระบบ เพื่อให้ผู้ดูแลระบบได้ทราบเมื่อเกิดปัญหาหรือสถานการณ์ที่ผิดปกติ ทำให้ผู้ดูแลระบบสามารถรับทราบข้อมูลและดำเนินการแก้ไขปัญหาอย่างทันท่วงที ผลการดำเนินโครงการพบว่า ระบบ Smart Sever Rack ทำงานได้อย่างมีประสิทธิภาพ สามารถส่งข้อมูลและตอบสนองต่อคำสั่งจาก Dashboard ได้แบบรวดเร็ว สามารถตรวจข้อมูลสภาพแวดล้อมภายในตู้แร็คและควบคุมได้ทุกที่ ทุกเวลา เมื่อเชื่อมต่ออินเตอร์เน็ต  มีความปลอดภัยมากขึ้นด้วยการปลดล็อคประตูด้วยบัตร RFID และตรวจสอบความผิดปกติได้อย่างแม่นยำ แจ้งเตือนผ่าน Line Notify ระบบทำความเย็นสามารถลดอุณหภูมิภายในตู้แร็คได้ 5-9°C และลดความชื้นลงได้ 18-20 % จากการตั้งเครื่องปรับอากาศที่อุณหภูมิ 16°C เทียบจากอุณภูมิ 29°C และความชื้น 79% ที่อากาศภายในห้อง ระบบ Smart Server Rack จะช่วยเพิ่มประสิทธิภาพในการดูแลรักษาและบำรุงรักษาเซิร์ฟเวอร์และอุปกรณ์ ลดความเสี่ยงในการหยุดทำงานของระบบ และประหยัดทรัพยากรที่เกี่ยวข้องกับการดูแลรักษาเซิร์ฟเวอร์และอุปกรณ์อย่างมากยิ่งขึ้น</p>

# ความสามารถของระบบ
<li>ระบบสามารถแสดงข้อมูลเกี่ยวกับอุณหภูมิ ความชื้น แรงดันไฟฟ้า กระแสไฟฟ้ากำลังไฟฟ้าที่ใช้ แบบ Real-time</li>
<li>ระบบสามารถแจ้งเตือนความผิดปกติของ อุณหภูมิ ความชื้น ควัน ไฟฟ้า ได้</li>
<li>ระบบสามารถตั้งค่าการแจ้งเตือนความผิดปกติของค่าต่างๆได้</li>
<li>ระบบสามารถตัดการจ่ายไฟฟ้าภายในตู้ได้หากตรวจจับควันได้</li>
<li>สามารถดูข้อมูลย้อนหลังได้</li>
<li>ปลดล็อคประตูตู้แร็คด้วย RFID</li>
<li>ปลดล็อคประตูตู้แร็คผ่าน Dashboard</li>
<li>ควบคุมเครื่องอุณหภูมิปรับอากาศได้</li>
<li>ดูสถานะ UPS ได้ </li>
<li>สามารถสั่งปิด UPS เมื่อไฟดับและแบตเตอรี่ต่ำ และกลับมาเปิด UPS เมื่อไฟฟ้ากลับมาแบบอัตโนมัติ </li>
<li>เมื่อไฟฟ้าดับ และไฟฟ้ากลับมา ระบบสามารถสั่งเปิดเครื่องปรับอากาศ และคืนค่าอุณหภูมิและความเร็วพัดลมไว้ที่ตั้งไว้ได้อัตโนมัติ </li>

# อุปกรณ์ที่ใช้ในโครงการ
<li>PZEM-004T</li>
<li>ESP32S</li>
<li>MQ-2 Smoke Sensor</li>
<li>Solid State Relay 40A Tpye Input DC Output AC</li>
<li>PN532 RFID/NFC Module</li>
<li>Solenoid Electromagnetic Door Lock 12VDC</li>
<li>DHT22 Sensor</li>
<li>IR Transmitter infrared Module</li>
<li>RS-232 TO TTL Module</li>
<li>DC-DC Step Down Module LM2596</li>
<li>Adapter 12 VDC</li>
<li>APC Smart-UPS SC 1000VA </li>
<li>Air conditioner 12VDC Type</li>
<li>Switching power supply 12V 120A</li>

# ภาพรวมของระบบ
![Overall System](https://github.com/user-attachments/assets/6f49f413-cbf9-4dc2-9365-64dc2cd3f69d)

# Schematic Diagram
![Project_schem](https://github.com/user-attachments/assets/b19c138a-f306-4f22-acdc-96816d81695c)

# APC UPS RS-232 Cable Pinout
![image](https://github.com/user-attachments/assets/1359c07d-0f4e-4f3d-8e48-5b75b44c3b6c)
<p>การเชื่อมต่อแบบอนุกรม 9 Pin ปกติ TxD อยู่ที่ PIN 3 และ RxD อยู่ที่ PIN 2 พอร์ต RS-232 ของ APC จะวาง TxD ไว้ที่ PIN 1 และ RxD ไว้ที่ PIN 2 จึงไม่สามารถทำให้เราสามารถใช้สาย RS-232 ทั่วไปได้ จะต้องใช้สาย Cable Smart Signalling ของ APC โดยเฉพาะ ดังนั้นจะต้องทำการเข้าสาย RS-232 ใหม่ ตามรูปภาพ</p>
<p>* รูปภาพจาก [Network UPS Tools (NUT)] (https://networkupstools.org/) * </p>

# Prototype 
![ด้านหน้า](https://github.com/user-attachments/assets/06780755-61a2-4100-a4b3-aca781fe68ef)
<p>ต้นแบบตู้ Rack</p>

![ด้านบน](https://github.com/user-attachments/assets/c590e47c-aab4-4909-ab8c-cd69c685124f)
<p>ตำแหน่งการติดตั้ง Sensor MQ-2 และ DHT22</p>

![กลอนไฟฟ้า](https://github.com/user-attachments/assets/818abd05-5575-41ff-bbb9-64ff5458c842)
<p>ตำแหน่งติดตั้งกลอนไฟฟ้า</p>

![RFID](https://github.com/user-attachments/assets/cfeeeb71-b73b-4503-b5a4-dad80b3fcf42)
<p>ตำแหน่งติดตั้ง RFID Module</p>

![แอร์](https://github.com/user-attachments/assets/2dc4319e-f07b-4178-aded-b8302cc9f387)
<p>ตู้แอร์และอุปกรณ์</p>

![ssr](https://github.com/user-attachments/assets/cbad6ed9-64a1-4f6e-81e0-d1be98217a1f)
<p>ตำแหน่งติดตั้ง Solid State Relay</p>

![อุปกรณ์](https://github.com/user-attachments/assets/eb0876b7-bf8f-49a2-8896-c252368d9884)
<p>ตำแหน่งการติดตั้งกล่องอุปกรณ์</p>

![IR](https://github.com/user-attachments/assets/3b0ebbb7-375e-4ebc-8ef0-6fe40c84a7c8)    
<p>ตำแหน่งการติดตั้ง IR Transmitter</p>

# รูปภาพ
![20240816_114813](https://github.com/user-attachments/assets/5ba868b9-4b81-47a4-81f2-73ffbc6f51b8)

![20240814_203102_1](https://github.com/user-attachments/assets/4dabddfb-5814-4e65-9249-ac642bd8980e)

![20240814_212433_1](https://github.com/user-attachments/assets/53092e3b-217b-4d98-86af-acf7a0e28133)

![20240815_163912_1](https://github.com/user-attachments/assets/e63d1b9e-a454-4045-a184-1e858d4156f1)
