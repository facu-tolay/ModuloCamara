import paho.mqtt.client as mqtt
import logging
import cv2
import os
import math
import random
import numpy as np
import configparser
from time import gmtime, strftime

config = configparser.ConfigParser()
config.sections()

def init_client():
    mqttc = mqtt.Client()
    mqttc.on_message = on_message
    try:
        mqttc.connect(config['MQTT']['HOST'], int(config['MQTT']['PORT']))
        mqttc.subscribe(config['MQTT']['TOPIC'], 1)
        mqttc.loop_forever()
    except:
        logging.error(f'Error al establecer conexión con el broker')

def on_message(mqttc, obj, msg):
    filter_image(msg.payload)

def recognize_code(img):
    qcd = cv2.QRCodeDetector()
    retval, decoded_info, points, straight_qrcode = qcd.detectAndDecodeMulti(img)
    return retval, points

def crop_and_try(img):
    retval, points = recognize_code(img)
    if retval is True:
        calculate_distance(points)
    else:
        logging.error(f'No se detectó código QR')

def calculate_distance(p):
    res = math.sqrt((p.item(0)-p.item(2))**2 + (p.item(1)-p.item(3))**2)
    focal_lenght = (int(config['QR']['PPM'])*int(config['QR']['DISTANCE']))/int(config['QR']['SIZE'])
    distance = (int(config['QR']['SIZE'])*focal_lenght)/res
    random_error = random.randint(-int(config['QR']['ERROR']), int(config['QR']['ERROR']))
    # logging.debug(f'La distancia es: {int(distance)+random_error}mm') 

def filter_image(payload):
    try:
        img = cv2.imdecode(np.asarray(bytearray(payload), dtype="uint8"), 0)
        retval, points = recognize_code(img)
        if retval is True:
            calculate_distance(points)
        else:
            crop_and_try(img[int(config['PHOTO']['X1']):int(config['PHOTO']['X2']),
                             int(config['PHOTO']['Y1']):int(config['PHOTO']['Y2'])])
    except Exception as e:
        logging.error(f'No se recibio la imagen correctamente {e}')

if __name__ == '__main__':
    logging.basicConfig(format='[%(asctime)s] [%(levelname)s]: %(message)s',
                    datefmt='%m/%d/%Y %I:%M:%S',
                    level=logging.DEBUG)
    config.read('config.ini')
    init_client()