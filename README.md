# Final Project Documentation

## Sending data to AWS

> public ip: 3.101.191.132 

The data to send to AWS should be formatted as such, and varies according to the type of data. 

content-type: application/json

### Weather Data
send to endpoint -> /data/weather

```json
{
   "temp":"float_value",
   "humidity":"float_value",
   "send_notification":"true"
}
```

> temp is sent in Celsius, humidity is in percentage
### Gyro/Accel Data
send to endpoint -> /data/gyro

```json
{
    "gyroX": "float_value",
    "gyroY": "float_value", 
    "gyroZ": "float_value",
    "accelX": "float_value",
    "accelY": "float_value", 
    "accelZ": "float_value",
    "gyroRMS": "float_value",
    "accelRMS": "float_value",
    "send_notification": "true"
}
```

RMS = root mean square

## Database Schema

### weather table
- temp -> real
- humidity -> real
- time -> text

### gyro table
- gyroX -> real
- gyroY -> real
- gyroZ ->real
- accelX ->real
- accelY ->real
- accel Z ->real
- time -> text 
- gyroRMS -> real
- accelRMS -> real

sqlite does not have the timestamp field, instead can be converted into text w/ datetime('now')