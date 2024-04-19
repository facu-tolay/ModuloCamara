import paho.mqtt.client as mqtt
import logging
import cv2
import os
import math
import json
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
    distances = filter_image(msg.payload)
    if distances != None:
        mqttc.publish('/topic/distances', distances)

def recognize_code(img):
    qcd = cv2.QRCodeDetector()
    retval, decoded_info, points, straight_qrcode = qcd.detectAndDecodeMulti(img)
    return retval, points

def crop_and_try(img):
    retval, points = recognize_code(img)
    if retval is True:
        return get_distances(points)
    else:
        logging.error(f'No se detectó código QR')
        return None

def get_distances(points):
    focal_lenght = (int(config['QR']['PPM'])*int(config['QR']['DISTANCE']))/int(config['QR']['SIZE'])
    distance, distance_right, distance_left = calculate_distances(calculate_resolution(points))
    return parse_json(distance, distance_right, distance_left)

def calculate_resolution(points):
    res = math.sqrt((points.item(0) - points.item(2))**2 + (points.item(1) - points.item(3))**2)
    res_right = math.sqrt((points.item(0) - 1280)**2)
    res_left = math.sqrt((points.item(0))**2)
    return res, res_right, res_left

def calculate_distances(res, res_right, res_left, focal_lenght):
    random_error = random.randint(-int(config['QR']['ERROR']), int(config['QR']['ERROR']))
    distance = (int(config['QR']['SIZE'])*focal_lenght)/res+random_error
    distance_right = (int(config['QR']['SIZE'])*focal_lenght)/res_right+random_error
    distance_left = (int(config['QR']['SIZE'])*focal_lenght)/res_left+random_error
    return distance, distance_right, distance_left

def parse_json(distance, distance_right, distance_left):
    data = {
        "distance": {distance},
        "distance_right": {distance_right},
        "distance_left": {distance_left}
    }
    return json.dumps(data)

def filter_image(payload):
    try:
        img = cv2.imdecode(np.asarray(bytearray(payload), dtype="uint8"), 0)
        retval, points = recognize_code(img)
        if retval is True:
            return get_distances(points)
        else:
            return crop_and_try(img[int(config['PHOTO']['X1']):int(config['PHOTO']['X2']),
                             int(config['PHOTO']['Y1']):int(config['PHOTO']['Y2'])])
    except Exception as e:
        logging.error(f'No se recibio la imagen correctamente {e}')
        return None

if __name__ == '__main__':
    logging.basicConfig(format='[%(asctime)s] [%(levelname)s]: %(message)s',
                    datefmt='%m/%d/%Y %I:%M:%S',
                    level=logging.DEBUG)
    config.read('config.ini')
    init_client()