from flask import Flask, jsonify, request
import time

app = Flask(__name__)

start_time = round(time.time() * 1000)

klokke_data = {
	"current_time": round(time.time() * 1000) - start_time,
	"clicked_time": -10000,
	"diff": 0,
	"format": "millis"
}

klokke_data["diff"] = klokke_data["current_time"] - klokke_data["clicked_time"]

@app.route("/")
def hello_world():
    return "<p>Hello, World!</p>"

@app.route("/emulate")
def emulate():
    klokke_data["clicked_time"] = round(time.time() * 1000) - start_time
    return "<p>Emulated press</p>"

@app.route("/klokke")
def klokke():
    klokke_data["current_time"] = round(time.time() * 1000) - start_time
    klokke_data["diff"] = klokke_data["current_time"] - klokke_data["clicked_time"]
    return jsonify(klokke_data)

if __name__ == "__main__":
    print("Before")
    app.run(debug=True, port=8080)
    print("After")