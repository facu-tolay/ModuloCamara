from http.server import BaseHTTPRequestHandler, HTTPServer

class ImageHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        # content_length = self.headers['Content-Length']
        post_data = self.rfile.read()
        with open("received_image.jpg", "wb") as f:
            f.write(post_data)
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b'Image received successfully')

def run(server_class=HTTPServer, handler_class=ImageHandler, port=5000):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == '__main__':
    run()
