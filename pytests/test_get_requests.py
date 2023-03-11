import pytest
import requests


def test_test_dont_touch():
    resp = requests.get("http://localhost:8082/test_dont_touch.html")
    assert resp.status_code == 200
    assert "HTML FOR TEST" in resp.text


def test_hello_world():
    resp = requests.get("http://localhost:8082/hello.html")
    assert resp.status_code == 200
    assert "HELLO WORLD!" in resp.text


def test_404():
    resp = requests.get("http://localhost:8082/notfound")
    assert resp.status_code == 404
    assert "404 Page Not Found" in resp.text
