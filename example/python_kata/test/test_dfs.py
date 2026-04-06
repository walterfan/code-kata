#!/usr/bin/env python3

import unittest
from basic_algo import bfs

class DfsTest(unittest.TestCase):

    def test_sorted_array(self):
        graph = {
            'A': ['B', 'C'],
            'B': ['A', 'D', 'E'],
            'C': ['A', 'F'],
            'D': ['B'],
            'E': ['B', 'F'],
            'F': ['C', 'E']
        }

        # 从节点 'A' 开始运行 BFS
        result = bfs(graph, 'A')
        print("BFS 结果:", result)
        self.assertEqual(result, ['A', 'B', 'C', 'D', 'E', 'F'])

if __name__ == '__main__':
    unittest.main()

