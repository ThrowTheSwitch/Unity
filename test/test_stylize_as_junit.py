#!/usr/bin/env python3
# Copyright (c) 2026 Unity Project
# SPDX-License-Identifier: MIT

"""Regression tests for the Unity-to-JUnit converter."""

import importlib.util
from pathlib import Path
import tempfile
import unittest
import xml.etree.ElementTree as ET


SCRIPT = Path(__file__).resolve().parents[1] / 'auto' / 'stylize_as_junit.py'
SPEC = importlib.util.spec_from_file_location('stylize_as_junit', SCRIPT)
CONVERTER = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(CONVERTER)


class WindowsPathsTest(unittest.TestCase):
    def test_paths_preserve_failure_details(self):
        paths = [
            r'C:\Path\To\My File\test_example.c',
            'C:/Path/To/My File/test_example.c',
            'D:/path/test_example.c',
            '/tmp/test_example.c',
            'test_example.c',
        ]
        for path in paths:
            with self.subTest(path=path), tempfile.TemporaryDirectory() as tmp:
                result = Path(tmp) / 'example.testfail'
                output = Path(tmp) / 'result.xml'
                result.write_text(f'{path}:24:test_example:FAIL:expected 2 was 1\n')
                summary = CONVERTER.UnityTestSummary()
                summary.set_targets([str(result)])
                summary.set_output(str(output))
                summary.run()
                suite = ET.parse(output).getroot()
                case = suite.find('.//testcase')
                self.assertEqual(case.attrib['name'], 'test_example')
                self.assertEqual(case.attrib['classname'], 'test_example')
                failure = case.find('failure')
                self.assertIsNotNone(failure)
                self.assertEqual(failure.attrib['message'], 'expected 2 was 1')
                self.assertIn('[File]=' + path + ', [Line]=24', failure.text)

    def test_windows_pass_and_ignore(self):
        with tempfile.TemporaryDirectory() as tmp:
            result = Path(tmp) / 'example.testpass'
            output = Path(tmp) / 'result.xml'
            result.write_text(
                'C:/My Tests/test_example.c:12:test_pass:PASS\n'
                'C:\\My Tests\\test_example.c:13:test_ignore:IGNORE:not ready\n'
            )
            summary = CONVERTER.UnityTestSummary()
            summary.set_targets([str(result)])
            summary.set_output(str(output))
            summary.run()
            cases = ET.parse(output).getroot().findall('.//testcase')
            self.assertEqual([case.attrib['name'] for case in cases],
                             ['test_pass', 'test_ignore'])
            self.assertTrue(all(case.attrib['classname'] == 'test_example'
                                for case in cases))
            self.assertIsNone(cases[0].find('failure'))
            self.assertEqual(cases[1].find('skipped').attrib['message'],
                             'not ready')


if __name__ == '__main__':
    unittest.main()
