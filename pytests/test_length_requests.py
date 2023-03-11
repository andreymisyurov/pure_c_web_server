import pytest
import requests


def test_zero_get_request():
    resp = requests.get("http://localhost:8082/")
    assert resp.status_code == 404
    assert "404" in resp.text


def test_4347alphas_get_request():
    test_string = "http://localhost:8082/"
    count = 1080
    while count:
        test_string += "long"
        count -= 1
    test_string += ".html"

    resp = requests.get(test_string)
    assert resp.status_code == 404
    assert "404" in resp.text
