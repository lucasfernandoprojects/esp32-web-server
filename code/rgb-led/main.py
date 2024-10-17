# ========================================================================================
# Use a web page to control a RGB LED
# Created on 17 October 2024
# Created by Lucas Fernando (https://www.youtube.com/@lucasfernandochannel)
# You are free to use this code the way you want
# ========================================================================================

def set_up_leds():
    redLed = Pin(18, Pin.OUT)
    greenLed = Pin(19, Pin.OUT)
    blueLed = Pin(21, Pin.OUT)
    
    redLed.value(0)
    greenLed.value(0)
    blueLed.value(0)
    
    return redLed, greenLed, blueLed


def website():
    if redLed.value() == 1:
        redState = "ON"
    else:
        redState = "OFF"
        
    if greenLed.value() == 1:
        greenState = "ON"
    else:
        greenState = "OFF"
        
    if blueLed.value() == 1:
        blueState = "ON"
    else:
        blueState = "OFF"
        
    page = """
        <!DOCTYPE html>
        <html lang="en">
            <head>
                <meta charset="utf-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>RGB Led Controller</title>
                <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css" rel="stylesheet">
                <style>
                    body, html {
                        height: 100%;
                        margin: 0;
                        padding: 0;
                        background-color: #f0f0f0;
                        display: flex;
                        flex-direction: column;
                    }
                    h1, h2 {
                        text-align: center;
                        margin: 20px 0;
                    }
                    .card {
                        border-radius: 12px;
                        margin-bottom: 20px;
                        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
                    }
                    .btn {
                        width: 100%;
                        padding: 15px;
                        font-size: 1.2rem;
                    }
                    /* Add left and right margins to the main content */
                    main {
                        padding-left: 15px;
                        padding-right: 15px;
                    }
                    footer {
                        margin-top: auto;
                        background-color: #343a40;
                        color: white;
                        padding: 20px;
                        text-align: center;
                    }
                    /* Style links with a border and different color */
                    .footer-link {
                        color: #f1c40f; /* Change link color */
                        text-decoration: none;
                        border: 2px solid #f1c40f; /* Add border around links */
                        padding: 5px 10px;
                        border-radius: 5px;
                    }
                    .footer-link:hover {
                        text-decoration: underline;
                        background-color: #f1c40f;
                        color: black;
                    }
                </style>
            </head>
            <body>
                <main class="container mt-5">
                    <div class="row">
                        <h1>RGB Led Controller</h1>
                        <h2>Control your RGB LED remotely</h2>
                    </div>
                    <div class="row justify-content-center">
                        <div id="redCard" class="card col-12 col-md-3 mx-3">
                            <div class="card-body text-center">
                                <h5 class="card-title text-danger">Red Light</h5>
                                <p class="card-text">Turn the red light on/off.</p>
                                <a href="/?red" class="btn btn-danger">""" + redState + """</a>
                            </div>
                        </div>
                        <div id="greenCard" class="card col-12 col-md-3 mx-3">
                            <div class="card-body text-center">
                                <h5 class="card-title text-success">Green Light</h5>
                                <p class="card-text">Turn the green light on/off.</p>
                                <a href="/?green" class="btn btn-success">""" + greenState + """</a>
                            </div>
                        </div>
                        <div id="blueCard" class="card col-12 col-md-3 mx-3">
                            <div class="card-body text-center">
                                <h5 class="card-title text-primary">Blue Light</h5>
                                <p class="card-text">Turn the blue light on/off.</p>
                                <a href="/?blue" class="btn btn-primary">""" + blueState + """</a>
                            </div>
                        </div>
                    </div>
                </main>
                <footer>
                    <p>Created by <a href="https://www.youtube.com/@lucasfernandochannel" target="_blank" class="footer-link">Lucas Fernando</a></p>
                    <p>Check out the project on <a href="https://github.com/lucasfernandoprojects" target="_blank" class="footer-link">GitHub</a></p>
                </footer>
                <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js"></script>
            </body>
        </html>
    """
    return page


redLed, greenLed, blueLed = set_up_leds()
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(('', 80))
server.listen(5)

while True:
    conn, addr = server.accept()
    print(f"The server got a connection from {str(addr)}")
    request = conn.recv(1024)
    request = str(request)
  
    red = request.find("/?red")
    green = request.find("/?green")
    blue = request.find("/?blue")
  
    if (red == 6):
        if redLed.value() == 1:
            redLed.value(0)
        else:
            redLed.value(1)
        
    if (green == 6):
        if greenLed.value() == 1:
            greenLed.value(0)
        else:
            greenLed.value(1)
        
    if (blue == 6):
        if blueLed.value() == 1:
            blueLed.value(0)
        else:
            blueLed.value(1)
            
    response = website()
    conn.send("HTTP/1.1 200 OK\n")
    conn.send("Content-Type: text/html\n")
    conn.send("Connection: close\n\n")
    conn.sendall(response)
    conn.close()
