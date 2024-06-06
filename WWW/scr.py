import os

print("Content-Type: text/html\n")
print("<html><body>")
print("<h1>Hello from Python CGI</h1>")

# Print environment variables
print("<h2>Environment Variables</h2>")
print("<pre>")
for key, value in os.environ.items():
    print(f"{key}: {value}")
print("</pre>")

print("</body></html>")