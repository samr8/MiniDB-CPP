from flask import Flask, request, jsonify
import subprocess

app = Flask(__name__)

@app.route('/')
def home():
    return "MiniDB Flask API is running!"

@app.route('/run', methods=['GET'])
def run_minidb():
    try:
        result = subprocess.run(['./minidb'], capture_output=True, text=True)
        return jsonify({
            'output': result.stdout,
            'error': result.stderr
        })
    except Exception as e:
        return jsonify({'error': str(e)})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=10000)
