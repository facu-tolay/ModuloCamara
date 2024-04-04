import paho.mqtt.client as mqtt
import logging
import cv2
import os
import numpy as np
from time import gmtime, strftime

def init_client():
    mqttc = mqtt.Client()

    # mqttc.on_connect = on_connect
    # mqttc.on_publish = on_publish
    # mqttc.on_subscribe = on_subscribe
    # mqttc.on_disconnect = on_disconnect
    # mqttc.message_callback_add(__topicRobotID, on_publish_common)
    mqttc.on_message = on_message
    try:
        mqttc.connect('192.168.0.87', 1883)
        mqttc.subscribe("/topic/upload", 0)
        mqttc.loop_forever()
    except:
        logging.error(f'Error al establecer conexión con el broker')

def on_message(mqttc, obj, msg):
    image_name = f'{strftime("%Y-%m-%d_%H:%M:%S", gmtime())}.jpeg'
    # receive_image(image_name, msg.payload)
    print(image_name)
    filter_image(msg.payload)

# def receive_image(file, payload):
#     f = open(file, "wb")
#     f.write(payload)
#     f.close()

def filter_image(payload):
    try:
        file = np.asarray(bytearray(payload), dtype="uint8") 
        img = cv2.imdecode(file, 0)
        qcd = cv2.QRCodeDetector()
        retval, decoded_info, points, straight_qrcode = qcd.detectAndDecodeMulti(img)
        if retval == False:
            logging.error(f'No se detectó código QR')
        else:
            print(f'Imagen recibida')
    except Exception as e:
        logging.error(f'No se recibio la imagen correctamente {e}')

if __name__ == '__main__':
    init_client()