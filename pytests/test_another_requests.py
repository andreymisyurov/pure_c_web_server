import pytest
import requests


def test_post_request():
    resp = requests.get("http://localhost:8082/test_dont_touch.html")
    assert resp.status_code == 200
    assert "HTML FOR TEST" in resp.text


def test_put_request():
    resp = requests.put("http://localhost:8082/test_dont_touch.html")
    assert resp.status_code == 200
    assert "HTML FOR TEST" in resp.text
