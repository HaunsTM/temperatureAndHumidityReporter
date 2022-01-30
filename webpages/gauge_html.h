#ifndef GAUGE_HTML_H
#define GAUGE_HTML_H

const char GAUGE_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <title>Gauge - Temperature &amp; humidity reporter</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="content-type" content="text/html;charset=UTF-8">
    <link rel="stylesheet" href="/styleSimple_css">
    
    <style>
        .flex-container {
            display: flex;

            justify-content: flex-start; 
            align-items: flex-start;
            flex-wrap: wrap; 
        
            height: 100%;
            padding: 0.5rem;
        }
      
        .gauge-container {
            display: flex;
            flex-direction: column;
            align-items: center;
        }

        .measurement-value {
            font-family: 'Orbitron', sans-serif;
            font-size: 16pt;
            background-color: LightGrey;
            color: Gray;
        }
    </style>
</head>
<body onload="javascript:start();">
    <h1 class="title">Gauge - Temperature &amp; humidity reporter</h1>
    <h2>Software</h2>
    <p>This software is intended for Arduino and compatible PLCs. The purpose is to use the PLC with AZ-Delivery DHT22 Temperature Sensor.</p>
    <h3>Device</h3>
    
    <div class="flex-container">
    
        <div id="gaugeTemperature" class="gauge-container">
            <div>
                <h4>Temperature</h4>
            </div>
            <div>
                <canvas id="canvasThermometer"></canvas>
            </div>
            <div class="measurement-value">
                <span data-bind="text: measurement.temperatureC"></span><span>  &deg;C</span>
            </div>
        </div>

        <div id="gaugeHygrometer" class="gauge-container">
            <div>
                <h4>Humidity</h4>                
            </div>
            <div>                
                <canvas id="canvasHygrometer"></canvas>
            </div>
            <div class="measurement-value">
                <span data-bind="text: measurement.humidityPercent"></span><span>  %RH</span>       
            </div>
        </div>

    </div>

    <nav class="navigation-links"><div><a data-bind="attr: { href: computed.href.gauge }">Gauge </a></div><div>|</div><div><a data-bind="attr: { href: computed.href.info }">Device info</a></div></nav>
    <div class="info">
        <p>This software is distributed under <a href = "https://en.wikipedia.org/wiki/MIT_License">MIT License</a>. Source code on <a href="https://github.com/HaunsTM">Github - HaunsTM</a></p>
    </div>


    <script src="/javascriptKnockout_js">
    </script>

    <script src="/constJavascriptParameters_js">
    </script>
    
    <script src="/javascriptGauge_js">        
    </script>
    
    <script type="text/javascript">
        const optsThermometer = {
            angle: 0,
            lineWidth: 0.2,
            radiusScale: 0.9, 
            pointer: {
                length: 0.5,
                strokeWidth: 0.05,
                color: '#000000'
            },
            limitMin: false,
            limitMax: false,

            strokeColor: '#000000',

            highDpiSupport: true,

            renderTicks: {
                divisions: 12,
                divWidth: 1,
                divLength: 0.5,
                divColor: '#000000',
                subDivisions: 6,
                subLength: 0.25,
                subWidth: 0.5,
                subColor: '#5b5b5b'
            },

            staticLabels: {
                font: "10px sans-serif",
                labels: [-40, -30, -20, -10, 0, 10, 20, 30, 40, 50, 60, 70, 80],
            },

            staticZones: [
                {strokeStyle: "#72caf0", min: -40, max: 0}, // blue
                {strokeStyle: "#ffffff", min: 0, max: 50},  // white
                {strokeStyle: "#f44336", min: 40, max: 80}  // red
            ],            
        };
        const canvasThermometer = document.getElementById('canvasThermometer');
        const gaugeThermometer = new Gauge(canvasThermometer).setOptions(optsThermometer);
        gaugeThermometer.maxValue = 80;
        gaugeThermometer.setMinValue(-40);

        gaugeThermometer.set(0);
    </script>

    <script type="text/javascript">
        const optsHygrometer = {
            angle: 0,
            lineWidth: 0.2,
            radiusScale: 0.9, 
            pointer: {
                length: 0.5,
                strokeWidth: 0.05,
                color: '#000000'
            },
            limitMin: false,
            limitMax: false,

            strokeColor: '#000000',

            highDpiSupport: true,

            renderTicks: {
                divisions: 10,
                divWidth: 1,
                divLength: 0.5,
                divColor: '#000000',
                subDivisions: 6,
                subLength: 0.25,
                subWidth: 0.5,
                subColor: '#5b5b5b'
            },

            staticLabels: {
                font: "10px sans-serif",
                labels: [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100],
            },

            staticZones: [
                {strokeStyle: "#ffffff", min: 0, max: 30},  // white
                {strokeStyle: "#d9ead3", min: 30, max: 50}, // green
                {strokeStyle: "#f44336", min: 70, max: 100} // red
            ],            
        };

        const canvasHygrometer = document.getElementById('canvasHygrometer');
        const gaugeHygrometer = new Gauge(canvasHygrometer).setOptions(optsHygrometer); 
        gaugeHygrometer.maxValue = 100;
        gaugeHygrometer.setMinValue(0);

        gaugeHygrometer.set(0);
    </script>

    <script>
                
        let viewModelKnockout = (function() {
            let obj = {};           

            obj.measurement = {
                temperatureC: ko.observable(),
                humidityPercent: ko.observable(),
            };

            obj.computed = {};

            obj.computed.href = {
                base: ko.pureComputed( function() { return "//" + constJavascriptParameters.wifi.localIP + "/" ; }),
                gauge: ko.pureComputed( function() { return "//" + constJavascriptParameters.wifi.localIP + "/gauge" }),
                info: ko.pureComputed( function() { return "//" + constJavascriptParameters.wifi.localIP + "/info" }),
                sensorData: ko.pureComputed( function() { return "//" + constJavascriptParameters.wifi.localIP + "/sensorData" }),
            }

            ko.applyBindings(obj);
            return obj;
        })();

    </script>
    

    <script>
        
        viewModelKnockout.measurement.temperatureC('**.*');
        viewModelKnockout.measurement.humidityPercent('**.*');

        function start() {
            let websock;
            websock = new WebSocket('ws://' + constJavascriptParameters.wifi.localIP + ':' + constJavascriptParameters.webSocket.serverPort + '/');
            websock.onopen = function(evt) { console.log('websock open'); };
            websock.onclose = function(evt) { console.log('websock close'); };
            websock.onerror = function(evt) { console.log(evt); };
            websock.onmessage = function(message) {
                const currentMeasurement = JSON.parse(message.data);
                gaugeThermometer.set(currentMeasurement.temperatureC);
                gaugeHygrometer.set(currentMeasurement.humidityPercent);

                viewModelKnockout.measurement.temperatureC(currentMeasurement.temperatureC);
                viewModelKnockout.measurement.humidityPercent(currentMeasurement.humidityPercent);
            }
        }
    </script>
</body>
</html>
)=====";

#endif