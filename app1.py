import os
from flask import Flask, request, jsonify
# from dotenv import load_dotenv  <- REMOVED
import google.generativeai as genai
from PIL import Image

# load_dotenv()  <- REMOVED

# --- CONFIGURATION ---
try:
    # --- THIS IS THE CHANGE ---
    # Paste your API Key directly into this string
    YOUR_API_KEY = "Your_API_Key"
    
    # Configure the client with the hardcoded key
    genai.configure(api_key=YOUR_API_KEY)
    print("Gemini API configured successfully.")
except Exception as e:
    print(f"Error configuring Google AI client: {e}")
# --- FLASK APP ---
app = Flask(__name__)

def query_gemini_api(image_stream):
    """Opens an image from a stream, sends it to the Gemini API, and gets a prediction."""
    
    print("Sending image to Gemini API...")
    try:
        # Open the image file using the Pillow library from the stream
        img = Image.open(image_stream)

        # Initialize the stable Gemini Pro Vision model
        model = genai.GenerativeModel('gemini-2.5-flash')

        # The API takes a list containing the text prompt and the image object
        response = model.generate_content([
            "The image which i will provide you, you just want to detect which alphabet its represent or number or any other thing but your answer should be in single word like A,B,1,0 or chair, human, etc. If there are multiple, choose the clearest one. Respond with only that character.", 
            img
        ])
        
        prediction = response.text
        print(f"Received response from Gemini: '{prediction}'")
        return {"prediction": prediction}
    
    except Exception as e:
        error_message = f"Gemini API request failed: {e}"
        print(error_message)
        return {"error": error_message}

# --- API ROUTE FOR ESP32 ---
@app.route('/predict_esp', methods=['POST'])
def predict_esp():
    """Receives an image file from the ESP32, gets a prediction, and returns it."""
    
    # Check if the post request has the file part
    if 'imageFile' not in request.files:
        return jsonify({"error": "No 'imageFile' part in the request"}), 400
    
    file = request.files['imageFile']
    
    # If the user does not select a file, the browser submits an empty file
    if file.filename == '':
        return jsonify({"error": "No selected file"}), 400
        
    if file:
        # Pass the file's stream directly to the Gemini function
        prediction_result = query_gemini_api(file.stream)
        return jsonify(prediction_result)

# --- MAIN ---
if __name__ == '__main__':
    # Run on 0.0.0.0 to make the server accessible from any device on your network
    app.run(host='0.0.0.0', port=5000, debug=True)


