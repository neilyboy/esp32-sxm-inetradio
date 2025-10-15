# m3u8XM Server Setup Guide

This guide explains how to run the m3u8XM SiriusXM streaming server on a Raspberry Pi or home server to provide stream URLs to your ESP32 device.

## Architecture Overview

```
┌─────────────────┐         ┌──────────────────┐         ┌─────────────┐
│   SiriusXM      │◄────────┤  Raspberry Pi    │◄────────┤   ESP32     │
│   Servers       │  HTTPS  │  (m3u8XM Server) │  HTTP   │   Device    │
└─────────────────┘         └──────────────────┘         └─────────────┘
                                     │
                                     │ WiFi
                                     ▼
                            ┌──────────────────┐
                            │  Phone Hotspot   │
                            └──────────────────┘
```

**Why this approach?**
- ✅ ESP32 cannot run Python natively
- ✅ Leverage existing m3u8XM Python code
- ✅ Server handles complex SiriusXM API authentication
- ✅ ESP32 just requests stream URLs via simple HTTP

---

## Prerequisites

### Hardware
- **Raspberry Pi** (3B+, 4, or Zero 2W)
  - Or any Linux server (Ubuntu, Debian, etc.)
  - Or even a PC/Mac left running
- **Network connection** (WiFi or Ethernet)
- **SD card** with Raspberry Pi OS (if using Pi)

### Software
- **Python 3.7+**
- **Git**
- **pip** (Python package manager)

### Accounts
- **Active SiriusXM subscription**

---

## Installation Steps

### Step 1: Set Up Raspberry Pi (if needed)

If using Raspberry Pi, install Raspberry Pi OS:
```bash
# Download Raspberry Pi Imager
# https://www.raspberrypi.com/software/

# Flash SD card with Raspberry Pi OS Lite (64-bit)
# Enable SSH in imager settings
# Set WiFi credentials in imager settings
```

Boot Pi and SSH in:
```bash
ssh pi@raspberrypi.local
# Default password: raspberry (change it!)
```

Update system:
```bash
sudo apt update && sudo apt upgrade -y
```

### Step 2: Install Dependencies

```bash
# Install Python and tools
sudo apt install -y python3 python3-pip git

# Install FFmpeg (for audio processing)
sudo apt install -y ffmpeg
```

### Step 3: Clone m3u8XM Repository

```bash
cd ~
git clone https://github.com/myselfondiscord/m3u8XM.git
cd m3u8XM
```

### Step 4: Install Python Requirements

```bash
pip3 install -r requirements.txt
```

**If `requirements.txt` doesn't exist or has issues, install manually:**
```bash
pip3 install requests flask
```

### Step 5: Configure SiriusXM Credentials

```bash
# Copy example config
cp config.example.ini config.ini

# Edit config file
nano config.ini
```

**Add your SiriusXM credentials:**
```ini
[SiriusXM]
email = your-email@example.com
password = your-password
```

Save and exit (Ctrl+X, Y, Enter)

### Step 6: Create API Server Wrapper

The m3u8XM project generates playlists, but we need an API server. Create a Flask wrapper:

```bash
nano sxm_server.py
```

**Add this code:**
```python
#!/usr/bin/env python3
"""
SiriusXM API Server for ESP32
Wraps m3u8XM functionality as a REST API
"""

from flask import Flask, jsonify, request
import subprocess
import json
import os
import configparser

app = Flask(__name__)

# Load SXM credentials from config
config = configparser.ConfigParser()
config.read('config.ini')

SXM_EMAIL = config['SiriusXM']['email']
SXM_PASSWORD = config['SiriusXM']['password']

# Store authenticated state
authenticated = False
channels_cache = []

def authenticate():
    """Authenticate with SiriusXM"""
    global authenticated
    # Authentication handled by sxm.py
    authenticated = True
    return True

def get_channels():
    """Get list of available channels"""
    try:
        # Run sxm.py with -l flag to list channels
        result = subprocess.run(
            ['python3', 'sxm.py', '-l'],
            capture_output=True,
            text=True,
            timeout=30
        )
        
        channels = []
        for line in result.stdout.split('\n'):
            if line.strip() and not line.startswith('#'):
                # Parse channel info (format depends on sxm.py output)
                parts = line.split('|')
                if len(parts) >= 3:
                    channels.append({
                        'id': parts[0].strip(),
                        'number': parts[0].strip(),
                        'name': parts[1].strip(),
                        'genre': parts[2].strip() if len(parts) > 2 else 'Unknown'
                    })
        
        return channels
    except Exception as e:
        print(f"Error getting channels: {e}")
        return []

def get_stream_url(channel_id):
    """Get stream URL for a channel"""
    try:
        # Run sxm.py to get stream URL
        result = subprocess.run(
            ['python3', 'sxm.py', '-c', channel_id],
            capture_output=True,
            text=True,
            timeout=30
        )
        
        # Parse output for stream URL
        for line in result.stdout.split('\n'):
            if 'http' in line and 'm3u8' in line:
                return line.strip()
        
        return None
    except Exception as e:
        print(f"Error getting stream URL: {e}")
        return None

@app.route('/api/login', methods=['POST'])
def login():
    """
    Login endpoint
    POST /api/login
    Body: {"username": "email", "password": "pass"}
    """
    data = request.get_json()
    
    if not data or 'username' not in data or 'password' not in data:
        return jsonify({'success': False, 'error': 'Missing credentials'}), 400
    
    # Verify credentials match config
    if data['username'] == SXM_EMAIL and data['password'] == SXM_PASSWORD:
        if authenticate():
            return jsonify({'success': True, 'token': 'sxm_authenticated'})
    
    return jsonify({'success': False, 'error': 'Invalid credentials'}), 401

@app.route('/api/channels', methods=['GET'])
def channels():
    """
    Get channel list
    GET /api/channels
    """
    global channels_cache
    
    # Refresh cache if empty
    if not channels_cache:
        channels_cache = get_channels()
    
    return jsonify({'channels': channels_cache})

@app.route('/api/stream/<channel_id>', methods=['GET'])
def stream(channel_id):
    """
    Get stream URL for channel
    GET /api/stream/<channel_id>
    """
    stream_url = get_stream_url(channel_id)
    
    if stream_url:
        return jsonify({'streamUrl': stream_url})
    
    return jsonify({'error': 'Channel not found or unavailable'}), 404

@app.route('/api/health', methods=['GET'])
def health():
    """Health check endpoint"""
    return jsonify({'status': 'ok', 'service': 'sxm-server'})

if __name__ == '__main__':
    print("Starting SiriusXM API Server...")
    print(f"Server will be accessible at: http://<your-pi-ip>:5000")
    
    # Run on all interfaces, port 5000
    app.run(host='0.0.0.0', port=5000, debug=False)
```

Save and exit.

Make it executable:
```bash
chmod +x sxm_server.py
```

### Step 7: Test the Server

```bash
python3 sxm_server.py
```

You should see:
```
Starting SiriusXM API Server...
 * Serving Flask app 'sxm_server'
 * Running on http://0.0.0.0:5000
```

**Test from another terminal:**
```bash
curl http://localhost:5000/api/health
# Should return: {"status":"ok","service":"sxm-server"}
```

### Step 8: Find Your Server's IP Address

```bash
hostname -I
```

Example output: `192.168.1.100`

**This is the IP address you'll configure on the ESP32!**

### Step 9: Set Up Auto-Start (Optional but Recommended)

Create a systemd service to auto-start the server:

```bash
sudo nano /etc/systemd/system/sxm-server.service
```

**Add:**
```ini
[Unit]
Description=SiriusXM API Server
After=network.target

[Service]
Type=simple
User=pi
WorkingDirectory=/home/pi/m3u8XM
ExecStart=/usr/bin/python3 /home/pi/m3u8XM/sxm_server.py
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
```

Save and exit.

Enable and start:
```bash
sudo systemctl enable sxm-server
sudo systemctl start sxm-server
```

Check status:
```bash
sudo systemctl status sxm-server
```

View logs:
```bash
sudo journalctl -u sxm-server -f
```

---

## ESP32 Configuration

### Update config.h

Edit `/home/neil/Documents/esp32-sxm-intradio/include/config.h`:

```cpp
// Set to true to enable server mode
#define USE_SXM_SERVER true

// Replace with your server's IP:PORT
#define DEFAULT_SXM_SERVER "192.168.1.100:5000"
```

### Rebuild and Flash

```bash
cd /home/neil/Documents/esp32-sxm-intradio
pio run --target upload
```

### Configure on Device

When the ESP32 boots:
1. It will ask for WiFi (connect to same network as Pi)
2. It will ask for SiriusXM credentials
3. It will use the server IP from config.h

**To change server IP later:**
- Add a "SXM Server" field in the settings menu
- Store with `settings.setSXMServer("192.168.1.100:5000")`

---

## Testing the Complete Setup

### Test 1: Server Health

```bash
curl http://192.168.1.100:5000/api/health
```

Expected: `{"status":"ok","service":"sxm-server"}`

### Test 2: Login

```bash
curl -X POST http://192.168.1.100:5000/api/login \
  -H "Content-Type: application/json" \
  -d '{"username":"your@email.com","password":"yourpass"}'
```

Expected: `{"success":true,"token":"sxm_authenticated"}`

### Test 3: Get Channels

```bash
curl http://192.168.1.100:5000/api/channels
```

Expected: JSON array of channels

### Test 4: Get Stream URL

```bash
curl http://192.168.1.100:5000/api/stream/hits1
```

Expected: `{"streamUrl":"https://..."}`

### Test 5: ESP32 Connection

Power on ESP32 and watch serial monitor:
```
SXM: Using server mode
SXM: Connecting to server at http://192.168.1.100:5000/api/login
SXM: Server login successful
SXM: Fetching channels from http://192.168.1.100:5000/api/channels
SXM: Loaded 200 channels from server
```

---

## Troubleshooting

### Server won't start

**Check Python version:**
```bash
python3 --version
# Should be 3.7+
```

**Check dependencies:**
```bash
pip3 list | grep -i flask
pip3 list | grep -i requests
```

**Check port availability:**
```bash
sudo netstat -tulpn | grep 5000
```

### ESP32 can't connect

**Verify network:**
- ESP32 and Pi must be on same WiFi network
- Check firewall on Pi:
```bash
sudo ufw status
sudo ufw allow 5000/tcp  # If firewall is active
```

**Test from ESP32's network:**
```bash
# From phone connected to same hotspot
curl http://192.168.1.100:5000/api/health
```

**Check server IP:**
```bash
hostname -I
```

### Channels not loading

**Check m3u8XM functionality:**
```bash
cd ~/m3u8XM
python3 sxm.py -l
```

**Verify config.ini:**
```bash
cat config.ini
```

**Check SiriusXM subscription:**
- Log into siriusxm.com and verify subscription is active

### Streams won't play

**Check FFmpeg:**
```bash
ffmpeg -version
```

**Test stream URL manually:**
```bash
curl -I "http://192.168.1.100:5000/api/stream/hits1"
```

---

## Advanced Configuration

### Use Static IP for Raspberry Pi

Edit `/etc/dhcpcd.conf`:
```bash
sudo nano /etc/dhcpcd.conf
```

Add:
```
interface wlan0
static ip_address=192.168.1.100/24
static routers=192.168.1.1
static domain_name_servers=8.8.8.8
```

### Enable HTTPS (Optional)

For security, use HTTPS with self-signed cert:

```bash
pip3 install pyopenssl

# Generate certificate
openssl req -x509 -newkey rsa:4096 -nodes \
  -out cert.pem -keyout key.pem -days 365
```

Update `sxm_server.py`:
```python
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, 
            ssl_context=('cert.pem', 'key.pem'))
```

Update ESP32 config:
```cpp
#define DEFAULT_SXM_SERVER "https://192.168.1.100:5000"
```

### Multiple ESP32 Devices

The server can handle multiple ESP32 devices simultaneously. No additional configuration needed.

### Cache Optimization

Add caching to reduce SiriusXM API calls:

```python
from functools import lru_cache
import time

@lru_cache(maxsize=100)
def get_stream_url_cached(channel_id, timestamp):
    return get_stream_url(channel_id)

# Call with current hour as timestamp
# URLs valid for ~1 hour typically
stream_url = get_stream_url_cached(channel_id, int(time.time() / 3600))
```

---

## Performance Considerations

**Raspberry Pi Requirements:**
- **CPU:** Any Pi 3+ works fine
- **RAM:** 512MB minimum, 1GB+ recommended
- **Storage:** 4GB+ SD card

**Network:**
- 5-10 Mbps internet connection
- Low latency to SiriusXM servers
- WiFi or Ethernet (Ethernet preferred for stability)

**Power:**
- Official Pi power supply recommended (5V 3A for Pi 4)
- Use quality power supply to avoid crashes

---

## Security Considerations

1. **Change default Pi password**
   ```bash
   passwd
   ```

2. **Keep system updated**
   ```bash
   sudo apt update && sudo apt upgrade -y
   ```

3. **Use firewall**
   ```bash
   sudo apt install ufw
   sudo ufw allow 22  # SSH
   sudo ufw allow 5000  # API server
   sudo ufw enable
   ```

4. **Don't expose server to internet** - Keep on local network only

5. **Consider VPN** if accessing remotely

---

## Alternative: Docker Container (Advanced)

Create `Dockerfile`:
```dockerfile
FROM python:3.9-slim

WORKDIR /app

RUN apt-get update && apt-get install -y ffmpeg git && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/myselfondiscord/m3u8XM.git .

RUN pip install --no-cache-dir flask requests

COPY sxm_server.py .
COPY config.ini .

EXPOSE 5000

CMD ["python", "sxm_server.py"]
```

Build and run:
```bash
docker build -t sxm-server .
docker run -d -p 5000:5000 --name sxm-server --restart unless-stopped sxm-server
```

---

## Monitoring

### Check server logs:
```bash
sudo journalctl -u sxm-server -f
```

### Monitor resource usage:
```bash
htop  # Install with: sudo apt install htop
```

### Check uptime:
```bash
systemctl status sxm-server
```

---

## Summary

**You now have:**
- ✅ m3u8XM server running on Raspberry Pi
- ✅ REST API accessible at `http://your-pi-ip:5000`
- ✅ ESP32 configured to use server mode
- ✅ Auto-start on boot (if enabled)
- ✅ Full SiriusXM channel streaming

**Next steps:**
1. Configure ESP32 with server IP
2. Test streaming on device
3. Install in vehicle
4. Enjoy your in-car SiriusXM!

---

**For issues or questions:**
- Check m3u8XM repository: https://github.com/myselfondiscord/m3u8XM
- Review ESP32 serial output for errors
- Verify network connectivity between devices
