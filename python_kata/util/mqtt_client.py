#!/usr/bin/env python3
import os
import sys
import time
import random
from dataclasses import dataclass
from paho.mqtt import client as paho
import json
import uuid
import argparse
import yaml_config
import test_util
import ssl
from loguru import logger

logger.add(sys.stdout,
           format="{time} {message}",
           filter="client",
           level="DEBUG")

CURRENT_PATH = os.path.dirname(os.path.realpath(__file__))
CONFIG_SECTION = "mqtt_messages"
DEFAULT_CMD = "mark"

class MsgTemplates:

    def __init__(self, config_file):
        self._yaml_config = yaml_config.YamlConfig(config_file)
        self._msg_config = self._yaml_config.get_config_section(CONFIG_SECTION)

    def get_msg_tpl(self, cmd, category="request"):
        return self._msg_config.get(cmd, {}).get(category)

class MsgBuilder:

    def __init__(self, msg_tpl = ""):
        self._data = {}
        if msg_tpl:
            self._data = json.loads(msg_tpl)
        self._seq = 0

    def cmd(self, command):
        self._data["cmd"] = command
        return self

    def sender(self, sender):
        self._data["from"] = sender
        return self

    def receiver(self, receiver):
        self._data["to"] = receiver
        return self

    def action(self, action):
        self._data["action"] = action
        return self

    def seq(self, seqNum=0):
        if not seqNum:
            self._seq += 1
            self._data["seq"] = self._seq
        else:
            self._data["seq"] = seqNum
        return self

    def time(self, timestamp=0):
        if not timestamp:
            self._data["time"] = int(time.time() * 1000)
        else:
            self._data["time"] = timestamp
        return self

    def set_track_id(self, track_id):
        self._data[track_id] = track_id
        return self

    def set_field(self, field_name, field_value):
        self._data[field_name] = field_value
        return self
    
    def get_field(self, field_name):
        return self._data.get(field_name)
    
    def build(self):
        return json.dumps(self._data)

@dataclass
class MqttConfig:
    broker: str = 'localhost'
    port: int = 1883
    username: str = 'hbzn'
    password: str = 'hbzn123456'

def read_mqtt_config(config_dict):
    cfg = MqttConfig()
    cfg.broker = config_dict.get("broker")
    cfg.port = config_dict.get("port")
    cfg.username = config_dict.get("username")
    cfg.password = config_dict.get("password")
    return cfg

class MqttClient:

    def __init__(self, mqttConfig, **kwargs):
        self._mqtt_config = mqttConfig
        # Generate a Client ID with the subscribe prefix.
        self._connected = False
        self._started = False
        self._client = None
        self._messages = []
        self._client_id = kwargs.get("client_id", "")
        if not self._client_id:
            self._client_id = f"client-{random.randint(0, 10000)}"

    def connect(self):
        if self._connected:
            return

        self._client = paho.Client(self._client_id, clean_session= False)

        if self._mqtt_config.port == 8883:
            self._client.tls_set(cert_reqs=ssl.CERT_NONE)
            self._client.tls_insecure_set(True)

        self._client.username_pw_set(self._mqtt_config.username, self._mqtt_config.password)
        self._client.on_connect = self.on_connect
        self._client.connect(self._mqtt_config.broker, self._mqtt_config.port)
        self.start()
        print(f"connect {self._mqtt_config} ...")
        return self._client

    def disconnect(self):
        if not self._client:
            print("Error: have not connected broker")
            return
        self.stop()
        self._client.disconnect()

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            if not self._connected:
                print(f"Connected to MQTT Broker: {str(self._mqtt_config)}")
                self._connected = True
        else:
            print("Failed to connect, return_code={}".format(rc))
            self._connected = False

    def is_connected(self):
        return self._connected
    
    def clear_message(self):
        self._messages.clear()

    def get_messages(self):
        return self._messages

    def message_count(self):
        return len(self._messages)

    def find_message(self, keyword):
        for msg in self._messages:
            if keyword in msg:
                return msg
        return None

    def on_message(self, client, userdata, msg):
        payload = msg.payload.decode()
        self._messages.append(payload)
        logger.debug(f"Received `{payload}` from `{msg.topic}` topic, count={len(self._messages)}")

    def start(self):
        if not self._started:
            self._client.loop_start()
            self._started = True

    def stop(self):
        if self._started:
            self._client.loop_stop()
            self._started = False

    def publish(self, topic, msg, retry_count = 3):
        if not self._client:
            print("Error: have not connected broker")
            return
        
        self.start()
        msg_count = 1
        
        while True:
            qos = 1
            retrain = True
            result = self._client.publish(topic, msg, qos, retrain)
            # result: [0, 1]
            status = result[0]
            if status == 0:
                print(f"Sent `{msg}` to topic `{topic}`")
                break
            else:
                print(f"Failed to send message to topic {topic}")
                msg_count += 1
                if msg_count > retry_count:
                    break
        

    def subscribe(self, topic, timeout=0, msg_cb=None):
        if not self._client:
            print("Error: have not connected broker")
            return
        
        self.start()

        self._client.subscribe(topic)
        if msg_cb:
            self._client.on_message = msg_cb
        else:    
            self._client.on_message = self.on_message
            


if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('--action','-a', required=True, action='store', dest='action', help='specify action: pub|sub|test')
    parser.add_argument('--broker','-b', action='store', dest='broker', help='specify the broker address')
    parser.add_argument('--port', type=int, action='store', dest='port', help='specify the broker port', default=1883)
    parser.add_argument('-u', action='store', dest='username', help='specify a username')
    parser.add_argument('-p', action='store', dest='password', help='specify a password')
    parser.add_argument('-m', action='store', dest='command', help='specify a command')
    parser.add_argument('-t', action='store', dest='topic', help='specify a topic', default="req")
    parser.add_argument('-s', type=int, action='store', dest='sequence', help='specify a sequence', default=1)
    parser.add_argument('--payload', action='store', dest='payload', help='specify custom payload')
    parser.add_argument('--client', action='store', dest='client', help='specify custom client id')
    parser.add_argument('--duration', action='store', dest='duration', default=60, help='specify wait duration, it is 60s by default')
    parser.add_argument('--count', '-c', action='store', dest='count', default=20, help='specify received msg count, it is 20 by default')

    args = parser.parse_args()

    mqtt_config = MqttConfig()
    if args.broker:
        mqtt_config.broker = args.broker
    if args.port:
        mqtt_config.port = args.port
    if args.username:
        mqtt_config.username = args.username
    if args.password:
        mqtt_config.password = args.password

    client = MqttClient(mqtt_config, client_id = args.client)
    msg_templates = MsgTemplates("./test_config.yml")

    if args.action == "test":
        msg_tpl = msg_templates.get_msg_tpl(args.command)
        builder = MsgBuilder(msg_tpl).time().seq(args.sequence)
        builder.set_field("track_id", str(uuid.uuid4()))
        builder.set_field("spot_id", str(uuid.uuid4()))
        msg = builder.build()
        client.connect()
        test_util.wait_until(lambda: client.is_connected(), 10)
        client.publish(args.topic, msg)
        client.disconnect()
        exit(0)
    
    if args.command:
        default_msg = msg_templates.get_msg_tpl(args.command)
    else:
        default_msg = msg_templates.get_msg_tpl(DEFAULT_CMD)

    client.connect()
    ret = test_util.wait_until(lambda: client.is_connected(), 10)
    if  not ret:
        print("Error: connect to broker failed")
        exit(-1)

    if args.action == 'sub':
        client.subscribe(args.topic, 10)
        test_util.wait_until(lambda: client.message_count() > args.count, int(args.duration))
    elif args.action == "pub":
        if args.payload:
            msg = args.payload
        else:
            msg = MsgBuilder(default_msg).time().seq(args.sequence).build()
        client.publish(args.topic, msg)
    else:
        print('usage: ./mqtt_client.py -a <sub|pub|test> -t <topic> -m <message>')
        print('e.g. ./mqtt_client.py -b <host> -a pub -t <topic> -m <msg_type>')
        print('\t ./mqtt_client.py -b <host> -a sub -t <topic>')
        print('\t ./mqtt_client.py -b <host> -a sub -t <topic> --payload "{\"key\": \"value\"}"')
        

    client.disconnect()
