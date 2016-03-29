import unittest

from lib import testsetup


class TestWebi(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        testsetup.setup_docker(cls.asset)

        cls.db = testsetup.setup_db()
        cls.db.recreate()

        cls.browser = testsetup.setup_browser()
        cls.browser.start()

        cls.confd = testsetup.setup_confd()

    @classmethod
    def tearDownClass(cls):
        cls.browser.stop()
        testsetup.cleanup_docker(cls.asset)

    def setUp(self):
        self.addCleanup(self.confd.clear)
