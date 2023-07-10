from flask import Flask, request
import json

app = Flask(__name__)

@app.route('/data', methods=['POST'])
def receive_data():
    data = request.get_json()
    item = data.get('item')
    change = data.get('change')

    print(f"Received data: {data}")

    return 'Data received'

if __name__ == '_main_':
    app.run(host='0.0.0.0', port=5000)