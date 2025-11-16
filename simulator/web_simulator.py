#!/usr/bin/env python3
"""
ILI9225 LCD Web Simulator
Web-based simulator using Flask for GitHub Codespaces compatibility
"""

from flask import Flask, render_template, jsonify, request
from flask_socketio import SocketIO, emit
import json
import base64
from io import BytesIO
from PIL import Image, ImageDraw, ImageFont
import threading
import time
import socket
import select

app = Flask(__name__)
app.config['SECRET_KEY'] = 'ili9225-simulator-secret'
socketio = SocketIO(app, cors_allowed_origins="*")

class DisplayConfig:
    """ILI9225 Display Configuration"""
    WIDTH = 176
    HEIGHT = 220

class Color:
    """RGB565 to RGB888 color conversion"""
    @staticmethod
    def rgb565_to_rgb888(rgb565):
        """Convert 16-bit RGB565 to 24-bit RGB888"""
        r = ((rgb565 >> 11) & 0x1F) << 3
        g = ((rgb565 >> 5) & 0x3F) << 2
        b = (rgb565 & 0x1F) << 3
        # Expand to full 8-bit range
        r = r | (r >> 5)
        g = g | (g >> 6)
        b = b | (b >> 5)
        return (r, g, b)
    
    # Common colors in RGB565
    BLACK   = 0x0000
    WHITE   = 0xFFFF
    RED     = 0xF800
    GREEN   = 0x07E0
    BLUE    = 0x001F
    YELLOW  = 0xFFE0
    CYAN    = 0x07FF
    MAGENTA = 0xF81F

class WebSimulator:
    def __init__(self, width=DisplayConfig.WIDTH, height=DisplayConfig.HEIGHT):
        """Initialize the LCD simulator"""
        self.width = width
        self.height = height
        self.image = Image.new('RGB', (width, height), (0, 0, 0))
        self.draw = ImageDraw.Draw(self.image)
        self.draw_count = 0
        self.command_queue = []
        
        # Try to load a font for text rendering
        try:
            self.font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 8)
        except:
            self.font = ImageFont.load_default()
    
    def draw_pixel(self, x, y, color):
        """Draw a single pixel"""
        if 0 <= x < self.width and 0 <= y < self.height:
            rgb = Color.rgb565_to_rgb888(color)
            self.image.putpixel((x, y), rgb)
            self.draw_count += 1
    
    def draw_line(self, x0, y0, x1, y1, color):
        """Draw a line"""
        rgb = Color.rgb565_to_rgb888(color)
        self.draw.line([(x0, y0), (x1, y1)], fill=rgb, width=1)
        self.draw_count += 1
    
    def draw_rect(self, x, y, w, h, color):
        """Draw a rectangle outline"""
        rgb = Color.rgb565_to_rgb888(color)
        self.draw.rectangle([(x, y), (x + w, y + h)], outline=rgb)
        self.draw_count += 1
    
    def fill_rect(self, x, y, w, h, color):
        """Draw a filled rectangle"""
        rgb = Color.rgb565_to_rgb888(color)
        self.draw.rectangle([(x, y), (x + w, y + h)], fill=rgb)
        self.draw_count += 1
    
    def draw_circle(self, x, y, r, color):
        """Draw a circle outline"""
        rgb = Color.rgb565_to_rgb888(color)
        bbox = [(x - r, y - r), (x + r, y + r)]
        self.draw.ellipse(bbox, outline=rgb)
        self.draw_count += 1
    
    def fill_circle(self, x, y, r, color):
        """Draw a filled circle"""
        rgb = Color.rgb565_to_rgb888(color)
        bbox = [(x - r, y - r), (x + r, y + r)]
        self.draw.ellipse(bbox, fill=rgb)
        self.draw_count += 1
    
    def draw_text(self, x, y, text, color, size=1):
        """Draw text"""
        rgb = Color.rgb565_to_rgb888(color)
        # Scale font if size > 1
        if size > 1:
            try:
                font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 8 * size)
            except:
                font = self.font
        else:
            font = self.font
        self.draw.text((x, y), text, fill=rgb, font=font)
        self.draw_count += 1
    
    def clear(self, color=Color.BLACK):
        """Clear the screen"""
        rgb = Color.rgb565_to_rgb888(color)
        self.draw.rectangle([(0, 0), (self.width, self.height)], fill=rgb)
        self.draw_count = 0
    
    def handle_command(self, cmd):
        """Handle a drawing command from dict or JSON string"""
        # Accept both dict and JSON string
        if isinstance(cmd, str):
            import json
            cmd = json.loads(cmd)
        
        cmd_type = cmd.get("type")
        
        if cmd_type == "pixel":
            self.draw_pixel(cmd["x"], cmd["y"], cmd["color"])
        elif cmd_type == "line":
            self.draw_line(cmd["x0"], cmd["y0"], cmd["x1"], cmd["y1"], cmd["color"])
        elif cmd_type == "rect":
            self.draw_rect(cmd["x"], cmd["y"], cmd["w"], cmd["h"], cmd["color"])
        elif cmd_type == "fill_rect":
            self.fill_rect(cmd["x"], cmd["y"], cmd["w"], cmd["h"], cmd["color"])
        elif cmd_type == "circle":
            self.draw_circle(cmd["x"], cmd["y"], cmd["r"], cmd["color"])
        elif cmd_type == "fill_circle":
            self.fill_circle(cmd["x"], cmd["y"], cmd["r"], cmd["color"])
        elif cmd_type == "text":
            self.draw_text(cmd["x"], cmd["y"], cmd["text"], cmd["color"], cmd.get("size", 1))
        elif cmd_type == "clear":
            self.clear(cmd.get("color", Color.BLACK))
        elif cmd_type == "framebuffer":
            self.update_from_framebuffer(cmd["pixels"], cmd.get("width", self.width), cmd.get("height", self.height))
        
        # Notify connected clients
        self.broadcast_update()

    def update_from_framebuffer(self, encoded_pixels, width, height):
        """Replace display content from a base64 encoded RGB565 framebuffer"""
        try:
            raw = base64.b64decode(encoded_pixels)
        except Exception as exc:
            print(f"Failed to decode framebuffer: {exc}")
            return

        expected_size = width * height * 2
        if len(raw) != expected_size:
            print(f"Invalid framebuffer size: expected {expected_size}, got {len(raw)}")
            return

        new_image = Image.new('RGB', (width, height))
        pixels = new_image.load()

        # ILI9225 in portrait mode (0x0018) uses origin at bottom-right
        # and increments vertically then horizontally (bit 3 AM=1, bit 4 ID1=1)
        # The framebuffer from C code is in row-major order (y, x)
        # but the display coordinates need to be flipped vertically
        idx = 0
        for y in range(height):
            for x in range(width):
                value = (raw[idx] << 8) | raw[idx + 1]
                idx += 2
                # Flip vertically: height - 1 - y
                pixels[x, height - 1 - y] = Color.rgb565_to_rgb888(value)

        self.image = new_image
        self.draw = ImageDraw.Draw(self.image)
        self.draw_count += 1
    
    def get_image_data(self):
        """Get image as base64 encoded PNG"""
        buffer = BytesIO()
        self.image.save(buffer, format='PNG')
        buffer.seek(0)
        img_str = base64.b64encode(buffer.getvalue()).decode()
        return f"data:image/png;base64,{img_str}"
    
    def broadcast_update(self):
        """Broadcast display update to all connected clients"""
        socketio.emit('display_update', {
            'image': self.get_image_data(),
            'draw_count': self.draw_count
        })

# Global simulator instance
simulator = WebSimulator()

# Flask routes
@app.route('/')
def index():
    """Main page"""
    return render_template('index.html', width=DisplayConfig.WIDTH, height=DisplayConfig.HEIGHT)

@app.route('/api/command', methods=['POST'])
def api_command():
    """Handle drawing command via REST API"""
    cmd = request.json
    simulator.handle_command(cmd)
    return jsonify({'status': 'ok', 'draw_count': simulator.draw_count})

@app.route('/api/image')
def api_image():
    """Get current display image"""
    return jsonify({
        'image': simulator.get_image_data(),
        'draw_count': simulator.draw_count
    })

@app.route('/api/clear', methods=['POST'])
def api_clear():
    """Clear the display"""
    color = request.json.get('color', Color.BLACK) if request.json else Color.BLACK
    simulator.clear(color)
    return jsonify({'status': 'ok'})

# SocketIO events
@socketio.on('connect')
def handle_connect():
    """Handle client connection"""
    print('Client connected')
    emit('display_update', {
        'image': simulator.get_image_data(),
        'draw_count': simulator.draw_count
    })

@socketio.on('command')
def handle_command(data):
    """Handle drawing command via WebSocket"""
    simulator.handle_command(data)

@socketio.on('clear')
def handle_clear(data):
    """Handle clear command"""
    color = data.get('color', Color.BLACK)
    simulator.clear(color)

def tcp_server_thread(port=9225):
    """TCP server for C code connections"""
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind(('0.0.0.0', port))
    server_socket.listen(5)
    server_socket.setblocking(0)
    
    print(f"TCP server listening on port {port} for C code connections")
    
    clients = []
    client_buffers = {}
    
    while True:
        # Accept new connections
        readable, _, _ = select.select([server_socket] + clients, [], [], 0.1)
        
        for sock in readable:
            if sock is server_socket:
                # New connection
                client_socket, addr = server_socket.accept()
                client_socket.setblocking(0)
                clients.append(client_socket)
                client_buffers[client_socket] = ""
                print(f"C client connected from {addr}")
            else:
                # Data from existing client
                try:
                    data = sock.recv(4096)
                    if data:
                        try:
                            text = data.decode('utf-8')
                        except UnicodeDecodeError as exc:
                            print(f"Invalid UTF-8 from client: {exc}")
                            continue

                        buffer = client_buffers.get(sock, "") + text
                        while '\n' in buffer:
                            line, buffer = buffer.split('\n', 1)
                            if line.strip():
                                try:
                                    cmd = json.loads(line)
                                    simulator.handle_command(cmd)
                                except json.JSONDecodeError as e:
                                    print(f"Invalid JSON: {line[:50]}... Error: {e}")
                        client_buffers[sock] = buffer
                    else:
                        # Client disconnected
                        print("C client disconnected")
                        clients.remove(sock)
                        client_buffers.pop(sock, None)
                        sock.close()
                except (ConnectionResetError, BrokenPipeError):
                    print("C client disconnected (error)")
                    clients.remove(sock)
                    client_buffers.pop(sock, None)
                    sock.close()
                except Exception as e:
                    print(f"Error handling C client: {e}")


def run_demo():
    """Run demo animation"""
    time.sleep(2)
    
    # Clear to white
    simulator.clear(Color.WHITE)
    time.sleep(0.5)
    
    # Draw pixels
    for i in range(0, 176, 10):
        simulator.draw_pixel(i, 10, Color.RED)
        time.sleep(0.1)
    
    # Draw lines
    simulator.draw_line(10, 30, 166, 30, Color.GREEN)
    time.sleep(0.3)
    simulator.draw_line(10, 40, 166, 50, Color.BLUE)
    time.sleep(0.3)
    
    # Draw rectangles
    simulator.draw_rect(20, 60, 40, 30, Color.RED)
    time.sleep(0.3)
    simulator.fill_rect(70, 60, 40, 30, Color.GREEN)
    time.sleep(0.3)
    
    # Draw circles
    simulator.draw_circle(40, 130, 20, Color.BLUE)
    time.sleep(0.3)
    simulator.fill_circle(100, 130, 20, Color.MAGENTA)
    time.sleep(0.3)
    
    # Draw text
    simulator.draw_text(10, 170, "ILI9225 Simulator", Color.BLACK, 1)
    simulator.draw_text(10, 185, "Web Version", Color.BLACK, 1)
    simulator.draw_text(10, 200, "GitHub Codespaces", Color.BLACK, 1)

def main():
    """Main entry point"""
    import argparse
    
    parser = argparse.ArgumentParser(description="ILI9225 Web LCD Simulator")
    parser.add_argument('--demo', action='store_true', help='Run demo mode')
    parser.add_argument('--port', type=int, default=5000, help='Web server port (default: 5000)')
    parser.add_argument('--tcp-port', type=int, default=9225, help='TCP port for C clients (default: 9225)')
    parser.add_argument('--host', default='0.0.0.0', help='Host address (default: 0.0.0.0)')
    args = parser.parse_args()
    
    # Start TCP server thread for C code connections
    tcp_thread = threading.Thread(target=tcp_server_thread, args=(args.tcp_port,), daemon=True)
    tcp_thread.start()
    
    if args.demo:
        # Start demo in background thread
        demo_thread = threading.Thread(target=run_demo, daemon=True)
        demo_thread.start()
    
    print("\n" + "="*60)
    print("ILI9225 LCD Web Simulator")
    print("="*60)
    print(f"Web UI: http://{args.host}:{args.port}")
    print(f"TCP Server for C code: port {args.tcp_port}")
    print("Open the Web UI in your browser to see the display")
    print("Connect your C code to TCP port to test functions")
    print("="*60 + "\n")
    
    socketio.run(app, host=args.host, port=args.port, debug=False, allow_unsafe_werkzeug=True)

if __name__ == "__main__":
    main()
