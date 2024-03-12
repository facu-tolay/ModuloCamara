import paho.mqtt.client as mqtt
import logging
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
        mqttc.connect('localhost', 1883, 60)
        mqttc.loop_start()
        mqttc.subscribe("/topic/upload", 1)
    except:
        logging.error(f'Cant connect to the broker')

def on_message(mqttc, obj, msg):
    image_name = f'{strftime("%Y-%m-%d_%H:%M:%S", gmtime())}.jpeg'
    f = open(f'{image_name}', "wb")
    f.write(msg.payload)
    print(f'Image Received: {image_name}')
    f.close()

def main():
    init_client()
    print("AAAA")
    while 1:
        pass

if __name__ == '__main__':
    main()