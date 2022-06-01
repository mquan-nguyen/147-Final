#!/bin/bash
from flask import Flask, render_template
from flask import request
import sqlite3
from twilio.rest import Client
import json

# for safety purposes, authentication key is taken out
twilio_SID = ""
twilio_auth = ""
twilio_phone = ""

app = Flask(__name__, static_folder="static")
app.config["DEBUG"] = True
client = Client(twilio_SID, twilio_auth)

@app.route("/")
def hello():
    return render_template("home.html")

@app.route("/data/weather", methods=["POST", "GET"])
def receive_weather_data():
    if request.method == "POST":
        #this assumes that the ESP32 is sending data in json format
        #with header "application/json"
        data = request.get_json()
        print(data)
        
        # we should still send the notification even if writing to the db fails
        if (data["send_notification"] == True):
            send_weather_text(data["temp"], data["humidity"])

        try: 
            
            with sqlite3.connect('data.db') as con:
                cur = con.cursor()
                cur.execute("INSERT INTO weather (temp, humidity, time) VALUES (:temp, :humidity, datetime('now','localtime'))", data)

                con.commit()
                cur.close()
        except sqlite3.Error as e:
            err_str = "An error has occurred inserting weather data into the db " + e.args[0]
            print(err_str)
            return err_str
            
        return "Successfully wrote data to db\n" + str(data) 

    #technically not needed if we're going to bake in the data into flask template but useful for debugging 
    if request.method == "GET":
        try:
            return json.dumps(get_weather_data())
        except sqlite3.Error as e:
            err_str = "An error has occurred retrieving weather data from the db " + e.args[0]
            print(err_str)
            return err_str

# binds weater data to variable "weather data" for access in template
@app.context_processor
def inject_data():
    return {"weather_data": get_weather_data(), "gyro_data": get_gyro_data()}

def get_weather_data():
    return query_data("weather","SELECT * FROM weather ORDER BY datetime(time) ASC LIMIT 100")

def get_gyro_data():
    return query_data("gyro", "SELECT * FROM gyro ORDER BY datetime(time) ASC LIMIT 100")

def query_data(data_label, query):
    with sqlite3.connect('data.db') as con:
        # comes into effect when fetching results from db
        # returns sqlite "row" obj (much like dict but can use indexing) instead of returning tuples only
        con.row_factory = sqlite3.Row 

        cur = con.cursor()
        cur.execute(query)
        return_data = cur.fetchall()
        # we have to return it as a value of "data" since that is proper json :/
        return {data_label + "_data": [dict(x) for x in return_data]}

def send_weather_text(temp, humidity):
    message = client.messages.create(to="+17147880266", from_=twilio_phone,
            body="Temp/Humidity Alert: " + str(temp) + " C, " + str(humidity) + "%h")
    print("\n ** SENT OUT A WEATHER ALERT ** \n")
    #TODO: write to notification database as well


@app.route("/data/gyro", methods=["POST"])
def receive_gyro_data():
    if request.method == "POST":
        data = request.get_json()
        print(data)
        
        # we should still send the notification even if writing to the db fails
        if (data["send_notification"] == True):
            send_fall_text()

        try: 
            
            with sqlite3.connect('data.db') as con:
                cur = con.cursor()
                cur.execute("INSERT INTO gyro (gyroX, gyroY, gyroZ, accelX, accelY, accelZ, time, gyroRMS, accelRMS) VALUES (:gyroX, :gyroY, :gyroZ, :accelX, :accelY, :accelZ, datetime('now', 'localtime'), :gyroRMS, :accelRMS)", data)

                con.commit()
                cur.close()
        except sqlite3.Error as e:
            err_str = "An error has occurred inserting gyro/accel data into the db " + e.args[0]
            print(err_str)
            return err_str
            
        return "Successfully wrote data to db\n" + str(data) 

def send_fall_text():
    message = client.messages.create(to="+17147880266", from_=twilio_phone,
            body="Fall alert!") 
    print("\n ** SENT OUT A FALL ALERT ** \n")
    #TODO: write to notification database as well
