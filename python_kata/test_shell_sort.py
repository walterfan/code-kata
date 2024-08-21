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

logger.add(sys.stdout,
           format="{time} {message}",
           filter="client",
           level="DEBUG")

def shell_sort(arr):
    n = len(arr)
    gap = n // 2

    while gap > 0:
        for i in range(gap, n):
            temp = arr[i]
            j = i
            while j >= gap and arr[j - gap] > temp:
                arr[j] = arr[j - gap]
                j -= gap
            arr[j] = temp
        gap //= 2

    return arr


class ShellSortTest(unittest.TestCase):

    def setUp(self):
        logger.debug("# --- {} begin ---", self.id())

    def tearDown(self):
        logger.debug("# --- {} end ---", self.id())

    def test_sorted_array(self):
        arr = [1, 2, 3, 4, 5]
        sorted_arr = shell_sort(arr.copy())
        self.assertEqual(sorted_arr, sorted(arr))

    def test_reverse_sorted_array(self):
        arr = [5, 4, 3, 2, 1]
        sorted_arr = shell_sort(arr.copy())
        self.assertEqual(sorted_arr, sorted(arr))

    def test_unsorted_array(self):
        arr = [64, 34, 25, 12, 22, 11, 90]
        sorted_arr = shell_sort(arr.copy())
        self.assertEqual(sorted_arr, sorted(arr))

    def test_array_with_duplicates(self):
        arr = [5, 3, 8, 5, 2, 9, 1, 5]
        sorted_arr = shell_sort(arr.copy())
        self.assertEqual(sorted_arr, sorted(arr))

    def test_single_element_array(self):
        arr = [1]
        sorted_arr = shell_sort(arr.copy())
        self.assertEqual(sorted_arr, arr)

    def test_empty_array(self):
        arr = []
        sorted_arr = shell_sort(arr.copy())
        self.assertEqual(sorted_arr, arr)

if __name__ == '__main__':
    if os.environ.get('HTML'):
        runner = HtmlTestRunner.HTMLTestRunner(combine_reports=True, report_name="test_report")
        unittest.main(testRunner=runner)
    else:
        unittest.main()

