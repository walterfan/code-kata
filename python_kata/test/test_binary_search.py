#!/usr/bin/env python3
import HtmlTestRunner
import unittest
import json
import urllib3
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)
import sys
import os
import yaml_config
import test_util
from tabulate import tabulate
from loguru import logger
from basic_algo import binary_search

logger.add(sys.stdout,
           format="{time} {message}",
           filter="client",
           level="DEBUG")




class ShellSortTest(unittest.TestCase):

    def setUp(self):
        logger.debug("# --- {} begin ---", self.id())

    def tearDown(self):
        logger.debug("# --- {} end ---", self.id())

    def test_element_found(self):
        arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        target = 6
        result = binary_search(arr, target)
        self.assertEqual(result, arr.index(target))

    def test_element_not_found(self):
        arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        target = 11
        result = binary_search(arr, target)
        self.assertEqual(result, -1)

    def test_empty_array(self):
        arr = []
        target = 5
        result = binary_search(arr, target)
        self.assertEqual(result, -1)

    def test_single_element_found(self):
        arr = [5]
        target = 5
        result = binary_search(arr, target)
        self.assertEqual(result, 0)

    def test_single_element_not_found(self):
        arr = [5]
        target = 3
        result = binary_search(arr, target)
        self.assertEqual(result, -1)

    def test_first_element(self):
        arr = [1, 2, 3, 4, 5]
        target = 1
        result = binary_search(arr, target)
        self.assertEqual(result, 0)

    def test_last_element(self):
        arr = [1, 2, 3, 4, 5]
        target = 5
        result = binary_search(arr, target)
        self.assertEqual(result, len(arr) - 1)

if __name__ == '__main__':
    if os.environ.get('HTML'):
        runner = HtmlTestRunner.HTMLTestRunner(combine_reports=True, report_name="test_report")
        unittest.main(testRunner=runner)
    else:
        unittest.main()

