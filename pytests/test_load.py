import pytest
import requests
import time


def test_post_request():
    start_time = time.perf_counter()

    for i in range(8):
        resp = requests.get("http://localhost:8082/test_dont_touch.html")
        assert resp.status_code == 200
        assert "HTML FOR TEST" in resp.text

    end_time = time.perf_counter()
    execution_time = end_time - start_time
