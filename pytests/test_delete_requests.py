import pytest
import requests
from pathlib import Path


def test_success_delete():
    fle = Path('./src/templates/data.html')
    fle.touch(exist_ok=True)
    resp = requests.delete("http://localhost:8082/data.html")
    assert resp.status_code == 200
    assert "SUCCESS" in resp.text


def test_404():
    resp = requests.delete("http://localhost:8082/notfound.html")
    assert resp.status_code == 404
    assert "FAIL" in resp.text
