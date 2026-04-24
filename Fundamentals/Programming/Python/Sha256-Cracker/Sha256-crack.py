from pwn import *
import sys
import hashlib

def sha256sumhex(data):
	return hashlib.sha256(data).hexdigest()

def mutate(password):
	return [
		password,
		password.lower(),
		password.upper(),
		password.capitalize(),
		password + b"123",
		password + b"1",
		password + b"!",
		password + b"@",
		password + b"2024",
		password + b"2025",
		b"123" + password,
		password.replace(b"a", b"@"),
		password.replace(b"o", b"0"),
	]

if len(sys.argv) != 2:
	print("Invalid arguments!")
	print(">> {} <sha256sum>".format(sys.argv[0]))
	exit()

wanted_hash = sys.argv[1].lower()
print(wanted_hash)

password_file = "xato-net-10-million-passwords-100000.txt"
attempts = 0

p = log.progress("Attempting to crack {}".format(wanted_hash))

with open(password_file, "r", encoding='latin-1') as password_cato:
	for password in password_cato:
		password = password.strip().encode('latin-1')

		for variant in mutate(password):
			password_hash = sha256sumhex(variant)

			if attempts % 2000 == 0:
				p.status("[{}] Trying...".format(attempts))

			if password_hash == wanted_hash:
				p.success("Password found after {} attempts! '{}' hashes to {}"
				          .format(attempts, variant.decode('latin-1'), password_hash))
				exit()

			attempts += 1

p.failure("Password hash not found!")