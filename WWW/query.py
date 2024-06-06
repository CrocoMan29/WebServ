#!/usr/bin/python3

import os
import cgi

print("Content-Type: text/html\n")
print("<html><body>")
print("<h1>Hello from Python CGI</h1>")

# Get the query string from the environment
query_string = os.environ.get("QUERY_STRING", "")

# Parse the query string
params = cgi.parse_qs(query_string)

print("<h2>Query String</h2>")
print(f"<p>Raw Query String: {query_string}</p>")
print("<h3>Parsed Parameters</h3>")
print("<ul>")
for key, values in params.items():
    for value in values:
        print(f"<li>{key}: {value}</li>")
print("</ul>")

print("</body></html>")
