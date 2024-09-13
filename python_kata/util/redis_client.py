from pytz import timezone
from datetime import datetime, timedelta
import sys
from rediscluster import RedisCluster
import redis
from redis.client import Redis

"""
usage:

    client = RedisClient("192.168.56.113:7001,192.168.56.114:7001,192.168.56.115:7001", 'password')
    conn = client.connect()
    values = conn.hgetall(key)

"""

class RedisClient:
    def __init__(self, connection_string, password=None):
        self.startup_nodes = []
        nodes = connection_string.split(',')
        for node in nodes:
            host_port = node.split(':')
            self.startup_nodes.append({'host': host_port[0], 'port': host_port[1]})

        self.password = password
        #logger.info(self.startup_nodes)
        self.redis_pool = None
        self.redis_instance = None
        self.redis_cluster = None

    def connect(self):
        if(len(self.startup_nodes) < 2):
            host = self.startup_nodes[0].get('host')
            port = self.startup_nodes[0].get('port')
            if self.password:
                self.redis_pool = redis.ConnectionPool(host=host, port=port, db=0)
            else:
                self.redis_pool = redis.ConnectionPool(host=host, port=port, password = self.password, db=0)

            self.redis_instance = Redis(connection_pool=self.redis_pool, decode_responses=True)
            return self.redis_instance
        #, skip_full_coverage_check=True
        self.redis_cluster = RedisCluster(startup_nodes=self.startup_nodes, password=self.password)
        return self.redis_cluster
