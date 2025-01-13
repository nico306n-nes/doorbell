from flask import Flask, jsonify, request
import json

app = Flask(__name__)

klokke_data = {
	"current_time": 9834,
	"clicked_time": 8944,
	"diff": 890,
	"format": "millis"
}

@app.route("/")
def hello_world():
    return "<p>Hello, World!</p>"

@app.route("/klokke")
def klokke():
    return jsonify(klokke_data)

if __name__ == "__main__":
    app.run(debug=True, port=8080)