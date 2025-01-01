#! python3
# =========================================================================
#   Unity - A Test Framework for C
#   ThrowTheSwitch.org
#   Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
#   SPDX-License-Identifier: MIT
# =========================================================================

import sys
import os
from glob import glob
import argparse

from pyparsing import *
from junit_xml import TestSuite, TestCase


class UnityTestSummary:
    def __init__(self):
        self.report = ''
        self.total_tests = 0
        self.failures = 0
        self.ignored = 0
        self.targets = 0
        self.root = None
        self.output = None
        self.test_suites = dict()

    def run(self):
        # Clean up result file names
        results = []
        for target in self.targets:
            results.append(target.replace('\\', '/'))

        # Dig through each result file, looking for details on pass/fail:
        for result_file in results:
            lines = list(map(lambda line: line.rstrip(), open(result_file, "r").read().split('\n')))
            if len(lines) == 0:
                raise Exception("Empty test result file: %s" % result_file)

            # define an expression for your file reference
            entry_one = Combine(
                oneOf(list(alphas)) + ':/' +
                Word(alphanums + '_-./'))

            entry_two = Word(printables + ' ', excludeChars=':')
            entry = entry_one | entry_two

            delimiter = Literal(':').suppress()
            # Format of a result line is `[file_name]:line:test_name:RESULT[:msg]`
            tc_result_line = Group(ZeroOrMore(entry.setResultsName('tc_file_name'))
                + delimiter + entry.setResultsName('tc_line_nr')
                + delimiter + entry.setResultsName('tc_name')
                + delimiter + entry.setResultsName('tc_status') +
                Optional(delimiter + entry.setResultsName('tc_msg'))).setResultsName("tc_line")

            eol = LineEnd().suppress()
            sol = LineStart().suppress()
            blank_line = sol + eol

            # Format of the summary line is `# Tests # Failures # Ignored`
            tc_summary_line = Group(Word(nums).setResultsName("num_of_tests") + "Tests" + Word(nums).setResultsName(
                "num_of_fail") + "Failures" + Word(nums).setResultsName("num_of_ignore") + "Ignored").setResultsName(
                "tc_summary")
            tc_end_line = Or(Literal("FAIL"), Literal('Ok')).setResultsName("tc_result")

            # run it and see...
            pp1 = tc_result_line | Optional(tc_summary_line | tc_end_line)
            pp1.ignore(blank_line | OneOrMore("-"))

            result = list()
            for l in lines:
                result.append((pp1.parseString(l)).asDict())
            # delete empty results
            result = filter(None, result)

            tc_list = list()
            for r in result:
                if 'tc_line' in r:
                    tmp_tc_line = r['tc_line']

                    # get only the file name which will be used as the classname
                    if 'tc_file_name' in tmp_tc_line:
                        file_name = tmp_tc_line['tc_file_name'].split('\\').pop().split('/').pop().rsplit('.', 1)[0]
                    else:
                        file_name = result_file.strip("./")
                    tmp_tc = TestCase(name=tmp_tc_line['tc_name'], classname=file_name)
                    if 'tc_status' in tmp_tc_line:
                        if str(tmp_tc_line['tc_status']) == 'IGNORE':
                            if 'tc_msg' in tmp_tc_line:
                                tmp_tc.add_skipped_info(message=tmp_tc_line['tc_msg'],
                                                        output=r'[File]={0}, [Line]={1}'.format(
                                                            tmp_tc_line['tc_file_name'], tmp_tc_line['tc_line_nr']))
                            else:
                                tmp_tc.add_skipped_info(message=" ")
                        elif str(tmp_tc_line['tc_status']) == 'FAIL':
                            if 'tc_msg' in tmp_tc_line:
                                tmp_tc.add_failure_info(message=tmp_tc_line['tc_msg'],
                                                        output=r'[File]={0}, [Line]={1}'.format(
                                                            tmp_tc_line['tc_file_name'], tmp_tc_line['tc_line_nr']))
                            else:
                                tmp_tc.add_failure_info(message=" ")

                    tc_list.append((str(result_file), tmp_tc))

            for k, v in tc_list:
                try:
                    self.test_suites[k].append(v)
                except KeyError:
                    self.test_suites[k] = [v]
        ts = []
        for suite_name in self.test_suites:
            ts.append(TestSuite(suite_name, self.test_suites[suite_name]))

        with open(self.output, 'w') as f:
            TestSuite.to_file(f, ts, prettyprint='True', encoding='utf-8')

        return self.report

    def set_targets(self, target_array):
        self.targets = target_array

    def set_root_path(self, path):
        self.root = path

    def set_output(self, output):
        self.output = output


if __name__ == '__main__':
    uts = UnityTestSummary()
    parser = argparse.ArgumentParser(description=
        """Takes as input the collection of *.testpass and *.testfail result
        files, and converts them to a JUnit formatted XML.""")
    parser.add_argument('targets_dir', metavar='result_file_directory',
                        type=str, nargs='?', default='./',
                        help="""The location of your results files.
                        Defaults to current directory if not specified.""")
    parser.add_argument('root_path', nargs='?',
                        default='os.path.split(__file__)[0]',
                        help="""Helpful for producing more verbose output if
                        using relative paths.""")
    parser.add_argument('--output', '-o', type=str, default="result.xml",
                        help="""The name of the JUnit-formatted file (XML).""")
    args = parser.parse_args()

    if args.targets_dir[-1] != '/':
        args.targets_dir+='/'
    targets = list(map(lambda x: x.replace('\\', '/'), glob(args.targets_dir + '*.test*')))
    if len(targets) == 0:
        raise Exception("No *.testpass or *.testfail files found in '%s'" % args.targets_dir)
    uts.set_targets(targets)

    # set the root path
    uts.set_root_path(args.root_path)

    # set output
    uts.set_output(args.output)

    # run the summarizer
    print(uts.run())
