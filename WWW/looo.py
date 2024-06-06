import time

print("Content-Type: text/html\n")
print("<html><body>")
print("<h1>Testing Timeout</h1>")
print("<p>This script will sleep for 10 seconds.</p>")
time.sleep(5000)  # Sleep for 10 seconds
print("<p>If you see this message, the timeout was not enforced properly.</p>")
print("</body></html>")
