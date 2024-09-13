import sys
import ssh_client
import requests
import json
import time
from loguru import logger

logger.add(sys.stdout,
           format="{time} {message}",
           filter="client",
           level="DEBUG")

def validate_ip(s):
    a = s.split('.')
    if len(a) != 4:
        return False
    for x in a:
        if not x.isdigit():
            return False
        i = int(x)
        if i < 0 or i > 255:
            return False
    return True

def get(url, params={}, accessToken="", tokenPrefix = 'Bearer '):
    get_headers = {}
    get_headers["Content-Type"] = "application/json"
    if accessToken:
        get_headers["Authorization"] = tokenPrefix + accessToken

    logger.debug("{}, {}", url, get_headers)

    response = requests.get(url, headers=get_headers, verify=False)

    content = ""
    if response.status_code >= 200 and response.status_code < 300:
        content = response.text
        #logger.info(str(json.dumps(results, indent=4, sort_keys=True)))
    else:
        logger.error("get %s error, response code is %d: %s" % (url, response.status_code, response.text))

    return response.status_code, content

def post(url, post_headers={}, post_datas={}, accessToken=""):
    if accessToken:
        post_headers["TOKEN"] = accessToken
    logger.debug("{}, {}, {}", url, post_headers, post_datas)

    response = requests.post(url, headers=post_headers, data=json.dumps(post_datas), verify=False)
        
    content = ""
    if response.status_code >= 200 and response.status_code < 300:
        content = response.text
    else:
        logger.error("get %s error, response code is %d: %s" % (url, response.status_code, response.text))
    return response.status_code, content

def wait_until(check_func, timeout, period=0.1, *args, **kwargs):
  time_to_exit = time.time() + timeout
  while time.time() < time_to_exit:
    if check_func():
        return True
    print(".", end="", flush=True)
    time.sleep(period)

  return False

