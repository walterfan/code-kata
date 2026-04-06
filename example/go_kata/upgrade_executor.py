#!/usr/bin/env python3
import sys
import argparse
import os
import json
import time
import subprocess
import flask
from flask_httpauth import HTTPBasicAuth
from flask import make_response
from flask import Flask
from flask import Response
from flask import request
from werkzeug.exceptions import NotFound, ServiceUnavailable
from flask_cors import CORS,cross_origin
from loguru import logger

import random
import string
    
logger.add(sys.stdout,
           format="{time} {message}",
           filter="client",
           level="INFO")

DEFAULT_PORT = 9443

app = Flask(__name__)

cors = CORS(app, resources={r"/api/*": {"origins": "*"}})

current_path = os.path.dirname(os.path.realpath(__file__))

COMMAND_JSON_FILE = "{}/commands.json".format(current_path)
COMMANDS_API_PATH = '/api/v1/commands'
COMMANDS_WHITE_LIST = ['docker', 'kill' , 'bash']
REQUEST_VARS_KEY = 'per_request_vars'
REQUEST_VARS_ITEMS = ['tracking_id', 'user_id']
TRACKING_ID = "Tracking-Id"


"""API:
POST http://127.0.0.1:5000/
{
"name":"docker",
"args": "ps -a",
"verify_code": "aaaa",
"upgrade_id": "bbb",
"track_id": "ccc"
}

"""

register_codes = """
p6vWJJsu
eYkPvbSv
pfoxcW1u
""".split('\n')

executed_commands = {}


def get_random_string(length):
    # except string.punctuation
    characters = string.ascii_letters + string.digits
    return ''.join(random.choice(characters) for i in range(length))

def set_my_request_var(name, value):
    if REQUEST_VARS_KEY not in flask.g:
        flask.g.per_request_var = {}
    flask.g.per_request_var[name] = value

def get_my_request_var(name):
    if REQUEST_VARS_KEY not in flask.g:
        return ''
    return flask.g.per_request_var.get(name, '')

@app.after_request
def per_request_callbacks(response):
    values = flask.g.get(REQUEST_VARS_KEY, {})
    for item_name in REQUEST_VARS_ITEMS:
        if item_name in values:
            values.pop(item_name)

    return response

def read_data():
    start = time.time()

    if not os.path.exists(COMMAND_JSON_FILE):
        save_data(executed_commands)

    with open(COMMAND_JSON_FILE) as json_fp:
        return json.load(json_fp)
    logger.info("read_data: %d, %s", time.time() - start, get_my_request_var(TRACKING_ID))

def save_data(commands):
    start = time.time()

    with open(COMMAND_JSON_FILE, "w") as json_fp:
        json.dump(commands, json_fp, sort_keys=True, indent=4)
    logger.info("save_data: %d, %s", time.time() - start, get_my_request_var(TRACKING_ID))

def execute_command(name, args):
    cmd_with_args = [ name ]
    if ' ' in args:
        cmd_with_args = cmd_with_args + [x.strip() for x in args.split(' ')]
    else:
        cmd_with_args.append(args.strip())

    result = subprocess.run(cmd_with_args,
        stdout = subprocess.PIPE,
        stderr = subprocess.PIPE,
        universal_newlines = True
    )
    return result

def generate_response(arg, response_code=200, contentType="application/json"):
    response = make_response(json.dumps(arg, sort_keys = True, indent=4))
    response.headers['Content-type'] = contentType
    response.status_code = response_code

    response.headers.add("Access-Control-Allow-Origin", "*")
    response.headers.add("Access-Control-Allow-Headers", "*")
    response.headers.add("Access-Control-Allow-Methods", "*")
    
    return response

@app.route('/', methods=['GET'])
def checkhealth():
    return generate_response({ "status": "OKOKOK", 
                              "url": f"{request.base_url}api/v1/commands"})

@app.route(COMMANDS_API_PATH, methods=['GET'])
def list_command():
    trackingId = request.headers.get(TRACKING_ID, "")
    logger.info("list command: trackingId=".format(trackingId))
    commands = read_data()
    return generate_response(commands)

@app.route(COMMANDS_API_PATH, methods=['OPTIONS'])
def test_command():
    return generate_response({})

@app.route(COMMANDS_API_PATH, methods=['POST'])
def create_command():
    command = request.json
    trackingId = request.headers.get(TRACKING_ID, "")
    logger.info("create command: {}, trackingId={}".format(command, trackingId))
    name = command.get("name", "")
    args = command.get("args", "")
    verify_code = command.get("verify_code", "")

    if not verify_code or not name:
        return generate_response({"error": "bad request"}, 400)

    if not verify_code in register_codes and verify_code != "88888888":
        return generate_response({"error": "not found the verify code"}, 404)

    commands = read_data()
    if verify_code in commands and verify_code != "88888888":
        return generate_response({"error": "conflict"}, 409)

    if name in COMMANDS_WHITE_LIST:
        ret = execute_command(name, args)
        command["result"] = ret.stdout
        if ret.stderr:
            command["result"] = ret.stderr
    else:
        logger.warning(f"forbidden command {name}, {args}")
        command["result"] = "error"
        command["error"] = f"{name} is forbidden"

    
    commands[verify_code] = command
    save_data(commands)

    return generate_response(command)


"""
http --auth walter:pass --json POST http://localhost:5000/api/v1/commands name=docker args=ps 
"""
if __name__ == "__main__":

    parser = argparse.ArgumentParser()

    parser.add_argument('-p', action='store', dest='port', default=5000,  help='port')
    parser.add_argument('-d', action='store', dest='debug', default=True,  help='specify debug flag')
    parser.add_argument('-r', action='store', dest='random_str_count', default=0,  help='generate N random code')

    args = parser.parse_args()

    if args.random_str_count:
        length = 7
        for i in range(int(args.random_str_count)):
            ret_str = get_random_string(length)
            if i % 2 == 0:
                ret_str = "p" + ret_str
            else:
                ret_str = "e" + ret_str
            print(ret_str)
        exit(0)
    app.run(host="0.0.0.0", port=args.port, debug=args.debug)

