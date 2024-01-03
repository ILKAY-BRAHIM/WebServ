#!/usr/bin/env python
import cgi
import os

form = cgi.FieldStorage()


# Get the submitted values
name = form.getvalue('name')
email = form.getvalue('email')
message = form.getvalue('message')
pdf_upload = form['pdfUpload'] if 'pdfUpload' in form else None

# Create the HTML response
print("<html>")
print("<head>")
print("<title>Form Data</title>")

print("<style>")
print("   * {")
print("    margin: 0;")
print("    padding: 0;")
print("    box-sizing: border-box;")
print("    font-family: 'Courier New', Courier, monospace;")
print("    /* background-color: #f4f4f4; */")
print("   }")
print("   h2 {")
# print("    text-align: center;")
print("    margin: 1em;")
print("   }")
print("   body {")
print("    background-color: #d7d3d3;")
print("    display: flex;")
print("    flex-direction: column;")
print("    padding: 1rem 15%;")
print("   }")
print("   p {")
print("    margin: 0.5em;")
print("    word-wrap: break-word;")
print("   }")
print("  </style>")

print("</head>")
print("<body>")
print("<h2>Submitted Form Data:</h2>")
print("<p><strong>Name:</strong> " + str(name) + "</p>")
print("<p><strong>Email:</strong> " + str(email) + "</p>")
print("<p><strong>Message:</strong> " + str(message) + "</p>")

# Get the uploaded image
if 'imageUpload' in form:
    image_file = form['imageUpload']
    if image_file.filename:
        file_path = os.path.join('./ServerData/cgi/uploads', image_file.filename)
        with open(file_path, 'wb') as file:
            file.write(image_file.file.read())
        print("<h2>Image Uploaded Successfully</h2>")
        print("<div class='i'>")
        download_link = '<a href="/cgi/uploads/' + image_file.filename + '" download="' + image_file.filename + '">' + image_file.filename + '</a>'
        print('<p>Uploaded Image:' + download_link + '</p>')
        print('<img src="/cgi/uploads/' + image_file.filename + '" alt="Uploaded Image" " width="200rem" height="200rem">')
        print("</div>")
    else:
        print("<h2>No image selected.</h2>")

else:
    print("<h2>Image upload failed.</h2>")

# Get the uploaded PDF file
if 'pdfUpload' in form:
    pdf_file = form['pdfUpload']
    if pdf_file.filename:
        file_path = os.path.join('./ServerData/cgi/uploads', pdf_file.filename)
        with open(file_path, 'wb') as file:
            file.write(pdf_file.file.read())
        print("<h2>PDF Uploaded Successfully</h2>")
        # Provide a download link for the uploaded PDF
        download_link = '<a href="/cgi/uploads/' + pdf_file.filename + '" download="' + pdf_file.filename + '">' + pdf_file.filename + '</a>'
        print('<p>Uploaded PDF: ' + download_link + '</p>')
        print('<embed src="/cgi/uploads/' + pdf_file.filename + '" width="500px" height="600px" />')
    else:
        print("<h2>No PDF selected.</h2>")
else:
    print("<h2>PDF upload failed.</h2>")

print("</body></html>")