from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/upload', methods=['POST'])
def upload_file():
    if '11111' not in request.files:
        return jsonify({'error': 'No file part'}), 400
    
    file = request.files['11111']
    
    if file.filename == '':
        return jsonify({'error': 'No selected file'}), 400
    
    file.save(file.filename)
    
    return jsonify({'message': 'File uploaded successfully'}), 200

if __name__ == '__main__':
    app.run(host="0.0.0.0", debug=True)
