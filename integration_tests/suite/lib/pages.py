import urllib

from functools import partial

from selenium.webdriver.support.ui import WebDriverWait, Select
from selenium.webdriver.support import expected_conditions as ec
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.alert import Alert
from selenium.webdriver.common.action_chains import ActionChains
from selenium.common.exceptions import NoSuchElementException

TIMEOUT = 4
CONFIG = {'base_url': 'http://localhost:8080'}


class Page(object):

    def __init__(self, driver):
        self.driver = driver

    def build_url(self, *parts, **kwargs):
        path = '/'.join(parts)
        url = "{}/{}".format(CONFIG['base_url'].rstrip('/'), path.lstrip('/'))
        if kwargs:
            url += "?{}".format(urllib.urlencode(kwargs))
        return url

    def wait_find(self, by, arg):
        condition = ec.presence_of_element_located((by, arg))
        WebDriverWait(self.driver, TIMEOUT).until(condition)

    def wait_visible(self, by, arg):
        condition = ec.visibility_of_element_located((by, arg))
        WebDriverWait(self.driver, TIMEOUT).until(condition)

    def fill(self, by, arg, value, root=None):
        root = root or self.driver
        element = root.find_element(by, arg)
        element.send_keys(value)

    def fill_name(self, name, value, root=None):
        self.fill(By.NAME, name, value, root)

    def fill_id(self, id_, value, root=None):
        self.fill(By.ID, id_, value, root)

    def select(self, by, arg, value, root=None):
        root = root or self.driver
        element = root.find_element(by, arg)
        Select(element).select_by_visible_text(value)

    def select_name(self, name, value, root=None):
        self.select(By.NAME, name, value, root)

    def select_id(self, id_, value, root=None):
        self.select(By.ID, id_, value, root)


class LoginPage(Page):

    PATH = "/index.php"

    def login(self, username, password, language='en'):
        url = self.build_url(self.PATH,
                             login=username,
                             password=password,
                             language=language,
                             go="/service/ipbx/index.php")
        self.driver.get(url)
        self.wait_find(By.ID, 'loginbox')


class UserListPage(Page):

    PATH = "/service/ipbx/index.php/pbx_settings/users/"
    LINE_XPATH = "//tr[td[contains(@title, '{name}')]]"

    def go(self):
        url = self.build_url(self.PATH)
        self.driver.get(url)
        self.wait_find(By.NAME, "fm-users-list")
        return self

    def add(self):
        url = self.build_url(self.PATH, act='add')
        self.driver.get(url)

        self.wait_find(By.ID, 'sr-users')
        return UserPage(self.driver)

    def edit(self, name):
        xpath = self.LINE_XPATH.format(name=name)
        line = self.driver.find_element_by_xpath(xpath)

        selector = "a[title='Edit']"
        button = line.find_element_by_css_selector(selector)
        button.click()

        self.wait_find(By.ID, 'sr-users')
        return UserPage(self.driver)

    def delete(self, name):
        xpath = self.LINE_XPATH.format(name=name)
        line = self.driver.find_element_by_xpath(xpath)

        selector = "a[title='Delete']"
        button = line.find_element_by_css_selector(selector)
        button.click()

        condition = ec.alert_is_present()
        WebDriverWait(self.driver, TIMEOUT).until(condition)

        Alert(self.driver).accept()

        condition = ec.presence_of_element_located((By.XPATH, xpath))
        WebDriverWait(self.driver, TIMEOUT).until_not(condition)


class UserPage(Page):

    def fill_form(self, **kwargs):
        for name, value in kwargs.iteritems():
            id_ = "it-userfeatures-{}".format(name)
            element = self.driver.find_element_by_id(id_)
            if element.tag_name == 'select':
                Select(element).select_by_visible_text(value)
            else:
                element.send_keys(value)

    def funckeys(self):
        link = self.driver.find_element_by_css_selector("a[href='#funckeys']")
        link.click()
        self.wait_visible(By.ID, 'sb-part-funckeys')

        return FuncKeyTab(self.driver)

    def save(self):
        btn = self.driver.find_element_by_id("it-submit")
        btn.click()
        self.wait_find(By.NAME, 'fm-users-list')


class FuncKeyTab(Page):

    KEY_XPATH = "//tr[td/select[@name='phonefunckey[fknum][]']/option[@selected='selected' and @value='{key}']]"

    AUTOCOMPLETE = ('User',
                    'Group',
                    'Queue',
                    'Conference room',
                    'Paging',
                    'Connect/Disconnect an agent',
                    'Connect an agent',
                    'Disconnect an agent')

    def get(self, keynum):
        xpath = self.KEY_XPATH.format(key=keynum)
        line = self.driver.find_element_by_xpath(xpath)

        key = line.find_element_by_name('phonefunckey[fknum][]')
        type = line.find_element_by_name('phonefunckey[type][]')
        label = line.find_element_by_name('phonefunckey[label][]')
        supervision = line.find_element_by_name('phonefunckey[supervision][]')

        key_value = int(key.get_attribute('value'))
        type_value = type.find_element_by_css_selector('option[selected="selected"]').text
        label_value = label.get_attribute('value')
        supervision_value = supervision.get_attribute('value') == 'Enabled'

        destination_value = self.get_destination_value(line)

        return {'key': key_value,
                'type': type_value,
                'label': label_value,
                'supervision': supervision_value,
                'destination': destination_value}

    def get_destination_value(self, line):
        try:
            destination = line.find_element_by_name('phonefunckey[typevalidentity][]')
        except NoSuchElementException:
            destination = line.find_element_by_name('phonefunckey[fkbsfilter][]')

        if destination.tag_name == 'select':
            return destination.find_element_by_css_selector('option[selected="selected"]').text

        return destination.get_attribute('value')

    def add(self, type, key=None, destination=None, label=None, supervision=None):
        line = self.add_line()
        self._fill(line, type, key, destination, label, supervision)

    def edit(self, key, type=None, destination=None, label=None, supervision=None):
        xpath = self.KEY_XPATH.format(key=key)
        line = self.driver.find_element_by_xpath(xpath)
        self._fill(line, type, key, destination, label, supervision)

    def _fill(self, line, type, key, destination, label, supervision):
        if type:
            self.select_name('phonefunckey[type][]', type, line)

        if key:
            self.select_name('phonefunckey[fknum][]', str(key), line)

        if label:
            line.find_element_by_name('phonefunckey[label][]').clear()
            self.fill_name('phonefunckey[label][]', label, line)

        if supervision is not None:
            value = "Enabled" if supervision else "Disabled"
            self.select_name('phonefunckey[supervision][]', value, line)

        if destination:
            self.fill_destination(line, type, destination)

    def add_line(self):
        total = self.count_lines()

        btn = self.driver.find_element_by_id('add_funckey_button')
        btn.click()

        waiter = WebDriverWait(self.driver, TIMEOUT)
        waiter.until(partial(self.check_count, total))

        return self.last_line()

    def count_lines(self):
        selector = "tbody#phonefunckey tr"
        try:
            return len(self.driver.find_elements_by_css_selector(selector))
        except NoSuchElementException:
            return 0

    def check_count(self, total, driver):
        return self.count_lines() > total

    def last_line(self):
        selector = 'tbody#phonefunckey tr:last-child'
        return self.driver.find_element_by_css_selector(selector)

    def fill_destination(self, line, fktype, destination):
        element = self.find_destination(line)
        element.clear()

        if element.tag_name == 'select':
            Select(element).select_by_visible_text(destination)
        elif fktype in self.AUTOCOMPLETE:
            self.select_autocomplete(line, element, destination)
        else:
            element.send_keys(destination)

    def find_destination(self, line):
        try:
            return line.find_element_by_name("phonefunckey[typevalidentity][]")
        except NoSuchElementException:
            return line.find_element_by_name("phonefunckey[fkbsfilter][]")

    def select_autocomplete(self, line, element, destination):
        selector = '.ui-autocomplete[style*="display: block"]'
        condition = ec.presence_of_element_located((By.CSS_SELECTOR, selector))

        # make sure there isn't any other autocomplete still lying around
        WebDriverWait(self.driver, TIMEOUT).until_not(condition)

        element.send_keys(destination)
        WebDriverWait(self.driver, TIMEOUT).until(condition)
        print "element found"

        ActionChains(self.driver).send_keys_to_element(element, Keys.DOWN, Keys.RETURN).perform()
        WebDriverWait(self.driver, TIMEOUT).until_not(condition)
