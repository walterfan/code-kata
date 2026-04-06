#!/usr/bin/env python3
import unittest
import xmlrunner
import datetime
import os
import subprocess
def generate_unique_filename():
    now = datetime.datetime.now()
    timestamp = now.strftime('%Y-%m-%d_%H-%M-%S')
    report_dir = 'reports'
    os.makedirs(report_dir, exist_ok=True)
    return os.path.join(report_dir, f'test-results_{timestamp}.xml')

def main():

    suite = unittest.defaultTestLoader.discover('./test', pattern='test*.py')

    report_file = generate_unique_filename()

    with open(report_file, 'wb') as output:
        runner = xmlrunner.XMLTestRunner(output=output)
        result = runner.run(suite)


    html_report_file = report_file.replace('.xml', '.html')
    subprocess.run(['junit2html', report_file, html_report_file])

    print(f'Test report generated: {html_report_file}')

    if not result.wasSuccessful():
        print("Some tests failed.")
        exit(1)
    else:
        print("All tests passed.")

if __name__ == '__main__':
    main()
