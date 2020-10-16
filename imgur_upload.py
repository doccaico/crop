#!/usr/bin/env python3

import json
import os
import sys
import webbrowser

import requests

CLIENT_ID_PATH = os.path.expanduser("~/.imgur")

HELP = """
Prerequirement
\t1. save your Client ID in ~/.imgur
\t2. (install) pip install requests

Usage
\tpython3 imgur.py ~/image.png"""


if __name__ == "__main__":

    if len(sys.argv) != 2:
        print(HELP)
        sys.exit(1)

    image_path = sys.argv[1]

    with open(CLIENT_ID_PATH) as f:
        client_id = f.read().strip()

    with open(image_path, mode="rb") as image:
        headers = {"authorization": f"Client-ID {client_id}"}
        files = {"image": image}
        r = requests.post(
            "https://api.imgur.com/3/upload", headers=headers, files=files
        )

    url = json.loads(r.text)["data"]["link"]
    webbrowser.open_new_tab(url)
